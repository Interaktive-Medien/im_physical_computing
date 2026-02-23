<?php

/***********************************************************************
 * config.php
 * Database connection
 * Replce $host, $name, $user, $pass by your own database credentials. 
 * Do not upload this file on GitHub GitHub
 ***********************************************************************/


$host = "zimalofo.mysql.db.internal";  // "rv9w2f.myd.infomaniak.com";
$db   = "zimalofo_heulradar";  // "rv9w2f_webapp";  // Change to your DB name
$user = "zimalofo_papi"; // "rv9w2f_jan";   // Change to your DB user
$pass = "Heulradar!!!!!";       // Change to your DB pass if needed

try {
    $dsn = "mysql:host=$host;dbname=$db;charset=utf8mb4";
    $pdo = new PDO($dsn, $user, $pass);
    // Optional: Set error mode
    $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
} catch (Exception $e) {
    echo "Database connection error: " . $e->getMessage();
    exit;
}
?>






