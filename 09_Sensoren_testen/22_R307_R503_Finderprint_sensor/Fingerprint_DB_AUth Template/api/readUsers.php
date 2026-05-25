<?php
/*****************************************************
 * unload.php
 * Gibt alle User-Templates aus. Sortiert nach dem 
 * aktuellsten Login aus der History-Tabelle.
 * Duplikate werden serverseitig herausgefiltert.
 ******************************************************/
require_once("../system/config.php");
header('Content-Type: application/json');

try {
    /**
     * Die SQL-Logik im Detail:
     * 1. Wir waehlen die ID und das Template aus 'users'.
     * 2. Wir verknuepfen die 'history'-Tabelle ueber einen LEFT JOIN, damit auch User 
     * ohne bisherigen Login (frisch registriert) in der Liste auftauchen.
     * 3. GROUP BY users.id sorgt dafuer, dass jeder User exakt nur EINMAL im Ergebnis vorkommt.
     * 4. ORDER BY sortiert zuerst nach dem neuesten Login-Zeitpunkt (MAX(history.zeit) DESC).
     * Falls zwei User noch nie eingeloggt waren, greift als Fallback die Sortierung nach der 
     * neuesten Registrierungs-ID (users.id DESC).
     */
    $sql = "SELECT users.id, users.fingerprintHex 
            FROM users 
            LEFT JOIN history ON users.id = history.user_id 
            GROUP BY users.id 
            ORDER BY MAX(history.zeit) DESC, users.id DESC";
            
    $stmt = $pdo->query($sql);
    $allUsers = $stmt->fetchAll(PDO::FETCH_ASSOC);

    echo json_encode(["success" => true, "users" => $allUsers]);
} catch (PDOException $e) {
    echo json_encode(["success" => false, "message" => "Datenbankfehler: " . $e->getMessage()]);
}
?>