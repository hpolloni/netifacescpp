# netifacescpp
A cross-platform header only library to fetch network interfaces. Inspired by https://pypi.org/project/netifaces/

# Goals
* Compatible with MacOSX, Linux and Windows.
* Compatible with C++14.
* Header-only.
* No dependencies. 

# Installation
Just copy the include directory to your project.

# Example usage
```cpp
#include "netifaces.hpp"
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

# TODO
* Ethernet support
* IPv6 support
* Windows support
