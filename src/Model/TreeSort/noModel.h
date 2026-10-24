#ifndef NO_MODEL_H
#define NO_MODEL_H

#include <stddef.h>

typedef struct NoModel {
    float valor;
    size_t repeticoes;
    struct NoModel *esquerda;
    struct NoModel *direita;
} NoModel;

NoModel* criarNo(float valor);
void simularFalhaNaAlocacaoDoNo(size_t numeroAlocacao);
int inserir(NoModel** raiz, float valor);
void emOrdem(const NoModel* raiz, float* saida, size_t* indice);
void destruirArvore(NoModel* raiz);

#endif