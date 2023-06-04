#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => isto é uma instrução
    // 02|33|12|01 => isto é outra instrução
            
    // 0 => salvar na memória
    // 1 => opcode => somar
    // 2 => opcode => subtrair
    //-1 => halt
    
    // 22 => significa um endereço da RAM (10 endereço) 
    // 13 => significa 2o endereço
    // 45 => significa 3o endereco
    //ramSize => ESTA FORA DO INTERVALO DE 0 A ramSize DA MEMÓRIA RAM

    Instruction* instructions = (Instruction*) malloc(10 * sizeof(Instruction));

    for (int i=0; i<9; i++){
        instructions[i].opcode = rand() % 4; //0, 1, 2, 3
        instructions[i].info1 = rand() % ramSize; //0 ... RAM_SIZE
        do {
            instructions[i].info2 = rand() % ramSize; //0 ... RAM_SIZE
        } while (instructions[i].info1 == instructions[i].info2);
        instructions[i].info3 = rand() % ramSize; //0 ... RAM_SIZE
    }
    
    //inserindo a ultima instrucao do programa que nao faz nada que presta
    instructions[9].opcode =-1;
    instructions[9].info1 = -1;
    instructions[9].info2 = -1;
    instructions[9].info3 = -1;
    
    return instructions;
}

Instruction* readInstructions(char* fileName, int* ramSize) {
    printf("FILE -> %s\n", fileName);
    FILE* file = fopen(fileName, "r"); // Abrindo arquivo no modo leitura
    
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto.\n");
        exit(1);
    }

    int n, i = 0;
    fscanf(file, "%d %d", ramSize, &n);
    Instruction* instructions = (Instruction*) malloc(n * sizeof(Instruction));
    while (i < n) {
        fscanf(file, "%d %d %d %d", &instructions[i].opcode, &instructions[i].info1, &instructions[i].info2, &instructions[i].info3);
        i++;
    }
    fclose(file); // Fechando o arquivo

    return instructions;
}       
//                                                                             n -> numero de instrucoes.
Instruction* generateInstructionsMultiply(int num1, int num2, int ramSize, int n){
    /*1: Instrucao -> Levar valor 1 para memória ram */
    /*2: Instrucao -> Levar valor 2 para memória ram */
    /*3: Instrucao -> Multiplicaçao:  Somas num2 vezes */
    /*4: Finalizar máquina */

    // 3 Instrucoes alem das somas (mover (2x) e finalizar maquina)
    Instruction* instructions = (Instruction*) malloc((3 + num2)  * sizeof(Instruction));
    
    int randomAddress_Num1 = rand() % ramSize;  //Num1
    int randomAddress_Num2;                     //Num2
    int randomAddress_Num3;                     //Num3

    do { 
        randomAddress_Num2 = rand() % ramSize;
    } while (randomAddress_Num2 == randomAddress_Num1); //Sempre endereços diferentes

    do {
        randomAddress_Num3 = rand() % ramSize;
    } while (randomAddress_Num3 == randomAddress_Num2 || randomAddress_Num3 == randomAddress_Num1); //Endereços diferentes.


    // Levar informações para a memória.
    instructions[n].opcode  = 0;
    instructions[n].info1   = num1;     //Valor 1
    instructions[n].info2   = randomAddress_Num1; //Endereço para valor 1

    instructions[n + 1].opcode  = 0;
    instructions[n + 1].info1   = num2; //Valor 2
    instructions[n + 1].info2   = randomAddress_Num2; //Endereço para valor 2;

    //Realizando somas
    for (int i = n + 2; i < num2 + 2; i++) { //Somas enquanto menor que o numero 2;
        instructions[i].opcode  = 1;
        instructions[i].info1   = randomAddress_Num1; //Somar endereço valor num1
        instructions[i].info2   = randomAddress_Num3; //Endereço valor num1
        instructions[i].info3   = randomAddress_Num3; //Endereço valor resultado
    }


    instructions[2 + num2].opcode =-1;    //Ultima posicao
    instructions[2 + num2].info1 = -1;
    instructions[2 + num2].info2 = -1;
    instructions[2 + num2].info3 = -1;

    return instructions;
}

Instruction* generateInstructionsFibonacci(int termos, int ramSize, int n){
    /*1: Instrucao -> Levar valor num1 para memória ram */
    /*2: Instrucao -> Reservar valor antigo para memória ram */
    /*3: Instrucao -> Soma Fibonacci */
    /*4: Finalizar máquina */

     /* quantTermos é duplicado pois a cada soma, 
     há uma instrucao de mover os valores
     Duplicando a quantidade de operacoes. 
     É somado 4 para instruções de Mover Dados/Finalizar operação. 
     */
    int quantTermos = (termos * 2) + 4;

    Instruction* instructions = (Instruction*) malloc(quantTermos * sizeof(Instruction)); //Mudar quantidade de instrucoes

    /*Endereço para resposta*/
    int randomAddress_Res = rand() % ramSize;     //Quantidade termos
    
    /* Endereço para valor antigo */
    int randomAddress_oldValue;
    do { 
        randomAddress_oldValue = rand() % ramSize;
    } while (randomAddress_oldValue == randomAddress_Res); //Sempre endereços diferentes

    // Valor Resposta
    instructions[n].opcode  = 0;
    instructions[n].info1   = 0;     //Valor inicial -> 0
    instructions[n].info2   = randomAddress_Res; //Endereço para valor 1


    //Valor Fib Termo antigo
    instructions[n + 1].opcode  = 0;
    instructions[n + 1].info1   = 1; //Antigo valor = 1;
    instructions[n + 1].info2   = randomAddress_oldValue; //Endereço para valor 2;

    for (int i = n + 2; i < quantTermos; i += 2){ //Pula-se dois pois há as instrucoes de soma e instrucoes de subtracao
        //SOMAR OldValue com Res;
        instructions[i].opcode = 1;
        instructions[i].info1 = randomAddress_Res; //Soma valor antigo
        instructions[i].info2 = randomAddress_oldValue; //Com oldValue
        instructions[i].info3 = randomAddress_Res;      //Armazena em Res

        //SUBTRAIR Res com OldValue, resultado armazenado em OldValue;
        instructions[i + 1].opcode = 2;
        instructions[i + 1].info1 = randomAddress_Res;
        instructions[i + 1].info2 = randomAddress_oldValue;
        instructions[i + 1].info3 = randomAddress_oldValue; 
        //i += 2; //Pula-se dois pois há as instrucoes de soma e instrucoes de subtracao
   }

   instructions[quantTermos - 1].opcode = -1;
   instructions[quantTermos - 1].info1 = -1;
   instructions[quantTermos - 1].info2 = -1;
   instructions[quantTermos - 1].info3 = -1;

   return instructions;


}