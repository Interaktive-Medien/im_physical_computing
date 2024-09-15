<?php

require_once("../02_server2db/server_config.php");
// echo "This script fetches database entries and makes them available to use for visualizing.";

try {
    $wert1 = [];
    $wert2 = [];
    $zeit = [];

    $pdo = new PDO($dsn, $db_user, $db_pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    $sql = "SELECT * FROM sensordata ORDER BY id DESC LIMIT 20"; # nur letzten 20 Daten holen

    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $wert1[] = $row['wert1'];
        $wert2[] = $row['wert2'];
        $zeit[] = $row['zeit'];
    }

    // chart.js will zwei Arrays haben, damit eines für die Werte der Kurve und eines für die Beschriftungen auf der x-Achse verwendet werden kann
    $data = [
        'wert1' => array_map('floatval', $wert1),
        'wert2' => array_map('floatval', $wert2),
        'zeit' => array_values($zeit)
    ];
    $json = json_encode(['data' => $data]);

    echo $json;


} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}
