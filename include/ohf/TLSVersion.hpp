//
// Created by Good_Pudge.
//

#ifndef OKHTTPFORK_TLSVERSION_HPP
#define OKHTTPFORK_TLSVERSION_HPP

namespace ohf {
    enum class TLSVersion {
        SSLv23,
        SSLv2,
        SSLv3,
        TLS,
        TLSv1,
        TLSv1_1,
        TLSv1_2
    };
}

#endif //OKHTTPFORK_TLSVERSION_HPP
