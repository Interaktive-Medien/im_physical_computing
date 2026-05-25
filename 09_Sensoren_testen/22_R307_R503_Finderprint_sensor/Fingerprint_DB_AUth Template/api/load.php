<?php
/*****************************************************
 * load.php
 * Empfängt neuen Fingerprint-Hex und legt User an.
 ******************************************************/
require_once("../system/config.php");
header('Content-Type: application/json');

$inputJSON = file_get_contents('php://input'); 
$input = json_decode($inputJSON, true); 

if (isset($input["fingerprintHex"])) {
    $rawHex = $input["fingerprintHex"];

    $sql = "INSERT INTO users (fingerprintHex) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    
    if ($stmt->execute([$rawHex])) {
        echo json_encode(["status" => "success", "message" => "Neuer User erfolgreich angelegt."]);
    } else {
        echo json_encode(["status" => "error", "message" => "Datenbankfehler."]);
    }
} else {
    echo json_encode(["status" => "error", "message" => "Keine Daten empfangen."]);
}
?>