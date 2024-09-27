<?php

########################################################## Wenn website.js dieses Script per HTTP aufruft (fetch()), bekommt es einen JSON-String im Body der Return-HTTP-Nachricht zurück.

require_once("server_config.php");
// echo "This script fetches database entries and makes them available to use for visualizing.";

try {
    $zeit = [];
    $wert = [];

    $pdo = new PDO($dsn, $db_user, $db_pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = "SELECT * FROM sensordata ORDER BY id DESC LIMIT 100"; # nur letzten 200 Daten holen

    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $wert[] = $row['wert'];
        $zeit[] = $row['zeit'];
    }

    // chart.js will zwei Arrays haben, damit eines für die Werte der Kurve und eines für die Beschriftungen auf der x-Achse verwendet werden kann
    $data = [
        'wert' => array_map('floatval', $wert),
        'zeit' => array_values($zeit)
    ];
    $json = json_encode(['data' => $data]);

    echo $json;


} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}
