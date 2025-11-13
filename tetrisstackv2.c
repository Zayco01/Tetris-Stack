#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5   // tamanho fixo da fila de peças futuras
#define MAX_PILHA 3  // tamanho máximo da pilha de reserva

// ---------- ESTRUTURAS DE DADOS ----------

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // tipo da peça ('I', 'O', 'T', 'L')
    int id;    // identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

// Estrutura da pilha (peças reservadas)
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------- FUNÇÕES AUXILIARES ----------

// Inicializa a fila como vazia
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Inicializa a pilha como vazia
void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

// Verificadores
int filaCheia(Fila* fila) { return fila->quantidade == MAX_FILA; }
int filaVazia(Fila* fila) { return fila->quantidade == 0; }
int pilhaCheia(Pilha* pilha) { return pilha->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha* pilha) { return pilha->topo == -1; }

// Gera uma nova peça automaticamente
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------- OPERAÇÕES COM A FILA ----------

// Insere nova peça no final da fila (enqueue)
void enfileirar(Fila* fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n[AVISO] A fila está cheia! Não foi possível adicionar nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
}

// Remove peça da frente da fila (dequeue)
Peca desenfileirar(Fila* fila) {
    Peca removida = { '-', -1 };
    if (filaVazia(fila)) {
        printf("\n[AVISO] A fila está vazia! Nenhuma peça para remover.\n");
        return removida;
    }
    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->quantidade--;
    return removida;
}

// ---------- OPERAÇÕES COM A PILHA ----------

// Adiciona peça no topo da pilha
void empilhar(Pilha* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("\n[AVISO] A pilha está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->itens[++pilha->topo] = peca;
    printf("\n[INFO] Peça [%c %d] reservada com sucesso!\n", peca.nome, peca.id);
}

// Remove peça do topo da pilha
Peca desempilhar(Pilha* pilha) {
    Peca removida = { '-', -1 };
    if (pilhaVazia(pilha)) {
        printf("\n[AVISO] Nenhuma peça reservada para usar.\n");
        return removida;
    }
    removida = pilha->itens[pilha->topo--];
    printf("\n[INFO] Peça reservada [%c %d] usada!\n", removida.nome, removida.id);
    return removida;
}

// ---------- EXIBIÇÃO ----------

// Exibe a fila e a pilha de forma organizada
void exibirEstado(Fila* fila, Pilha* pilha) {
    printf("\n=========================================\n");
    printf("   ESTADO ATUAL DO JOGO\n");
    printf("=========================================\n");

    printf("\nFila de peças futuras:\n");
    if (filaVazia(fila))
        printf("(vazia)\n");
    else {
        for (int i = 0; i < fila->quantidade; i++) {
            int index = (fila->frente + i) % MAX_FILA;
            printf("[%c %d] ", fila->itens[index].nome, fila->itens[index].id);
        }
        printf("\n");
    }

    printf("\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(pilha))
        printf("(vazia)\n");
    else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->itens[i].nome, pilha->itens[i].id);
        }
        printf("\n");
    }

    printf("=========================================\n");
}

// ---------- FUNÇÃO PRINCIPAL ----------

int main() {
	setlocale(LC_ALL, "Portuguese");
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int idAtual = 0;
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1: {
            if (!filaVazia(&fila)) {
                Peca jogada = desenfileirar(&fila);
                printf("\n[INFO] Peça [%c %d] jogada!\n", jogada.nome, jogada.id);
                // Mantém a fila sempre cheia
                enfileirar(&fila, gerarPeca(idAtual++));
            }
            break;
        }
        case 2: {
            if (!filaVazia(&fila)) {
                if (!pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    // Mantém a fila sempre cheia
                    enfileirar(&fila, gerarPeca(idAtual++));
                }
                else {
                    printf("\n[AVISO] Pilha cheia! Libere espaço antes de reservar outra.\n");
                }
            }
            break;
        }
        case 3: {
            if (!pilhaVazia(&pilha)) {
                desempilhar(&pilha);
            }
            else {
                printf("\n[AVISO] Nenhuma peça reservada disponível.\n");
            }
            break;
        }
        case 0:
            printf("\nEncerrando o programa...\n");
            break;
        default:
            printf("\n[ERRO] Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
