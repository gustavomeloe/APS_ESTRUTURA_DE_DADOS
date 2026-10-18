#include "insertionSortModel.h"

void ordenarRegistrosInsertionSort(RegistroQueimada registros[], int tamanho) {
    for (int i = 1; i < tamanho; i++) {
        RegistroQueimada chave = registros[i];
        int j = i - 1;

        while (j >= 0 && registros[j].risco_fogo > chave.risco_fogo) {
            registros[j + 1] = registros[j];
            j--;
        }

        registros[j + 1] = chave;
    }
}