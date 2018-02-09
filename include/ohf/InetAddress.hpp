//
// Created by Good_Pudge.
//

#ifndef OKHTTPFORK_INETADDRESS_HPP
#define OKHTTPFORK_INETADDRESS_HPP

#include <string>
#include <vector>
#include "Config.hpp"

namespace ohf {
    class InetAddress {
    public:
        static InetAddress BROADCAST;
        static InetAddress ANY;

        InetAddress() = default;

        InetAddress(const char *x);

        explicit InetAddress(Uint32 address);

        explicit InetAddress(const std::string &x);

        explicit InetAddress(const std::vector<Uint8> &ip);

        static std::vector<InetAddress> getAllByName(const std::string &host);

        std::vector<Uint8> address() const;

        std::string hostAddress() const;

        std::string hostName() const;

        std::string canonicalName() const;

        std::vector<std::string> aliases() const;

        Uint32 toUint32() const;

        friend std::ostream& operator<<(std::ostream &stream, const InetAddress &address);

    private:
        std::string mHostName;
        std::string mCanonName;
        std::vector<Uint8> mIP;
        std::vector<std::string> mAliases;
    };
}

#endif //OKHTTPFORK_INETADDRESS_HPP
