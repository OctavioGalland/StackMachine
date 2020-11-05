#ifndef _INSTRUCTIONS_H_
#define _INSTRUCTIONS_H_

// no operands

#define OP_READ 0x01
#define OP_WRITE 0x02
#define OP_DUP 0x03
#define OP_MUL 0x04
#define OP_ADD 0x05
#define OP_SUB 0x06
#define OP_GT 0x07
#define OP_LT 0x08
#define OP_EQ 0x09
#define OP_JMPZ 0x0A
#define OP_OFF 0x0B

// 1 operand

#define OP_PUSH 0x81
#define OP_POP 0x82
#define OP_ROT 0x83

#define OPERATOR_TAKES_OPERAND(op) (op == OP_PUSH || op == OP_POP || op == OP_ROT)

#endif // _INSTRUCTIONS_H_
