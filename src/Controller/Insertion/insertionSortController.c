#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../../Model/Insertion/insertionSortModel.h"
#include "insertionSortController.h"

static int extrairRiscoFogo(const char *linha, double *riscoFogo) {
    char copia[TAMANHO_LINHA_CSV];
    char *campo;
    int indice = 0;

    snprintf(copia, sizeof(copia), "%s", linha);
    campo = strtok(copia, ",");

    while (campo != NULL) {
        if (indice == 10) {
            char *fim;
            double valor = strtod(campo, &fim);

            if (fim == campo) {
                return 0;
            }

            *riscoFogo = valor;
            return 1;
        }

        campo = strtok(NULL, ",");
        indice++;
    }

    return 0;
}

static void exibirRegistros(
    const RegistroQueimada registros[],
    int total,
    const char *titulo
) {
    int limite = total < 10 ? total : 10;

    printf("\n=== %s ===\n", titulo);

    for (int i = 0; i < limite; i++) {
        printf("Risco: %.6f | %s\n",
               registros[i].risco_fogo,
               registros[i].linha);
    }

    if (total > limite) {
        printf("Exibindo os primeiros %d de %d registros.\n",
               limite, total);
    }
}

static int validarOrdenacao(
    const RegistroQueimada registros[],
    int total
) {
    for (int i = 1; i < total; i++) {
        if (registros[i - 1].risco_fogo > registros[i].risco_fogo) {
            return 0;
        }
    }

    return 1;
}

static void ordenarEExibir(RegistroQueimada registros[], int total) {
    exibirRegistros(registros, total, "Antes da ordenacao");

    clock_t inicio = clock();
    ordenarRegistrosInsertionSort(registros, total);
    clock_t fim = clock();

    double tempoSegundos = (double)(fim - inicio) / CLOCKS_PER_SEC;

    exibirRegistros(
        registros,
        total,
        "Depois da ordenacao (risco_fogo crescente)"
    );

    printf("\n=== Validacao ===\n");

    if (validarOrdenacao(registros, total)) {
        printf("Validacao: ordenacao correta.\n");
    } else {
        printf("Validacao: ERRO na ordenacao.\n");
    }

    printf("\n=== Desempenho ===\n");
    printf("Registros ordenados: %d\n", total);
    printf("Tempo de ordenacao: %.6f segundos\n", tempoSegundos);
}

void executarInsertionSortCSV(const char *caminhoArquivo, int quantidade) {
    FILE *arquivo = fopen(caminhoArquivo, "r");

    if (arquivo == NULL) {
        printf("Erro: nao foi possivel abrir o arquivo '%s'.\n",
               caminhoArquivo);
        printf("Execute o programa a partir da pasta raiz do projeto.\n");
        return;
    }

    RegistroQueimada *registros =
        malloc((size_t)quantidade * sizeof(RegistroQueimada));

    if (registros == NULL) {
        printf("Erro: nao foi possivel reservar memoria para os registros.\n");
        fclose(arquivo);
        return;
    }

    char linha[TAMANHO_LINHA_CSV];

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        printf("Erro: o arquivo CSV esta vazio.\n");
        free(registros);
        fclose(arquivo);
        return;
    }

    int total = 0;
    int linhasInvalidas = 0;

    while (total < quantidade &&
           fgets(linha, sizeof(linha), arquivo) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (linha[0] == '\0') {
            continue;
        }

        double riscoFogo;

        if (!extrairRiscoFogo(linha, &riscoFogo)) {
            linhasInvalidas++;
            continue;
        }

        snprintf(registros[total].linha,
                 sizeof(registros[total].linha),
                 "%s",
                 linha);

        registros[total].risco_fogo = riscoFogo;
        total++;
    }

    fclose(arquivo);

    if (total == 0) {
        printf("Nenhum registro valido foi encontrado no CSV.\n");
        free(registros);
        return;
    }

    printf("\nRegistros carregados: %d\n", total);
    ordenarEExibir(registros, total);

    if (linhasInvalidas > 0) {
        printf("Linhas ignoradas por formato invalido: %d\n",
               linhasInvalidas);
    }

    free(registros);
}

void executarInsertionSortAleatorio(int quantidade) {
    RegistroQueimada *registros =
        malloc((size_t)quantidade * sizeof(RegistroQueimada));

    if (registros == NULL) {
        printf("Erro: nao foi possivel reservar memoria para os registros.\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        double riscoFogo =
            (double)rand() / (double)RAND_MAX * 100.0;

        snprintf(registros[i].linha,
                 sizeof(registros[i].linha),
                 "Registro aleatorio %d",
                 i + 1);

        registros[i].risco_fogo = riscoFogo;
    }

    printf("\nRegistros gerados: %d\n", quantidade);
    ordenarEExibir(registros, quantidade);

    free(registros);
}