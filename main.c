#include "cpu.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char**argv) {

    srand(time(NULL));   // Inicializacao da semente para os numeros aleatorios.
 
    if ( argc < 3) {
        printf("Numero de argumentos invalidos!\n");
        printf("Linha de execucao: ./exe TIPO_INSTRUCAO [TAMANHO_RAM|ARQUIVO_DE_INSTRUCOES]\n");
        printf("\tExemplo 1 de execucao: ./exe random 10\n");
        printf("\tExemplo 3 de execucao: ./exe file arquivo_de_instrucoes.txt\n");
        return 0;
    }

    int ramSize, num1, num2;
    Machine machine;
    Instruction *instructions;

    printf("\033[0;31m");

    if (strcmp(argv[1], "random") == 0) {
        ramSize = atoi(argv[2]);
        instructions = generateRandomInstructions(ramSize);

    } else if (strcmp(argv[1], "file") == 0) {
        instructions = readInstructions(argv[2], &ramSize);

    } else if (strcmp(argv[1], "multi") == 0){   //multi ramSize num1 num2
        ramSize = atoi(argv[2]);       
        if(ramSize < 3)
            printf("O tamanho da RAM deve ser superior a 2.\n");
        else {
            num1    = atoi(argv[3]);
            num2    = atoi(argv[4]);
            printf("Será multiplicado %d com %d.\n", num1, num2);
            instructions = generateInstructionsMultiply(num1, num2, ramSize, 0, 0);
        }
    } else if (strcmp(argv[1], "fibo") == 0){   //fibo ramSize termos
        ramSize = atoi(argv[2]);

        if(ramSize < 3)
            printf("O tamanho da RAM deve ser superior a 1 para Fibonacci.\n");
        else {
            num1    = atoi(argv[3]);
            printf("Será calculado a sequencia de Fibonacci até o termo %d.\n", num1);
            instructions = generateInstructionsFibonacci(num1, ramSize, 0);
        }
    } else if (strcmp(argv[1], "expo") == 0){   //expo ramSize num1 num2
        ramSize = atoi(argv[2]);
        if(ramSize < 2)
            printf("O tamanho da RAM deve ser superior a 2.\n");
        else {
            num1 = atoi(argv[3]);
            num2 = atoi(argv[4]);
            printf("Será calculado a potencia de %d elevado a %d.\n", num1, num2);
            instructions = generateInstructionsExpo(num1, num2, ramSize, 0, 1);
        }
    } else {
        printf("Opcao invalida.\n");
        return 0;
    }
    
    printf("\033[0m");
    

    printf("Iniciando a maquina...\n");
    start(&machine, instructions, ramSize);
    printRAM(&machine);
    run(&machine);
    printRAM(&machine);
    stop(&machine);
    printf("Finalizando a maquina...\n");
    return 0;
}