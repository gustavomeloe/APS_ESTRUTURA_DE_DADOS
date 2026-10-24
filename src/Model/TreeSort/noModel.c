#include <stdio.h>
#include <stdlib.h>
#include "noModel.h"

/* Usado somente pelo teste controlado de falha de memoria. Zero desativa. */
static size_t falharNaAlocacaoDoNo = 0;
static size_t contadorAlocacaoNo = 0;

void simularFalhaNaAlocacaoDoNo(size_t numeroAlocacao) {
    falharNaAlocacaoDoNo = numeroAlocacao;
    contadorAlocacaoNo = 0;
}

NoModel* criarNo(float valor) {
    contadorAlocacaoNo++;
    if (falharNaAlocacaoDoNo != 0 &&
        contadorAlocacaoNo == falharNaAlocacaoDoNo) {
        falharNaAlocacaoDoNo = 0;
        return NULL;
    }

    NoModel* novo = malloc(sizeof(NoModel));
    if (novo == NULL) {
        return NULL;
    }

    novo->valor = valor;
    novo->repeticoes = 1;
    novo->esquerda = NULL;
    novo->direita = NULL;

    return novo;
}

int inserir(NoModel** raiz, float valor) {
    if (*raiz == NULL) {
        *raiz = criarNo(valor);
        return *raiz != NULL;
    }

    if (valor < (*raiz)->valor) {
        return inserir(&(*raiz)->esquerda, valor);
    }
    if (valor > (*raiz)->valor) {
        return inserir(&(*raiz)->direita, valor);
    }

    (*raiz)->repeticoes++;
    return 1;
}

void emOrdem(const NoModel* raiz, float* saida, size_t* indice) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda, saida, indice);
        for (size_t i = 0; i < raiz->repeticoes; i++) {
            saida[(*indice)++] = raiz->valor;
        }
        emOrdem(raiz->direita, saida, indice);
    }
}

void destruirArvore(NoModel* raiz) {
    if (raiz != NULL) {
        destruirArvore(raiz->esquerda);
        destruirArvore(raiz->direita);
        free(raiz);
    }
}