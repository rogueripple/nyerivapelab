<?php
$servername = "your-live-server-hostname"; // This might be 'localhost' or a specific IP address/domain
$username = "your_live_db_username";
$password = "your_live_db_password";
$dbname = "your_live_db_name";
$port = 3306; // Default MySQL port, may vary depending on your hosting provider

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname, $port);

// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}
echo "Connected successfully";
?>
