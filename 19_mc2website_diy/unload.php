<?php

/***************************************************************************************************************************
 * unload.php 
 * https://fiessling.ch/iot1/06_sensor2website_diy/unload.php
 * Daten aus der Datenbank holen und als JSON-String zurückgeben, sobald website.js dieses Script per HTTP aufruft (fetch())
***************************************************************************************************************************/



########################################################## Wenn website.js dieses Script per HTTP aufruft (fetch()), bekommt es einen JSON-String im Body der Return-HTTP-Nachricht zurück.
header('Content-Type: application/json');
require_once("server_config.php");
// echo "This script fetches database entries and makes them available to use for visualizing.";

try {
    $pdo = new PDO($dsn, $db_user, $db_pass);
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);



    // $sql = "SELECT HOUR(zeit) AS stunde, SUM(CAST(wert AS DECIMAL(10, 2))) AS summe_pro_stunde
    //     FROM sensordata
    //     GROUP BY HOUR(zeit)";


    // $stmt = $pdo->prepare($sql);
    // $stmt->execute();

    // $result = $stmt->fetchAll(PDO::FETCH_ASSOC);
    // echo json_encode(['data' => $result]);






    // Hole die Daten aus der Datenbank
    $sql = "SELECT HOUR(zeit) AS stunde, SUM(CAST(wert AS DECIMAL(10, 2))) AS summe_pro_stunde
    FROM sensordata
    GROUP BY HOUR(zeit)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute();

    $data = [];
    while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
        $data[$row['stunde']] = floatval($row['summe_pro_stunde']);
    }

    // Fülle fehlende Stunden mit 0
    $stundenData = [];
    for ($i = 0; $i < 24; $i++) {
        $stundenData[] = [
            'stunde' => $i,
            'summe_pro_stunde' => isset($data[$i]) ? $data[$i] : 0
        ];
    }

    $summeProStundeArray = array_column($stundenData, 'summe_pro_stunde');
    echo json_encode(['data' => $summeProStundeArray]);






} catch (PDOException $e) {
    echo "Error: " . $e->getMessage();
}

?>