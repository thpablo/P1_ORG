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
Instruction* generateInstructionsMultiply(int num1, int num2, int ramSize, int n, int isExpo){
    /*1: Instrucao -> Levar valor 1 para memória ram */
    /*2: Instrucao -> Levar valor 2 para memória ram */
    /*3: Instrucao -> Multiplicaçao:  Somas num2 vezes */
    /*4: Finalizar máquina */

    // 4 Instrucoes alem das somas (mover (3x) e finalizar maquina)
    int quantAloca = 4 + num2;

    //Se for Exponencial
    if(isExpo){

/*         São num1 * (num2 - 1) Instrucoes de soma
                        +
           (num2) Instrucoes de anular valores
           em um endereço (Mover valor 0 ao endereço).
           Para realizar a exponenciação.        
*/
        quantAloca = (num1 * (num2 - 1)) + num2;

        quantAloca += 4;    // 3 Primeiras instrucoes de mover valores para memoria RAM
                            // E finalizar máquina
    }

    Instruction* instructions = (Instruction*) malloc( 100 * sizeof(Instruction));
    
    int randomAddress_Num1 = rand() % ramSize;  //Num1
    int randomAddress_Num2;                     //Num2
    int randomAddress_Res;                      //Res

    do { //Sempre endereços diferentes
        randomAddress_Num2 = rand() % ramSize;
    } while (randomAddress_Num2 == randomAddress_Num1); 

    do {    //Endereços diferentes.
        randomAddress_Res = rand() % ramSize;
    } while (randomAddress_Res == randomAddress_Num2 || randomAddress_Res == randomAddress_Num1); 
    

    // Levar informações para a memória.

    //Levar Valor 1
    instructions[n].opcode  = 0;
    instructions[n].info1   = num1;     //Valor 1
    instructions[n].info2   = randomAddress_Num1; //Endereço para valor 1

    //Levar Valor 2
    instructions[n + 1].opcode  = 0;
    instructions[n + 1].info1   = num2; //Valor 2
    instructions[n + 1].info2   = randomAddress_Num2; //Endereço para valor 2;
    
    //Levar Valores de resposta para Exponencial
    instructions[n + 2].opcode  = 0;

    //Se expoente for 0
    if (isExpo && (num2 == 0))      { instructions[n + 2].info1   = 1; }  

    //Se expoente for 1  
    else if (isExpo && (num2 == 1)) { instructions[n + 2].info1   = num1; }

    //Se expoente for diferente de 0 e 1
    else                            { instructions[n + 2].info1   = 0; }

    instructions[n + 2].info2   = randomAddress_Res; //Endereço para valor 2


    int i = n + 2;  //Soma-se 2 por causa das instrucoes de mover

    num2 += 2;      //Soma-se 2 por causa das instrucoes de mover
    int numExpo = 2;
    int somaNum2 = num1;
    //Expo
    if(isExpo){
        i++;    //Soma-se um por causa da instrucao de anular valor no expoente
        numExpo = num2 - 2;
        num2 = num1 + 3; //Soma-se 3 por causa das instrucoes de mover
    }

    //Realizando somas
    while(i < num2 && numExpo > 1) { //Somas enquanto menor que o numero 2;
        instructions[i].opcode  = 1;
        instructions[i].info1   = randomAddress_Num1; //Somar endereço valor num1
        instructions[i].info2   = randomAddress_Res; //Endereço valor num1
        instructions[i].info3   = randomAddress_Res; //Endereço valor resultado
        i++;

        /*  Verifica se é uma exponenciação
            e se realizou a quantidade somas
            para uma multiplicação (Num1 * Num2)
        */
        if(isExpo && i == num2){

            //Anula valor no endereço do numero 1
            instructions[i].opcode = 0;
            instructions[i].info1 = 0;
            instructions[i].info2 = randomAddress_Num1;
            i++;

            //Leva valor do resultado da multiplicao para o numero 1
            instructions[i].opcode = 1;
            instructions[i].info1 = randomAddress_Num1;
            instructions[i].info2 = randomAddress_Res;
            instructions[i].info3 = randomAddress_Num1;
            i++;

            //Aumenta o limite da multiplicação para somas futuras
            num2 += (somaNum2 + 1);

            //Reduz a quantidade de multiplicacoes restantes da Exponenciacao
            numExpo--;
        }
    }


    instructions[i].opcode =-1;    //Ultima posicao
    instructions[i].info1 = -1;
    instructions[i].info2 = -1;
    instructions[i].info3 = -1;

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

Instruction* generateInstructionsExpo(int valor1, int valor2, int ramSize, int n, int isExponencial){
    Instruction * instructions = generateInstructionsMultiply( valor1, valor2, ramSize, n, isExponencial );

    return instructions;
}