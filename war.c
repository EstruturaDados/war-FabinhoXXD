#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20

/* =========================
   STRUCTS
========================= */

typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int tropas;
} Territorio;

typedef enum {
    MISSAO_DESTRUIR_VERDE = 1,
    MISSAO_CONQUISTAR_3_TERRITORIOS = 2
} TipoMissao;

/* =========================
   FUNCOES AUXILIARES
========================= */

/* Remove o '\n' deixado pelo fgets */
void removerQuebraLinha(char *texto) {
    texto[strcspn(texto, "\n")] = '\0';
}

/* Limpa o buffer do teclado */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Conta quantos territorios ainda possuem tropas */
int contarTerritoriosAtivos(const Territorio *territorios, int total) {
    int i;
    int ativos = 0;

    for (i = 0; i < total; i++) {
        if (territorios[i].tropas > 0) {
            ativos++;
        }
    }

    return ativos;
}

/* Conta quantos territorios pertencem ao jogador (Azul) */
int contarTerritoriosAzuis(const Territorio *territorios, int total) {
    int i;
    int quantidade = 0;

    for (i = 0; i < total; i++) {
        if (territorios[i].tropas > 0 &&
            strcmp(territorios[i].corExercito, "Azul") == 0) {
            quantidade++;
        }
    }

    return quantidade;
}

/* Verifica se ainda existe algum territorio Verde */
int existeExercitoVerde(const Territorio *territorios, int total) {
    int i;

    for (i = 0; i < total; i++) {
        if (territorios[i].tropas > 0 &&
            strcmp(territorios[i].corExercito, "Verde") == 0) {
            return 1;
        }
    }

    return 0;
}

/* =========================
   NIVEL NOVATO
   Cadastro inicial
========================= */

void cadastrarTerritorios(Territorio *territorios, int total) {
    int i;

    printf("=====================================\n");
    printf("     CADASTRO DOS TERRITORIOS\n");
    printf("=====================================\n\n");

    for (i = 0; i < total; i++) {
        printf("Territorio %d\n", i + 1);

        printf("Nome: ");
        fgets(territorios[i].nome, MAX_NOME, stdin);
        removerQuebraLinha(territorios[i].nome);

        printf("Cor do exercito: ");
        fgets(territorios[i].corExercito, MAX_COR, stdin);
        removerQuebraLinha(territorios[i].corExercito);

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer();

        printf("\n");
    }
}

/* Exibe o estado atual do mapa */
void exibirMapa(const Territorio *territorios, int total) {
    int i;

    printf("\n=====================================\n");
    printf("         ESTADO ATUAL DO MAPA\n");
    printf("=====================================\n");

    for (i = 0; i < total; i++) {
        printf("[%d] %-15s | Exercito: %-10s | Tropas: %d\n",
               i + 1,
               territorios[i].nome,
               territorios[i].corExercito,
               territorios[i].tropas);
    }

    printf("=====================================\n\n");
}

/* =========================
   NIVEL MESTRE
   Missoes
========================= */

TipoMissao sortearMissao() {
    return (rand() % 2) + 1;
}

void exibirMissao(TipoMissao missao) {
    printf("\n========== SUA MISSAO ==========\n");

    if (missao == MISSAO_DESTRUIR_VERDE) {
        printf("Destruir completamente o exercito Verde.\n");
    } else {
        printf("Conquistar 3 territorios com o exercito Azul.\n");
    }

    printf("================================\n\n");
}

/* Retorna 1 se a missão foi concluída */
int verificarMissao(const Territorio *territorios, int total, TipoMissao missao) {
    if (missao == MISSAO_DESTRUIR_VERDE) {
        if (!existeExercitoVerde(territorios, total)) {
            return 1;
        }
    } else if (missao == MISSAO_CONQUISTAR_3_TERRITORIOS) {
        if (contarTerritoriosAzuis(territorios, total) >= 3) {
            return 1;
        }
    }

    return 0;
}

/* =========================
   NIVEL AVENTUREIRO
   Batalhas
========================= */

void atacar(Territorio *territorios, int total) {
    int atacante, defensor;
    int dadoAtaque, dadoDefesa;

    exibirMapa(territorios, total);

    printf("Escolha o territorio atacante (1 a %d): ", total);
    scanf("%d", &atacante);

    printf("Escolha o territorio defensor (1 a %d): ", total);
    scanf("%d", &defensor);

    limparBuffer();

    atacante--;
    defensor--;

    /* Validacoes */
    if (atacante < 0 || atacante >= total ||
        defensor < 0 || defensor >= total) {
        printf("Territorio invalido!\n\n");
        return;
    }

    if (atacante == defensor) {
        printf("O territorio nao pode atacar a si mesmo!\n\n");
        return;
    }

    if (territorios[atacante].tropas <= 1) {
        printf("O territorio atacante precisa ter pelo menos 2 tropas.\n\n");
        return;
    }

    if (territorios[defensor].tropas <= 0) {
        printf("O territorio defensor ja esta sem tropas.\n\n");
        return;
    }

    /* Sorteio dos dados */
    dadoAtaque = (rand() % 6) + 1;
    dadoDefesa = (rand() % 6) + 1;

    printf("\n=========== BATALHA ===========\n");
    printf("Atacante: %s (%s)\n",
           territorios[atacante].nome,
           territorios[atacante].corExercito);

    printf("Defensor: %s (%s)\n",
           territorios[defensor].nome,
           territorios[defensor].corExercito);

    printf("Dado do atacante: %d\n", dadoAtaque);
    printf("Dado do defensor: %d\n", dadoDefesa);

    /* Empate favorece o atacante */
    if (dadoAtaque >= dadoDefesa) {
        printf("Resultado: Atacante venceu!\n");

        territorios[defensor].tropas--;

        /* Se o defensor ficou sem tropas, o territorio é conquistado */
        if (territorios[defensor].tropas <= 0) {
            printf("Territorio conquistado!\n");

            strcpy(territorios[defensor].corExercito,
                   territorios[atacante].corExercito);

            territorios[defensor].tropas = 1;
            territorios[atacante].tropas--;
        }
    } else {
        printf("Resultado: Defensor venceu!\n");
        territorios[atacante].tropas--;
    }

    printf("===============================\n\n");
}

/* =========================
   MENU PRINCIPAL
========================= */

void menu() {
    printf("============== MENU ==============\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("3 - Exibir Mapa\n");
    printf("0 - Sair\n");
    printf("==================================\n");
    printf("Opcao: ");
}

/* =========================
   FUNCAO PRINCIPAL
========================= */

int main() {
    Territorio *territorios;
    TipoMissao missao;
    int opcao;
    int territoriosAtivos;

    /* Inicializa aleatoriedade */
    srand((unsigned int)time(NULL));

    /* NIVEL AVENTUREIRO:
       Alocacao dinamica com calloc */
    territorios = (Territorio *)calloc(TOTAL_TERRITORIOS, sizeof(Territorio));

    if (territorios == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    /* NIVEL NOVATO:
       Cadastro manual dos territorios */
    cadastrarTerritorios(territorios, TOTAL_TERRITORIOS);

    /* NIVEL MESTRE:
       Sorteio e exibicao da missao */
    missao = sortearMissao();
    exibirMissao(missao);

    /* Loop principal */
    do {
        territoriosAtivos = contarTerritoriosAtivos(
            territorios,
            TOTAL_TERRITORIOS
        );

        /* Se restar apenas 1 territorio com tropas, fim do jogo */
        if (territoriosAtivos == 1) {
            printf("Restou apenas um territorio com tropas!\n");
            printf("Fim do jogo!\n\n");
            exibirMapa(territorios, TOTAL_TERRITORIOS);
            break;
        }

        menu();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                atacar(territorios, TOTAL_TERRITORIOS);
                exibirMapa(territorios, TOTAL_TERRITORIOS);

                if (verificarMissao(
                        territorios,
                        TOTAL_TERRITORIOS,
                        missao)) {
                    printf("PARABENS! VOCE CUMPRIU SUA MISSAO!\n");
                    printf("VITORIA!\n\n");
                    exibirMapa(territorios, TOTAL_TERRITORIOS);
                    opcao = 0;
                }
                break;

            case 2:
                if (verificarMissao(
                        territorios,
                        TOTAL_TERRITORIOS,
                        missao)) {
                    printf("Missao concluida!\n");
                    printf("VITORIA!\n\n");
                    opcao = 0;
                } else {
                    printf("Missao ainda nao concluida.\n\n");
                }
                break;

            case 3:
                exibirMapa(territorios, TOTAL_TERRITORIOS);
                break;

            case 0:
                printf("Encerrando o jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n\n");
        }

    } while (opcao != 0);

    /* Libera memoria */
    free(territorios);

    return 0;
}