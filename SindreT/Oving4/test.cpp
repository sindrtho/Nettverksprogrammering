#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using namespace std;
using namespace boost::asio::ip;

class EchoServer {
private:
    boost::asio::io_service io_service;

    udp::socket socket;

    void accept() {
        // socket.async_receive_from stores the client endpoint to this variable, this endpoint is then reused in socket.async_send_to
        // make_shared creates an object with automatic memory management through reference counting
        auto endpoint = make_shared<udp::endpoint>();

        // read buffer with automatic memory management through reference counting
        auto read_buffer = make_shared<boost::asio::streambuf>();

        // 65536 is max bytes to receive
        // endpoint and read_buffer is captured in the lambda to ensure that these objects are kept alive
        socket.async_receive_from(read_buffer->prepare(65536), *endpoint, [this, endpoint, read_buffer](const boost::system::error_code &ec, unsigned long bytes_transferred) {
            // Immediately start receiving another request
            accept();

            // If not error:
            if (!ec) {
                // Make bytes transferred readable in the read_buffer
                read_buffer->commit(bytes_transferred);
                // Retrieve message from client as string :
                string message;
                // Create stream to read from buffer
                stringstream ss;
                ss << read_buffer.get();
                message = ss.str();

                cout << "server: message received from client: " << message << endl;

                //write buffer with automatic memory management through reference counting
                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());
                write_stream << message;

                // endpoint and write_buffer is captured in the lambda to ensure that these objects are kept alive
                socket.async_send_to(write_buffer->data(), *endpoint, [endpoint, write_buffer](const boost::system::error_code &ec, unsigned long) {
                    // If not error:
                    if (!ec) {
                        cout << "server: message sent to client" << endl;
                    }
                });
            }
        });
    }

public:
    EchoServer() : socket(io_service, udp::endpoint(udp::v4(), 8080)) {}

    void start() {
        accept();

        // Start asynchronous io service (event-loop)
        io_service.run();
    }
};

class EchoClient {
    boost::asio::io_service io_service;
    udp::resolver resolver;
    udp::socket socket;

public:
    EchoClient(const std::string &host, unsigned short port) : resolver(io_service), socket(io_service, udp::v4()) {
        // Create query from host and port
        auto query = udp::resolver::query(udp::v4(), host, to_string(port));
        // Resolve query (DNS-lookup if needed)
        resolver.async_resolve(query, [this](const boost::system::error_code &ec,
                                             udp::resolver::iterator it) {
            // If not error:
            if (!ec) {
                cout << "client: query resolved" << endl;

                // server endpoint found during async_resolve is stored here
                // make_shared creates an object with automatic memory management through reference counting
                auto endpoint = make_shared<udp::endpoint>(*it);

                //read buffer with automatic memory management through reference counting
                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());
                write_stream << "test";

                // endpoint and write_buffer is captured in the lambda to ensure that these objects are kept alive
                socket.async_send_to(write_buffer->data(), *endpoint, [this, endpoint, write_buffer](const boost::system::error_code &ec, unsigned long) {
                    // If not error:
                    if (!ec) {
                        cout << "client: message sent to server" << endl;

                        // read buffer with automatic memory management through reference counting
                        auto read_buffer = make_shared<boost::asio::streambuf>();

                        // 65536 is max bytes to receive
                        // endpoint and read_buffer is captured in the lambda to ensure that these objects are kept alive
                        socket.async_receive_from(read_buffer->prepare(65536), *endpoint, [endpoint, read_buffer](const boost::system::error_code &ec, unsigned long bytes_transferred) {
                            // If not error:
                            if (!ec) {
                                // Make bytes transferred readable in the read_buffer
                                read_buffer->commit(bytes_transferred);

                                // Retrieve message from client as string :
                                string message;
                                // Create stream to read from buffer
                                stringstream ss;
                                ss << read_buffer.get();
                                message = ss.str();

                                cout << "client: message received from server: " << message << endl;
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

    cout << "Starting echo server" << endl;

    thread client_thread([] {
        this_thread::sleep_for(1s);
        cout << "Communicating with echo server using echo client:" << endl;
        EchoClient echo_client("localhost", 8080);
    });
    client_thread.detach();

    echo_server.start();
}
