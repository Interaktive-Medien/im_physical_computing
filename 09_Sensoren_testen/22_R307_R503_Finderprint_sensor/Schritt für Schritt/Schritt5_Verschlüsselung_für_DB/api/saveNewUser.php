<?php
/*****************************************************
 * saveNewUser.php
 * Empfängt neuen Fingerprint-Hex und legt User an.
 ******************************************************/
require_once("../system/config.php");
header('Content-Type: application/json');

// Sicherheitsgurt: Falls ENCRYPTION_KEY nicht in config.php definiert ist,
// definieren wir hier lokal einen festen Schlüssel, damit PHP niemals abstürzt.
if (!defined('ENCRYPTION_KEY')) {
    define('ENCRYPTION_KEY', 'PhysicalComputingIM4_SuperSecretKey2026!');
}

$inputJSON = file_get_contents('php://input'); 
$input = json_decode($inputJSON, true); 

if (isset($input["fingerprintHex"])) {
    $rawHex = trim($input["fingerprintHex"]);

    // --- SYMMETRISCHE VERSCHLÜSSELUNG (AES-256) ---
    $cipherMethod = "AES-256-CBC";
    
    // 1. Initialisierungsvektor generieren
    $ivLength = openssl_cipher_iv_length($cipherMethod);
    $iv = openssl_random_pseudo_bytes($ivLength);
    
    // 2. Verschlüsseln
    $encryptedData = openssl_encrypt($rawHex, $cipherMethod, ENCRYPTION_KEY, 0, $iv);
    
    // 3. Wenn die Verschlüsselung fehlschlägt, loggen wir das im JSON
    if ($encryptedData === false) {
        echo json_encode(["status" => "error", "message" => "Verschluesselung fehlgeschlagen. OpenSSL Fehler."]);
        exit;
    }
    
    // 4. Das korrekte Speicherformat bauen
    $dbValue = base64_encode($iv) . ":" . $encryptedData;
    // ----------------------------------------------

    // Speichern in DB
    $sql = "INSERT INTO users (fingerprintHex) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    
    if ($stmt->execute([$dbValue])) {
        echo json_encode(["status" => "success", "message" => "Neuer User erfolgreich verschluesselt angelegt."]);
    } else {
        echo json_encode(["status" => "error", "message" => "Datenbankfehler beim Insert."]);
    }
} else {
    echo json_encode(["status" => "error", "message" => "Keine Daten im JSON-Body empfangen."]);
}
?>