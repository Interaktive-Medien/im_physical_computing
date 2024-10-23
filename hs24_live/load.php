<?php
/************************************************************************
 * load.php
 * https://fiessling.ch/iot1/hs24_live/load.php
 * Daten als JSON-String vom MC empfangen und Daten in die Datenbank einfügen
 ************************************************************************/ 

require_once("server_config.php");
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




###################################### Prüfen, ob die JSON-Daten erfolgreich dekodiert wurden
### folgender Block nicht zwingend notwendig, nur für Troubleshooting: Die rohen JSON-Daten in die Tabelle receiveddata einfügen

// if (json_last_error() === JSON_ERROR_NONE && !empty($input)) {
//     $sql = "INSERT INTO receiveddata (msg) VALUES (?)";
//     $stmt = $pdo->prepare($sql);
//     $stmt->execute([$inputJSON]);
// }

// echo "</br></br> Zeig die letzten 5 empfangenen HTTP Requests";
// $sql = "SELECT * FROM receiveddata ORDER BY id DESC LIMIT 5";
// $stmt = $pdo->prepare($sql);
// $stmt->execute();
// $receiveddata = $stmt->fetchAll();

// echo "<ul>";
// foreach ($receiveddata as $data) {
//     echo "<li>" . $data['msg'] . "</li>";
// }
// echo "</ul>";


###################################### receiving a post request from a HTML form, later from ESP32 C6

$wert = $input["wert"];
# insert new user into db
$sql = "INSERT INTO sensordata (wert) VALUES (?)";
$stmt = $pdo->prepare($sql);
$stmt->execute([$wert]);

?>