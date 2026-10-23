# APS – QUICKSORT

## Introdução

Este projeto foi desenvolvido para a matéria semestral Atividade Prática Supervisionada, vinculada à disciplina de Estrutura de Dados.

O objetivo foi implementar o algoritmo de ordenação **QuickSort** aplicado sobre um conjunto real de dados de focos de queimadas, lidos a partir de um arquivo CSV.

---

## QuickSort

O QuickSort é um algoritmo de ordenação baseado na estratégia de **divisão e conquista**. A ideia central é escolher um elemento chamado **pivô** e reorganizar os demais elementos de forma que todos os menores fiquem à sua esquerda e todos os maiores à sua direita. Esse processo é repetido recursivamente para cada sub-array resultante.

Nesta implementação, o pivô é sempre o **elemento do meio** do intervalo atual. A partição utiliza a abordagem de dois ponteiros que caminham em direções opostas até se cruzarem, garantindo que a ordenação ocorra **no próprio array** (in-place), sem alocação de memória extra.

O campo utilizado como critério de ordenação é o **risco_fogo** de cada registro, um valor float presente nos dados de queimadas.

O QuickSort possui complexidade **O(n log n)** no caso médio e **O(n²)** no pior caso, embora a escolha do pivô pelo elemento do meio reduza bastante a probabilidade de cair no pior caso.

---

## Estrutura do Projeto

```
APS_ESTRUTURA_DE_DADOS/
├── src/
│   ├── main.c
│   ├── Model/
│   │   └── QuickSort/
│   │       └── modelo_satelite.h
│   ├── Controller/
│   │   └── QuickSort/
│   │       ├── dados_controller.h
│   │       ├── dados_controller.c
│   │       ├── quick_sort.h
│   │       └── quick_sort.c
│   └── Resource/
│       └── dados_queimadas.csv
└── README.md
```

---

## TAD RegistroFoco

Estrutura que representa um registro de foco de queimada lido do CSV.

```c
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
```

Cada campo corresponde a uma coluna do arquivo CSV. O campo principal para a ordenação é o **risco_fogo**.

---

## Funções Implementadas

### carregar_dados_csv()

Realiza a leitura do arquivo CSV e carrega os registros na memória dinamicamente.

Aloca espaço para até 125.000 registros, lê linha por linha com `fgets` e tokeniza cada campo com `strtok`. A primeira linha do arquivo (cabeçalho) é descartada automaticamente.

**Parâmetros:** caminho do arquivo e ponteiro para o contador de registros.  
**Retorno:** ponteiro para o array de `RegistroFoco` alocado em heap.

---

### executar_quick_sort()

Ponto de entrada do algoritmo. Verifica se o array possui mais de um elemento e chama a função recursiva interna.

**Parâmetros:** ponteiro para o array de `RegistroFoco` e o tamanho total.

---

### quick_sort_recursivo()

Função recursiva que divide o array ao redor do pivô e ordena cada metade independentemente.

**Parâmetros:** ponteiro para o array, índice de início e índice de fim do intervalo atual.

---

### particionar()

Realiza a partição do array em torno do pivô (elemento do meio). Usa dois ponteiros `i` e `j` que avançam em direções opostas. Quando `i >= j`, retorna `j` como ponto de divisão.

**Parâmetros:** ponteiro para o array, índice de início e índice de fim.  
**Retorno:** índice do ponto de divisão.

---

### trocar()

Realiza a troca de dois elementos do tipo `RegistroFoco` utilizando uma variável temporária.

**Parâmetros:** dois ponteiros para `RegistroFoco`.

---

### clonar_dados()

Cria uma cópia independente do array original utilizando `memcpy`. Isso permite que o array original permaneça intacto enquanto cada algoritmo ordena sua própria cópia.

**Parâmetros:** ponteiro para o array original e o tamanho.  
**Retorno:** ponteiro para o novo array clonado.

---

### clock()

Função da biblioteca `<time.h>` utilizada para medir o tempo de execução da ordenação. O tempo é calculado pela diferença entre os valores de `clock_t` capturados antes e depois da chamada do algoritmo, dividido por `CLOCKS_PER_SEC`.

---

## Como Compilar e Executar

Certifique-se de ter o GCC instalado. A partir da raiz do projeto:

```bash
gcc src/main.c src/Controller/QuickSort/dados_controller.c src/Controller/QuickSort/quick_sort.c -o aps_ordenacao_quickSort
```

Em seguida, execute:

```bash
./aps_ordenacao_quickSort
```

O programa irá carregar os dados do CSV, ordenar pelo campo `risco_fogo` e exibir o tempo de execução no terminal.
