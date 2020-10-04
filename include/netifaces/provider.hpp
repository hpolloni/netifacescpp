#pragma once
#ifndef NETIFACES_PROVIDER_HPP
#define NETIFACES_PROVIDER_HPP


#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

namespace netifaces {
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
            auto proto = convertToProto(ifa->ifa_addr->sa_family);
            
            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET) {
                char inet_addr[INET_ADDRSTRLEN];
                auto sa = reinterpret_cast<sockaddr_in*>(ifa->ifa_addr);
                inet_ntop(ifa->ifa_addr->sa_family, &(sa->sin_addr), inet_addr, INET_ADDRSTRLEN);
                return NetworkAddress(proto, inet_addr);
            }

            if (ifa->ifa_addr && ifa->ifa_addr->sa_family == AF_INET6) {
                char inet_addr[INET6_ADDRSTRLEN];
                auto sa = reinterpret_cast<sockaddr_in6*>(ifa->ifa_addr);
                inet_ntop(ifa->ifa_addr->sa_family, &(sa->sin6_addr), inet_addr, INET6_ADDRSTRLEN);
                return NetworkAddress(proto, inet_addr);
            }
            return NetworkAddress(proto, "UNKNOWN_ADDRESS");
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
}

#endif