#pragma once
#ifndef NETIFACES_HPP
#define NETIFACES_HPP

#include <string>

#include "basic_types.hpp"
#include "provider.hpp"

namespace netifaces {
    NetworkInterfaces interfaces() {
        static NetworkInterfaceProvider provider;
        NetworkInterfaces result;
        provider.interfaces(std::move(result));
        return result;
    }
}

std::string to_string(const netifaces::NetworkProto& proto) {
    switch(proto) {
        case netifaces::NetworkProto::IPv4:
            return "IPv4";
        case netifaces::NetworkProto::IPv6:
            return "IPv6";
        case netifaces::NetworkProto::UNKNOWN:
            return "UNKNOWN";
    }
}

std::ostream& operator<<(std::ostream& os, const netifaces::NetworkProto& proto) {
    os << to_string(proto);
    return os;
}
#endif