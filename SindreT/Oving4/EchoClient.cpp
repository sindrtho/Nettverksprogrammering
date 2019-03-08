#include <boost/asio.hpp>
#include <iostream>

using namespace boost::asio::ip;
using namespace std;

class EchoClient {
    boost::asio::io_service io_service;
    udp::resolver resolver;
    udp::socket sock;

public:
    EchoClient(const std::string &host, unsigned short port) : resolver(io_service), sock(io_service, udp::v4()) {
        auto query = udp::resolver::query(udp::v4(), host, to_string(port));

        resolver.async_resolve(query, [this](const boost::system::error_code &ec, udp::resolver::iterator it) {
            if(!ec) {
                cout << "Client: query resolved" << endl;

                auto endpoint = make_shared<udp::endpoint>(*it);

                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());
                write_stream << "test";

                sock.async_send_to(write_buffer->data(), *endpoint, [this, endpoint, write_buffer](const boost::system::error_code &ec, unsigned long) {
                    if(!ec) {
                        cout << "client: message sent to server" << endl;
                        auto read_buffer = make_shared<boost::asio::streambuf>();

                        sock.async_receive_from(read_buffer->prepare(65536), *endpoint, [endpoint, read_buffer](const boost::system::error_code &ec, unsigned long bytes_transferred) {
                            if(!ec) {
                                read_buffer->commit(bytes_transferred);
                                string message;
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
        io_service.run();
    }
};

int main() {
    EchoClient client("localhost", 80);
}