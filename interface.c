//Dev: Nicole
#include <stdio.h>
#include "interface.h"
#include "utils.h" // Usa a função limparBuffer

// Função para listar todos os territórios cadastrados
void listar_territorios(const Territorio territorios[], int num_territorios) {
    printf("==============================================\n");
    printf("          MAPA DO MUNDO - ESTADO ATUAL        \n");
    printf("==============================================\n");
    printf("\n");
    
    for (int i = 0; i < num_territorios; i++) {
        printf("%d. %s \t (Exercito: %s\t|\tTropas: %d\t|\tOcupado por: %s)\n",
                i+1, territorios[i].nome, territorios[i].cor_dominante, 
                territorios[i].tropas, territorios[i].dono != NULL ? territorios[i].dono->nome : "Neutro");
        printf("\n");
    }
}

int selecionar_territorio_atacante() {
    int numTerritorioAtacante;
    int valid = 0; // Flag para verificar se a entrada é válida. 0 = inválida, 1 = válida

    do {
        printf("Escolha o territorio atacante (1 a %d, ou 0 para sair): ", MAX_TERRITORIOS);
        scanf("%d", &numTerritorioAtacante);
        limparBuffer(); // Limpa o buffer para evitar problemas com entradas futuras

        if (numTerritorioAtacante == 0) {
            printf("Encerrando o jogo. Ate a proxima!\n");
            break; // Sai do loop e encerra o programa
        }else if (numTerritorioAtacante < 1 || numTerritorioAtacante > MAX_TERRITORIOS) {
            printf("Numero invalido. Tente novamente.\n");
            continue; // Reinicia o loop
        }

        valid = 1; // Entrada valida
    } while (!valid); // Continua o loop ate uma entrada valida ser fornecida
    return numTerritorioAtacante;
}

int selecionar_territorio_defensor(int numTerritorioAtacante) {
    int numTerritorioDefensor;
    int valid = 0;

    do {
        printf("Escolha o territorio defensor (1 a %d, ou 0 para sair): ", MAX_TERRITORIOS);
        scanf("%d", &numTerritorioDefensor);
        limparBuffer();

        if (numTerritorioDefensor == 0) {
            printf("Encerrando o jogo. Ate a proxima!\n");
            break; // Sai do loop
        }else if (numTerritorioDefensor < 1 || numTerritorioDefensor > MAX_TERRITORIOS) {
            printf("Numero invalido. Tente novamente.\n");
            continue;
        }else if (numTerritorioAtacante == numTerritorioDefensor) {
            printf("O territorio atacante e o defensor nao podem ser o mesmo. Tente novamente.\n");
            continue;
        }
        valid = 1; // Entrada valida
    } while (!valid); // Continua o loop ate uma entrada valida ser fornecida

    return numTerritorioDefensor;
}

void mostrar_missao_jogador(const Jogador* jogador) {
    if (jogador == NULL) {
        printf("Jogador invalido.\n");
        return;
    }

    printf("Missao secreta de %s: %s\n", jogador->nome, jogador->missao_atual.descricao);
}
