<?php
require_once("db_config.php");
echo "This script receives HTTP POST messages and pushes their content into the database.";


###################################### connect to db
try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    echo "</br> DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    error_log("DB Error: " . $e->getMessage());
    echo json_encode(["status" => "error", "message" => "DB connection failed"]);
}


###################################### Empfangen der JSON-Daten

$inputJSON = file_get_contents('php://input'); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true); // Dekodieren der JSON-Daten in ein Array


###################################### receiving a post request from a HTML form, later from ESP32 C6

$wert = $input["wert"];
# insert new user into db
$sql = "INSERT INTO temperatur4 (wert) VALUES (?)";
$stmt = $pdo->prepare($sql);
$stmt->execute([$wert]);

?>