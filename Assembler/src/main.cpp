#include <iostream>

#include "Program.h"

int main (int argc, char *argv[]) {
    std::string outputFilename = "a.out";

    if (argc < 2 || argc > 3) {
        std::cout << "Usage: assembler inputFile [outputFile]" << std::endl;
        std::cout << "\tBy default [outputFile] is equal to a.out" << std::endl;
        return -1;
    }
    if (argc == 3) {
        outputFilename = argv[2];
    }
    try {
        Program p(argv[1]);
        p.dumpToFile(outputFilename.c_str());
    } catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
