#ifndef INSERTION_SORT_MODEL_H
#define INSERTION_SORT_MODEL_H

#define TAMANHO_LINHA_CSV 2048

typedef struct {
    char linha[TAMANHO_LINHA_CSV];
    double risco_fogo;
} RegistroQueimada;

void ordenarRegistrosInsertionSort(RegistroQueimada registros[], int tamanho);

#endif