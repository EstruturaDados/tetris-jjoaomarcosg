#include <stdio.h>
#include <stdlib.h>     // Para rand(), srand() e system()
#include <string.h>     // Para strcpy()
#include <time.h>       // Para srand(time(NULL))

// --- Constantes Globais ---
#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define MAX_NOME 10

// --- Estrutura de Dados ---
struct Peca {
    int id;
    char nome[MAX_NOME];
};

// --- Variáveis Globais (Fila e Pilha) ---
// Fila Circular (usando um vetor)
struct Peca fila[TAMANHO_FILA];
int fila_inicio = 0; // Índice de onde sai a peça
int fila_fim = 0;    // Índice de onde entra a nova peça
int fila_contador = 0; // Quantidade de itens na fila

// Pilha Linear (usando um vetor)
struct Peca pilha[TAMANHO_PILHA];
int pilha_topo = -1; // Pilha vazia

// ID global para garantir que cada peça seja única
int proximo_id_peca = 1;

// --- Protótipos das Funções ---
void limparBufferEntrada();
void pausarParaContinuar();
struct Peca gerarPeca();

void inicializarFila();
void exibirFila();
void exibirPilha();

void jogarPeca();          // (Dequeue da Fila + Enqueue de nova peça)
void reservarPeca();       // (Dequeue da Fila + Push para Pilha + Enqueue de nova peça)
void usarPecaReservada();  // (Pop da Pilha)


// --- Função Principal (main) ---
int main() {
    // Inicializa a semente para números aleatórios
    srand(time(NULL));
    
    // Preenche a fila com as 5 peças iniciais
    inicializarFila();
    
    int escolha = -1;

    do {
        system("clear"); // Use "cls" se estiver no Windows
        printf("--- DESAFIO TETRIS STACK (NIVEL AVENTUREIRO) ---\n\n");
        
        exibirFila();
        exibirPilha();

        printf("\n--- MENU DE ACOES ---\n");
        printf("1. Jogar Peca (Usar peca da fila)\n");
        printf("2. Reservar Peca (Mover da fila para reserva)\n");
        printf("3. Usar Peca Reservada\n");
        printf("0. Sair\n");
        printf("\nSua escolha: ");

        scanf("%d", &escolha);
        limparBufferEntrada();

        switch (escolha) {
            case 1:
                jogarPeca();
                break;
            case 2:
                reservarPeca();
                break;
            case 3:
                usarPecaReservada();
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpcao invalida! Tente novamente.\n");
                break;
        }

        if (escolha != 0) {
            pausarParaContinuar();
        }

    } while (escolha != 0);

    return 0;
}

// --- Implementação das Funções Utilitárias ---

void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausarParaContinuar() {
    printf("\nPressione Enter para continuar...");
    getchar();
}

/*
 * gerarPeca(): Cria e retorna uma nova peça aleatória.
 */
struct Peca gerarPeca() {
    struct Peca novaPeca;
    char tipos[] = {'I', 'O', 'T', 'L', 'J', 'S', 'Z'};
    
    novaPeca.id = proximo_id_peca++;
    // Sorteia um dos 7 tipos de peça
    sprintf(novaPeca.nome, "Peca %c", tipos[rand() % 7]);
    
    return novaPeca;
}

// --- Implementação da Fila Circular ---

/*
 * inicializarFila(): Preenche a fila com 5 peças iniciais.
 */
void inicializarFila() {
    for (int i = 0; i < TAMANHO_FILA; i++) {
        fila[fila_fim] = gerarPeca();
        fila_fim = (fila_fim + 1) % TAMANHO_FILA; // Lógica circular
    }
    fila_contador = 5;
    proximo_id_peca = 6; // Reseta o ID para o próximo
}

/*
 * exibirFila(): Mostra as 5 peças na fila.
 */
void exibirFila() {
    printf("FILA DE PROXIMAS PECAS (Frente -> Fim):\n");
    if (fila_contador == 0) {
        printf("[ Fila Vazia ]\n");
        return;
    }
    
    printf("[ ");
    for (int i = 0; i < fila_contador; i++) {
        int indice = (fila_inicio + i) % TAMANHO_FILA;
        printf("%s (ID: %d) | ", fila[indice].nome, fila[indice].id);
    }
    printf("]\n");
}

/*
 * jogarPeca(): Simula o uso da peça da frente da fila.
 * (Dequeue da Fila + Enqueue de nova peça)
 */
void jogarPeca() {
    if (fila_contador == 0) {
        printf("\nA FILA ESTA VAZIA! (Isso nao deveria acontecer no Nivel Aventureiro)\n");
        return;
    }

    // 1. Pega a peça da frente (Dequeue)
    struct Peca pecaJogada = fila[fila_inicio];
    fila_inicio = (fila_inicio + 1) % TAMANHO_FILA;
    
    // 2. Gera uma nova peça e a insere no fim (Enqueue)
    struct Peca novaPeca = gerarPeca();
    fila[fila_fim] = novaPeca;
    fila_fim = (fila_fim + 1) % TAMANHO_FILA;
    
    // A contagem não muda (sai 1, entra 1)
    
    printf("\n--> PECA JOGADA: %s (ID: %d)\n", pecaJogada.nome, pecaJogada.id);
    printf("--> NOVA PECA NA FILA: %s (ID: %d)\n", novaPeca.nome, novaPeca.id);
}

// --- Implementação da Pilha (Reserva) ---

/*
 * exibirPilha(): Mostra as peças na reserva (pilha).
 */
void exibirPilha() {
    printf("\nRESERVA DE PECAS (Base -> Topo):\n");
    if (pilha_topo == -1) {
        printf("[ Reserva Vazia ]\n");
        return;
    }
    
    printf("[ ");
    for (int i = 0; i <= pilha_topo; i++) {
        printf("%s (ID: %d) | ", pilha[i].nome, pilha[i].id);
    }
    printf("]\n");
}

/*
 * reservarPeca(): Tira da fila e coloca na pilha.
 * (Dequeue da Fila + Push para Pilha + Enqueue de nova peça)
 */
void reservarPeca() {
    // 1. Verifica se a pilha está cheia
    if (pilha_topo == TAMANHO_PILHA - 1) {
        printf("\nERRO: A reserva (pilha) esta cheia! Use uma peca antes.\n");
        return;
    }
    
    // 2. Verifica se a fila está vazia (não deve acontecer)
    if (fila_contador == 0) {
        printf("\nERRO: A fila esta vazia!\n");
        return;
    }

    // 3. Pega a peça da frente (Dequeue)
    struct Peca pecaParaReservar = fila[fila_inicio];
    fila_inicio = (fila_inicio + 1) % TAMANHO_FILA;

    // 4. Adiciona a peça na pilha (Push)
    pilha_topo++;
    pilha[pilha_topo] = pecaParaReservar;

    // 5. Adiciona uma nova peça na fila (Enqueue)
    struct Peca novaPeca = gerarPeca();
    fila[fila_fim] = novaPeca;
    fila_fim = (fila_fim + 1) % TAMANHO_FILA;

    printf("\n--> PECA RESERVADA: %s (ID: %d)\n", pecaParaReservar.nome, pecaParaReservar.id);
    printf("--> NOVA PECA NA FILA: %s (ID: %d)\n", novaPeca.nome, novaPeca.id);
}

/*
 * usarPecaReservada(): Tira da pilha e "usa" no jogo.
 * (Pop da Pilha)
 */
void usarPecaReservada() {
    // 1. Verifica se a pilha está vazia
    if (pilha_topo == -1) {
        printf("\nERRO: A reserva (pilha) esta vazia!\n");
        return;
    }

    // 2. Tira a peça do topo (Pop)
    struct Peca pecaUsada = pilha[pilha_topo];
    pilha_topo--;

    printf("\n--> PECA DA RESERVA USADA: %s (ID: %d)\n", pecaUsada.nome, pecaUsada.id);
}