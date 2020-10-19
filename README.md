# netifacescpp
A cross-platform header only library to fetch network interfaces. Inspired by https://pypi.org/project/netifaces/

# Goals
* Compatible with MacOSX, Linux and Windows. (Windows support TBD)
* Compatible with C++14.
* Header-only.
* No dependencies. 

# Installation
Just copy the include directory to your project.

# Example usage
```cpp
#include "netifaces/netifaces.hpp"
#include <iostream>

int main(int argc, char** argv) {
    auto interfaces = netifaces::interfaces();
    for (auto& interface : interfaces) {
        std::cout << interface->name() << std::endl;
        for (const auto addr : interface->addrs()) {
            std::cout << " " << addr.proto() << " " << addr.str() << std::endl;
        }
    }
    return 0;
}
```

This will output a list of interfaces in the running machine with their corresponding addresses. Something like:
```
en0
 ETHERNET 8c:85:90:ad:2b:da
 IPv6 fe80::1cfd:9573:6ddc:e265%en0
 IPv4 192.168.1.240
lo0
 ETHERNET
 IPv4 127.0.0.1
 IPv6 ::1
 IPv6 fe80::1%lo0
```
