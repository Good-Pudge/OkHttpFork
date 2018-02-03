#include <catch.hpp>
#include "exception_matcher.hpp"
#include <ohf/InetAddress.hpp>

TEST_CASE("InetAddress") {
    ohf::InetAddress address = "localhost";
    REQUIRE(address.hostAddress() == "127.0.0.1");
}