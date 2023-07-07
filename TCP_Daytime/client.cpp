#include <iostream>
#include <string>
#include <inttypes.h>

#include <boost/asio/connect.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>

#include <boost/array.hpp>
#include <boost/system/error_code.hpp>

const std::string SERVER_IP = "127.0.0.1";
const uint16_t SERVER_PORT = 1234;

int main(int argc, char **argv){
    // if (argc != 2) {
    //     std::cerr << "Usage client <host>" << std::endl;
    //     return 1;
    // }

    // char *host_name = argv[1];

    boost::asio::io_context iocontext;
    boost::asio::ip::tcp::resolver resolver(iocontext);

    // boost::asio::ip::tcp::resolver::results_type endpoint = resolver.resolve(host_name, "daytime");
    boost::asio::ip::tcp::endpoint endpoint(
        boost::asio::ip::address::from_string(SERVER_IP),
        SERVER_PORT
    );
    boost::asio::ip::tcp::socket socket(iocontext);
    //boost::asio::connect(socket, endpoint);
    socket.connect(endpoint);

    boost::array<char, 128> buf;
    boost::system::error_code error;
    uint64_t len(0);
    while (true) {
        len = socket.read_some(boost::asio::buffer(buf), error);

        if (error == boost::asio::error::eof) {
            break;  // connection closed cleanly by peer;
        } else if(error) {
            std::cerr << "Some errors happened during data reading. Error " << error.message() << std::endl;
            break;
        }

        std::cout.write(buf.data(), len);
    }

    socket.close();

    return 0;
}