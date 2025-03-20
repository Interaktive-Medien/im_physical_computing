-- db_table.sql
-- Datenbanktabelle bauen (geht auch per UI mit phpmyadmin)

CREATE TABLE sensordata (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    wert DECIMAL(7,2), -- wert: 1. Gesamtzahl der Stellen inkl. Nachkommastellen,   2. Nachkommastellen
    zeit TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
)

-- INSERT INTO `sensordata`(`wert`) VALUES (99)


CREATE TABLE receiveddata (
    id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
    msg VARCHAR(100) NOT NULL
)