#include <stdio.h>
#include <string.h>

struct Territorio
{
    char nome[30];
    char cor[20];
    int qtTropas;
};

int main(){

    struct Territorio territorios[5];

    int i;

    for (i = 0; i < 5; i++)
    {
        printf("\n===============================\n");
        printf(" CADASTRO DO TERRITORIO %i\n", i + 1);
        printf("===============================\n");

        printf("Digite o nome do territorio: ");
        fgets(territorios[i].nome, 30, stdin);

        printf("Digite a cor do exercito: ");
        fgets(territorios[i].cor, 20, stdin);

        printf("Digite o numero de tropas: ");
        scanf("%d", &territorios[i].qtTropas);
        getchar();
    }

    // Exibicao dos territorios cadastrados
    printf("\n\n=====================================\n");
    printf("      ESTADO ATUAL DO MAPA WAR\n");
    printf("=====================================\n");

    for (i = 0; i < 5; i++)
    {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: %s", territorios[i].nome);
        printf("Cor do Exercito: %s", territorios[i].cor);
        printf("Numero de Tropas: %d\n", territorios[i].qtTropas);
    }


    return 0;
}