//Dev: Nicole
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "logica_jogo.h"
#include "utils.h" // usar função limparBuffer

// Ponteiro para vetor dinâmico de missões
static Missao* baralho_de_missoes = NULL; 
static int num_total_missoes_no_baralho = 0;

// Constantes para definir baralho de missões
#define NUM_MISSOES_ESTATICAS 2
#define NUM_MISSOES_DINAMICAS 3

void inicializar_random() {
    srand(time(NULL));
}

int jogar_dado() {
    return (rand() % 6) + 1; // Retorna um valor entre 1 e 6
}

void cadastrar_territorios(Territorio territorios[], int num_territorios) {
    printf("======================\n");
    printf("Vamos cadastrar os %d territorios iniciais do nosso mundo.\n", num_territorios);
    printf("\n");

    for (int i = 0; i < num_territorios; i++) {
        printf("Cadastrando Territorio %d\n", i + 1);
        printf("Nome do territorio: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = 0;

        printf("Cor do exercito: ");
        fgets(territorios[i].cor_dominante, sizeof(territorios[i].cor_dominante), stdin);
        territorios[i].cor_dominante[strcspn(territorios[i].cor_dominante, "\n")] = 0;

        printf("Numero de tropas: ");
        scanf("%d", &territorios[i].tropas);
        limparBuffer();
        printf("\n");
    }
}

void cadastrar_jogadores(Jogador jogadores[], Territorio territorios[], int num_jogadores) {
    for (int i = 0; i < num_jogadores; i++) {
        // Auto incremento para id do jogador
        jogadores[i].id = i + 1;

        // Nome padrão: Jogador 1, Jogador 2, ...
        snprintf(jogadores[i].nome, sizeof(jogadores[i].nome), "Jogador %d", i + 1);

        // Cor do jogador definida pela cor dominante do território correspondente
        snprintf(jogadores[i].cor, sizeof(jogadores[i].cor), "%s", territorios[i].cor_dominante);
    }
}

void atribuir_donos_aos_territorios(Jogador jogadores[], Territorio territorios[], int num_jogadores) {
    // Atribui cada território ao jogador correspondente
    for (int i = 0; i < num_jogadores; i++) {
        territorios[i].dono = &jogadores[i];
    }
}

void neutralizar_territorio(Territorio* territorio) {
    printf("O territorio %s agora esta neutro e sem dono.\n", territorio->nome);
    strcpy(territorio->cor_dominante, "Neutro");
    territorio->tropas = 0; // Garante que as tropas sejam zeradas
    territorio->dono = NULL;
}

/**
 * Lógica de conquista e transferência de propriedade.
 */
void conquistar_territorio(Territorio* t_atacante, Territorio* t_defensor) {
    // O atacante precisa de pelo menos 2 tropas para poder mover 1
    if (t_atacante->tropas > 1) {
        printf("CONQUISTA! O territorio %s foi dominado pelo Exercito %s\n", t_defensor->nome, t_atacante->cor_dominante);

        //Atualiza os dados do território
        strcpy(t_defensor->cor_dominante, t_atacante->cor_dominante);
        t_defensor->dono = t_atacante->dono; // Transfere a propriedade
        
        //Move as tropas
        t_defensor->tropas = 1;
        t_atacante->tropas--;
    } else { 
        printf("%s nao tem tropas suficientes para ocupar o territorio!\n", t_atacante->nome);
        // O território defensor fica neutro
        neutralizar_territorio(t_defensor);
    }
}

/**
 * Lógica principal da batalha.
 */
void batalhar(Territorio* t_atacante, Territorio* t_defensor) {

    if (t_defensor->tropas <= 0) {
        printf("O territorio %s ja esta desprotegido! Conquistando...\n", t_defensor->nome);
        conquistar_territorio(t_atacante, t_defensor);
        return;
    }

    // Lógica de rolagem de dados
    int dadoAtacante = jogar_dado();
    printf("O atacante %s rolou: %d\n", t_atacante->nome, dadoAtacante);
    int dadoDefensor = jogar_dado();
    printf("O defensor %s rolou: %d\n", t_defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        t_defensor->tropas--;
        printf("VITORIA DO ATAQUE! O defensor perdeu 1 tropa.\n");
    } else if (dadoDefensor > dadoAtacante) {
        t_atacante->tropas--;
        printf("DEFESA BEM-SUCEDIDA! O atacante perdeu 1 tropa.\n");
    } else {
        printf("EMPATE! Ninguem perdeu tropas.\n");
    }

    // Verifica se o atacante perdeu seu território
    if (t_atacante->tropas <= 0) {
        printf("O atacante %s perdeu todas as tropas no ataque!\n", t_atacante->nome);
        neutralizar_territorio(t_atacante);
    }

    // Verifica se o defensor foi conquistado
    if (t_defensor->tropas <= 0) {
        conquistar_territorio(t_atacante, t_defensor);
    }
}

void verificar_jogadores_dorrotados(Territorio territorios[], Jogador jogadores[], Jogador jogadores_derrotados[]) {
    // Para cada indice de jogador é inicializado uma flag (jogador_ativo)
    for (int i = 0; i < MAX_JOGADORES; i++) {
        int jogador_ativo = 0; // flag para verificar se o jogador possui territórios

        // Verifica se o jogador ainda possui territórios
        for (int j = 0; j < MAX_TERRITORIOS; j++) {
            if (territorios[j].dono != NULL && territorios[j].dono->id == jogadores[i].id) {
                jogador_ativo = 1; // Jogador ainda possui territórios
                break; // Sai do loop interno
            }
        }

        // Se o jogador não possui mais territórios, ele é derrotado
        if (!jogador_ativo) {
            int ja_derrotado = 0; // Flag para verificar se o jogador já foi marcado como derrotado
            for (int k = 0; k < MAX_JOGADORES; k++) {
                if (jogadores_derrotados[k].id == jogadores[i].id) {
                    ja_derrotado = 1; // Jogador já foi marcado como derrotado
                    break; // Sai do loop interno
                }
            }

            // Marca o jogador como derrotado se ainda não foi marcado
            if (!ja_derrotado) {
                jogadores_derrotados[i] = jogadores[i];
                printf("O %s foi derrotado!\n", jogadores_derrotados[i].nome);
            }
        }
    }

}

void inicializar_baralho_de_missoes(Jogador jogadores[], int num_jogadores, Territorio territorios[], int num_territorios) {
    num_total_missoes_no_baralho = NUM_MISSOES_ESTATICAS + NUM_MISSOES_DINAMICAS;
    
    //Aloca memória para o baralho completo
    baralho_de_missoes = (Missao*) malloc(num_total_missoes_no_baralho * sizeof(Missao));
    
    if (baralho_de_missoes == NULL) {
        printf("Falha ao alocar memoria para o baralho de missoes!\n");
        exit(1); // Sai do programa em caso de erro
    }
    
    int indice_baralho = 0;
    
    //Adiciona as missões ESTÁTICAS
    baralho_de_missoes[indice_baralho].id = 1;
    strcpy(baralho_de_missoes[indice_baralho].descricao, "Conquistar o mundo inteiro.");
    indice_baralho++;

    baralho_de_missoes[indice_baralho].id = 2;
    strcpy(baralho_de_missoes[indice_baralho].descricao, "Conquistar um novo territorio.");
    indice_baralho++;
    
    // Gera e adiciona as missões DINÂMICAS
    // Vamos sortear 3 cores de jogadores aleatoriamente para serem os alvos.
    for (int i = 0; i < NUM_MISSOES_DINAMICAS; i++) {
        // Sorteia um jogador aleatório para ser o alvo
        int indice_jogador_alvo = rand() % num_jogadores;
        const char* cor_alvo = jogadores[indice_jogador_alvo].cor;

        int indice_territorio_alvo = rand() % num_territorios;
        const char* territorio_alvo = territorios[indice_territorio_alvo].nome;
        
        baralho_de_missoes[indice_baralho].id = 100 + i; // IDs diferentes

        // Alterna entre os tipos de missão dinâmica
        if(i % 2 == 0) {
            sprintf(baralho_de_missoes[indice_baralho].descricao, "Destruir completamente o exercito %s.", cor_alvo);
        } else {
            sprintf(baralho_de_missoes[indice_baralho].descricao, "Conquistar o territorio %s.", territorio_alvo);
        }
        
        indice_baralho++;
    }
}

void distribuir_missoes(Jogador jogadores[], int num_jogadores, Territorio territorios[], int num_territorios) {
    if (num_total_missoes_no_baralho == 0) {
        printf("Erro: O baralho de missoes nao foi inicializado!\n");
        return;
    }
    if (num_jogadores > num_total_missoes_no_baralho) {
        printf("Erro: Nao ha missoes suficientes para todos os jogadores!\n");
        return;
    }

    // Cria um vetor com os índices de todas as missões
    int indices_missoes[num_total_missoes_no_baralho];
    for (int i = 0; i < num_total_missoes_no_baralho; i++) {
        indices_missoes[i] = i;
    }

    // Vetor ANTES e DEPOIS de embaralhar (ex: 5 missões):
    // [0, 1, 2, 3, 4] --> [3, 0, 4, 1, 2]
    embaralhar_indices(indices_missoes, num_total_missoes_no_baralho);

    // Atribui uma missão embaralhada para cada jogador
    for (int i = 0; i < num_jogadores; i++) {
        // ex: Jogador de índice 0 pega a missão de índice 3...
        int indice_da_missao_sorteada = indices_missoes[i];

        if(strstr(baralho_de_missoes[indice_da_missao_sorteada].descricao, jogadores[i].cor) == NULL &&
           strstr(baralho_de_missoes[indice_da_missao_sorteada].descricao, territorios[i].nome) == NULL) {
            jogadores[i].missao_atual = baralho_de_missoes[indice_da_missao_sorteada];
        } else {
            // se pegou, sorteia novamente até pegar uma missão válida
            int nova_missao_valida_encontrada = 0;
            for(int tentativa = 0; tentativa < num_total_missoes_no_baralho; tentativa++) {
                int novo_indice = indices_missoes[(i + tentativa + 1) % num_total_missoes_no_baralho];
                if(strstr(baralho_de_missoes[novo_indice].descricao, jogadores[i].cor) == NULL &&
                   strstr(baralho_de_missoes[novo_indice].descricao, territorios[i].nome) == NULL) {
                    jogadores[i].missao_atual = baralho_de_missoes[novo_indice];
                    nova_missao_valida_encontrada = 1;
                    break;
                }
            }
            // Se não encontrou nenhuma missão válida (caso extremo), atribui a missão original
            if(!nova_missao_valida_encontrada) {
                jogadores[i].missao_atual = baralho_de_missoes[indice_da_missao_sorteada];
            }
        }
    }
}

int verificarMissao(Territorio territorios[], int num_territorios){
    // Checar se as missões dinâmicas foram cumpridas

    for(int i = 0; i < num_territorios; i++) {
        // Missão: Destruir completamente o exército de uma cor específica
        // Verifica se a missão do dono do território atual é destruir um exercito de cor específica
        if (strstr(territorios[i].dono != NULL ? territorios[i].dono->missao_atual.descricao : "", territorios[i].dono != NULL ? territorios[i].dono->cor : "") != NULL) {
            int tropas_totais = 0;
            for (int j = 0; j < num_territorios; j++) {
                // Soma as tropas dos territórios pertencentes à cor alvo da missão
                if (territorios[j].dono != NULL && strcmp(territorios[j].dono->cor, territorios[i].dono->cor) == 0) {
                    tropas_totais += territorios[j].tropas;
                }
            }

            if (tropas_totais == 0) {
                printf("MISSAO CUMPRIDA: %s\n", territorios[i].dono->missao_atual.descricao);
                printf("%s, COM SEU EXERCITO %S VENCEU!\n", territorios[i].dono->nome, territorios[i].dono->cor);
                return 1; // Missão cumprida
            }
        }

        // Missão: Conquistar um território específico
        // Verifica se a missão do dono do território atual é conquistar outro território
        if (strstr(territorios[i].dono != NULL ? territorios[i].dono->missao_atual.descricao : "", territorios[i].nome) != NULL) {
            // Verifica se o território alvo da missão é dominado pelo jogador dono da missão
            if (territorios[i].dono != NULL && strcmp(territorios[i].dono->cor, territorios[i].dono->cor) == 0) {
                printf("Missao cumprida: %s\n", territorios[i].dono->missao_atual.descricao);
                return 1; // Missão cumprida
            }
        }
    }

    // Checar se as missões estáticas foram cumpridas

    // Missão: Conquistar o mundo inteiro
    for (int i = 0; i < num_territorios; i++) {
        // Verifica se a missão do dono do território atual é conquistar o mundo inteiro
        if(strstr(territorios[i].dono != NULL ? territorios[i].dono->missao_atual.descricao : "", "Conquistar o mundo inteiro.") != NULL) {
            int total_territorios_dominados = 0;
            for (int j = 0; j < num_territorios; j++) {
                // Conta quantos territórios o jogador dominou
                if (territorios[j].dono != NULL && strcmp(territorios[j].dono->cor, territorios[i].dono->cor) == 0) {
                    total_territorios_dominados++;
                }
            }
            // Quando o número de territórios dominados pelo jogador[i] for igual ao número total de territórios, a missão é cumprida
            if (total_territorios_dominados == num_territorios) {
                printf("MISSAO CUMPRIDA: %s\n", territorios[i].dono->missao_atual.descricao);
                printf("%s, COM SEU EXERCITO %s VENCEU!\n", territorios[i].dono->nome, territorios[i].dono->cor);
                return 1; // Missão cumprida
            }
        }
    }

    // Missão: Conquistar um novo territorio (qualquer território que não seja o natal)
    for (int i = 0; i < num_territorios; i++) {
        // Verifica se a missão do dono do território atual é conquistar um novo território
        if(strstr(territorios[i].dono != NULL ? territorios[i].dono->missao_atual.descricao : "", "Conquistar um novo territorio.") != NULL) {
            for (int j = 0; j < num_territorios; j++) {
                // Conta quantos territórios o jogador dominou que não sejam o território natal
                if (territorios[j].dono != NULL && strcmp(territorios[j].dono->cor, territorios[i].dono->cor) == 0 &&
                    strcmp(territorios[j].nome, territorios[i].nome) != 0) {
                    printf("MISSAO CUMPRIDA: %s\n", territorios[i].dono->missao_atual.descricao);
                    printf("%s, COM SEU EXERCITO %s VENCEU!\n", territorios[i].dono->nome, territorios[i].dono->cor);
                    return 1; // Missão cumprida
                }
            }
        }
    }

    return 0; // Nenhuma missão cumprida
}

void liberar_baralho_de_missoes() {
    if (baralho_de_missoes != NULL) {
        free(baralho_de_missoes);
        baralho_de_missoes = NULL;
    }
}

/**
 * Embaralha um vetor de inteiros usando o algoritmo Fisher-Yates.
 * * @param array O ponteiro para o início do vetor a ser embaralhado.
 * @param n O número de elementos no vetor.
 */
void embaralhar_indices(int* array, int n) {
    // Começa do último elemento e vai até o segundo (índice 1)
    // Não precisamos rodar para i = 0, pois seria "trocar o item 0 com o item 0"
    for (int i = n - 1; i > 0; i--) {
        // 1. Sorteia um índice 'j' entre 0 e i
        // rand() % (i + 1) garante um número no intervalo [0, i]
        int j = rand() % (i + 1);

        // 2. Troca os elementos nas posições 'i' e 'j'
        int temp = array[i]; // Guarda o valor de 'i' temporariamente
        array[i] = array[j]; // Coloca o valor de 'j' na posição 'i'
        array[j] = temp;     // Coloca o valor de 'i' (guardado) na posição 'j'
    }
}
