#pragma once
#ifndef NETIFACES_BASIC_TYPES_HPP
#define NETIFACES_BASIC_TYPES_HPP

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>

namespace netifaces {
    enum class NetworkProto {
        UNKNOWN, IPv4, IPv6
    };

    class NetworkAddress {
        std::string str_;
        NetworkProto proto_;
    public:
        NetworkAddress(NetworkProto proto, std::string str) 
            : proto_(proto), str_(str) {}

        NetworkProto proto() const {
            return proto_;
        }

        std::string str() const {
            return str_;
        }
    };

    class NetworkInterface {
        std::string name_;
        std::vector<NetworkAddress> addrs_;

        std::vector<NetworkAddress>& mutable_addrs() {
            return addrs_;
        }

        friend class NetworkInterfaceProvider;
    public:
        NetworkInterface(std::string name) : name_(name), addrs_() {}

        std::string name() const {
            return name_;
        }

        const std::vector<NetworkAddress> addrs() const {
            return addrs_;
        }
    };

    using NetworkInterfaces = std::set<std::unique_ptr<NetworkInterface>>;
}

#endif