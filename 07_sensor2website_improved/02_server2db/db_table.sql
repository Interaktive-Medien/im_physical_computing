CREATE TABLE sensordata (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    wert1 DECIMAL(5,2),
    wert2 DECIMAL(5,2),
    zeit TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
)
-- wert1: 1. Gesamtzahl der Stellen an, einschließlich der Nachkommastellen,   2. Nachkommastellen
-- INSERT INTO `sensordata`(`sensor`, `wert1`, 'wert2') VALUES ("huhu", 99, 33.3)


CREATE TABLE receiveddata (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    msg VARCHAR(100) NOT NULL
)