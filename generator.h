#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction* generateInstructionsMultiply(int , int , int , int, int);
Instruction* generateInstructionsFibonacci(int , int , int);
Instruction* readInstructions(char*, int*);
Instruction* generateInstructionsExpo(int, int, int, int, int);


#endif // !GENERATOR_H