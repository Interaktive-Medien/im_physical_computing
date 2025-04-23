<?php

 /*************************************************************
 * Kapitel 15: Demo-Projekt Fridge Box
 * Schritt 8 ("Landing Page")
 * unload.php: 
 * Daten aus der Datenbank per SQL abfragen, sortieren
 * und ein entsprechendes Array als JSON-String per echo bereitstellen, 
 * das der client (script.js) mit fetch() abholen kann.
 * Datenbank-Verbindung:
 * Ersetze $db_host, $db_name, $db_user, $db_pass durch deine eigenen Daten. 
 * GitHub: 
 **************************************************************/


require_once('db_config.php');

###################################### connect to db

try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    // echo "DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    error_log("DB Error: " . $e->getMessage());
    echo json_encode(["status" => "error", "message" => "DB connection failed"]);
}

###################################### Datenbankabfrage: Anzahl Öffnungen pro Tag

function count_opens($pdo){
    // SQL-Statement: Anzahl Einträge für die letzten 7 Tage (inkl. heute)
    $sql = 
        "SELECT DATE(zeit) AS datum,COUNT(*) AS anzahl
        FROM sensordata
        WHERE zeit >= DATE_SUB(CURDATE(), INTERVAL 6 DAY)
        GROUP BY DATE(zeit)";
 
    $stmt = $pdo->prepare($sql);
    $stmt->execute();
    $rows = $stmt->fetchAll(PDO::FETCH_ASSOC); // [['datum' => '2025-04-23', 'anzahl' => 12], ...]
    $result = array_fill(0, 7, 0);   // Array anlegen: array_fill(start_index, count, value):

    foreach ($rows as $row) {
        $tageZurueck = (new DateTime())->diff(new DateTime($row['datum']))->days;
        if ($tageZurueck < 7) {
            $result[$tageZurueck] = (int)$row['anzahl'];
        }
    }
    return $result; // [0,0,140,0,20,0,0] // Beispiel-Array
}


###################################### Datenbankabfrage: Objekt zusammenbauen, in JSON konvertieren und per echo verfügbar machen.

try {
    $dataObject = new stdClass();
    $dataObject->count_opens = count_opens($pdo);

    // JSON zurückgeben
    header('Content-Type: application/json');
    echo json_encode($dataObject);  // {"count_opens":[0,0,140,0,0,22,0]}

} catch (Exception $e) {
    http_response_code(500);
    echo json_encode(['error' => $e->getMessage()]);
}
