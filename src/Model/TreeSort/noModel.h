#ifndef NO_MODEL_H
#define NO_MODEL_H

#include <stddef.h>

typedef struct NoModel {
    unsigned char valor;
    size_t repeticoes;
    struct NoModel *esquerda;
    struct NoModel *direita;
} NoModel;

NoModel* criarNo(unsigned char valor);
int inserir(NoModel** raiz, unsigned char valor);
void emOrdem(const NoModel* raiz, unsigned char* saida, size_t* indice);
void destruirArvore(NoModel* raiz);

#endif