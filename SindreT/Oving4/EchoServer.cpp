#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include "Kalkulator.cpp"

using namespace std;
using namespace boost::asio::ip;

class EchoServer {
private:
    boost::asio::io_service io_service;

    udp::socket sock;

    void accept() {
        auto endpoint = make_shared<udp::endpoint>();

        auto read_buffer = make_shared<boost::asio::streambuf>();

        sock.async_receive_from(read_buffer->prepare(65536), *endpoint, [this, endpoint, read_buffer](const boost::system::error_code &ec, unsigned long bytes_transferred) {
            accept();

            if(!ec) {
                read_buffer->commit(bytes_transferred);
                string message;
                stringstream ss;
                ss << read_buffer.get();
                message = ss.str();

                cout << message << endl;

                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());
                write_stream << message;

                sock.async_send_to(write_buffer->data(), *endpoint, [endpoint, write_buffer](const boost::system::error_code &ec, unsigned long) {
                    if(!ec) {
                        cout << "server: message sent to client" << endl;
                    }
                });
            }
        });
    }
public:
    EchoServer() : sock(io_service, udp::endpoint(udp::v4(), 80)) {}

    void start() {
        accept();
        io_service.run();
    }
};

int main() {
    EchoServer server;

    server.start();
}