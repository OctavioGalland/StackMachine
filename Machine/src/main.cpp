#include <iostream>
#include <exception>
#include <cstring>

#include "Machine.h"

int main(int argc, char* argv[]) {
    if (argc != 2 && argc != 4) {
        std::cout << "Usage: machine [-m memorySize] binaryPath" << std::endl;
        return -1;
    }
    const char* filePath = argv[argc - 1];
    uint32_t memorySize = 0;
    if (argc == 4) {
        if (strcmp(argv[1], "-m") != 0) {
            std::cerr << "Unrecognized option ";
            std::cerr << argv[1];
            std::cerr << std::endl;
        } else {
            memorySize = std::atoi(argv[2]);
        }
    }
    if (memorySize == 0) {
        memorySize = 4096;
    }

    try {
        Machine m(filePath, memorySize);
        m.run();
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
