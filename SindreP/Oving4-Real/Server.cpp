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
    char data_[max_length];

    const std::string helpMenu = "add:     Lets you add 2 numbers\nsub:     lets you subtract two numbers\nhelp:    show this menu again \nexit:    Exits the program and closes connecction to the server.\n";
    const std::string errorMessage = "We have some problem understanding your inentions.\nWrite 'help' for valid commands.\n";

public:
    Server(boost::asio::io_service& io_service, short port)
    : io_service_(io_service),
        socket_(io_service, udp::endpoint(udp::v4(), port))
    {
        cout << "Starting up" << endl;
        socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&Server::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive_from(const boost::system::error_code& error,
                             size_t bytes_recvd)
    {
        if (!error && bytes_recvd > 0)
        {
            cout << "Recieved something: " << data_ << endl;
            string message = data_;
            vector<string> params;
            boost::algorithm::split(params, message, boost::is_any_of(" "));
            Kalkulator k = Kalkulator::getInstance();
            if(message == "exit"){
                return;
            }

            string temp;
            if(params[0] == "add" && params.size() >= 3) {
                //cout << "Got here" << endl;
                try {
                    temp = k.calculate(params[1], params[2], '+') + "";
                    strcpy(data_, temp.c_str());
                } catch(exception &e) {
                    temp = e.what();
                    strcpy(data_, temp.c_str());
                }
            } else if(params[0] == "sub" && params.size() >= 3) {
                try {
                    temp = k.calculate(params[1], params[2], '-');
                    strcpy(data_, temp.c_str());
                } catch(exception &e) {
                    temp = e.what();
                    strcpy(data_, temp.c_str());
                }
            } else if(params[0] == "help") {
                temp = helpMenu;
                strcpy(data_, temp.c_str());
            } else if(params[0] == "clear") {
                temp = "";
                strcpy(data_, temp.c_str());
            } else {
                temp = errorMessage;
                strcpy(data_, temp.c_str());
            }
            bytes_recvd = sizeof(data_);
            socket_.async_send_to(
                    boost::asio::buffer(data_, bytes_recvd), sender_endpoint_,
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
                                boost::asio::placeholders::bytes_transferred));
        }
    }

    void handle_send_to(const boost::system::error_code& /*error*/,
                        size_t /*bytes_sent*/)
    {
        cout << "Sending to!" << endl;
        socket_.async_receive_from(
                boost::asio::buffer(data_, max_length), sender_endpoint_,
                boost::bind(&Server::handle_receive_from, this,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
    }
};

int main()
{
    boost::asio::io_service io_service;
    Server s(io_service, 8080);
    io_service.run();
}