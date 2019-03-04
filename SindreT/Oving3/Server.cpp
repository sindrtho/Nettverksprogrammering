#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include "Kalkulator.cpp"

using namespace std;
using namespace boost::asio::ip;

class EchoServer {
private:
    class Connection {
    public:
        tcp::socket sock;
        Connection(boost::asio::io_service &io_service) : sock(io_service) {}
    };

    const std::string helpMenu = "add:     Lets you add 2 numbers\nsub:     lets you subtract two numbers\nhelp:    show this menu again \nexit:    Exits the program and closes connecction to the server.\n";
    const std::string errorMessage = "We have some problem understanding your inentions.\nWrite 'help' for valid commands.\n";

    Kalkulator k = Kalkulator::getInstance();

    boost::asio::io_service io_service;

    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    void handle_request(shared_ptr<Connection> conn) {
        auto read_buffer = make_shared<boost::asio::streambuf>();

        async_read_until(conn->sock, *read_buffer, "\r\n",
                [this, conn, read_buffer](const boost::system::error_code &ec, size_t) {
            if(!ec) {
                istream read_stream(read_buffer.get());
                string message;
                getline(read_stream, message);
                message.pop_back();

                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());

                cout << "Client says: " << message << endl;

                vector<string> params;
                boost::algorithm::split(params, message, boost::is_any_of(" "));

                if(message == "exit")
                    return;

                if(params[0] == "add" && params.size() >= 3) {
                    try {
                        write_stream << k.calculate(params[1], params[2], '+') <<  "\r\n";
                    } catch(exception &e) {
                        write_stream << e.what() << "\r\n";
                    }
                } else if(params[0] == "sub" && params.size() >= 3) {
                    try {
                        write_stream << k.calculate(params[1], params[2], '-') << "\r\n";
                    } catch(exception &e) {
                        write_stream << e.what() << "\r\n";
                    }
                } else if(params[0] == "help") {
                    write_stream << helpMenu << "\r\n";
                } else if(params[0] == "clear") {
                    write_stream << "";
                } else {
                    write_stream << errorMessage << "\r\n";
                }

                async_write(conn->sock, *write_buffer,
                        [this, conn, write_buffer](const boost::system::error_code &ec, size_t) {
                    if(!ec)
                        handle_request(conn);
                });
            }
        });
    }

    void accept() {
        auto conn = make_shared<Connection>(io_service);

        acceptor.async_accept(conn->sock, [this, conn](const boost::system::error_code &ec) {
            accept();
            if(!ec) {
                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());

//                write_stream << "Successfully connected to server.\r\n" << endl;
                write_stream << helpMenu << "\r\n";

                async_write(conn->sock, *write_buffer,
                            [this, conn, write_buffer](const boost::system::error_code &ec, size_t) {
                                if(!ec)
                                    handle_request(conn);
                            });
            }
//            if(!ec) {
//                handle_request(conn);
//            }
        });
    }

public:
    EchoServer() : endpoint(tcp::v4(), 80), acceptor(io_service, endpoint) {}

    void start() {
        accept();
        io_service.run();
    }
};

int main() {
    EchoServer echoServer;

    cout << "Starting server on port 80" << endl;

    echoServer.start();
    return 0;
}