<?php
/*************************************************************
 * server_config.php
 * Datenbank-Verbindung
 * https://fiessling.ch/iot1/06_sensor2website/server_config.php
 *************************************************************/

// Datenbankverbindungsparameter
$db_host = "rv9w2f.myd.infomaniak.com";  // beim FHGR Edu-Server und xampp steht hier "localhost"
$db_name = "rv9w2f_fiessling";  //  Edu-Server: "650665_4_1", xampp: "sensor2website"
$db_user = "rv9w2f_fiessling";  //  Edu-Server: "650665_4_1", xampp: "root"
$db_pass = "Strenggehe1m";      //  Edu-Server: "=rTjuEQDYvC9", xampp: ""

$db_charset = "utf8";

$dsn = "mysql:host=$db_host;dbname=$db_name;charset=$db_charset";
$options = [
    PDO::ATTR_ERRMODE            => PDO::ERRMODE_EXCEPTION,
    PDO::ATTR_DEFAULT_FETCH_MODE => PDO::FETCH_ASSOC,
    PDO::ATTR_EMULATE_PREPARES   => false,
];
?>

