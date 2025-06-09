#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <ESP32Servo.h>
#include <math.h>
#include "Param.h"
#include "Timer.h"
#include "Motor.h"
#include "MoveController.h"
#include "Pinout.h"
#include "VehicleCommand.h"

#define GRAVITATION_ACCELERATION 9.7

Motor motor_right(Pinout::MOTOR_RIGHT_PIN1, Pinout::MOTOR_RIGHT_PIN2, Param::MOTOR_RIGHT_CHANNEL_1, Param::MOTOR_RIGHT_CHANNEL_2);
Motor motor_left(Pinout::MOTOR_LEFT_PIN1, Pinout::MOTOR_LEFT_PIN2, Param::MOTOR_LEFT_CHANNEL_1, Param::MOTOR_LEFT_CHANNEL_2);

MoveController driver(motor_right, motor_left);

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

Adafruit_MPU6050 mpu;

JsonDocument jsonDocument;
char buffer[1024];

VehicleCommand vehicleCommand;

// Timer ultrasonicReadTimer(40);
Timer sensorReadTimer(50);
// Timer linetrackingReadTimer(60);
Timer remoteCommandTimer(209);

Servo ultrasonic_servo;

void wifi_setup();
void mqtt_setup();
void mqtt_reconnect();
void addJsonObject(float, float, float);
void mqtt_handle_msg(char*, byte*, unsigned int);
void driverController();
void mpu_setup();
void mpu_calibrate();
void mpu_fix_measure(sensors_event_t*, sensors_event_t*);
void ultrasonic_setup();
float ultrasonic_getDistance();
void ultrasonic_spin();
void linetracking_setup();


float gyro_offset_x = 0;
float gyro_offset_y = 0;
float gyro_offset_z = 0;
float acc_offset_x = 0;
float acc_offset_y = 0;
float acc_offset_z = 0;

unsigned long duration;

void setup() {
    Serial.begin(9600);
    Serial.println("\nSerial communication initialized.");

    Serial.print("Setting up WiFi connection...");
    wifi_setup();

    Serial.print("Setting up MQTT configuration...");
    mqtt_setup();
    
    Serial.print("Setting up MPU6050...");
    mpu_setup();

    Serial.print("Calibrating MPU6050...");
    mpu_calibrate();

    Serial.print("Setting up ultrasonic sensor...");
    ultrasonic_setup();

    Serial.print("Setting up line tracking sensor...");
    linetracking_setup();

    driver.Stop();
}

void loop() {

    if(!mqttClient.connected()){
        mqtt_reconnect();
    }
    mqttClient.loop();

    if(remoteCommandTimer.overlapsed()) {
        driver.Stop();
    }

    if(sensorReadTimer.overlapsed()) {

        // MPU sensor
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        
        mpu_fix_measure(&a, &g);
        
        jsonDocument.clear();
        addJsonObject(a.acceleration.x, a.acceleration.y, a.acceleration.z);
        serializeJson(jsonDocument, buffer);

        mqttClient.publish("cybertruck/rotation", buffer);
        
        // Battery sensor
        int battery_level = map(analogRead(Pinout::BATTERY_SENSOR), 510, 4095, 0, 100);
        char str[8];
        itoa(battery_level, str, 10);
        mqttClient.publish("cybertruck/battery", str);

        sensorReadTimer.reset();
    }

    // if(ultrasonicReadTimer.overlapsed()) {
    //     float distance = ultrasonic_getDistance();
    //     dtostrf(distance, 3, 2, buffer);

    //     mqttClient.publish("cybertruck/distance", buffer);

    //     ultrasonicReadTimer.reset();
    // }

    // if(linetrackingReadTimer.overlapsed()) {
    //     bool linetracking_ir2 = digitalRead(Pinout::LINETRACKING_IR2);
    //     bool linetracking_ir4 = digitalRead(Pinout::LINETRACKING_IR4);

    //     if(linetracking_ir2 || linetracking_ir4) {
    //         mqttClient.publish("cybertruck/linetracking", "1");
    //     } else {
    //         mqttClient.publish("cybertruck/linetracking", "0");
    //     }

    //     linetrackingReadTimer.reset();
    // }

}


void wifi_setup() {
    WiFi.mode(WIFI_STA);

    WiFi.begin(Param::WIFI_SSID, Param::WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED) {
        delay(200);
        Serial.print(".");
    }

    // wifiClient.setInsecure();

    Serial.println("done");
    Serial.print("IPv4:\t");
    Serial.println(WiFi.localIP());
}

void mqtt_setup() {
    mqttClient.setServer(Param::MQTT_BROKER_ADDRESS, Param::MQTT_PORT);
    mqttClient.setCallback(mqtt_handle_msg);
    Serial.println("done");
}

void mqtt_reconnect() {
    Serial.print("Trying connection to MQTT Broker...");
    String clientID = "esp32-client-" + WiFi.macAddress();

    Timer timer(5000);

    while(!mqttClient.connected()) {

        if(timer.overlapsed()) {
            Serial.println("Connection attempt timed out.");
            return;
        }

        mqttClient.connect(clientID.c_str());
        delay(100);
        Serial.print(".");
    }

    Serial.println("done");
    mqttClient.subscribe(Param::DRIVE_TOPIC);
    mqttClient.subscribe(Param::DISTANCE_TOPIC);
}

void addJsonObject(float x, float y, float z) {
    JsonObject obj = jsonDocument.add<JsonObject>();
    obj["x"] = x;
    obj["y"] = y;
    obj["z"] = z;
}


void mqtt_handle_msg(char* topic, byte* payload, unsigned int msg_size) {
    Serial.print("A message arrived on topic: \"");
    Serial.print(topic);
    Serial.println("\". Deserializing...");
    
    String message = "";
    for(int i=0; i<msg_size; i++) {
        message += (char) payload[i];
    }

    Serial.println(message);

    jsonDocument.clear();
    DeserializationError error = deserializeJson(jsonDocument, payload, msg_size);

    if(error) {
        Serial.println("Deserialization failed.");
    }

    vehicleCommand.vehicleId = jsonDocument["vehicleId"].as<String>();
    vehicleCommand.agentId = jsonDocument["agentId"].as<String>();
    vehicleCommand.action = jsonDocument["action"].as<String>();
    vehicleCommand.cmd = jsonDocument["cmd"].as<String>();
    vehicleCommand.value = jsonDocument["value"].as<int>();

    Serial.println(vehicleCommand.vehicleId);
    Serial.println(vehicleCommand.agentId);
    Serial.println(vehicleCommand.action);
    Serial.println(vehicleCommand.cmd);
    Serial.println(vehicleCommand.value);

    remoteCommandTimer.reset();

    if(vehicleCommand.action.equalsIgnoreCase("move")) driverController();
    else if(vehicleCommand.action.equalsIgnoreCase("ultr_spin")) ultrasonic_spin();
}

void driverController() {
    if(vehicleCommand.cmd.equalsIgnoreCase("front")) 
        driver.MoveForward(Param::MOTOR_DEFAULT_SPEED);

    else if(vehicleCommand.cmd.equalsIgnoreCase("back")) 
        driver.MoveBackward(Param::MOTOR_DEFAULT_SPEED);

    else if(vehicleCommand.cmd.equalsIgnoreCase("right")) 
        driver.TurnRight(Param::MOTOR_DEFAULT_SPEED);
        
    else if(vehicleCommand.cmd.equalsIgnoreCase("left")) 
        driver.TurnLeft(Param::MOTOR_DEFAULT_SPEED);
}

void mpu_setup() {
    while(!mpu.begin()) {
        Serial.print(".");
    }

    mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
    mpu.setGyroRange(MPU6050_RANGE_500_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    Serial.println("done");
}


void mpu_calibrate() {
    int samples = 500;
    float gSumX = 0, gSumY = 0, gSumZ = 0,
        aSumX = 0, aSumY = 0, aSumZ = 0;

    for (int i = 0; i < samples; i++) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);

        gSumX += g.gyro.x;
        gSumY += g.gyro.y;
        gSumZ += g.gyro.z;
        aSumX += a.acceleration.x;
        aSumY += a.acceleration.y;
        aSumZ += a.acceleration.z;
        delay(5);
    }

    gyro_offset_x = gSumX / samples;
    gyro_offset_y = gSumY / samples;
    gyro_offset_z = gSumZ / samples;
    acc_offset_x = aSumX / samples;
    acc_offset_y = aSumY / samples;
    acc_offset_z = (aSumZ / samples) - GRAVITATION_ACCELERATION;

    Serial.println("done");
}

void mpu_fix_measure(sensors_event_t* a, sensors_event_t* g) {
    a->acceleration.x = a->acceleration.x - acc_offset_x;
    a->acceleration.y = a->acceleration.y - acc_offset_y;
    a->acceleration.z = a->acceleration.z - acc_offset_z;

    g->gyro.x = g->gyro.x - gyro_offset_x;
    g->gyro.y = g->gyro.y - gyro_offset_y;
    g->gyro.z = g->gyro.z - gyro_offset_z;
}

void ultrasonic_setup() {
    pinMode(Pinout::ULTRASONIC_TRIGGER, OUTPUT);
    pinMode(Pinout::ULTRASONIC_ECHO, INPUT);
    ultrasonic_servo.attach(Pinout::ULTRASONIC_MOTOR);
    ultrasonic_servo.write(93);

    Serial.println("done");
}

float ultrasonic_getDistance() {
    digitalWrite(Pinout::ULTRASONIC_TRIGGER, LOW);
    delayMicroseconds(10);

    digitalWrite(Pinout::ULTRASONIC_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(Pinout::ULTRASONIC_TRIGGER, LOW);

    duration = pulseIn(Pinout::ULTRASONIC_ECHO, HIGH);

    return duration * Param::SOUND_SPEED / 2;
}

void ultrasonic_spin() {
    ultrasonic_servo.write(vehicleCommand.value);
}

void linetracking_setup() {
    pinMode(Pinout::LINETRACKING_IR2, INPUT);
    pinMode(Pinout::LINETRACKING_IR4, INPUT);

    Serial.println("done");
}
