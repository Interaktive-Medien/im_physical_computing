<?php
/*****************************************************
 * login.php
 * Schreibt einen erfolgreichen Login in die History-Tabelle.
 ******************************************************/
require_once("../system/config.php");
header('Content-Type: application/json');

$inputJSON = file_get_contents('php://input'); 
$input = json_decode($inputJSON, true); 

if (isset($input["user_id"])) {
    $userId = intval($input["user_id"]);

    // Schreibt den Datensatz in die History-Tabelle (zeit wird per DEFAULT CURRENT_TIMESTAMP befaellt)
    $sql = "INSERT INTO history (user_id) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    
    if ($stmt->execute([$userId])) {
        echo json_encode(["success" => true, "message" => "Login erfolgreich in History protokolliert."]);
    } else {
        echo json_encode(["success" => false, "message" => "Fehler beim Schreiben der History."]);
    }
} else {
    echo json_encode(["success" => false, "message" => "Ungueltige User-ID erhalten."]);
}
?>