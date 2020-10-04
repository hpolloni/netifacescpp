#include "netifaces/netifaces.hpp"
#include <iostream>

int main(int argc, char** argv) {
    const auto& interfaces = netifaces::interfaces();
    for (const auto& interface : interfaces) {
        std::cout << interface.name() << std::endl;
        for (const auto& addr : interface.addrs()) {
            std::cout << " " << addr.proto() << " " << addr.str() << std::endl;
        }
    }
    return 0;
}
