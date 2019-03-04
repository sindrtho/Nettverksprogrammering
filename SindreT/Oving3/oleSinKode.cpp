#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace std;
using namespace boost::asio::ip;

class EchoServer {
private:
    boost::asio::io_service io_service;

    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    void handle_request(const shared_ptr<tcp::socket> &socket) {
        // read buffer with automatic memory management through reference counting
        auto read_buffer = make_shared<boost::asio::streambuf>();
        // Read from client until newline ("\r\n")
        async_read_until(*socket, *read_buffer, "\r\n", [this, socket, read_buffer](const boost::system::error_code &ec, size_t) {
            // If not error:
            if (!ec) {
                // Retrieve message from client as string:
                string message;
                // Create stream to read from buffer
                istream read_stream(read_buffer.get());
                getline(read_stream, message);
                message.pop_back(); // Remove '\r' at end of string

                cout << "server: received message from a client: " << message << endl;

                // Close socket when "exit" is retrieved from client
                if (message == "exit") {
                    cout << "server: closing connection" << endl;
                    return;
                }

                // Write buffer with automatic memory management through reference counting
                auto write_buffer = make_shared<boost::asio::streambuf>();
                // Create stream to write to buffer
                ostream write_stream(write_buffer.get());

                // Add message to be written to client:
                write_stream << message << "\r\n";

                // Write to client
                async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
                    // If not error:
                    if (!ec) {
                        cout << "server: reply sent to client" << endl;
                        handle_request(socket);
                    }
                });
            }
        });
    }

    void accept() {
        // The (client) socket (connection) is added to the lambda and handle_request
        // in order to keep the object alive for as long as it is needed.
        // make_shared creates an object with automatic memory management through reference counting
        auto socket = make_shared<tcp::socket>(io_service);

        // Accepts a new (client) connection. On connection, immediately start accepting a new connection
        acceptor.async_accept(*socket, [this, socket](const boost::system::error_code &ec) {
            accept();
            // If not error:
            if (!ec) {
                cout << "server: new client connected" << endl;
                handle_request(socket);
            }
        });
    }

public:
    EchoServer() : endpoint(tcp::v4(), 8080), acceptor(io_service, endpoint) {}

    void start() {
        accept();

        // Start asynchronous io service (event-loop)
        io_service.run();
    }
};

class EchoClient {
    boost::asio::io_service io_service;
    tcp::resolver resolver;

public:
    EchoClient(const std::string &host, unsigned short port) : resolver(io_service) {
        // Create query from host and port
        auto query = tcp::resolver::query(host, to_string(port));
        // Resolve query (DNS-lookup if needed)
        resolver.async_resolve(query, [this](const boost::system::error_code &ec,
                                             boost::asio::ip::tcp::resolver::iterator it) {
            // If not error:
            if (!ec) {
                cout << "client: query resolved" << endl;

                // The socket (connection) is added to the lambda in order to keep the object alive for as long as it is needed.
                // make_shared creates an object with automatic memory management through reference counting
                auto socket = make_shared<tcp::socket>(io_service);
                boost::asio::async_connect(*socket, it, [this, socket](const boost::system::error_code &ec,
                                                                       boost::asio::ip::tcp::resolver::iterator /*it*/) {
                    // If not error:
                    if (!ec) {
                        cout << "client: connected to server" << endl;
                        // write buffer with automatic memory management through reference counting
                        auto write_buffer = make_shared<boost::asio::streambuf>();
                        ostream write_stream(write_buffer.get());

                        // Add message to be written to client:
                        write_stream << "hello\r\n";

                        // Write to client
                        async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
                            // If not error:
                            if (!ec) {
                                cout << "client: message \"hello\" sent to server" << endl;
                                // read buffer with automatic memory management through reference counting
                                auto read_buffer = make_shared<boost::asio::streambuf>();
                                // Read from client until newline ("\r\n")
                                async_read_until(*socket, *read_buffer, "\r\n", [this, socket, read_buffer](const boost::system::error_code &ec, size_t) {
                                    // If not error:
                                    if (!ec) {
                                        // Retrieve message from client as string:
                                        string message;
                                        // Create stream to read from buffer
                                        istream read_stream(read_buffer.get());
                                        getline(read_stream, message);
                                        message.pop_back(); // Remove '\r' at end of string

                                        cout << "client: received message from server: " << message << endl
                                             << "client: closing connection" << endl;
                                        // Connection is closed when all copies of the socket shared pointer is destroyed
                                    }
                                });
                            }
                        });
                    }
                });
            }
        });

        // Start asynchronous io service (event-loop)
        io_service.run();
    }
};

int main() {
    EchoServer echo_server;

    cout << "Starting echo server" << endl
         << "Connect in a terminal with: telnet localhost 8080. Type 'exit' to end connection." << endl;

    thread client_thread([] {
        this_thread::sleep_for(1s);
        cout << "Connecting and communicating with echo server using echo client:" << endl;
        EchoClient echo_client("localhost", 8080);
    });
    client_thread.detach();

    echo_server.start();
}