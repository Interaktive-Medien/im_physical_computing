<?php
/*****************************************************
 * readUsers.php
 * Gibt alle User-Templates aus. Sortiert nach dem 
 * aktuellsten Login aus der History-Tabelle.
 * Duplikate werden serverseitig herausgefiltert.
 ******************************************************/
require_once("../system/config.php");
header('Content-Type: application/json');

// Sicherheitsgurt für den Verschlüsselungs-Key
if (!defined('ENCRYPTION_KEY')) {
    define('ENCRYPTION_KEY', 'PhysicalComputingIM4_SuperSecretKey2026!');
}

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

    // --- ERGÄNZUNG: Live-Entschlüsselung der Templates ---
    $decryptedUsers = [];
    $cipherMethod = "AES-256-CBC";

    foreach ($allUsers as $user) {
        $dbValue = $user['fingerprintHex'];
        
        // Prüfen, ob der Eintrag dem Verschlüsselungsformat entspricht (enthält einen Doppelpunkt)
        if (strpos($dbValue, ':') !== false) {
            // IV und verschlüsselte Daten wieder trennen
            $parts = explode(":", $dbValue, 2);
            
            // KORREKTUR: Ersetze eventuelle Leerzeichen, die durch die HTTP-Übertragung 
            // aus dem "+" entstanden sind, wieder zurück in ein gültiges Base64 "+"
            $fixedIV = str_replace(' ', '+', $parts[0]);
            $fixedData = str_replace(' ', '+', $parts[1]);
            
            $iv = base64_decode($fixedIV);
            
            // Live entschlüsseln mit dem in config.php definierten ENCRYPTION_KEY
            $decryptedHex = openssl_decrypt($fixedData, $cipherMethod, ENCRYPTION_KEY, 0, $iv);
            
            if ($decryptedHex !== false) {
                $decryptedUsers[] = [
                    "id" => $user['id'],
                    "fingerprintHex" => $decryptedHex
                ];
            }
        } else {
            // Fallback: Falls noch alte unverschlüsselte Testdaten in der DB liegen
            $decryptedUsers[] = [
                "id" => $user['id'],
                "fingerprintHex" => $dbValue
            ];
        }
    }

    // Die entschlüsselten Daten als JSON ausgeben
    echo json_encode(["success" => true, "users" => $decryptedUsers]);

} catch (PDOException $e) {
    echo json_encode(["success" => false, "message" => "Datenbankfehler: " . $e->getMessage()]);
}
?>