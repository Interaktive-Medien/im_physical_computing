<?php
require_once("../config.php");

# receiving a post request from the ESP32 C6

$received_api_key = $sensor = $wert = "";

if (isset($_POST["api_key"])) {
    echo "POST request received";
    $received_api_key = test_input($_POST["api_key"]);
    if($received_api_key == $api_key_value) {
        echo "received api key matches";

        $sensor = test_input($_POST["sensor"]);
        $wert = test_input($_POST["wert"]);



        # connect to db
        try{
            $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
            echo "DB Verbindung ist erfolgreich";
             # insert new user into db
            $sql = "INSERT INTO sensordata (sensor, wert) VALUES (?, ?)";

            $stmt = $pdo->prepare($sql);
            $stmt->execute([$sensor, $wert]);
            echo json_encode(["status" => "success", "message" => "Data inserted successfully"]);

        }
        catch(PDOException $e){
            error_log("DB Error: " . $e->getMessage());
            echo json_encode(["status" => "error", "message" => "DB connection failed"]);
        }





        
       
    }
    else {
        echo json_encode(["status" => "error", "message" => "Wrong API Key provided."]);
    }
}
else {
    echo json_encode(["status" => "error", "message" => "No data posted with HTTP POST."]);
}


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}




?>











<!-- <!DOCTYPE html> -->
<html>
<head>
    <title>Example Sender</title>
</head>
<body>
    <form method="POST" action = "example_sender.php">
        <label for="sensor">Sensor:</label>
        <input type="text" id="sensor" name="sensor" required><br><br>
        
        <label for="wert">Wert:</label>
        <input type="text" id="wert" name="wert" required><br><br>
        
        <input type="hidden" name="api_key" value="tPmAT5Ab3j7F9"> <!-- Add the api_key field -->
        
        <input type="submit" name="submit" value="Submit">
    </form>
</body>
</html>




<!-- @todo mit fetch -->

