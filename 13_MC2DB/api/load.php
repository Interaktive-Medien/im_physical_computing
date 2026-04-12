<?php
 /*****************************************************
 * Kapitel 12: Website2DB > Schritt 2: Website -> DB
 * load.php
 * Daten als JSON-String vom Formular sender.html (später vom MC) serverseitig empfangen und Daten in die Datenbank einfügen
 * Datenbank-Verbindung
**************************/


require_once("../system/config.php");
// echo "This script receives HTTP POST messages and pushes their content into the database.";



###################################### Empfangen der JSON-Daten

$inputJSON = file_get_contents('php://input'); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true); 


###################################### receiving a post request from a HTML form, later from ESP

$wert = $input["wert"];         // Hol den Wert an der Stelle "wert" aus dem JS-Objekt (ehemals JSON-String)
# insert new user into db
$sql = "INSERT INTO sensordata (wert) VALUES (?)";
$stmt = $pdo->prepare($sql);
$stmt->execute([$wert]);

?>