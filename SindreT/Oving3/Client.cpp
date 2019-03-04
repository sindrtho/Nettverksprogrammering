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
    EchoClient(const std::string &host, unsigned short port) : resolver(io_service) {
        auto query = tcp::resolver::query(host, to_string(port));

        resolver.async_resolve(query, [this](const boost::system::error_code &ec,
                tcp::resolver::iterator it) {
            if(!ec) {
                auto socket = make_shared<tcp::socket>(io_service);
                boost::asio::async_connect(*socket, it, [this, socket](const boost::system::error_code &ec,
                        tcp::resolver::iterator it) {
                    if(!ec) {
                        std::cout << "Connected" << std::endl;

                        auto write_buffer = make_shared<boost::asio::streambuf>();
                        ostream write_stream(write_buffer.get());

                        write_stream << "Hello server!\r\n";
                        cout << "Hello client!" << endl;
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
                                 });
                            }
                        });
                    }
                });
            }
        });
        io_service.run();
    }

    void sendMessage(const shared_ptr<tcp::socket> socket) {
        auto write_buffer = make_shared<boost::asio::streambuf>();
        ostream write_stream(write_buffer.get());

        string msg;
        cin >> msg;
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
                    }
                });
                sendMessage(socket);
            }
        });
//        io_service.run();
    }
};

int main() {
    EchoClient client("localhost", 80);
}