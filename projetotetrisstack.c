#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5  // tamanho fixo da fila

// Struct que representa uma peça do Tetris
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único da peça
} Peca;

// Struct que representa a fila circular de peças
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// ---------- FUNÇÕES AUXILIARES ----------

// Inicializa a fila vazia
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila* fila) {
    return fila->quantidade == MAX_FILA;
}

// Verifica se a fila está vazia
int filaVazia(Fila* fila) {
    return fila->quantidade == 0;
}

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.nome = tipos[rand() % 4]; // escolhe aleatoriamente um tipo
    nova.id = id;
    return nova;
}

// Insere nova peça no final da fila (enqueue)
void enfileirar(Fila* fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n⚠️  A fila está cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
    printf("\n✅ Peça [%c %d] adicionada com sucesso!\n", nova.nome, nova.id);
}

// Remove a peça da frente da fila (dequeue)
void desenfileirar(Fila* fila) {
    if (filaVazia(fila)) {
        printf("\n⚠️  A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->quantidade--;
    printf("\n🎮 Peça [%c %d] jogada!\n", removida.nome, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(Fila* fila) {
    printf("\n===== Fila de Peças =====\n");
    if (filaVazia(fila)) {
        printf("(vazia)\n");
        return;
    }
    int i, index;
    for (i = 0; i < fila->quantidade; i++) {
        index = (fila->frente + i) % MAX_FILA;
        printf("[%c %d] ", fila->itens[index].nome, fila->itens[index].id);
    }
    printf("\n==========================\n");
}

// ---------- FUNÇÃO PRINCIPAL ----------

int main() {
	setlocale(LC_ALL, "Portuguese");
    Fila fila;
    inicializarFila(&fila);
    srand(time(NULL)); // inicializa gerador de números aleatórios

    int idAtual = 0; // contador de IDs das peças

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    int opcao;

    do {
        exibirFila(&fila);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Inserir nova peça\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1:
            desenfileirar(&fila);
            break;
        case 2:
            enfileirar(&fila, gerarPeca(idAtual++));
            break;
        case 0:
            printf("\nEncerrando o programa...\n");
            break;
        default:
            printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
