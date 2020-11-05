#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <vector>
#include <map>
#include <string>

#include "instructions.h"

class Program {
public:
    Program(const char* filename);

    void dumpToFile(const char* filename);
private:
    void parseCode(const char* filename);
    uint8_t parseOperator(const std::string& string);
    bool parseOperand(const std::string& string);
    void resolveLabels(const std::map<uint32_t, std::string>& unresolvedLabels, const std::map<std::string, uint32_t>& knownLabels);

    const std::map<std::string, uint8_t> operatorValues = {
        {"read", OP_READ},
        {"write", OP_WRITE},
        {"dup", OP_DUP},
        {"mul", OP_MUL},
        {"add", OP_ADD},
        {"sub", OP_SUB},
        {"gt", OP_GT},
        {"lt", OP_LT},
        {"eq", OP_EQ},
        {"jmpz", OP_JMPZ},
        {"off", OP_OFF},
        {"push", OP_PUSH},
        {"pop", OP_POP},
        {"rot", OP_ROT},
    };

    std::vector<uint8_t> bytes;
};

#endif // _PROGRAM_H_
