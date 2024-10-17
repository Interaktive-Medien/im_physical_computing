<?php
/***********************************************************************************
 * zz_IM3_website2server2db.php
 * Formular und PHP-Serverscript in Einem -> Formulardaten werden in DB geschrieben
 * https://fiessling.ch/04_website2db/zz_IM3_website2server2db.php
 ***********************************************************************************/

require_once("server_config.php");


###################################### connect to db

try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    echo "DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    throw new PDOException($e->getMessage(), (int)$e->getCode());
    echo "Error: DB Verbindung nicht erfolgreich";
}



###################################### Suche nach Wert in der DB

if (isset($_POST['searchAll'])){
    $searchString = $_POST['searchString'];

    $sql = "SELECT * FROM sensordata WHERE wert LIKE ?";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([$searchString]);
    $searchResults = $stmt->fetchAll();
}



###################################### Füge neuen Wert in die DB ein

if(isset($_POST['insert'])){
    $wert = $_POST['wert'];
    $sql = "INSERT INTO sensordata (wert) VALUES (?)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([$wert]);
}


###################################### delete from db

if (isset($_POST['delete'])) {
    $zu_loeschender_wert = $_POST['zu_loeschender_wert'];

    $sql = "DELETE FROM sensordata WHERE wert = ?";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([$zu_loeschender_wert]);
}


###################################### read from db

$sql = "SELECT * FROM sensordata";
$stmt = $pdo->prepare($sql);
$stmt->execute();
$tabelle = $stmt->fetchAll();

$json = json_encode($tabelle);
echo "<h2> JSON-String </h1>";
echo $json;
echo ""; // line break
?>




<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>sensor2website</title>
</head>
<body>


<!-- display db entries as a  table -->
    <h2>Tabeleneinträge</h2>
    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>Wert</th>
                <th>Timestamp</th>
            </tr>
        </thead>
        <tbody>
            <?php 
            foreach ($tabelle as $tabelleneintrag): ?>
                <tr>
                    <td><?php echo $tabelleneintrag['id']; ?></td>
                    <td><?php echo $tabelleneintrag['wert']; ?></td>
                    <td><?php echo $tabelleneintrag['zeit']; ?></td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>


   


    <!-- Zeige DB Einträge als Liste an, PHP dazu ist weiter oben -->

    <h2>Suche Wert</h2>

    <form method="post">
        <input type="text" name="searchString" placeholder="wert">
        <button type="submit" name="searchAll">Search</button>
    </form>

    <ul>
        <?php foreach ($searchResults as $result): ?>
            <li><?php echo $result['wert']; ?></li>
        <?php endforeach; ?>
    </ul>



    <!-- Neuen Wert einfügen -->

    <h2>Füge neuen Wert ein</h2>
    
    <form method="post">
        <input type="text" name="wert" placeholder="wert">
        <button type="submit" name="insert">Submit</button>
    </form>



    <!-- Lösche DB Eintrag anhand des Wertes-->

    <h2>Lösche Wert</h2>

    <form method="post">
        <input type="text" name="zu_loeschender_wert" placeholder="Wert">
        <button type="submit" name="delete">Delete</button>
    </form>

</body>
</html>