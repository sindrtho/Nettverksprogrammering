#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include "Kalkulator.cpp"

using namespace boost::asio::ip;
using namespace std;

class EchoClient {
    boost::asio::io_service io_service;
    tcp::resolver resolver;

public:
    EchoClient() : resolver(io_service) {}

    /**
     * Initializes the client program.
     * @param host Specifies the host to connect to.
     * @param port Specifies the port to connect to the host through.
     */
    void init(const std::string &host, unsigned short port) {
        auto query = tcp::resolver::query(host, to_string(port));

        resolver.async_resolve(query, [this](const boost::system::error_code &ec, tcp::resolver::iterator it) {
            if(!ec) {
                auto socket = make_shared<tcp::socket>(io_service);
                boost::asio::async_connect(*socket, it, [this, socket](const boost::system::error_code &ec, tcp::resolver::iterator it) {
                    if(!ec) {
                        std::cout << "Connected" << std::endl;
                        //sendMessage(socket);

                        auto write_buffer = make_shared<boost::asio::streambuf>();
                        ostream write_stream(write_buffer.get());

                        cout << "Hello client!" << endl;
                        auto read_buffer = make_shared<boost::asio::streambuf>();
                        async_read_until(*socket, *read_buffer, "\r\n", [this, socket, read_buffer](
                                const boost::system::error_code &ec, size_t) {
                            if (!ec) {
                                cout << "From server: ";
                                std::string message;
                                istream read_stream(read_buffer.get());
                                while(getline(read_stream, message)) {
                                    message.pop_back();

                                    cout << message << std::endl;
                                }
                                sendMessage(socket);
                            }
                        });
                    }
                });
            }
        });
    }

    /**
     * Sends a messaege to the connected host.
     * @param socket
     */
    void sendMessage(const shared_ptr<tcp::socket> socket) {
        auto write_buffer = make_shared<boost::asio::streambuf>();
        ostream write_stream(write_buffer.get());

        string msg;
        getline(cin, msg);
        write_stream << msg << "\r\n";

        async_write(*socket, *write_buffer, [this, socket, write_buffer](const boost::system::error_code &ec, size_t) {
            if (!ec) {
                auto read_buffer = make_shared<boost::asio::streambuf>();
                async_read_until(*socket, *read_buffer, "\r\n", [this, socket, read_buffer](
                        const boost::system::error_code &ec, size_t) {
                    if (!ec) {
                        cout << "From server: ";
                        std::string message;
                        istream read_stream(read_buffer.get());
                        while(getline(read_stream, message)) {
                            message.pop_back();

                            cout << message << std::endl;
                        }
                        sendMessage(socket);
                    }
                    //sendMessage(socket);
                });
            }
        });
    }

    /**
     * Starts the server.
     * @param host
     * @param port
     */
    void start(const std::string &host, unsigned short port) {
        init(host, port);
        io_service.run();
    }
};

int main() {
    EchoClient client;
    client.start("10.22.157.5", 80);
}