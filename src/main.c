#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include <fcntl.h>
#include <unistd.h>

#include "Controller/Insertion/insertionSortController.h"
#include "Controller/TreeSort/treeSortController.h"
#include "Model/TreeSort/noModel.h"
#include "Controller/QuickSort/dados_controller.h"
#include "Controller/QuickSort/quick_sort.h"

void heap_sort(float dados[], size_t tamanho);

/* ===================================================================== */
/*                     FUNCOES AUXILIARES DE INTERFACE                   */
/* ===================================================================== */

#define LARGURA 103  /* largura total de cada linha, incluindo as bordas */

/* Separador da tabela do resumo comparativo (soma 103 colunas). */
#define SEP "+---------------------------------+----------------+" \
            "----------------+----------------+----------------+\n"

/* Conta caracteres UTF-8 (printf conta bytes e desalinha com acentos). */
static int larguraUtf8(const char *s) {
    int n = 0;

    for (; *s; s++) {
        if (((unsigned char)*s & 0xC0) != 0x80) {
            n++;
        }
    }

    return n;
}

static void repetir(char c, int n) {
    while (n-- > 0) {
        putchar(c);
    }
}

/* +------------ Titulo ------------+ */
static void linhaTitulo(
    char esq,
    char fill,
    char dir,
    const char *titulo
) {
    int livre = LARGURA - 2 - larguraUtf8(titulo) - 2;
    int a, b;

    if (livre < 0) {
        livre = 0;
    }

    a = livre / 2;
    b = livre - a;

    putchar(esq);
    repetir(fill, a);
    printf(" %s ", titulo);
    repetir(fill, b);
    putchar(dir);
    putchar('\n');
}

/* +--------------------------------+ */
static void linhaFechamento(char esq, char fill, char dir) {
    putchar(esq);
    repetir(fill, LARGURA - 2);
    putchar(dir);
    putchar('\n');
}

/* | texto formatado               | */
static void linhaTexto(const char *fmt, ...) {
    char buf[256];
    va_list ap;
    int pad;

    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);

    pad = LARGURA - 4 - larguraUtf8(buf);

    if (pad < 0) {
        pad = 0;
    }

    printf("| %s", buf);
    repetir(' ', pad);
    printf(" |\n");
}

/* Celula de tabela alinhada por caracteres (nao por bytes). */
static void celula(const char *s, int w) {
    printf("%s", s);
    repetir(' ', w - larguraUtf8(s));
}

/* ===================================================================== */
/*                              BENCHMARK                                */
/* ===================================================================== */

typedef enum {
    ALEATORIOS,
    ORDENADOS,
    INVERTIDOS,
    SEMIORDENADOS
} Cenario;

typedef struct {
    const char *nome;
    double tempos[4];
    int ok[4];
} Resultado;

/* Compara registros pelo risco de fogo. */
static int compararRisco(const void *a, const void *b) {
    float x = ((const RegistroFoco *)a)->risco_fogo;
    float y = ((const RegistroFoco *)b)->risco_fogo;

    return (x > y) - (x < y);
}

/* Verifica se os riscos estao em ordem crescente. */
static int validarRiscos(const RegistroFoco *registros, int total) {
    for (int i = 1; i < total; i++) {
        if (registros[i - 1].risco_fogo >
            registros[i].risco_fogo) {
            return 0;
        }
    }

    return 1;
}

/* Prepara os dados para cada cenario do benchmark. */
static void prepararCenario(
    RegistroFoco *dados,
    int total,
    Cenario cenario
) {
    if (cenario == ALEATORIOS) {
        for (int i = total - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            RegistroFoco temp = dados[i];
            dados[i] = dados[j];
            dados[j] = temp;
        }
    } else {
        qsort(
            dados,
            (size_t)total,
            sizeof(*dados),
            compararRisco
        );

        if (cenario == INVERTIDOS) {
            for (int i = 0; i < total / 2; i++) {
                RegistroFoco temp = dados[i];
                dados[i] = dados[total - 1 - i];
                dados[total - 1 - i] = temp;
            }
        } else if (cenario == SEMIORDENADOS) {
            int trocas = total / 5;

            for (int i = 0; i < trocas; i++) {
                int a = rand() % total;
                int b = rand() % total;
                RegistroFoco temp = dados[a];
                dados[a] = dados[b];
                dados[b] = temp;
            }
        }
    }
}

/* Simulacao controlada de falha de alocacao. */
static int falharProximaAlocacaoBuffer = 0;

static void simularFalhaProximaAlocacaoBuffer(void) {
    falharProximaAlocacaoBuffer = 1;
}

static void *alocarBufferBenchmark(size_t tamanho) {
    if (falharProximaAlocacaoBuffer) {
        falharProximaAlocacaoBuffer = 0;
        return NULL;
    }

    return malloc(tamanho);
}

/* Testa o tratamento de falha na alocacao de um buffer. */
static int testarFalhaBuffer(const char *algoritmo) {
    simularFalhaProximaAlocacaoBuffer();

    void *buffer = alocarBufferBenchmark(sizeof(RegistroFoco));

    if (buffer == NULL) {
        linhaTexto(
            "%-20s | buffer NULL reconhecido; "
            "Algoritmo interrompido",
            algoritmo
        );
        return 1;
    }

    free(buffer);

    linhaTexto(
        "%-20s | ERRO: falha de buffer não detectada",
        algoritmo
    );

    return 0;
}

/* Testa falha de alocacao de um no da Tree Sort. */
static int testarFalhaInternaTreeSort(void) {
    const float entrada[] = {2.0f, 1.0f, 3.0f};
    float saida[3];

    simularFalhaNaAlocacaoDoNo(2);

    int sucesso = ordenarRiscosComArvore(
        entrada,
        3,
        saida
    );

    if (!sucesso) {
        linhaTexto(
            "%-20s | falha interna detectada; "
            "árvore parcial liberada",
            "Tree Sort (nó)"
        );

        return 1;
    }

    linhaTexto(
        "%-20s | ERRO: ordenação continuou após falha de nó",
        "Tree Sort (nó)"
    );

    return 0;
}

/* Carrega o CSV sem deixar o controller imprimir mensagens proprias
   (evita o "Carregando ... registos" duplicado). Erros em stderr
   continuam aparecendo normalmente. */
static RegistroFoco *carregarCsvSilencioso(const char *caminho, int *total) {
    RegistroFoco *dados;

    fflush(stdout);

    int salvo = dup(STDOUT_FILENO);
    int nulo = open("/dev/null", O_WRONLY);

    if (salvo >= 0 && nulo >= 0) {
        dup2(nulo, STDOUT_FILENO);
    }

    if (nulo >= 0) {
        close(nulo);
    }

    dados = carregar_dados_csv(caminho, total);

    fflush(stdout);

    if (salvo >= 0) {
        dup2(salvo, STDOUT_FILENO);
        close(salvo);
    }

    return dados;
}

/* Executa o benchmark completo. */
static int executarBenchmark(int aleatorio) {
    int total = 0;

    printf("\n");
    linhaTitulo('=', '=', '=', "BENCHMARK | ALGORITMOS DE ORDENAÇÃO");
    printf("Iniciando leitura dos dados...\n");

    RegistroFoco *originais = NULL;

    if (!aleatorio) {
        originais = carregarCsvSilencioso(
            "src/Resource/dados_queimadas.csv",
            &total
        );
    } else {
        total = 12000;
        originais = calloc((size_t)total, sizeof(*originais));

        if (originais != NULL) {
            for (int i = 0; i < total; i++) {
                originais[i].risco_fogo =
                    (float)rand() / (float)RAND_MAX;
            }
        }
    }

    if (originais == NULL || total <= 0) {
        fprintf(stderr, "[ERRO] Falha ao carregar/gerar dados.\n");
        free(originais);
        return 1;
    }

    printf("Carregando %d registros na memória.\n", total);

    const char *cenarios[] = {
        "Dados não ordenados",
        "Dados ordenados",
        "Dados invertidos",
        "Dados semi-ordenados (80% ordenado + 20% aleatório)"
    };

    Resultado resultados[4] = {
        {"Quick Sort (Ryan R.)", {0}, {0}},
        {"Tree Sort (Tiago)", {0}, {0}},
        {"Heap Sort (Lucas)", {0}, {0}},
        {"Insertion Sort (Gustavo M.)", {0}, {0}}
    };

    for (int c = 0; c < 4; c++) {
        RegistroFoco *base = malloc(
            (size_t)total * sizeof(*base)
        );

        if (base == NULL) {
            printf(
                "Falha de memória ao criar cenário: %s\n",
                cenarios[c]
            );
            continue;
        }

        memcpy(base, originais, (size_t)total * sizeof(*base));
        prepararCenario(base, total, (Cenario)c);

        for (int alg = 0; alg < 4; alg++) {
            RegistroFoco *copia = alocarBufferBenchmark(
                (size_t)total * sizeof(*copia)
            );

            float *valores = NULL;

            if (copia == NULL) {
                resultados[alg].ok[c] = 0;
                continue;
            }

            memcpy(copia, base, (size_t)total * sizeof(*copia));

            int ok = 1;
            clock_t inicio = clock();

            if (alg == 0) {
                executar_quick_sort(copia, total);
            } else if (alg == 1) {
                float *entrada = alocarBufferBenchmark(
                    (size_t)total * sizeof(*entrada)
                );
                float *saida = alocarBufferBenchmark(
                    (size_t)total * sizeof(*saida)
                );

                if (entrada != NULL && saida != NULL) {
                    for (int i = 0; i < total; i++) {
                        entrada[i] = copia[i].risco_fogo;
                    }

                    ok = ordenarRiscosComArvore(
                        entrada,
                        (size_t)total,
                        saida
                    );

                    for (int i = 1; i < total && ok; i++) {
                        if (saida[i - 1] > saida[i]) {
                            ok = 0;
                        }
                    }
                } else {
                    ok = 0;
                }

                free(entrada);
                free(saida);
            } else if (alg == 2) {
                valores = alocarBufferBenchmark(
                    (size_t)total * sizeof(*valores)
                );

                if (valores != NULL) {
                    for (int i = 0; i < total; i++) {
                        valores[i] = copia[i].risco_fogo;
                    }

                    heap_sort(valores, (size_t)total);
                } else {
                    ok = 0;
                }
            } else {
                for (int i = 1; i < total; i++) {
                    RegistroFoco chave = copia[i];
                    int j = i - 1;

                    while (
                        j >= 0 &&
                        copia[j].risco_fogo > chave.risco_fogo
                    ) {
                        copia[j + 1] = copia[j];
                        j--;
                    }

                    copia[j + 1] = chave;
                }
            }

            clock_t fim = clock();

            if (alg == 0 || alg == 3) {
                ok = validarRiscos(copia, total);
            }

            if (alg == 2 && valores != NULL) {
                for (int i = 1; i < total; i++) {
                    if (valores[i - 1] > valores[i]) {
                        ok = 0;
                    }
                }
            }

            resultados[alg].tempos[c] =
                (double)(fim - inicio) / CLOCKS_PER_SEC;

            resultados[alg].ok[c] = ok;

            free(valores);
            free(copia);
        }

        free(base);
    }

    /* ---------------- Quadros individuais dos algoritmos ------------- */
    for (int alg = 0; alg < 4; alg++) {
        char titulo[100];
        snprintf(titulo, sizeof titulo, "Algoritmo: %s", resultados[alg].nome);

        printf("\n");
        linhaTitulo('+', '-', '+', titulo);
        linhaTexto("Total de registros: %d", total);
        linhaTexto("%s", "");

        const char *nomesCenario[4] = {
            "Dados não ordenados",
            "Dados ordenados",
            "Dados invertidos (ordem decrescente)",
            "Dados semi-ordenados (80% ordenado + 20% aleatório)"
        };

        for (int c = 0; c < 4; c++) {

            linhaTexto("%s", nomesCenario[c]);
            linhaTexto(
                "  Tempo de ordenação: %.6f segundos",
                resultados[alg].tempos[c]
            );

            if (c < 3) {
                linhaTexto("%s", "");
            }
        }

        linhaFechamento('+', '-', '+');
    }

    /* ----------------------- Resumo comparativo ---------------------- */
    printf("\n");
    linhaTitulo('=', '=', '=', "RESUMO COMPARATIVO");

    printf("| "); celula("Algoritmo", 31);
    printf(" | "); celula("Não ordenados", 14);
    printf(" | "); celula("Ordenados", 14);
    printf(" | "); celula("Invertidos", 14);
    printf(" | "); celula("Semi-ordenados", 14);
    printf(" |\n");
    printf(SEP);

    for (int alg = 0; alg < 4; alg++) {
        printf(
            "| %-31s | %12.6f s | %12.6f s | %12.6f s | %12.6f s |\n",
            resultados[alg].nome,
            resultados[alg].tempos[0],
            resultados[alg].tempos[1],
            resultados[alg].tempos[2],
            resultados[alg].tempos[3]
        );
    }

    printf(SEP);

    /* ------------------------- Ranking geral ------------------------- */
    /* Soma dos tempos dos quatro cenarios (aleatorio, ordenado, invertido
       e semiordenado). Menor soma = melhor desempenho geral. */
    int indices[4] = {0, 1, 2, 3};
    double totais[4];

    for (int i = 0; i < 4; i++) {
        const int cen[4] = {ALEATORIOS, ORDENADOS, INVERTIDOS, SEMIORDENADOS};
        double soma = 0.0;
        int valido = 1;

        for (int k = 0; k < 4; k++) {
            if (!resultados[i].ok[cen[k]]) {
                valido = 0;
                break;
            }

            soma += resultados[i].tempos[cen[k]];
        }

        totais[i] = valido ? soma : DBL_MAX;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = i + 1; j < 4; j++) {
            if (totais[indices[j]] < totais[indices[i]]) {
                int temp = indices[i];
                indices[i] = indices[j];
                indices[j] = temp;
            }
        }
    }

    printf("\n");
    linhaTitulo('+', '-', '+', "RANKING DOS ALGORITMOS");

    /* indices[] ja esta ordenado do mais rapido para o mais lento. */
    for (int i = 0; i < 4; i++) {
        int ind = indices[i];
        const char *obs = (i == 0) ? "(mais rápido)"
                        : (i == 3) ? "(mais lento)" : "";

        if (totais[ind] == DBL_MAX) {
            linhaTexto(
                "%d. %-27s - sem resultados válidos",
                i + 1,
                resultados[ind].nome
            );
        } else {
            linhaTexto(
                "%d. %-27s - Total: %.6f s %s",
                i + 1,
                resultados[ind].nome,
                totais[ind],
                obs
            );
        }
    }

    linhaFechamento('+', '-', '+');

    /* ------------------ Testes de falha de memoria ------------------- */
    printf("\n");
    linhaTitulo('+', '-', '+', "TESTES DE FALHA DE MEMÓRIA");

    testarFalhaBuffer("Quick Sort");
    testarFalhaBuffer("Tree Sort");
    testarFalhaBuffer("Heap Sort");
    testarFalhaBuffer("Insertion Sort");
    testarFalhaInternaTreeSort();

    linhaFechamento('+', '-', '+');

    free(originais);

    printf("\n");
    linhaTitulo('+', '=', '+', "Fim do teste de performance");

    return 0;
}

int main(void) {
    int opcao;

    srand((unsigned int)time(NULL));

    printf("\n");
    linhaTitulo('=', '=', '=', "SISTEMA DE BENCHMARK DE ORDENAÇÃO");
    printf("1 - Utilizar dados do arquivo CSV\n");
    printf("2 - Utilizar dados Aleatórios\n");
    linhaFechamento('=', '=', '=');
    printf("Escolha uma opção: ");

    if (scanf("%d", &opcao) != 1) {
        printf("Opção inválida.\n");
        return 1;
    }

    if (opcao != 1 && opcao != 2) {
        printf("Opção inválida.\n");
        return 1;
    }

    return executarBenchmark(opcao == 2);
}