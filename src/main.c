#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Controller/QuickSort/dados_controller.h"
#include "Controller/QuickSort/quick_sort.h"

RegistroFoco* clonar_dados(RegistroFoco* original, int tamanho) {
    RegistroFoco* copia = (RegistroFoco*) malloc(tamanho * sizeof(RegistroFoco));
    if (copia != NULL) {
        memcpy(copia, original, tamanho * sizeof(RegistroFoco));
    }
    return copia;
}

int main() {
    int total_registos = 0;
    
    printf("\n +============================== Log ====================================+\n");
    printf("\n Iniciando leitura dos dados...\n\n");
    RegistroFoco* dados_originais = carregar_dados_csv("src/Resource/dados_queimadas.csv", &total_registos);
    
    if (dados_originais == NULL || total_registos == 0) {
        printf("[ERRO] Erro fatal ao carregar dados. Encerrando processamento.\n");
        return 1;
    }

    printf("\n +================= Teste de Performance dos Algoritmos =================+\n\n"); //Adcionei assim penando no futuro quando tiver todos os algoritmos etc.
    printf("\n Algoritimo: Quick Sort (Ryan R.) \n");
    printf("\n Total de Registos Ordenados: %d \n", total_registos);

    RegistroFoco* dados_ryan = clonar_dados(dados_originais, total_registos);
    
    clock_t inicio_quick = clock();
    executar_quick_sort(dados_ryan, total_registos);
    clock_t fim_quick = clock();
    
    double tempo_quick = (double)(fim_quick - inicio_quick) / CLOCKS_PER_SEC;
    printf("\n Tempo de Ordenacao: %f segundos\n", tempo_quick);
    printf("\n +========================================================================+\n");

    free(dados_ryan);
    free(dados_originais);
    
    return 0;
}