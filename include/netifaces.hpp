#pragma once
#ifndef NETIFACES_HPP
#define NETIFACES_HPP

#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <memory>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

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

    class NetworkInterfaceProvider {
        NetworkProto convertToProto(sa_family_t family) {
            switch(family) {
                case AF_INET:
                    return NetworkProto::IPv4;
                case AF_INET6:
                    return NetworkProto::IPv6;
                default:
                    return NetworkProto::UNKNOWN;
            }
        }

        NetworkAddress convertToAddress(ifaddrs* ifa) {
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                auto sa = (struct sockaddr_in *) ifa->ifa_addr;
                return NetworkAddress(convertToProto(ifa->ifa_addr->sa_family), inet_ntoa(sa->sin_addr));
            }
            return NetworkAddress(convertToProto(ifa->ifa_addr->sa_family), "UNKNOWN");
        }
        
    public:
        void interfaces(NetworkInterfaces&& ifaces) {
            std::unordered_map<std::string, std::unique_ptr<NetworkInterface>> dedup;
            ifaddrs* ifaddr;
            getifaddrs (&ifaddr);
            for (ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
                auto iface = dedup.find(ifa->ifa_name);
                if (iface == dedup.end()) {
                    dedup[ifa->ifa_name] = std::make_unique<NetworkInterface>(ifa->ifa_name);
                }
                dedup[ifa->ifa_name]->mutable_addrs().emplace_back(convertToAddress(ifa));
            }
            freeifaddrs(ifaddr);

            for (auto& kv : dedup) {
                ifaces.insert(std::move(kv.second));
            }
        }
    };

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