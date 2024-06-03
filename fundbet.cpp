#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

// Aliases for readability
namespace beast = boost::beast;
namespace http = beast::http;
namespace net = boost::asio;
using tcp = net::ip::tcp;

// Function to serve a static HTML file
void serveFile(const std::string& filePath, tcp::socket& socket) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        http::response<http::string_body> res(http::status::not_found, 11);
        res.set(http::field::content_type, "text/plain");
        res.body() = "File not found";
        res.prepare_payload();
        http::write(socket, res);
        return;
    }

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    http::response<http::string_body> res(http::status::ok, 11);
    res.set(http::field::content_type, "text/html");
    res.body() = content;
    res.prepare_payload();
    http::write(socket, res);
}

// Function to handle POST requests
void handlePostRequest(const http::request<http::string_body>& req, tcp::socket& socket) {
    std::string requestBody = req.body();  // Read the POST request data

    // Example operation: Log the POST data
    std::ofstream logFile("post_data.log", std::ios_base::app);
    logFile << "POST data: " << requestBody << std::endl;

    // Send a response acknowledging the POST request
    http::response<http::string_body> res(http::status::ok, 11);
    res.set(http::field::content_type, "text/plain");
    res.body() = "POST data received and logged";
    res.prepare_payload();
    http::write(socket, res);
}

// Function to handle PUT requests
void handlePutRequest(const http::request<http::string_body>& req, tcp::socket& socket) {
    std::string requestBody = req.body();

    // Example operation: Update a resource with PUT data
    std::ofstream resourceFile("resource.txt", std::ios_base::out);
    resourceFile << requestBody << std::endl;

    http::response<http::string_body> res(http::status::ok, 11);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Resource updated with PUT data";
    res.prepare_payload();
    http::write(socket, res);
}

// Function to handle DELETE requests
void handleDeleteRequest(tcp::socket& socket) {
    // Example operation: Delete a resource
    std::remove("resource.txt");

    http::response<http::string_body> res(http::status::ok, 11);
    res.set(http::field::content_type, "text/plain");
    res.body() = "Resource deleted";
    res.prepare_payload();
    http::write(socket, res);
}

int main() {
    try {
        net::io_context ioc;
        tcp::acceptor acceptor(ioc, {tcp::v4(), 8080});

        std::cout << "Server is running on http://localhost:8080/" << std::endl;

        while (true) {
            tcp::socket socket(ioc);
            acceptor.accept(socket);

            beast::flat_buffer buffer;
            http::request<http::string_body> req;
            http::read(socket, buffer, req);

            switch (req.method()) {
                case http::verb::get:
                    // Serve a static HTML file
                    serveFile("/home/kamnija/Desktop/website/website.htm", socket);
                    break;
                case http::verb::post:
                    // Handle POST requests
                    handlePostRequest(req, socket);
                    break;
                case http::verb::put:
                    // Handle PUT requests
                    handlePutRequest(req, socket);
                    break;
                case http::verb::delete_:
                    // Handle DELETE requests
                    handleDeleteRequest(socket);
                    break;
                default:
                    http::response<http::string_body> res(http::status::bad_request, 11);
                    res.set(http::field::content_type, "text/plain");
                    res.body() = "Unsupported HTTP method";
                    res.prepare_payload();
                    http::write(socket, res);
                    break;
            }

            // Gracefully close the connection
            beast::error_code ec;
            socket.shutdown(tcp::socket::shutdown_send, ec);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}

