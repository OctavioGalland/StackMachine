#ifndef _MACHINE_H_
#define _MACHINE_H_

#include <vector>
#include <cinttypes>

class Machine {
public:
    Machine(const char* binaryPath, uint32_t initialSize = 4096);

    void run();
private:
    void fetch();
    void decode();
    void execute();

    uint32_t read(uint32_t address);
    void write(uint32_t address, uint32_t value);

    void push(uint32_t value);
    uint32_t pop();

    std::vector<uint8_t> memory;

    uint32_t programCounter;
    uint32_t stackPointer;
    uint8_t instructionRegister0;
    uint32_t instructionRegister1;
    int32_t tmpRegister0;
    int32_t tmpRegister1;
    int64_t longRegister0;
    bool running;
};

#endif // _MACHINE_H_
