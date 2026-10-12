#ifndef MODELO_SATELITE_H
#define MODELO_SATELITE_H

typedef struct {
    float latitude;
    float longitude;
    char data_pas[25];
    char satelite[30];
    char pais[30];
    char estado[30];
    char municipio[60];
    char bioma[30];
    int numero_dias_sem_chuva;
    float precipitacao;
    float risco_fogo;
    char id_area_industrial[20]; 
    float frp;                   
} RegistroFoco;

#endif