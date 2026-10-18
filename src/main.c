#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Controller/Insertion/insertionSortController.h"

int main(void) {
    int opcao;
    int quantidade;

    printf("=== Insertion Sort - Dados de queimadas ===\n");
    printf("1 - Ler dados do CSV\n");
    printf("2 - Gerar dados aleatorios\n");
    printf("Escolha a opcao: ");

    if (scanf("%d", &opcao) != 1 || (opcao != 1 && opcao != 2)) {
        printf("Opcao invalida.\n");
        return 1;
    }

    printf("Quantidade de registros para ordenar (1 a 120000): ");

    if (scanf("%d", &quantidade) != 1 ||
        quantidade < 1 || quantidade > 120000) {
        printf("Quantidade invalida.\n");
        return 1;
    }

    if (opcao == 1) {
        executarInsertionSortCSV(
            "src/Resource/dados_queimadas.csv",
            quantidade
        );
    } else {
        srand((unsigned int)time(NULL));
        executarInsertionSortAleatorio(quantidade);
    }

    return 0;
}