#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Controller/Insertion/insertionSortController.h"
#include "Controller/TreeSort/treeSortController.h"

static int lerInteiro(const char* mensagem, int* valor) {
    char linha[128];
    char* fim;

    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    errno = 0;
    long resultado = strtol(linha, &fim, 10);
    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (errno != 0 || fim == linha || *fim != '\0' ||
        resultado < INT_MIN || resultado > INT_MAX) {
        return 0;
    }

    *valor = (int)resultado;
    return 1;
}

static int lerTamanho(const char* mensagem, size_t* tamanho) {
    char linha[128];
    char* fim;

    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL) {
        return 0;
    }

    char* inicio = linha;
    while (isspace((unsigned char)*inicio)) {
        inicio++;
    }

    if (*inicio == '-') {
        return 0;
    }

    errno = 0;
    unsigned long long resultado = strtoull(inicio, &fim, 10);

    while (isspace((unsigned char)*fim)) {
        fim++;
    }

    if (errno != 0 || fim == inicio || *fim != '\0' ||
        resultado > SIZE_MAX) {
        return 0;
    }

    *tamanho = (size_t)resultado;
    return 1;
}

static int carregarArquivo(
    const char* caminho,
    unsigned char** dados,
    size_t* tamanho
) {
    FILE* arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        perror("Não foi possível abrir o arquivo");
        return 0;
    }

    size_t capacidade = 256;
    unsigned char* buffer = malloc(capacidade);
    if (buffer == NULL) {
        fclose(arquivo);
        return 0;
    }

    int caractere;
    while ((caractere = fgetc(arquivo)) != EOF) {
        if (isspace((unsigned char)caractere)) {
            continue;
        }

        if (*tamanho == capacidade) {
            if (capacidade > SIZE_MAX / 2) {
                free(buffer);
                fclose(arquivo);
                return 0;
            }

            capacidade *= 2;
            unsigned char* maior = realloc(buffer, capacidade);
            if (maior == NULL) {
                free(buffer);
                fclose(arquivo);
                return 0;
            }

            buffer = maior;
        }

        buffer[(*tamanho)++] = (unsigned char)caractere;
    }

    if (ferror(arquivo)) {
        free(buffer);
        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);
    *dados = buffer;
    return 1;
}

static void exibirConteudo(
    const char* rotulo,
    const unsigned char* dados,
    size_t tamanho
) {
    size_t exibidos = tamanho < 100 ? tamanho : 100;

    printf("%s (%zu caracteres): ", rotulo, tamanho);

    if (exibidos == 0) {
        printf("(vazio)");
    } else {
        fwrite(dados, sizeof(*dados), exibidos, stdout);
    }

    if (tamanho > exibidos) {
        printf("... [mostrando %zu de %zu]", exibidos, tamanho);
    }

    printf("\n");
}

static int gerarAleatorio(
    int tipo,
    unsigned char* dados,
    size_t tamanho
) {
    const char* numeros = "0123456789";
    const char* letras = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const char* misto = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const char* caracteres;

    if (tipo == 1) {
        caracteres = numeros;
    } else if (tipo == 2) {
        caracteres = letras;
    } else if (tipo == 3) {
        caracteres = misto;
    } else {
        return 0;
    }

    size_t opcoes = strlen(caracteres);

    for (size_t i = 0; i < tamanho; i++) {
        dados[i] = (unsigned char)caracteres[rand() % opcoes];
    }

    return 1;
}

static int executarInsertionSort(void) {
    int origem;
    int quantidade;

    if (!lerInteiro(
            "Dados para o Insertion Sort:\n"
            "1 - Ler dados do CSV\n"
            "2 - Gerar dados aleatórios\n"
            "Escolha a opção: ",
            &origem) ||
        (origem != 1 && origem != 2)) {
        fprintf(stderr, "Opção inválida.\n");
        return 1;
    }

    if (!lerInteiro(
            "Quantidade de registros para ordenar (1 a 120000): ",
            &quantidade) ||
        quantidade < 1 || quantidade > 120000) {
        fprintf(stderr, "Quantidade inválida.\n");
        return 1;
    }

    if (origem == 1) {
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

static int executarTreeSort(void) {
    int origem;

    if (!lerInteiro(
            "Dados para o Tree Sort:\n"
            "1 - Carregar arquivo txt\n"
            "2 - Gerar aleatoriamente\n"
            "Opção: ",
            &origem)) {
        fprintf(stderr, "Opção inválida.\n");
        return 1;
    }

    unsigned char* entrada = NULL;
    size_t tamanho = 0;

    if (origem == 1) {
        char caminho[1024];

        printf("Caminho do arquivo txt: ");

        if (fgets(caminho, sizeof(caminho), stdin) == NULL) {
            fprintf(stderr, "Não foi possível ler o caminho.\n");
            return 1;
        }

        caminho[strcspn(caminho, "\r\n")] = '\0';

        if (!carregarArquivo(caminho, &entrada, &tamanho)) {
            fprintf(stderr, "Falha ao ler o arquivo.\n");
            return 1;
        }

        printf("Espaços e quebras de linha do arquivo serão ignorados.\n");
    } else if (origem == 2) {
        int tipo;

        if (!lerInteiro(
                "Tipo de conteúdo aleatório:\n"
                "1 - Números\n"
                "2 - Letras\n"
                "3 - Misturado\n"
                "Opção: ",
                &tipo) ||
            tipo < 1 || tipo > 3) {
            fprintf(stderr, "Tipo inválido.\n");
            return 1;
        }

        if (!lerTamanho("Quantidade de caracteres: ", &tamanho)) {
            fprintf(stderr, "Quantidade inválida.\n");
            return 1;
        }

        entrada = malloc(tamanho == 0 ? 1 : tamanho);
        if (entrada == NULL) {
            fprintf(stderr, "Não foi possível reservar memória para os dados.\n");
            return 1;
        }

        srand((unsigned int)time(NULL));

        if (!gerarAleatorio(tipo, entrada, tamanho)) {
            free(entrada);
            fprintf(stderr, "Não foi possível gerar os dados.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Origem inválida.\n");
        return 1;
    }

    unsigned char* saida = malloc(tamanho == 0 ? 1 : tamanho);
    if (saida == NULL) {
        free(entrada);
        fprintf(stderr, "Não foi possível reservar memória para a ordenação.\n");
        return 1;
    }

    exibirConteudo("Antes", entrada, tamanho);

    clock_t inicio = clock();
    int sucesso = ordenarComArvore(entrada, tamanho, saida);
    clock_t fim = clock();

    if (!sucesso) {
        free(entrada);
        free(saida);
        fprintf(stderr, "Não foi possível ordenar os dados.\n");
        return 1;
    }

    exibirConteudo("Depois", saida, tamanho);

    if (inicio != (clock_t)-1 && fim != (clock_t)-1) {
        printf(
            "Tempo de ordenação: %.3f ms\n",
            1000.0 * (double)(fim - inicio) / CLOCKS_PER_SEC
        );
    } else {
        printf("Tempo de ordenação indisponível.\n");
    }

    free(entrada);
    free(saida);
    return 0;
}

int main(void) {
    int algoritmo;

    if (!lerInteiro(
            "Escolha o algoritmo:\n"
            "1 - Insertion Sort (registros de queimadas)\n"
            "2 - Tree Sort (caracteres)\n"
            "Opção: ",
            &algoritmo)) {
        fprintf(stderr, "Opção inválida.\n");
        return 1;
    }

    if (algoritmo == 1) {
        return executarInsertionSort();
    }

    if (algoritmo == 2) {
        return executarTreeSort();
    }

    fprintf(stderr, "Algoritmo inválido.\n");
    return 1;
}