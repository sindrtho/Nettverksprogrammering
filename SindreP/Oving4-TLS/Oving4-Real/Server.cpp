#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <thread>
#include <iostream>
#include <cstdlib>
#include "Kalkulator.cpp"
#include <boost/algorithm/string.hpp>

#define IPADDRESS "10.0.2.15" // "192.168.1.64"
#define UDP_PORT 8080

using namespace std;
using boost::asio::ip::udp;
using boost::asio::ip::address;

//Courtesy of https://www.boost.org/doc/libs/1_45_0/doc/html/boost_asio/example/echo/async_udp_echo_server.cpp

class Server {
    boost::asio::io_service& io_service_;
    udp::socket socket_;
    udp::endpoint sender_endpoint_;
    enum { max_length = 1024 };
    //char data_[max_length];

    const std::string helpMenu = "add:     Lets you add 2 numbers\nsub:     lets you subtract two numbers\nhelp:    show this menu again \nexit:    Exits the program and closes connecction to the server.\n";
    const std::string errorMessage = "We have some problem understanding your inentions.\nWrite 'help' for valid commands.\n";

public:
    Server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
        socket_(io_service, udp::endpoint(udp::v4(), port))
    {
        cout << "Starting up" << endl;
        char data_[max_length];
        socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&Server::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred, data_));
    }

    void handle_receive_from(const boost::system::error_code& error,
                             size_t bytes_recvd, char newData[])
    {
        char* data_ = newData;
        if (!error && bytes_recvd > 0)
        {
            string message = data_;
            cout << "Recieved something: " << message << endl;
            vector<string> params;
            boost::algorithm::split(params, message, boost::is_any_of(" "));
            Kalkulator k = Kalkulator::getInstance();
            if(message == "exit"){
                return;
            }

            auto write_buffer = make_shared<boost::asio::streambuf>();
            ostream write_stream(write_buffer.get());
            string temp;
            if(params[0] == "add" && params.size() >= 3) {
                try {
                    write_stream << k.calculate(params[1], params[2], '+');
                } catch(exception &e) {
                    write_stream << e.what();
                }
            } else if(params[0] == "sub" && params.size() >= 3) {
                try {
                    write_stream << k.calculate(params[1], params[2], '-');
                } catch(exception &e) {
                    write_stream << e.what();
                }
            } else if(params[0] == "help") {
                write_stream << helpMenu;
            } else if(params[0] == "clear") {
                write_stream << "";
            } else {
                write_stream << errorMessage;
            }

            socket_.async_send_to(
                    write_buffer->data(), sender_endpoint_,
                    boost::bind(&Server::handle_send_to, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred));
        }
        else
        {
            socket_.async_receive_from(
                    boost::asio::buffer(data_, max_length), sender_endpoint_,
                    boost::bind(&Server::handle_receive_from, this,
                                boost::asio::placeholders::error,
                                boost::asio::placeholders::bytes_transferred, data_));
        }
    }

    void handle_send_to(const boost::system::error_code& /*error*/,
                        size_t /*bytes_sent*/)
    {
        cout << "Sending to!" << endl;
        char data_[max_length];
        socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&Server::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred, data_));
    }
};

int main()
{
    boost::asio::io_service io_service;
    Server s(io_service, UDP_PORT);
    io_service.run();
}