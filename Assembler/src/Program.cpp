#include "Program.h"

#include <fstream>
#include <iostream>
#include <stdexcept>

#include "utils.h"
#include "instructions.h"

Program::Program(const char* filename) {
    parseCode(filename);
}

void Program::parseCode(const char* filename) {
    std::ifstream inputFile;
    inputFile.open(filename);

    if (inputFile.fail()) {
        std::string msg = "Failed to open input file ";
        msg.append(filename);
        throw std::runtime_error(msg);
    }

    // Parse input file into an array of array of strings, containing each word for each line
    // but omitting comments and whitepsace
    size_t lineNumber = 1;
    std::map<std::string, uint32_t> labels;
    std::map<uint32_t,std::string> unresolvedLabels;
    uint32_t address = 0;
    while (!inputFile.eof()) {
        std::string line;
        std::getline(inputFile, line);

        // Ignore beginning and trailing whitespace as well as everything after a "#"
        line = trim(line.substr(0, line.find('#')));

        // Split words
        if (line.size() > 0) {
            std::vector<std::string> words = split(line);
            std::string firstWord = words[0];
            uint32_t operatorIndex = 0;
            uint32_t operandIndex = 1;
            bool hasOperator = true;

            // Parse labels, if any
            if (firstWord[firstWord.size() - 1] == ':') {
                std::string label = firstWord.substr(0, firstWord.size() - 1);
                if (labels.count(label) != 0) {
                    throw std::runtime_error("Multiple definition of label \"" + label + "\" at line " + std::to_string(lineNumber));
                }
                labels[label] = address;
                if (words.size() == 1) {
                    hasOperator = false;
                } else {
                    operatorIndex = 1;
                    operandIndex = 2;
                }
            }
            // Parse operator and operand, if any
            if (hasOperator) {
                uint8_t op = parseOperator(words[operatorIndex]);
                address++;
                if (OPERATOR_TAKES_OPERAND(op)) {
                    if (words.size() <= operandIndex) {
                        throw std::runtime_error("Expected operand at line " + std::to_string(lineNumber));
                    }
                    bool isLiteral = parseOperand(words[operandIndex]);
                    // Defer label resolution
                    if (!isLiteral) {
                        unresolvedLabels[address] = words[operandIndex];
                    }
                    address += 4;
                }
            }
        }
        lineNumber++;
    }
    inputFile.close();

    resolveLabels(unresolvedLabels, labels);
}

void Program::dumpToFile(const char* filename) {
    std::ofstream output;
    output.open(filename, std::ios::binary | std::ios::trunc);

    if (output.fail()) {
        std::string msg = "Failed to open output file ";
        msg.append(filename);
        throw std::runtime_error(msg);
    }

    for (auto it = bytes.cbegin(); it != bytes.cend(); it++) {
        output << *it;
    }

    output.close();
}

uint8_t Program::parseOperator(const std::string& strOperator) {
    if (operatorValues.count(strOperator) == 0) {
        throw std::runtime_error("Unkown operator \"" + strOperator + "\"");
    }
    uint8_t operatorValue = operatorValues.at(strOperator);
    bytes.push_back(operatorValue);
    return operatorValue;
}

bool Program::parseOperand(const std::string& strOperand) {
    bool literal = true;
    uint32_t operandValue = 0;
    if (isNumeric(strOperand)) {
        operandValue = std::atoi(strOperand.c_str());
    } else {
        literal = false;
    }
    bytes.push_back(operandValue & 0xff);
    bytes.push_back((operandValue >> 8) & 0xff);
    bytes.push_back((operandValue >> 16) & 0xff);
    bytes.push_back((operandValue >> 24) & 0xff);
    return literal;
}

void Program::resolveLabels(const std::map<uint32_t, std::string>& unresolvedLabels, const std::map<std::string, uint32_t>& knownLabels) {
    for (auto it = unresolvedLabels.cbegin(); it != unresolvedLabels.cend(); it++) {
        std::string label = it->second;
        uint32_t labelAddress = 0;
        uint32_t binaryAddress = it->first;
        if (knownLabels.count(label) == 0) {
            throw std::runtime_error("Could not resolve label \"" + label + "\"");
        }
        labelAddress = knownLabels.at(label);
        bytes[binaryAddress] = (labelAddress & 0xff);
        bytes[binaryAddress + 1] = ((labelAddress >> 8) & 0xff);
        bytes[binaryAddress + 2] = ((labelAddress >> 16) & 0xff);
        bytes[binaryAddress + 3] = ((labelAddress >> 24) & 0xff);
    }
}
