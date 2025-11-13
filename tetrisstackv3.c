#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// ------------------- ESTRUTURAS -------------------

typedef struct {
    char nome; // Tipo da peça ('I', 'O', 'T', 'L')
    int id;    // Identificador único
} Peca;

typedef struct {
    Peca itens[MAX_FILA];
    int frente;
    int tras;
    int quantidade;
} Fila;

typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ------------------- FUNÇÕES AUXILIARES -------------------

// Inicialização
void inicializarFila(Fila* fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

void inicializarPilha(Pilha* pilha) {
    pilha->topo = -1;
}

// Verificações
int filaCheia(Fila* fila) { return fila->quantidade == MAX_FILA; }
int filaVazia(Fila* fila) { return fila->quantidade == 0; }
int pilhaCheia(Pilha* pilha) { return pilha->topo == MAX_PILHA - 1; }
int pilhaVazia(Pilha* pilha) { return pilha->topo == -1; }

// Geração de peças
Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = { 'I', 'O', 'T', 'L' };
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ------------------- OPERAÇÕES COM FILA -------------------

void enfileirar(Fila* fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("[AVISO] Fila cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->itens[fila->tras] = nova;
    fila->quantidade++;
}

Peca desenfileirar(Fila* fila) {
    Peca removida = { '-', -1 };
    if (filaVazia(fila)) {
        printf("[AVISO] Fila vazia!\n");
        return removida;
    }
    removida = fila->itens[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->quantidade--;
    return removida;
}

// ------------------- OPERAÇÕES COM PILHA -------------------

void empilhar(Pilha* pilha, Peca peca) {
    if (pilhaCheia(pilha)) {
        printf("[AVISO] Pilha cheia! Libere espaço antes.\n");
        return;
    }
    pilha->itens[++pilha->topo] = peca;
}

Peca desempilhar(Pilha* pilha) {
    Peca removida = { '-', -1 };
    if (pilhaVazia(pilha)) {
        printf("[AVISO] Pilha vazia! Nenhuma peça reservada.\n");
        return removida;
    }
    removida = pilha->itens[pilha->topo--];
    return removida;
}

// ------------------- EXIBIÇÃO -------------------

void exibirEstado(Fila* fila, Pilha* pilha) {
    printf("\n=========================================\n");
    printf("     ESTADO ATUAL DO JOGO\n");
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

// ------------------- FUNÇÕES DE TROCA -------------------

// Troca a peça da frente da fila com o topo da pilha
void trocarPeca(Fila* fila, Pilha* pilha) {
    if (filaVazia(fila)) {
        printf("[ERRO] A fila está vazia, não há peça para trocar.\n");
        return;
    }
    if (pilhaVazia(pilha)) {
        printf("[ERRO] A pilha está vazia, não há peça reservada para trocar.\n");
        return;
    }

    int indexFrente = fila->frente;
    Peca temp = fila->itens[indexFrente];
    fila->itens[indexFrente] = pilha->itens[pilha->topo];
    pilha->itens[pilha->topo] = temp;

    printf("[INFO] Troca simples realizada entre a frente da fila e o topo da pilha.\n");
}

// Troca múltipla entre as três primeiras da fila e as três da pilha
void trocaMultipla(Fila* fila, Pilha* pilha) {
    if (fila->quantidade < 3 || pilha->topo < 2) {
        printf("[ERRO] É necessário ter pelo menos 3 peças em cada estrutura para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indexFila = (fila->frente + i) % MAX_FILA;
        Peca temp = fila->itens[indexFila];
        fila->itens[indexFila] = pilha->itens[pilha->topo - i];
        pilha->itens[pilha->topo - i] = temp;
    }

    printf("[INFO] Troca múltipla realizada entre as 3 primeiras peças da fila e as 3 da pilha.\n");
}

// ------------------- FUNÇÃO PRINCIPAL -------------------

int main() {
	setlocale(LC_ALL, "Portuguese");
    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    srand(time(NULL));

    int idAtual = 0;
    int opcao;

    // Preenche a fila com 5 peças iniciais
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila, gerarPeca(idAtual++));
    }

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as 3 da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1: {
            if (!filaVazia(&fila)) {
                Peca jogada = desenfileirar(&fila);
                printf("[INFO] Peça [%c %d] jogada.\n", jogada.nome, jogada.id);
                enfileirar(&fila, gerarPeca(idAtual++)); // mantém fila cheia
            }
            else {
                printf("[AVISO] Fila vazia.\n");
            }
            break;
        }
        case 2: {
            if (!filaVazia(&fila)) {
                if (!pilhaCheia(&pilha)) {
                    Peca reservada = desenfileirar(&fila);
                    empilhar(&pilha, reservada);
                    printf("[INFO] Peça [%c %d] reservada na pilha.\n", reservada.nome, reservada.id);
                    enfileirar(&fila, gerarPeca(idAtual++)); // mantém fila cheia
                }
                else {
                    printf("[AVISO] Pilha cheia! Libere espaço antes.\n");
                }
            }
            break;
        }
        case 3: {
            if (!pilhaVazia(&pilha)) {
                Peca usada = desempilhar(&pilha);
                printf("[INFO] Peça [%c %d] da pilha foi usada.\n", usada.nome, usada.id);
            }
            else {
                printf("[AVISO] Nenhuma peça reservada disponível.\n");
            }
            break;
        }
        case 4:
            trocarPeca(&fila, &pilha);
            break;
        case 5:
            trocaMultipla(&fila, &pilha);
            break;
        case 0:
            printf("\nEncerrando o programa...\n");
            break;
        default:
            printf("[ERRO] Opção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
