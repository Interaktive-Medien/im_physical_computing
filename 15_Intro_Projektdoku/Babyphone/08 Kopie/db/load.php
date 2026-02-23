<?php
 /*************************************************************
 * load.php
 * receive data as a JSON string from the mc on the server and insert into database
 * Datenbank-Verbindung
 * replace $db_host, $db_name, $db_user, $db_pass durch deine eigenen Daten. 
 * Beispiel: https://heulradar.hasmaenner.ch/db/load.php
 *************************************************************/

require_once("config.php");
// echo "This script receives HTTP POST messages and pushes their content into the database.";



###################################### receive JSON data

$inputJSON = file_get_contents('php://input'); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true); // Dekodieren der JSON-Daten in ein Array

$is_heulsession = $input["is_heulsession"];
$heulsession_id = $input["heulsession_id"];   

try{ 
    if ($is_heulsession == 1){
        $sql = "INSERT INTO heulhistory (starttime) VALUES (NOW())";
        $result = $pdo->prepare($sql);
        $result->execute();
        $heulsession_id = $pdo->lastInsertId();
        echo json_encode([
            "status" => "success", 
            "message" => "Session started", 
            "heulsession_id" => $heulsession_id
        ]);
    }
    else if ($is_heulsession == 0){
        $sql = "UPDATE heulhistory SET endtime = NOW() WHERE id = :heulsession_id";
        $result = $pdo->prepare($sql);
        $result->execute(['heulsession_id' => $heulsession_id]);
        echo json_encode([
            "status" => "success", 
            "message" => "Session ended", 
            "heulsession_id" => $heulsession_id
        ]);
    }
} 

catch (Exception $e) {
    echo "Database error: " . $e->getMessage();
    exit;
}
?>