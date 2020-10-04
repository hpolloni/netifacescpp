#pragma once
#ifndef NETIFACES_BASIC_TYPES_HPP
#define NETIFACES_BASIC_TYPES_HPP

#include <string>
#include <vector>

namespace netifaces {
    enum class NetworkProto {
        UNKNOWN, IPv4, IPv6
    };

    class NetworkAddress {
        std::string str_;
        NetworkProto proto_;
    public:
        NetworkAddress(NetworkProto proto, const std::string& str)
            : proto_(proto), str_(str) {}

        NetworkProto proto() const {
            return proto_;
        }

        const std::string& str() const {
            return str_;
        }
    };

    class NetworkInterface {
        std::string name_;
        std::vector<NetworkAddress> addrs_;

        std::vector<NetworkAddress>& mutable_addrs() {
            return addrs_;
        }

    public:
        NetworkInterface(const std::string& name) : name_(name) {}

        const std::string& name() const {
            return name_;
        }

        const std::vector<NetworkAddress>& addrs() const {
            return addrs_;
        }

        void add(NetworkAddress&& address) {
            addrs_.push_back(std::move(address));
        }
    };

    using NetworkInterfaces = std::vector<NetworkInterface>;
}

#endif
