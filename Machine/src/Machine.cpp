#include "Machine.h"

#include <fstream>
#include <exception>
#include <stdexcept>
#include <iostream>

#include "instructions.h"

Machine::Machine(const char* programPath, uint32_t initialSize) : memory(initialSize), programCounter(0), stackPointer(memory.size()), running(true) {
    std::ifstream input;
    input.open(programPath, std::ios::binary);

    if (input.fail()) {
        std::string msg = "Failed to open input file ";
        msg.append(programPath);
        throw std::runtime_error(msg);
    }

    uint32_t addr = 0;
    while (!input.eof()) {
        char byte;
        input.read(&byte, 1);
        if (addr > initialSize) {
            throw std::runtime_error("Program size exceeds memory size");
        }
        memory[addr] = byte;
        addr++;
    }

    input.close();
}

void Machine::run() {
    while (running) {
        fetch();
        decode();
        execute();
    }
}

void Machine::fetch() {
    instructionRegister0 = memory[programCounter];
    programCounter++;
}

void Machine::decode() {
    if (OPERATOR_TAKES_OPERAND(instructionRegister0)) {
        instructionRegister1 = read(programCounter);
        programCounter += 4;
    }
}

void Machine::execute() {
    switch (instructionRegister0) {
        case OP_READ:
            std::cin >> tmpRegister0;
            push(tmpRegister0);
            break;
        case OP_WRITE:
            tmpRegister0 = pop();
            std::cout << tmpRegister0 << std::endl;
            break;
        case OP_DUP:
            tmpRegister0 = pop();
            push(tmpRegister0);
            push(tmpRegister0);
            break;
        case OP_MUL:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            tmpRegister0 *= tmpRegister1;
            push(tmpRegister0);
            break;
        case OP_ADD:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            push(tmpRegister0 + pop());
            break;
        case OP_SUB:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            push(tmpRegister0 - tmpRegister1);
            break;
        case OP_GT:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            push((uint32_t)(tmpRegister0 > tmpRegister1));
            break;
        case OP_LT:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            push((uint32_t)(tmpRegister0 < tmpRegister1));
            break;
        case OP_EQ:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            push((uint32_t)(tmpRegister0 == tmpRegister1));
            break;
        case OP_JMPZ:
            tmpRegister0 = pop();
            tmpRegister1 = pop();
            if (tmpRegister1 == 0) {
                programCounter = tmpRegister0;
            }
            break;
        case OP_OFF:
            running = false;
            break;
        case OP_PUSH:
            push(instructionRegister1);
            break;
        case OP_POP:
            for (longRegister0 = 0; longRegister0 < instructionRegister1; longRegister0++) {
                pop();
            }
            break;
        case OP_ROT:
            for (longRegister0 = 0; longRegister0 < instructionRegister1 - 1; longRegister0++) {
                tmpRegister0 = read(stackPointer + 4 * (instructionRegister1 - 1));
                tmpRegister1 = read(stackPointer + 4 * (instructionRegister1 - longRegister0 - 2));
                write(stackPointer + 4 * (instructionRegister1 - 1), tmpRegister1);
                write(stackPointer + 4 * (instructionRegister1 - longRegister0 - 2), tmpRegister0);
            }
            break;
        default:
            throw std::runtime_error("Undefined instruction");
            break;
    }
}

void Machine::push(uint32_t val) {
    stackPointer -= 4;
    write(stackPointer, val);
}

uint32_t Machine::pop() {
    uint32_t res = read(stackPointer);
    stackPointer += 4;
    return res;
}

void Machine::write(uint32_t address, uint32_t val) {
    if (address >= memory.size()) {
        throw std::runtime_error("Memory fault");
    }
    memory[address] = val & 0xff;
    memory[address + 1] = (val >> 8) & 0xff;
    memory[address + 2] = (val >> 16) & 0xff;
    memory[address + 3] = (val >> 24) & 0xff;
}

uint32_t Machine::read(uint32_t address) {
    if (address >= memory.size()) {
        throw std::runtime_error("Memory fault");
    }
    uint32_t val = 0;
    val = val | memory[address];
    val = val | (memory[address + 1] << 8);
    val = val | (memory[address + 2] << 16);
    val = val | (memory[address + 3] << 24);
    return val;
}

