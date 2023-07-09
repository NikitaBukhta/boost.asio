#include <iostream>
#include <string>
#include <ctime>
#include <inttypes.h>

#include <boost/asio/buffer.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/system/error_code.hpp>

const uint16_t SERVER_PORT = 1234;

std::string make_daytime_to_string(void) {
    time_t now = time(0);
    return ctime(&now);
}

int main(int argc, char **argv) {
    boost::asio::io_context iocontext;
    boost::asio::ip::tcp::acceptor tcp_acceptor(iocontext
        , boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), SERVER_PORT)
    );

    std::string message;
    boost::asio::ip::tcp::socket client_socket(iocontext);
    boost::system::error_code ignored_error;
    while (true) {
        tcp_acceptor.accept(client_socket);
        message = make_daytime_to_string();
        boost::asio::write(client_socket, boost::asio::buffer(message), ignored_error);
        client_socket.close();
    }

    return 0;
}