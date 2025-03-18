-- CREATES

CREATE TABLE Vehicle (
    id SERIAL PRIMARY KEY,
    registration TEXT NOT NULL
);

CREATE TABLE Logs (
    id SERIAL PRIMARY KEY,
    description TEXT,
    type TEXT,
    vehicle_id INTEGER NOT NULL,
    FOREIGN KEY (vehicle_id) REFERENCES Vehicle(id) ON DELETE CASCADE
);

CREATE TABLE Sensor (
    id SERIAL PRIMARY KEY,
    name TEXT NOT NULL,
    unity TEXT,
    serial_number TEXT,
    vehicle_id INTEGER NOT NULL,
    FOREIGN KEY (vehicle_id) REFERENCES Vehicle(id) ON DELETE CASCADE
);

CREATE TABLE Metrics (
    id SERIAL PRIMARY KEY,
    value TEXT NOT NULL,
    date TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    sensor_id INTEGER NOT NULL,
    FOREIGN KEY (sensor_id) REFERENCES Sensor(id) ON DELETE CASCADE
);
