//
// Created by Good_Pudge.
//

#include <ohf/tcp/Server.hpp>

namespace ohf {
    namespace tcp {
        Server::Connection::Connection(tcp::Socket *socket, const InetAddress &address, Uint16 port) :
                mSocket(socket),
                mAddress(address),
                mPort(port)
        {}

        Server::Connection::~Connection() {
            delete mSocket;
        }

        tcp::Socket& Server::Connection::socket() const {
            return *mSocket;
        }

        InetAddress Server::Connection::address() const {
            return mAddress;
        }

        Uint16 Server::Connection::port() const {
            return mPort;
        }

        void Server::Connection::close() const {
            mSocket->close();
        }
    }
}