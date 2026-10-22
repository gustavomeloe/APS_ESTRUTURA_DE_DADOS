#include <stdio.h>
#include <stdbool.h>

typedef struct binary_heap{
    int tamanho;
    int no[10];
} bHeap;

bHeap iniciarArvore(){
    bHeap arvore;
    arvore.tamanho = -1;
    return arvore;
}

void add(int valor, bHeap* arvore){
    arvore->tamanho++;
    int i = arvore->tamanho;
    arvore->no[i] = valor;
     
}

int parente(int indice, bHeap* arvore){
    // P ((i-1)/2)
    int indicePai =((indice-1)/2); 
    return indicePai;
}

bool eMaior(int indice1, int indice2, bHeap* arvore) {
    return arvore->no[indice1] > arvore->no[indice2];
}

void swap(int indiceFilho, int indicePai, bHeap* arvore){
    //valor1 filho vai ficar na posicao do pai
    //valor2 pai vai ficar na posicao do filho
    
    int valorNovoPai = arvore->no[indiceFilho];
    int valorNovoFilho = arvore->no[indicePai];

    arvore->no[indiceFilho] = valorNovoFilho;
    arvore->no[indicePai] = valorNovoPai;
}

// FUNCAO INSERT INCOMPLETA (TENHO QUE ATUALIZAR PARA ATUALIZAR TODA A ARVORE)
void insert(int valor, bHeap* arvore){
    add(valor, arvore);

    int i = arvore->tamanho-1;
    int indicePai = parente(i, arvore);
    
    
    
    if (eMaior(i,indicePai, arvore)){
        swap(i,indicePai, arvore);
    }
}


bool estaVazia(bHeap* arvore){
    
    if (arvore->tamanho == -1){
        printf("Arvore vazia. \n");
        return true;
    }
    return false;
    
}

void imprimirArvore(bHeap* arvore){
    if(estaVazia(arvore)){
        return;
    }
    for (int i = 0; i < arvore->tamanho+1; i++){
        printf("\nArvore indice[%d] - Elemento %d. ",i, arvore->no[i]);
    }

}

int main(){
    /*
    BINARY HEAP - uma estrutura de dados em "ARVORE BINARIA", 
    A Origem dessa arvore gera dois dependentes (no esquerdo e no direito) e estes dependentes geram mais dois, e assim sucessivamente. 
    
    Duas configuracoes: MAX-HEAP ou MIN-HEAP.
    MAX-HEAP -> Valor origem tem que ser MENOR que seus dependentes, e o mesmo se aplica para os outros dependentes.

    MIN-HEAP -> Valor de origem tem que ser MENOR que seus dependentes, e o mesmo de aplica para os outros dependentes.


    Seguimos a lógica matemática na qual o indice logico do elemento pai, eh encontrado por P ((i-1)/2).
    traduzindo: ((indice atual - 1) divido por 2).

    Para encontrar o elemento filho partir do pai, em caso de:
    No Esquerdo (2i + 1);
    No Direito (2i + 2).
    
    *continua*
    
    */ 
    
    bHeap arvore = iniciarArvore();
    // Lista para ser ajustada pelo Heap
    int lista[11] = {10,54,32,11,6,5,7,89,90,1};

    imprimirArvore(&arvore);

    int quantidade = sizeof(lista) / sizeof(lista[0]);
    for (int i = 0; i < quantidade-1; i++){
        printf("Indice [%d] - Elemento %d. \n", i, lista[i]);
        insert(lista[i], &arvore);
    }

    imprimirArvore(&arvore);
    return 0;
}