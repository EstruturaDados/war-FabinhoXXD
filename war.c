#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_TERRITORIOS 5
#define MAX_NOME 50
#define MAX_COR 20

typedef struct {
    char nome[MAX_NOME];
    char corExercito[MAX_COR];
    int tropas;
} Territorio;

typedef enum {
    MISSAO_DESTRUIR_VERDE = 1,
    MISSAO_CONQUISTAR_3_TERRITORIOS = 2
} TipoMissao;

/* =========================================
   FUNCOES AUXILIARES
========================================= */

void removerQuebraLinha(char *texto) {
    texto[strcspn(texto, "\n")] = '\0';
}

void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int contarTerritoriosAzuis(const Territorio *territorios, int total) {
    int i, contador = 0;

    for (i = 0; i < total; i++) {
        if (territorios[i].tropas > 0 &&
            strcmp(territorios[i].corExercito, "Azul") == 0) {
            contador++;
        }
    }

    return contador;
}

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

/* =========================================
   NIVEL NOVATO
========================================= */

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

void exibirMapa(const Territorio *territorios, int total) {
    int i;

    printf("\n=====================================\n");
    printf("         ESTADO ATUAL DO MAPA\n");
    printf("=====================================\n");

    /* Exibe somente territorios com tropas > 0 */
    for (i = 0; i < total; i++) {
        if (territorios[i].tropas > 0) {
            printf("[%d] %-15s | Exercito: %-10s | Tropas: %d\n",
                   i + 1,
                   territorios[i].nome,
                   territorios[i].corExercito,
                   territorios[i].tropas);
        }
    }

    printf("=====================================\n\n");
}

/* =========================================
   NIVEL MESTRE - MISSOES
========================================= */

TipoMissao sortearMissao(void) {
    return (rand() % 2) + 1;
}

void exibirMissao(TipoMissao missao) {
    printf("\n========== SUA MISSAO ==========\n");

    if (missao == MISSAO_DESTRUIR_VERDE) {
        printf("Destruir o exercito Verde.\n");
    } else {
        printf("Conquistar 3 territorios com o exercito Azul.\n");
    }

    printf("================================\n\n");
}

int verificarMissao(const Territorio *territorios,
                    int total,
                    TipoMissao missao) {
    if (missao == MISSAO_DESTRUIR_VERDE) {
        return !existeExercitoVerde(territorios, total);
    }

    if (missao == MISSAO_CONQUISTAR_3_TERRITORIOS) {
        return contarTerritoriosAzuis(territorios, total) >= 3;
    }

    return 0;
}

/* =========================================
   NIVEL AVENTUREIRO - BATALHAS
========================================= */

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

    /* Validacao dos indices */
    if (atacante < 0 || atacante >= total ||
        defensor < 0 || defensor >= total) {
        printf("Territorio invalido!\n\n");
        return;
    }

    /* Nao pode atacar a si mesmo */
    if (atacante == defensor) {
        printf("O territorio nao pode atacar a si mesmo!\n\n");
        return;
    }

    /* Atacante precisa ter tropas */
    if (territorios[atacante].tropas <= 0) {
        printf("O territorio atacante nao possui tropas.\n\n");
        return;
    }

    /* Defensor precisa ter tropas */
    if (territorios[defensor].tropas <= 0) {
        printf("O territorio defensor ja foi derrotado.\n\n");
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

    /* Empates favorecem o atacante */
    if (dadoAtaque >= dadoDefesa) {
        printf("Resultado: Atacante venceu!\n");

        /* O DEFENSOR perdeu a batalha e perde 1 tropa */
        territorios[defensor].tropas--;

        /* Se zerou, foi derrotado e nao aparecerá mais no mapa */
        if (territorios[defensor].tropas <= 0) {
            territorios[defensor].tropas = 0;

            /* Opcional: o territorio passa a pertencer ao atacante */
            strcpy(territorios[defensor].corExercito,
                   territorios[atacante].corExercito);

            printf("Territorio conquistado!\n");
            printf("As tropas do defensor foram zeradas.\n");
        }
    } else {
        printf("Resultado: Defensor venceu!\n");

        /* O ATACANTE perdeu a batalha e perde 1 tropa */
        territorios[atacante].tropas--;

        /* Se zerou, foi derrotado e nao aparecerá mais no mapa */
        if (territorios[atacante].tropas <= 0) {
            territorios[atacante].tropas = 0;

            printf("O territorio atacante foi derrotado!\n");
            printf("As tropas do atacante foram zeradas.\n");
        }
    }

    printf("===============================\n\n");
}

/* =========================================
   MENU PRINCIPAL
========================================= */

void menu(void) {
    printf("============== MENU ==============\n");
    printf("1 - Atacar\n");
    printf("2 - Verificar Missao\n");
    printf("3 - Exibir Mapa\n");
    printf("0 - Sair\n");
    printf("==================================\n");
    printf("Opcao: ");
}

/* =========================================
   MAIN
========================================= */

int main(void) {
    Territorio *territorios;
    TipoMissao missao;
    int opcao = -1;

    srand((unsigned int)time(NULL));

    /* Nivel Aventureiro: alocacao dinamica com calloc */
    territorios = (Territorio *)calloc(TOTAL_TERRITORIOS,
                                       sizeof(Territorio));

    if (territorios == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    /* Nivel Novato: cadastro */
    cadastrarTerritorios(territorios, TOTAL_TERRITORIOS);

    /* Nivel Mestre: missao */
    missao = sortearMissao();
    exibirMissao(missao);

    /* Menu principal */
    do {
        menu();
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                atacar(territorios, TOTAL_TERRITORIOS);
                exibirMapa(territorios, TOTAL_TERRITORIOS);

                if (verificarMissao(territorios,
                                    TOTAL_TERRITORIOS,
                                    missao)) {
                    printf("PARABENS! VOCE CUMPRIU SUA MISSAO!\n");
                    printf("VITORIA!\n");
                    opcao = 0;
                }
                break;

            case 2:
                if (verificarMissao(territorios,
                                    TOTAL_TERRITORIOS,
                                    missao)) {
                    printf("Missao concluida!\n");
                    printf("VITORIA!\n");
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

    free(territorios);

    return 0;
}