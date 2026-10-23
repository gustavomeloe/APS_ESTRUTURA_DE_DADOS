#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dados_controller.h"

RegistroFoco* carregar_dados_csv(const char* caminho_arquivo, int* total_registos) {
    FILE* arquivo = fopen(caminho_arquivo, "r");
    if (arquivo == NULL) {
        printf("Erro: Nao foi possivel abrir o ficheiro %s\n", caminho_arquivo);
        return NULL;
    }

    int capacidade = 125000; 
    RegistroFoco* dados = (RegistroFoco*) malloc(capacidade * sizeof(RegistroFoco));
    if (dados == NULL) {
        printf("Erro: Memoria insuficiente.\n");
        fclose(arquivo);
        return NULL;
    }

    char linha[1024]; 
    int contador = 0;

    if (fgets(linha, sizeof(linha), arquivo) == NULL) {
        fclose(arquivo);
        return dados;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL && contador < capacidade) {
        char* token = strtok(linha, ",");
        if (token == NULL) continue;
        dados[contador].latitude = atof(token);
        
        token = strtok(NULL, ",");
        if (token) dados[contador].longitude = atof(token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].data_pas, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].satelite, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].pais, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].estado, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].municipio, token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].bioma, token);
        
        token = strtok(NULL, ",");
        if (token) dados[contador].numero_dias_sem_chuva = atoi(token);
        
        token = strtok(NULL, ",");
        if (token) dados[contador].precipitacao = atof(token);
        
        token = strtok(NULL, ",");
        if (token) dados[contador].risco_fogo = atof(token);
        
        token = strtok(NULL, ",");
        if (token) strcpy(dados[contador].id_area_industrial, token);
        
        token = strtok(NULL, ",");
        if (token) dados[contador].frp = atof(token);

        contador++;
    }

    fclose(arquivo);
    *total_registos = contador; 
    

    printf(" Carregando %d registos na memoria.\n", contador);
    return dados;
}