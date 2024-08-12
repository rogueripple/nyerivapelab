<?php
// Database connection parameters
$servername = "localhost";
$username = "your_db_username";
$password = "your_db_password";
$dbname = "user_data";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

// Process form data
if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $phone_number = $_POST['phone_number'];
    $password = $_POST['password'];

    // Hash the password
    $hashed_password = password_hash($password, PASSWORD_DEFAULT);

    // Insert data into the database
    $stmt = $conn->prepare("INSERT INTO users (phone_number, password) VALUES (?, ?)");
    $stmt->bind_param("ss", $phone_number, $hashed_password);

    if ($stmt->execute()) {
        echo "New record created successfully";
    } else {
        echo "Error: " . $stmt->error;
    }

    // Close the statement and connection
    $stmt->close();
    $conn->close();
}
?>
