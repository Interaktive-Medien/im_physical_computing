<?php
 /*****************************************************
 * load.php
 * Daten als JSON-String vom MC serverseitig empfangen und Daten in die Datenbank einfügen
 * Datenbank-Verbindung
**************************/

require_once("../system/config.php");

###################################### Empfangen der JSON-Daten

$inputJSON = file_get_contents('php://input'); // JSON-Daten aus dem Body der Anfrage
$input = json_decode($inputJSON, true); 

if (isset($input["fingerprintHex"])) {
    
    $rawHex = $input["fingerprintHex"];
    
    // Verschlüsseln/Hashen des Hex-Codes mittels SHA-256
    $hashedFingerprint = hash('sha256', $rawHex);

    ###################################### Einfügen in die Tabelle "users"
    
    // Die ID-Spalte wird durch AUTO_INCREMENT in der DB automatisch befüllt
    $sql = "INSERT INTO users (fingerprintHex) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    
    if ($stmt->execute([$hashedFingerprint])) {
        echo json_encode(["status" => "success", "message" => "Fingerprint erfolgreich verschluesselt gespeichert."]);
    } else {
        echo json_encode(["status" => "error", "message" => "Datenbank-Fehler beim Einfuegen."]);
    }
    
} else {
    echo json_encode(["status" => "error", "message" => "Ungueltige Daten empfangen."]);
}
?>