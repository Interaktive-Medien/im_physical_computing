<?php
 /*************************************************************
 * get_selected_tracks.php
 * This script receives HTTP GET messages from the mc. It asks for 
 * data from database. Then it passes them to mc as a JSON string 
 * Example: https://heulradar.hausmaenner.ch/db/get_selected_tracks.php
 *************************************************************/

require_once("config.php");


header('Content-Type: application/json'); // sets Content-Type of the answer to JSON


try{ 
    $sql = "SELECT id, title FROM tracks WHERE selected = 1"; // SQL query to select all tracks where 'selected' is 1
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $data = $stmt->fetchAll(PDO::FETCH_ASSOC);
    echo json_encode($data);
}
catch (Exception $e) {
    echo json_encode([
        "status" => "error", 
        "message" => "Database error: " . $e->getMessage()
    ]);
    exit;
}
?>