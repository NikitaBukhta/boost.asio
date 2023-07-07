#include <string>
#include <ctime>
#include <inttypes.h>

#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <boost/asio/buffer.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/system/error_code.hpp>

std::string make_daytime_string(void) {
    time_t now = time(0);
    return ctime(&now);
}

class TCPConnection : public boost::enable_shared_from_this<TCPConnection> {
public:
    typedef boost::shared_ptr<TCPConnection> TCPConnectionPtr;

    static TCPConnectionPtr create(boost::asio::io_context &iocontext) {
        return TCPConnectionPtr(new TCPConnection(iocontext));
    }

    boost::asio::ip::tcp::socket& socket(void) {
        return m_socket;
    }

    void send_message(const std::string &message) {
        boost::asio::async_write(m_socket, boost::asio::buffer(message)
            , boost::bind(&handle_write, shared_from_this(), 
                boost::asio::placeholders::error, 
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

private:
    TCPConnection(boost::asio::io_context &iocontext)
        : m_socket(iocontext) 
    {
        
    }

    void handle_write(const boost::system::error_code &error, uint64_t len) {

    }

private:
    boost::asio::ip::tcp::socket m_socket;
    std::string m_message;
};

class TCPServer {
public:
    TCPServer(boost::asio::io_context &iocontext)
        : m_iocontext(iocontext)
        , m_acceptor(iocontext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), m_PORT))
    {
        start_accept();
    }

    ~TCPServer(void) = default;

private:
    void start_accept(void) {
        TCPConnection::TCPConnectionPtr new_connection = TCPConnection::create(m_iocontext);

        m_acceptor.async_accept(new_connection->socket(), boost::bind(&TCPServer::handleAccept, this
                , new_connection, boost::asio::placeholders::error
            )
        );

    }

    void handleAccept(TCPConnection::TCPConnectionPtr new_connection, const boost::system::error_code &error) {
        error ? start_accept() : new_connection->send_message(make_daytime_string());
    }

private:
    const uint16_t m_PORT = 1234;
    boost::asio::io_context &m_iocontext;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

int main(int argc, char **argv) {
    boost::asio::io_context iocontext;
    TCPServer tcp_server(iocontext);
    iocontext.run();

    return 0;
}