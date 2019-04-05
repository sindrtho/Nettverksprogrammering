#include <iostream>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <thread>
#include <regex>
#include <gcrypt.h>
#include <stdio.h>
#include <boost/uuid/sha1.hpp>

using namespace std;
using namespace boost::asio::ip;

// Code by René Nyffenegger rene.nyffenegger@adp-gmbh.ch
static const std::string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";


static inline bool is_base64(unsigned char c) {
    return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;

}

// Code by René Nyffenegger rene.nyffenegger@adp-gmbh.ch end


/**
 * Method for hashing and base64 encoding key.
 */
static string hashClientKey(string *clientKey) {
    string divinity = *clientKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";

//    cout << "Divine string: " << divinity << endl;
//    cout << "Client key: " << *clientKey << endl;
    gcry_md_hd_t x;
    gcry_md_open(&x, GCRY_MD_SHA1, GCRY_MD_FLAG_SECURE);
    gcry_md_write(x, divinity.c_str(), divinity.size());

    unsigned char *hashRes = gcry_md_read(x, GCRY_MD_SHA1);
    unsigned int l = gcry_md_get_algo_dlen(GCRY_MD_SHA1);

    string res = base64_encode(hashRes, l);

    return res;
}

class EchoServer {
private:
    class Connection {
    public:
        tcp::socket sock;
        Connection(boost::asio::io_service &io_service) : sock(io_service) {}
    };


    boost::asio::io_service io_service;

    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    void handle_request(shared_ptr<Connection> conn) {
        auto read_buffer = make_shared<boost::asio::streambuf>();

        async_read_until(conn->sock, *read_buffer, "\r\n",
                [this, conn, read_buffer](const boost::system::error_code &ec, size_t) {
            if(!ec) {
                istream read_stream(read_buffer.get());
                string clientKey;

                while(getline(read_stream, clientKey)) {
                    if (regex_search(clientKey, regex("Sec-WebSocket-Key:"))) {
                        vector<string> myVect;
                        boost::algorithm::split(myVect, clientKey, boost::is_any_of(" "));
                        clientKey = myVect[1];
                        break;
                    }
                }

                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());

                boost::algorithm::trim(clientKey);

                // Building response for client.
                string protocol_switch = "HTTP/1.1 101 Switching Protocols\r\n";
                string upgrade = "Upgrade: websocket\r\n";
                string connection = "Connection: Upgrade\r\n";
                string sec_websocket_accept = "Sec-WebSocket-Accept: " + hashClientKey(&clientKey) + "\r\n\r\n";

                cout << hashClientKey(&clientKey) << endl;

                write_stream << protocol_switch << upgrade << connection << sec_websocket_accept;

                async_write(conn->sock, *write_buffer,
                        [this, conn, write_buffer](const boost::system::error_code &ec, size_t) {
                    if(!ec) {
                        sendMessage(conn);
                    }
                });
            }
        });
    }

    void sendMessage(shared_ptr<Connection> conn) {
        auto write_buffer2 = make_shared<boost::asio::streambuf>();
        ostream write_stream2(write_buffer2.get());

        cout << "Welcome!" << endl;

        auto FIN = 0b10000001;
        string message = "Hello client!\nHandshake was a SUCCESS!";

        write_stream2 << (unsigned char)FIN;
        write_stream2 << (unsigned char)message.size() << message;

        async_write(conn->sock, *write_buffer2,
                    [this, conn, write_buffer2](const boost::system::error_code &ec, size_t) {

                    });
    }

    void accept() {
        auto conn = make_shared<Connection>(io_service);

        acceptor.async_accept(conn->sock, [this, conn](const boost::system::error_code &ec) {
            accept();
            if(!ec) {
                auto write_buffer = make_shared<boost::asio::streambuf>();
                ostream write_stream(write_buffer.get());

                async_write(conn->sock, *write_buffer,
                            [this, conn, write_buffer](const boost::system::error_code &ec, size_t) {
                                if(!ec)
                                    handle_request(conn);
                            });
            }
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
