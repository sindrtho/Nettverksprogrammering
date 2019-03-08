#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "Kalkulator.cpp"

#define IPADDRESS "10.0.2.15" // "192.168.1.64"
#define UDP_PORT_REC 8079
#define UDP_PORT 8080

using namespace std;
using boost::asio::ip::udp;
using boost::asio::ip::address;
using namespace boost::asio::ip;

//Courtesy of https://stackoverflow.com/questions/44273599/udp-communication-using-c-boost-asio

class Client {
    boost::asio::io_service io_service;
    udp::socket socket{io_service};
    boost::array<char, 1024> recv_buffer;
    udp::endpoint remote_endpoint;
public:
    Client() {}

    void handle_receive(const boost::system::error_code& error, size_t bytes_transferred) {
        if (error) {
            std::cout << "Receive failed: " << error.message() << "\n";
            return;
        }
        std::cout << "Received: '" << std::string(recv_buffer.begin(), recv_buffer.begin()+bytes_transferred) << "' (" << error.message() << ")\n";
        sendMessage();
    }

    void receive() {
        socket.async_receive_from(boost::asio::buffer(recv_buffer),
                                  remote_endpoint,
                                  boost::bind(&Client::handle_receive, this,
                                          boost::asio::placeholders::error,
                                          boost::asio::placeholders::bytes_transferred)
                                  );
    }

    void sendMessage() {
        cout << "Input your shit:" << endl;
        string msg;
        getline(cin, msg);
        if(msg == "exit")
        {
            socket.close();
            return;
        }

        remote_endpoint = udp::endpoint(address::from_string(IPADDRESS), UDP_PORT);

        boost::system::error_code err;
        auto sent = socket.send_to(boost::asio::buffer(msg), remote_endpoint, 0, err);
        std::cout << "Sent Payload --- " << sent << "\n";

        receive();
    }

    void Start()
    {
        socket.open(udp::v4());
        socket.bind(udp::endpoint(address::from_string(IPADDRESS), UDP_PORT_REC));

        sendMessage();

        std::cout << "Receiving...\n";
        io_service.run();
        std::cout << "Receiver exit\n";
    }
};

int main(int argc, char *argv[]) {
    Client client;
    client.Start();
}