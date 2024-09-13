<?php

require_once("config.php");
echo "This script receives HTTP POST messages and pushes their content into the database.";


# connect to db
try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    echo "</br> DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    error_log("DB Error: " . $e->getMessage());
    echo json_encode(["status" => "error", "message" => "DB connection failed"]);
}






# insert raw incoming HTTP message into a database only for troubleshooting purposes

if (!empty($_POST)) {
    $msg = json_encode($_POST);
    $sql = "INSERT INTO receiveddata (msg) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([$msg]);
}

echo "</br></br> Zeig die letzten 5 empfangenen HTTP Requests";
# read from db
$sql = "SELECT * FROM receiveddata ORDER BY id DESC LIMIT 5";
$stmt = $pdo->prepare($sql);
$stmt->execute();
$receiveddata = $stmt->fetchAll();

echo "<ul>";
foreach ($receiveddata as $data) {
    echo "<li>" . $data['msg'] . "</li>";
}
echo "</ul>";
// }








# receiving a post request from the ESP32 C6

$received_api_key = $sensor = $wert = "";

if (isset($_POST["api_key"])) {

    echo "API key received: " . $_POST["api_key"];
    
    $received_api_key = test_input($_POST["api_key"]);
    if($received_api_key == $api_key_value) {
        echo "received api key matches";
        $sensor = test_input($_POST["sensor"]);
        $wert = test_input($_POST["wert"]);

        # insert new user into db
        $sql = "INSERT INTO sensordata (sensor, wert) VALUES (?, ?)";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$sensor, $wert]);


    }
}


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>

