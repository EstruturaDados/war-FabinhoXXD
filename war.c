#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

struct Territorio
{
    char nome[30];
    char cor[20];
    int qtTropas;
};

// Conta quantos territorios ainda possuem tropas
int contarTerritoriosAtivos(struct Territorio *territorios, int quantidade)
{
    int i;
    int contador = 0;

    for (i = 0; i < quantidade; i++)
    {
        if (territorios[i].qtTropas > 0)
        {
            contador++;
        }
    }

    return contador;
}

// Exibe o estado atual do mapa
void exibirMapa(struct Territorio *territorios, int quantidade)
{
    int i;

    printf("\n\n=====================================\n");
    printf("      ESTADO ATUAL DO MAPA WAR\n");
    printf("=====================================\n");

    for (i = 0; i < quantidade; i++)
    {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s", territorios[i].nome);
        printf("Cor do Exercito: %s", territorios[i].cor);
        printf("Numero de Tropas: %d\n", territorios[i].qtTropas);
    }
}

// Simula UMA rodada de batalha
void atacar(struct Territorio *territorios, int atacante, int defensor)
{
    int dadoAtaque;
    int dadoDefesa;

    dadoAtaque = rand() % 6 + 1;
    dadoDefesa = rand() % 6 + 1;

    printf("\n=====================================\n");
    printf("            BATALHA\n");
    printf("=====================================\n");

    printf("Atacante: %s", territorios[atacante].nome);
    printf("Defensor: %s", territorios[defensor].nome);

    printf("Dado do atacante: %d\n", dadoAtaque);
    printf("Dado do defensor: %d\n", dadoDefesa);

    // Empates favorecem o atacante
    if (dadoAtaque >= dadoDefesa)
    {
        printf("\nO atacante venceu a batalha!\n");

        territorios[defensor].qtTropas--;

        printf("%s perdeu 1 tropa.\n",
               territorios[defensor].nome);

        // Se o defensor ficou sem tropas, o territorio foi conquistado
        if (territorios[defensor].qtTropas == 0)
        {
            printf("\n*** TERRITORIO CONQUISTADO! ***\n");

            // O territorio passa a ter a mesma cor do atacante
            strcpy(territorios[defensor].cor,
                   territorios[atacante].cor);

            // Move 1 tropa do atacante para ocupar o territorio
            if (territorios[atacante].qtTropas > 1)
            {
                territorios[atacante].qtTropas--;
                territorios[defensor].qtTropas = 1;
            }
            else
            {
                // Se o atacante tinha apenas 1 tropa,
                // ainda assim o territorio fica com 1 tropa
                territorios[defensor].qtTropas = 1;
            }

            printf("%s agora pertence ao exercito %s",
                   territorios[defensor].nome,
                   territorios[defensor].cor);
        }
    }
    else
    {
        printf("\nO defensor venceu a batalha!\n");

        territorios[atacante].qtTropas--;

        if (territorios[atacante].qtTropas < 0)
        {
            territorios[atacante].qtTropas = 0;
        }

        printf("%s perdeu 1 tropa.\n",
               territorios[atacante].nome);
    }
}

int main()
{
    struct Territorio *territorios;
    int quantidade = 5;
    int i;
    int atacante;
    int defensor;

    // Inicializa numeros aleatorios
    srand(time(NULL));

    // Alocacao dinamica
    territorios = calloc(quantidade, sizeof(struct Territorio));

    if (territorios == NULL)
    {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    // Cadastro dos territorios
    for (i = 0; i < quantidade; i++)
    {
        printf("\n===============================\n");
        printf(" CADASTRO DO TERRITORIO %d\n", i + 1);
        printf("===============================\n");

        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, 30, stdin);

        printf("Digite a cor do exercito: ");
        fgets(territorios[i].cor, 20, stdin);

        printf("Digite o numero de tropas: ");
        scanf("%d", &territorios[i].qtTropas);
        getchar(); // Remove o '\n' do buffer
    }

    // Continua ate restar apenas 1 territorio com tropas
    while (contarTerritoriosAtivos(territorios, quantidade) > 1)
    {
        exibirMapa(territorios, quantidade);

        printf("\n=====================================\n");
        printf("         FASE DE ATAQUE\n");
        printf("=====================================\n");

        printf("Escolha o territorio atacante (1 a %d): ",
               quantidade);
        scanf("%d", &atacante);

        printf("Escolha o territorio defensor (1 a %d): ",
               quantidade);
        scanf("%d", &defensor);

        // Ajusta para indices do vetor
        atacante--;
        defensor--;

        // Validacoes
        if (atacante < 0 || atacante >= quantidade ||
            defensor < 0 || defensor >= quantidade)
        {
            printf("Escolha invalida.\n");
            continue;
        }

        if (atacante == defensor)
        {
            printf("O atacante e o defensor nao podem ser o mesmo territorio.\n");
            continue;
        }

        if (territorios[atacante].qtTropas <= 0)
        {
            printf("O territorio atacante nao possui tropas.\n");
            continue;
        }

        if (territorios[defensor].qtTropas <= 0)
        {
            printf("O territorio defensor nao possui tropas.\n");
            continue;
        }

        // Executa uma rodada de batalha
        atacar(territorios, atacante, defensor);
    }

    // Exibe o mapa final
    exibirMapa(territorios, quantidade);

    // Descobre o vencedor
    for (i = 0; i < quantidade; i++)
    {
        if (territorios[i].qtTropas > 0)
        {
            printf("\n=====================================\n");
            printf("              VENCEDOR\n");
            printf("=====================================\n");
            printf("Territorio: %s", territorios[i].nome);
            printf("Exercito: %s", territorios[i].cor);
            printf("Tropas restantes: %d\n",
                   territorios[i].qtTropas);
            break;
        }
    }

    // Libera a memoria alocada dinamicamente
    free(territorios);

    return 0;
}