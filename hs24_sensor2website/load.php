<?php
/************************************************************************
 * load.php
 * https://fiessling.ch/iot1/hs24_sensor2website/load.php
 * Daten als JSON-String vom MC empfangen und Daten in die Datenbank einfügen
 ************************************************************************/ 

require_once("server_config.php");

$pdo = new PDO($dsn, $db_user, $db_pass, $options); 
echo "</br> DB Verbindung ist erfolgreich";

$inputJSON = file_get_contents("php://input"); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true);

$wert = $input["wert"];
$sql = "INSERT INTO kuehlschrank (wert) VALUES (?)";
$stmt = $pdo->prepare($sql);
$stmt->execute([$wert]);
?>