<?php

 /*************************************************************
 * Kap. 14 MC -> Dataviz 
 * unload.php
 **************************************************************/

header('Content-Type: application/json');

// Datenbankverbindung einbinden
require_once("db_config.php");




###################################### connect to db
try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    // echo "DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    error_log("DB Error: " . $e->getMessage());
    echo json_encode(["status" => "error", "message" => "DB connection failed"]);
}



try {
    $result = array_fill(0, 7, 0);      // Initialisiere Array mit 7 Nullen

    // SQL: Zählt die Anzahl der Einträge je Tag innerhalb der letzten 7 Tage   
    $sql = "SELECT 
            DATE(zeit) AS datum,
            COUNT(*) AS anzahl
        FROM sensordata
        WHERE zeit >= CURDATE() - INTERVAL 6 DAY
        GROUP BY datum";

    $stmt = $pdo->prepare($sql);    
    $stmt->execute();
    $rows = $stmt->fetchAll(PDO::FETCH_ASSOC);
    // echo "<br>" . json_encode($rows) . "<br>"; // Debugging: Zeige die Rohdaten an, z. B. [{"datum":"2025-04-18","anzahl":22},{"datum":"2025-04-21","anzahl":140},{"datum":"2025-04-24","anzahl":1}]

    // Anzahl der Einträge pro Tag werden in ein Array geschrieben, Reihenfolge absteigend, 
    // an Tagen ohne Eintrag erscheint jeweils eine 0. Beispiel: [1,0,0,140,0,0,22]

    foreach ($rows as $row) {
        $tageZurueck = (new DateTime())->diff(new DateTime($row['datum']))->days;
        if ($tageZurueck < 7) {
            $result[$tageZurueck] = (int)$row['anzahl'];
        }
    }
    // echo "<br>" . json_encode($result) . "<br>";  //[1,0,0,140,0,0,22]

    // JSON ausgeben, zB. {"count_opens":[1,0,0,140,0,0,22]}
    echo json_encode(["count_opens" => $result]);

} catch (PDOException $e) {
    // Im Fehlerfall eine leere Antwort mit Fehlerlog
    http_response_code(500);
    echo json_encode(["error" => "Database error", "details" => $e->getMessage()]);
}
