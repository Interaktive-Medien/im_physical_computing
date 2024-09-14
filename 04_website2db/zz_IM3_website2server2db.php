<?php
require_once("server_config.php");


# check if connection to db is successful
try{
    $pdo = new PDO($dsn, $db_user, $db_pass, $options); 
    // echo "DB Verbindung ist erfolgreich";
}
catch(PDOException $e){
    throw new PDOException($e->getMessage(), (int)$e->getCode());
    echo "Error: DB Verbindung nicht erfolgreich";
}


# insert form entries into db
if(isset($_POST['submit'])){
    $firstname = $_POST['firstname'];
    $lastname = $_POST['lastname'];
    $email = $_POST['email'];

    # insert new user into db
    $sql = "INSERT INTO User (firstname, lastname, email) VALUES (?, ?, ?)";
    $stmt = $pdo->prepare($sql);
    $stmt->execute([$firstname, $lastname, $email]);
}


# read from db
$sql = "SELECT * FROM User";
$stmt = $pdo->prepare($sql);
$stmt->execute();
$userList = $stmt->fetchAll();
// echo "<pre>";
// print_r($userList);
// echo "</pre>";

$json = json_encode($userList);
echo $json;
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

    <table>
        <thead>
            <tr>
                <th>ID</th>
                <th>First Name</th>
                <th>Last Name</th>
                <th>Email</th>
                <th>Timestamp</th>
            </tr>
        </thead>
        <tbody>
            <?php 
            foreach ($userList as $user): ?>
                <tr>
                    <td><?php echo $user['id']; ?></td>
                    <td><?php echo $user['firstname']; ?></td>
                    <td><?php echo $user['lastname']; ?></td>
                    <td><?php echo $user['email']; ?></td>
                    <td><?php echo $user['timestamp']; ?></td>
                </tr>
            <?php endforeach; ?>
        </tbody>
    </table>


   
   


    <!-- filter user by first name or last name -->
    <?php
    if (isset($_POST['searchAll'])){
        $searchString = $_POST['searchString'];

        $sql = "SELECT * FROM User WHERE firstname LIKE ? OR lastname LIKE ?";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$searchString, $searchString]);
        $searchResults = $stmt->fetchAll();
    }
    ?>




    <!-- Display search results, PHP dazu ist weiter oben -->
    <h2>Search Results</h2>

    <form method="post">
        <input type="text" name="searchString" placeholder="Search All">
        <button type="submit" name="searchAll">Search</button>
    </form>

    <ul>
        <?php foreach ($searchResults as $result): ?>
            <li><?php echo $result['firstname'] . ' ' . $result['lastname']; ?></li>
        <?php endforeach; ?>
    </ul>





    <!-- insert new user -->
    <h2>Insert new user</h2>
    
    <form method="post">
        <input type="text" name="firstname" placeholder="First Name">
        <input type="text" name="lastname" placeholder="Last Name">
        <input type="email" name="email" placeholder="Email">
        <button type="submit" name="submit">Submit</button>
    </form>





    <!-- delete a user by email address -->
    <h2>Delete User</h2>

    <form method="post">
        <input type="text" name="deleteEmail" placeholder="Email Address">
        <button type="submit" name="delete">Delete</button>
    </form>

    <?php
    if (isset($_POST['delete'])) {
        $deleteEmail = $_POST['deleteEmail'];

        $sql = "DELETE FROM User WHERE email = ?";
        $stmt = $pdo->prepare($sql);
        $stmt->execute([$deleteEmail]);
    }
    ?>
</body>
</html>