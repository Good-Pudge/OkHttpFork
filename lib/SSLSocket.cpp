//
// Created by Good_Pudge.
//

#include "../include/SSLSocket.hpp"
#include "../include/Exception.hpp"
#include "../util/util.hpp"
#include <openssl/ssl.h>

namespace ohf {
    struct OpenSSLInit {
        OpenSSLInit() {
            SSL_library_init();
            SSLeay_add_ssl_algorithms();
            SSL_load_error_strings();
        }
    };

    OpenSSLInit globalInitSSL;

    struct SSLSocket::impl {
        SSL *ssl;
        SSL_CTX *ssl_context;
    };

    SSLSocket::SSLSocket(const Protocol &protocol) : Socket(), pImpl(new impl) {
        const SSL_METHOD *method;
        switch (protocol) {
            case Protocol::SSLv23:
                method = SSLv23_method();
                break;
            case Protocol::SSLv2:
                method = SSLv2_method();
                break;
            case Protocol::SSLv3:
                method = SSLv3_method();
                break;
            case Protocol::TLSv1:
                method = TLSv1_method();
                break;
            case Protocol::TLSv1_1:
                method = TLSv1_1_method();
                break;
            case Protocol::TLSv1_2:
                method = TLSv1_2_method();
                break;
        }

        pImpl->ssl_context = SSL_CTX_new(method);
        if (!pImpl->ssl_context)
            throw Exception(Exception::Code::OPENSSL_CREATE_CONTEXT_ERROR, "OpenSSL create context error:" +
                                                                           util::getOpenSSLError());

        pImpl->ssl = SSL_new(pImpl->ssl_context);
        if (!pImpl->ssl)
            throw Exception(Exception::Code::OPENSSL_INIT_ERROR, "OpenSSL init error: " +
                                                                 util::getOpenSSLError());
    }

    SSLSocket::~SSLSocket() {
        SSL_CTX_free(pImpl->ssl_context);
        SSL_free(pImpl->ssl);
        delete pImpl;
    }

    std::iostream &SSLSocket::connect(const std::string &address, const int &port) {
        std::iostream &ios = Socket::connect(address, port);
        SSL_set_fd(pImpl->ssl, socket_fd);
        if (SSL_connect(pImpl->ssl) < 1)
            throw Exception(Exception::Code::FAILED_TO_CREATE_SSL_CONNECTION,
                            "Failed to create SSL connection: " + util::getOpenSSLError());

        return ios;
    }

    void SSLSocket::send(const char *data, int size) {
        int len = SSL_write(pImpl->ssl, data, size);
        if (len < 0) {
            int error = SSL_get_error(pImpl->ssl, len);
            if (error == SSL_ERROR_WANT_WRITE || error == SSL_ERROR_WANT_READ)
                return;
            throw Exception(Exception::Code::OPENSSL_ERROR, util::getOpenSSLError());
        }
    }

    std::string SSLSocket::receive(size_t size) {
        char *buffer = new char[size];
        int len = SSL_read(pImpl->ssl, buffer, size);
        if (len < 0) {
            int error = SSL_get_error(pImpl->ssl, len);
            if (error == SSL_ERROR_WANT_WRITE || error == SSL_ERROR_WANT_READ)
                return std::string(buffer, len);
            throw Exception(Exception::Code::OPENSSL_ERROR, util::getOpenSSLError());
        }
        return std::string(buffer, len);
    }
}
