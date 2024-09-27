<?php
// db host
$db_host = "localhost";  // geht sowohl bei lokalem als auch bei remote server -> PHP sucht die DB auch demselben Server, wo es sich selbst befindet

$db_name = "650665_4_1";
$db_user = "650665_4_1";
$db_pass = "=rTjuEQDYvC9";

// $db_name = "sensor2website";
// $db_user = "root";
// $db_pass = "";


$db_charset = "utf8";

$dsn = "mysql:host=$db_host;dbname=$db_name;charset=$db_charset";
$options = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "im";
?>