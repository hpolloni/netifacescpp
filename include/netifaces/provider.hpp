#pragma once
#ifndef NETIFACES_PROVIDER_HPP
#define NETIFACES_PROVIDER_HPP

#include <unordered_map>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>

// Should this be for all BSD systems?
#ifdef __APPLE__
#include <netdb.h>
#include <net/if_dl.h>
#endif

#ifdef __linux__
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <iomanip>
#include <sstream>
#endif

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
#ifdef __APPLE__
                    case AF_LINK:
                        return NetworkProto::ETHERNET;
#endif
#ifdef __linux__
                    case AF_PACKET:
                        return NetworkProto::ETHERNET;
#endif
                    default:
                        return NetworkProto::UNKNOWN;
                }
            }

            inline socklen_t socklen(sockaddr* sa) {
                switch(sa->sa_family) {
                    case AF_INET:
                        return sizeof(sockaddr_in);
                    case AF_INET6:
                        return sizeof(sockaddr_in6);
#ifdef __APPLE__
                    case AF_LINK:
                        return sizeof(sockaddr_dl);
#endif
                    default:
                        return -1; // TODO: this should be an assert
                }
            }

#ifdef __linux__
            inline NetworkAddress convertToAddress(sockaddr* sa) {
                auto proto = convertToProto(sa->sa_family);

                if (proto == NetworkProto::IPv4) {
                    char inet_addr[INET_ADDRSTRLEN];
                    auto sin = reinterpret_cast<sockaddr_in*>(sa);
                    inet_ntop(sa->sa_family, &(sin->sin_addr), inet_addr, INET_ADDRSTRLEN);
                    return NetworkAddress(proto, inet_addr);
                }

                if (proto == NetworkProto::IPv6) {
                    char inet_addr[INET6_ADDRSTRLEN];
                    auto sin = reinterpret_cast<sockaddr_in6*>(sa);
                    inet_ntop(sa->sa_family, &(sin->sin6_addr), inet_addr, INET6_ADDRSTRLEN);
                    return NetworkAddress(proto, inet_addr);
                }

                if (proto == NetworkProto::ETHERNET) {
                    auto sll = reinterpret_cast<sockaddr_ll*>(sa);
                    std::ostringstream ostr;
                    ostr << std::hex;
                    for (int i = 0; i < 6; ++i) {
                        ostr << std::setw(2) << std::setfill('0') 
                            << static_cast<unsigned int>(sll->sll_addr[i]) << (i < 5 ? ":" : "");
                    }
                    return NetworkAddress(proto, ostr.str());
                }
                return NetworkAddress(proto, "UNKNOWN_ADDRESS");
            }
#endif

#ifdef __APPLE__
            inline NetworkAddress convertToAddress(sockaddr* sa) {
                auto proto = convertToProto(sa->sa_family);

                if (proto != NetworkProto::UNKNOWN) {
                    socklen_t len = socklen(sa);
                    char buf[NI_MAXHOST];
                    getnameinfo(sa, len, buf, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
                    return NetworkAddress(proto, buf);
                }

                return NetworkAddress(proto, "UNKNOWN_ADDRESS");
            }
#endif
        }

        inline NetworkInterfaces interfaces() {
            std::unordered_map<std::string, NetworkInterface> dedup;
            ifaddrs* ifaddr;
            getifaddrs (&ifaddr);
            for (ifaddrs* ifa = ifaddr; ifa; ifa = ifa->ifa_next) {
                auto iface = dedup.find(ifa->ifa_name);
                if (iface == dedup.end()) {
                    auto inserted = dedup.emplace(ifa->ifa_name, ifa->ifa_name);
                    inserted.first->second.add(internal::convertToAddress(ifa->ifa_addr));
                    continue;
                }
                iface->second.add(internal::convertToAddress(ifa->ifa_addr));
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
