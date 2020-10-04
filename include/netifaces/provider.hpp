#pragma once
#ifndef NETIFACES_PROVIDER_HPP
#define NETIFACES_PROVIDER_HPP

#include <unordered_map>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

#include "basic_types.hpp"

namespace netifaces {
    namespace NetworkInterfaceProvider {
        namespace internal {
            inline NetworkProto convertToProto(sa_family_t family) {
                switch(family) {
                    case AF_INET:
                        return NetworkProto::IPv4;
                    case AF_INET6:
                        return NetworkProto::IPv6;
                    default:
                        return NetworkProto::UNKNOWN;
                }
            }

            inline NetworkAddress convertToAddress(ifaddrs* ifa) {
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
        }

        inline NetworkInterfaces interfaces() {
            std::unordered_map<std::string, NetworkInterface> dedup;
            ifaddrs* ifaddr;
            getifaddrs (&ifaddr);
            for (ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
                auto iface = dedup.find(ifa->ifa_name);
                if (iface == dedup.end()) {
                    auto inserted = dedup.emplace(ifa->ifa_name, ifa->ifa_name);
                    inserted.first->second.add(internal::convertToAddress(ifa));
                    continue;
                }
                iface->second.add(internal::convertToAddress(ifa));
            }
            freeifaddrs(ifaddr);

            NetworkInterfaces ifaces;
            for (auto& kv : dedup) {
                ifaces.push_back(std::move(kv.second));
            }
            return ifaces;
        }
    }
}

#endif
