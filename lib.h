#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <float.h>

typedef struct lista {
    int num;
    float peso;
    struct lista* prox;
}Lista;

typedef struct grafo{
    int v;  //quantidade de vertices
    Lista **listaAdj;
}Grafo;

typedef struct aresta {
    int origem, destino;
    float peso;
} Aresta;

void menu();
Grafo* lerArquivo(Grafo *grafo);
Lista* criarNo(int num,float peso);
void liberarLista(Lista* lista);
Grafo* criarGrafo(int v);
void addAresta(Grafo* grafo, int origem, int fim,float peso);
void printGrafo(Grafo* grafo);
int buscar(int subsets[], int i);
void unir(int subsets[], int x, int y);
int compararPeso(const void* a, const void* b);
Grafo* kruskal(Grafo* grafo);
void dijkstra(Grafo* grafo, int origem, int destino);

