//Dev: Nicole
#include <stdio.h>
#include "utils.h" // Inclui o cabeçalho para as funções utilitárias

// Limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para pausar a execução até o usuário pressionar Enter
void pausar_para_proximo_turno() {
    printf("\nPressione Enter para ir para o proximo turno...");
    limparBuffer();
}
