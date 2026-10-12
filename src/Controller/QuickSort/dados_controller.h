#ifndef DADOS_CONTROLLER_H
#define DADOS_CONTROLLER_H

#include "../../Model/QuickSort/modelo_satelite.h"

RegistroFoco* carregar_dados_csv(const char* caminho_arquivo, int* total_registos);

#endif