#include "treeSortController.h"
#include "../../Model/TreeSort/noModel.h"

int ordenarComArvore(const unsigned char* entrada, size_t tamanho, unsigned char* saida) {
    if (tamanho > 0 && (entrada == NULL || saida == NULL)) {
        return 0;
    }

    NoModel* raiz = NULL;
    for (size_t i = 0; i < tamanho; i++) {
        if (!inserir(&raiz, entrada[i])) {
            destruirArvore(raiz);
            return 0;
        }
    }

    size_t indice = 0;
    emOrdem(raiz, saida, &indice);
    destruirArvore(raiz);
    return indice == tamanho;
}