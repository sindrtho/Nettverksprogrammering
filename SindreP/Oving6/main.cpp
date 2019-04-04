#include <boost/asio.hpp>
#include <iostream>
#include <list>
#include <iterator>
#include <boost/algorithm/string_regex.hpp>
#include <boost/algorithm/string.hpp>
#include <regex>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <cstring>
#include <memory>
#include <string>
#include <vector>
#include <openssl/hmac.h>
#include <openssl/buffer.h>
#include <cassert>
#include <limits>
#include <stdexcept>
#include <cctype>
#include <chrono>
#include <thread>

using namespace std;
using namespace boost::asio::ip;

/*
namespace {
    struct BIOFreeAll { void operator()(BIO* p) { BIO_free_all(p); } };
}
std::string Base64Encode(const std::vector<unsigned char>& binary)
{
    std::unique_ptr<BIO,BIOFreeAll> b64(BIO_new(BIO_f_base64()));
    BIO_set_flags(b64.get(), BIO_FLAGS_BASE64_NO_NL);
    BIO* sink = BIO_new(BIO_s_mem());
    BIO_push(b64.get(), sink);
    BIO_write(b64.get(), binary.data(), binary.size());
    BIO_flush(b64.get());
    const char* encoded;
    const long len = BIO_get_mem_data(sink, &encoded);
    return std::string(encoded, len);
}*/

static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char reverse_table[128] = {
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
        64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
        64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
        64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
};

::std::string base64_encode(const ::std::string &bindata)
{
    using ::std::string;
    using ::std::numeric_limits;

    if (bindata.size() > (numeric_limits<string::size_type>::max() / 4u) * 3u) {
        throw ::std::length_error("Converting too large a string to base64.");
    }

    const ::std::size_t binlen = bindata.size();
    // Use = signs so the end is properly padded.
    string retval((((binlen + 2) / 3) * 4), '=');
    ::std::size_t outpos = 0;
    int bits_collected = 0;
    unsigned int accumulator = 0;
    const string::const_iterator binend = bindata.end();

    for (string::const_iterator i = bindata.begin(); i != binend; ++i) {
        accumulator = (accumulator << 8) | (*i & 0xffu);
        bits_collected += 8;
        while (bits_collected >= 6) {
            bits_collected -= 6;
            retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
        }
    }
    if (bits_collected > 0) { // Any trailing bits that are missing.
        assert(bits_collected < 6);
        accumulator <<= 6 - bits_collected;
        retval[outpos++] = b64_table[accumulator & 0x3fu];
    }
    assert(outpos >= (retval.size() - 2));
    assert(outpos <= retval.size());
    return retval;
}

class EchoServer {
private:
    class Connection {
    public:
        tcp::socket socket;
        Connection(boost::asio::io_service &io_service) : socket(io_service) {}
    };

    boost::asio::io_service io_service;

    tcp::endpoint endpoint;
    tcp::acceptor acceptor;

    void handle_request(shared_ptr<Connection> connection) {
        auto read_buffer = make_shared<boost::asio::streambuf>();
        // Read from client until newline ("\r\n")
        async_read_until(connection->socket, *read_buffer, "\r\n",
                [this, connection, read_buffer](const boost::system::error_code &ec, size_t) {
                 // If not error:
                     if (!ec) {
                         // Retrieve message from client as string:
                         istream read_stream(read_buffer.get());
                         std::string message;
                         getline(read_stream, message);
                         message.pop_back(); // Remove "\r" at the end of message

                         cout << ">>> New Msg >>>" << endl;

                         // Close connection when "exit" is retrieved from client
                         if (message == "exit")
                             return;

                         list <string> msgList;
                         list <string> :: iterator it;

                         while(getline(read_stream, message)){
                             msgList.push_back(message);
                         }

                         string key = "";
                         for(it = msgList.begin(); it != msgList.end(); ++it)
                         {
                             string s = *it;
                             cout << s << endl;
                            if(regex_search(*it, regex("Sec-WebSocket-Key:"))){
                                cout << "*** found key ***" << endl;
                                cout << s << endl;
                                cout << "*** --------- ***" << endl;
                                key = s;
                            }
                         }

                         string newKey = "";
                         std::vector<std::string> result;
                         boost::algorithm::split(result, key, boost::is_any_of(" "));
                         newKey = result[1];
                         cout << newKey << endl;

                         //string code = Base64.encode(SHA-1(newKey + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"));

                         boost::algorithm::trim(newKey);
                         newKey += (string)"258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
                         cout << newKey << endl;

                         int n = newKey.length();
                         // declaring character array
                         char char_array[n + 1];
                         strcpy(char_array, newKey.c_str());
                         unsigned char hash[20];
                         vector<unsigned char> myCharVec;
                         myCHarVec = SHA1((unsigned char*)&char_array, sizeof(char_array) - 1, hash);

                         vector<unsigned char> myCharVec;
                         for(auto& i : char_array){
                             myCharVec.push_back(char_array[i]);
                         }
                         string hasedKey (char_array);

                         cout << hasedKey << endl;

                         //string encoded = Base64Encode(myCharVec);
                         string encoded = base64_encode(hasedKey);

                         cout << "*** Encoded ***" << endl;
                         cout << encoded << endl;

                         auto write_buffer = make_shared<boost::asio::streambuf>();
                         ostream write_stream(write_buffer.get());

                         write_stream << "HTTP/1.1 101 Switching Protocols"<< "\r\n";
                         //write_stream << "HTTP/1.1 101 Web Socket Protocol Handshake" << "\r\n";
                         write_stream << "Upgrade: websocket"<< "\r\n";
                         write_stream << "Connection: Upgrade" << "\r\n";
                         write_stream << "Sec-WebSocket-Accept: " << encoded << "\r\n\r\n";
                         //write_stream << "\r\n";
                         //write_stream << "Sec-WebSocket-Protocol: chat \r\n\r\n";

                         //cout << write_buffer << endl;

                         async_write(connection->socket, *write_buffer,
                                 [this, connection, write_buffer](const boost::system::error_code &ec, size_t) {
                                     cout << "Wrote to socket" << endl;
                             if (!ec)
                                 std::this_thread::sleep_for(4s);
                                         cout << "No error, sending hellos" << endl;
                                     auto write_buffer2 = make_shared<boost::asio::streambuf>();
                                     ostream write_stream2(write_buffer2.get());
                                     //write_stream2 << "HTTP/1.1 200 OK\r\nContent-Length: 7"  << "\r\nContent-Type: text/html\r\n\r\n" << "HELLO!?" << "\r\n\r\n";
                                    //write_stream2 << "hello \r\n\r\n" << endl;

                                     vector<unsigned char> bytes{0x81, 0x83, 0xb4,0xb5, 0x03, 0x2a, 0xdc, 0xd0, 0x6a};
                                     size_t length = bytes[1] & 127;
                                     size_t mask_start = 2;
                                     size_t data_start = mask_start + 4;
                                     for(size_t i = data_start; i < data_start + length; ++i){
                                         cout << char(bytes[i] ^ bytes[mask_start + (i - data_start)%4]);
                                         write_stream2 << char(bytes[i] ^ bytes[mask_start + (i - data_start)%4]);
                                     }
                                     //cout << "\r\n\r\n" << endl;
                                    cout << endl;

                                     async_write(connection->socket, *write_buffer2,
                                                 [this, connection, write_buffer2](const boost::system::error_code &ec, size_t) {
                                                     // If not error:
                                                     if (!ec)
                                                         cout << "Finished." << endl;
                                                     handle_request(connection);
                                                 });
                         });

                     } else{
                         cout << ec << endl;
                     }
         });
    }

    void accept() {
        // The (client) connection is added to the lambda parameter and handle_request
        // in order to keep the object alive for as long as it is needed.
        auto connection = make_shared<Connection>(io_service);

        // Accepts a new (client) connection. On connection, immediately start accepting a new connection
        acceptor.async_accept(connection->socket, [this, connection](const boost::system::error_code &ec) {
            accept();
            // If not error:
            if (!ec) {
                handle_request(connection);
            }
        });
    }

public:
    EchoServer() : endpoint(tcp::v4(), 8080), acceptor(io_service, endpoint) {}

    void start() {
        accept();

        io_service.run();
    }
};

int main() {
    EchoServer echo_server;

    cout << "Starting echo server" << endl
         << "Connect in a broswer console with: localhost 8080." << endl;
    //let ws = new WebSocket('ws://localhost:8080'); ws.onmessage = e => console.log(e.data);

    echo_server.start();

    return 0;
}

// Sindre Thomassen++
