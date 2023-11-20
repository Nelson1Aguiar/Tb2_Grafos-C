#include "lib.h"

void menu() {
    Grafo* grafo;
    Grafo* arvoreGeradoraMin;
    int op,partida,destino;

    grafo=lerArquivo(grafo);

    Grafo* componentes[DFS(grafo)];
    arvoreGeradoraMin=kruskal(grafo);

    do{
        puts("------------------------------DIGITE SUA OPCAO------------------------------\n\n");
        printf("\t1 - Exibir grafo\n\t2 - Exibir arvore geradora minima\n\t3 - Informar valor de menor caminho\n\t4- Gerar arquivo de arvore de caminhos minimos\n\t0 - Sair\n");
        scanf("%d",&op);
        switch (op) {
            case 1:
                printGrafo(grafo);
                break;
            case 2:
                printGrafo(arvoreGeradoraMin);
                break;
            case 3:
                printf("\nInforme o vertice de partida: ");
                scanf("%d",&partida);
                printf("\nInforme o vertice de destino: ");
                scanf("%d",&destino);
                dijkstra(arvoreGeradoraMin,partida,destino);
                break;
            case 4:
                gerarArquivo(arvoreGeradoraMin);
                break;
            case 5:
                DFS(grafo);
                break;
            case 0:
                for (int i = 0; i < grafo->v; i++) {
                    liberarLista(grafo->listaAdj[i]);
                }
                free(grafo);
                break;
            default:
                printf("OPCAO INVALIDA!\n");
                break;
        }
    }while(op!=0);
}

Grafo* lerArquivo(Grafo* grafo){
    FILE *arquivo;
    char linha[100];
    int v, cont = 0;

    arquivo = fopen("C:\\Users\\User\\Desktop\\Grafo.txt", "r");

    if (arquivo == NULL) {
        printf("Não foi possível abrir o arquivo.\n");
        return NULL;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        cont += 1;
        if (cont == 1) {
            v = atoi(linha);
            grafo = criarGrafo(v);
        }
        else if(cont>2){
            int origem;
            int destino;
            float peso;
            char* token = strtok(linha, " ");
            origem = atoi(token);

            token = strtok(NULL, " ");
            destino = atoi(token);

            token = strtok(NULL, " ");
            peso=atof(token);

            addAresta(grafo, origem, destino,peso);
        }
    }
    fclose(arquivo);
    return grafo;
}



//FUNÇÃO QUE CRIA UM NO PARA A LISTA DE ADJACÊNCIA
Lista* criarNo(int num,float peso) {
    Lista* novoNo = (Lista*)malloc(sizeof(Lista));
    if (novoNo == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    novoNo->num = num;
    novoNo->peso=peso;
    novoNo->prox = NULL;
    return novoNo;
}

//FUNÇÃO QUE DESALOCA A LISTA DE ADJACÊNCIA DE CADA VÉRTICE
void liberarLista(Lista* lista){
    while (lista != NULL) {
        Lista* temp = lista;
        lista = lista->prox;
        free(temp);
    }
}

//FUNÇÃO QUE ALOCA O GRAFO COM TODOS OS SEUS VÉRTICES E SUAS LISTAS DE ADJACÊNCIA
Grafo* criarGrafo(int v){
    Grafo* grafo = (Grafo *)malloc(sizeof(Grafo));
    grafo->v = v;
    grafo->listaAdj = (Lista**)malloc(v * sizeof(Lista*));
    for (int i = 0; i < v; i++) {
        grafo->listaAdj[i] = NULL;
    }
    return grafo;
}

//FUNÇÃO QUE ADICIONA UMA ARESTA NO GRAFO
void addAresta(Grafo* grafo, int origem, int fim,float peso) {
    Lista* novoNo1 = criarNo(fim,peso);
    novoNo1->prox = grafo->listaAdj[origem];
    grafo->listaAdj[origem] = novoNo1;

    Lista* novoNo2 = criarNo(origem,peso);
    novoNo2->prox = grafo->listaAdj[fim];
    grafo->listaAdj[fim] = novoNo2;
}

//FUNÇÃO DE EXIBIR O GRAFO EM FORMA DE LISTA DE ADJACÊNCIA
void printGrafo(Grafo* grafo) {
    system("cls");
    for (int i = 0; i < grafo->v; i++) {
        Lista* corrente = grafo->listaAdj[i];
        printf("Vertice %d: ", i);
        while (corrente != NULL) {
            printf("%d, ", corrente->num);
            printf("%.1f -> ", corrente->peso);
            printf("|");
            corrente = corrente->prox;
        }
        printf("NULL\n");
    }
}

void DFSRecursivo(Grafo* grafo, int vertice, int visitados[]) {
    visitados[vertice] = 1;

    Lista* corrente = grafo->listaAdj[vertice];
    while (corrente != NULL) {
        int destino = corrente->num;
        if (!visitados[destino]) {
            DFSRecursivo(grafo, destino, visitados);
        }
        corrente = corrente->prox;
    }
}

// Função de busca em profundidade
int DFS(Grafo* grafo) {
    Grafo* componentes[100];
    int V = grafo->v,qtdComponentes=0;
    int *visitados = (int *)malloc(V * sizeof(int));

    // Inicializa o array de visitados
    for (int i = 0; i < V; i++) {
        visitados[i] = 0;
    }

    // Chama a função de busca em profundidade recursiva para cada vértice não visitado
    for (int i = 0; i < V; i++) {
        if (!visitados[i]) {
            DFSRecursivo(grafo, i, visitados);
            qtdComponentes+=1;
        }
    }
    return qtdComponentes;
}

int buscar(int subsets[], int i) {
    if (subsets[i] == -1)
        return i;
    return buscar(subsets, subsets[i]);
}

void unir(int subsets[], int x, int y) {
    int raiz_x = buscar(subsets, x);
    int raiz_y = buscar(subsets, y);
    subsets[raiz_x] = raiz_y;
}

int compararPeso(const void* a, const void* b) {
    Aresta* aresta1 = (Aresta*)a;
    Aresta* aresta2 = (Aresta*)b;
    return (aresta1->peso > aresta2->peso) - (aresta1->peso < aresta2->peso);
}

Grafo* kruskal(Grafo* grafo) {
    int V = grafo->v;
    Grafo* arvoreGeradoraMin= criarGrafo(V);
    Aresta* resultado = (Aresta*)malloc((V - 1) * sizeof(Aresta));
    int indice_resultado = 0;

    Aresta* arestas = (Aresta*)malloc(V * V * sizeof(Aresta));
    int indice_arestas = 0;

    for (int i = 0; i < V; ++i) {
        Lista* atual = grafo->listaAdj[i];
        while (atual != NULL) {
            if (i < atual->num) {
                arestas[indice_arestas].origem = i;
                arestas[indice_arestas].destino = atual->num;
                arestas[indice_arestas].peso = atual->peso;
                indice_arestas++;
            }
            atual = atual->prox;
        }
    }

    qsort(arestas, indice_arestas, sizeof(Aresta), compararPeso);

    int* subsets = (int*)malloc(V * sizeof(int));
    for (int v = 0; v < V; v++)
        subsets[v] = -1;

    for (int i = 0; i < indice_arestas && indice_resultado < V - 1; ++i) {
        int x = buscar(subsets, arestas[i].origem);
        int y = buscar(subsets, arestas[i].destino);

        if (x != y) {
            resultado[indice_resultado++] = arestas[i];
            unir(subsets, x, y);
        }
    }
    for (int i = 0; i < V - 1; i++) {
        addAresta(arvoreGeradoraMin,resultado[i].origem,resultado[i].destino,resultado[i].peso);
    }
    return arvoreGeradoraMin;
}

void dijkstra(Grafo* grafo, int origem, int destino) {
    int V = grafo->v,cont=V-1;
    float dist[V]; // Armazena as distâncias mínimas
    int visitado[V],rota[V];// Marca os vértices visitados

    // Inicializa as distâncias e os vértices visitados
    for (int i = 0; i < V; ++i) {
        dist[i] = FLT_MAX;
        visitado[i] = 0;
        rota[i]=INT_MAX;
    }

    // A distância de origem para ela mesma é sempre 0
    dist[origem] = 0;

    // Encontrar o caminho mais curto para todos os vértices
    for (int cont = 0; cont < V - 1; ++cont) {
        int u = -1;  // Inicializa u como -1 para detectar se nenhum vértice foi escolhido ainda
        float min_dist = FLT_MAX;

        // Escolhe o vértice com a menor distância ainda não visitado
        for (int i = 0; i < V; ++i) {
            if (!visitado[i] && dist[i] < min_dist) {
                min_dist = dist[i];
                u = i;
            }
        }

        // Verifica se algum vértice foi escolhido
        if (u == -1) {
            // Todos os vértices atingíveis foram visitados
            break;
        }

        // Marca o vértice escolhido como visitado
        visitado[u] = 1;

        // Atualiza a distância dos vértices adjacentes ao vértice escolhido
        Lista* temp = grafo->listaAdj[u];
        while (temp != NULL) {
            int v = temp->num;
            if (!visitado[v] && dist[u] != FLT_MAX && dist[u] + temp->peso < dist[v]) {
                dist[v] = dist[u] + temp->peso;
                grafo->listaAdj[v]->pai=u;
            }
            temp = temp->prox;
        }
    }
    //Ver a rota mínima entre a origem e o destino
    int corrent=destino;
    rota[cont]=corrent;
    cont-=1;
    while (corrent!=origem){
        corrent=grafo->listaAdj[corrent]->pai;
        rota[cont]=corrent;
        cont-=1;
    }

    // Exibe a distância mínima do vértice de origem para o destino
    printf("\nA menor distancia entre os vertices %d e %d: %.1f\n", origem, destino, dist[destino]);
    printf("Rota: ");
    for(int i=0;i<V;i++){
        if(rota[i]!=INT_MAX){
            printf("%d ",rota[i]);
        }
    }
    printf("\n");
}
void gerarArquivo(Grafo* grafo){
    FILE *arquivo; // Ponteiro para o arquivo

    // Abre o arquivo para escrita. Se o arquivo não existir, ele será criado.
    // Se o arquivo existir, seu conteúdo será truncado.
    arquivo = fopen("C:\\Users\\User\\Desktop\\Grafo novo.txt", "w");

    // Verifica se a abertura do arquivo foi bem-sucedida
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;// Encerra o programa com código de erro
    }

    // Escreve no arquivo
    for (int i = 0; i < grafo->v; i++) {
        Lista* corrente = grafo->listaAdj[i];
        while (corrente != NULL) {
            fprintf(arquivo, "%d %d %.2f\n",i,corrente->num,corrente->peso);
            corrente = corrente->prox;
        }
    }
    // Fecha o arquivo
    fclose(arquivo);

    printf("Arquivo criado com sucesso.\n");
}