#include <Arduino.h>
#include "../lib/MoveController/MoveController.h"
#include "../lib/Pinout/Pinout.h"

#define MOTOR_DEFAULT_SPEED 225

const uint8_t motor_right_channel = 0;
const uint8_t motor_left_channel = 1;

Motor motor_right(Pinout::MOTOR_RIGHT_PIN1, Pinout::MOTOR_RIGHT_PIN2, Pinout::MOTOR_RIGHT_EN, motor_right_channel);
Motor motor_left(Pinout::MOTOR_LEFT_PIN1, Pinout::MOTOR_LEFT_PIN2, Pinout::MOTOR_LEFT_EN, motor_left_channel);

MoveController move(motor_right, motor_left);

void setup() {
    Serial.begin(9600);
    Serial.println("\nComunicação serial inicializada.");

    move.Stop();
}

void loop() {

    while(Serial.available()) {

        String line = Serial.readStringUntil('\n');
        char buffer[line.length()];
        line.toCharArray(buffer, line.length());

        String cmd = strtok(buffer, " ");
        String instruction = strtok(NULL, " ");

        int speed = MOTOR_DEFAULT_SPEED;

        Serial.print("Entrada: ");
        Serial.println(line);


        if(cmd.equalsIgnoreCase("move")) {
            if(instruction.equalsIgnoreCase("--front")) {

                move.MoveForward(speed);
                Serial.println("Moving forward.");

                delay(2000);
                move.Stop();
            }

            else if(instruction.equalsIgnoreCase("--back")) {

                move.MoveBackward(speed);
                Serial.println("Moving backward.");
                
                delay(2000);
                move.Stop();
            }

            else if(instruction.equalsIgnoreCase("--left")) {
                move.TurnLeft(speed);
                Serial.println("Turning left.");
                
                delay(2000);
                move.Stop();
            }

            else if(instruction.equalsIgnoreCase("--right")) {
                move.TurnRight(speed);
                Serial.println("Turning right.");
                
                delay(2000);
                move.Stop();
            }

            else if(instruction.equalsIgnoreCase("--stop")) {
                move.Stop();
                Serial.println("Stopped.");
                
                delay(2000);
                move.Stop();
            }

            else {
                Serial.println("Instrução inválida.");
            }
        }
    }
}