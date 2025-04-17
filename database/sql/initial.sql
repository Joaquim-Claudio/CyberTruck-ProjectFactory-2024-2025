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

-- INSERTS

-- TB VEHICLE
INSERT INTO Vehicle (registration) VALUES ('EJL-2022') RETURNING id;


-- TB LOG
INSERT INTO Logs (description, type, vehicle_id)
VALUES 
    ('Falha no sensor de velocidade', 'Erro', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Revisão de manutenção realizada', 'Manutenção', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Sensor de bateria detectou baixa carga', 'Alerta', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Teste de calibração realizado', 'Teste', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Sistema reiniciado com sucesso', 'Info', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Desvio de rota detectado', 'Aviso', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Nova atualização de firmware aplicada', 'Atualização', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Perda momentânea de conexão com servidor', 'Erro', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Mudança de configuração de sensores', 'Configuração', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
    ('Verificação periódica dos sistemas concluída', 'Relatório', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022'));


-- TB SENSOR
INSERT INTO Sensor (name, unity, serial_number, vehicle_id)
VALUES
	  ('Ultrassónico', 'cm', 'HC-SR04-01', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
	  ('Acelerómetro', 'm/s', 'MPU6050-01', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
	  ('Giroscópio', 'ºgraus', 'MPU6050-01', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
	  ('Bateria', '%', 'ACS712-01', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022')),
	  ('Line Tracking', 'bool', 'TRCT-5000-01', (SELECT id FROM Vehicle WHERE registration = 'EJL-2022'));


-- TB METRICS
INSERT INTO Metrics (value, sensor_id)
VALUES
	  ('35.50', (SELECT id FROM Sensor WHERE serial_number = 'HC-SR04-01')),
	  ('8.50', (SELECT id FROM Sensor WHERE serial_number = 'MPU6050-01' AND name = 'Acelerómetro')),
	  ('99.25', (SELECT id FROM Sensor WHERE serial_number = 'ACS712-01')),
	  ('28.00', (SELECT id FROM Sensor WHERE serial_number = 'MPU6050-01' AND name = 'Giroscópio')),
	  ('0', (SELECT id FROM Sensor WHERE serial_number = 'TRCT-5000-01'));