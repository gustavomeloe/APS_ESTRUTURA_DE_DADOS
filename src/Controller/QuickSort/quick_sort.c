#include "quick_sort.h"

void trocar(RegistroFoco* a, RegistroFoco* b) {
    RegistroFoco temp = *a;
    *a = *b;
    *b = temp;
}

int particionar(RegistroFoco* array, int inicio, int fim) {
    
    int meio = inicio + (fim - inicio) / 2;
    float pivo = array[meio].risco_fogo; 
    
    int i = inicio - 1;
    int j = fim + 1;

    while (1) {
        
        do {
            i++;
        } while (array[i].risco_fogo < pivo);
  
        do {
            j--;
        } while (array[j].risco_fogo > pivo);

        if (i >= j) {
            return j;
        }

        trocar(&array[i], &array[j]);
    }
}

void quick_sort_recursivo(RegistroFoco* array, int inicio, int fim) {
    if (inicio < fim) {
        int pi = particionar(array, inicio, fim);

        quick_sort_recursivo(array, inicio, pi);
        quick_sort_recursivo(array, pi + 1, fim);
    }
}

void executar_quick_sort(RegistroFoco* array, int tamanho) {
    if (tamanho > 1) {
        quick_sort_recursivo(array, 0, tamanho - 1);
    }
}