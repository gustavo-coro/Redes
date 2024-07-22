#include "../include/yatzy.h"

int* initializeTable() {
    int* player = (int*)malloc(sizeof(int) * NUM_CATEGORIES);
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        player[i] = -1;
    }
    return player;
}

void freeTable(int* player1, int* player2) {
    free(player1);
    free(player2);
}

void printInstructions() {
    printf("*****************************\n");
    printf("*       REGRAS DO YATZY     *\n");
    printf("*****************************\n\n");

    printf("Objetivo:\n");
    printf(
        "O objetivo do Yatzy e marcar o maior numero de pontos rolando cinco "
        "dados para formar certas combinacoes.\n\n");

    printf("Jogabilidade:\n");
    printf("1. O jogo e jogado em 13 rodadas.\n");
    printf(
        "2. Em cada rodada, um jogador tem ate tres lancamentos dos cinco "
        "dados.\n");
    printf(
        "3. Apos cada lancamento, o jogador pode escolher manter alguns dados e "
        "lancar novamente os outros.\n");
    printf(
        "4. O jogador deve preencher uma pontuacao na tabela de pontuacao ao final de "
        "cada rodada.\n");
    printf("5. Cada combinacao so pode ser usada uma vez.\n\n");

    printf("Combinacoes de Pontuacao:\n");
    printf("1. Uns - Soma de todos os dados mostrando 1.\n");
    printf("2. Dois - Soma de todos os dados mostrando 2.\n");
    printf("3. Tres - Soma de todos os dados mostrando 3.\n");
    printf("4. Quatros - Soma de todos os dados mostrando 4.\n");
    printf("5. Cincos - Soma de todos os dados mostrando 5.\n");
    printf("6. Seis - Soma de todos os dados mostrando 6.\n");
    printf(
        "7. Trinca - Soma de todos os cinco dados se pelo menos tres forem iguais.\n");
    printf(
        "8. Quadra - Soma de todos os cinco dados se pelo menos quatro forem iguais.\n");
    printf("9. Full House - 25 pontos por uma trinca e um par.\n");
    printf(
        "10. Sequencia Pequena - 30 pontos por uma sequencia de quatro dados (1-2-3-4 "
        "ou 2-3-4-5 ou 3-4-5-6).\n");
    printf(
        "11. Sequencia Grande - 40 pontos por uma sequencia de cinco dados (1-2-3-4-5 "
        "ou 2-3-4-5-6).\n");
    printf("12. Yatzy - 50 pontos por cinco dados iguais.\n");
    printf("13. Chance - Soma de todos os dados (qualquer combinacao).\n\n");
    printf("Soma - Pontuacao total dos Uns aos Seis.\n");
    printf("Bonus - 50 pontos se a Soma for 63 ou mais.\n\n");

    printf("Vencendo o Jogo:\n");
    printf(
        "O jogador com a maior pontuacao total ao final das 13 rodadas "
        "vence o jogo.\n\n");

    printf("*********************************\n");
    printf("*   Divirta-se jogando Yatzy!   *\n");
    printf("*********************************\n");
}

void printYatzyTable(int* player1, int* player2) {
    const char* categories[] = {"1 - Uns",
                            "2 - Dois",
                            "3 - Tres",
                            "4 - Quatros",
                            "5 - Cincos",
                            "6 - Seis",
                            "Soma",
                            "Bonus",
                            "7 - Trinca",
                            "8 - Quadra",
                            "9 - Full House",
                            "10 - Sequencia Pequena",
                            "11 - Sequencia Grande",
                            "12 - Yatzy",
                            "13 - Chance"};

printf("+----------------------------+----------+----------+\n");
printf("| Categoria                  | Jogador 1| Jogador 2|\n");
printf("+----------------------------+----------+----------+\n");


    for (int i = 0; i < NUM_CATEGORIES - 1; i++) {
        char p1[10];
        char p2[10];

        if (player1[i] == -1) {
            sprintf(p1, "X");
        } else {
            sprintf(p1, "%d", player1[i]);
        }

        if (player2[i] == -1) {
            sprintf(p2, "X");
        } else {
            sprintf(p2, "%d", player2[i]);
        }

        printf("| %-22s     | %8s | %8s |\n", categories[i], p1, p2);
    }

    int p1 =
        (player1[NUM_CATEGORIES - 1] != -1) ? player1[NUM_CATEGORIES - 1] : 0;
    int p2 =
        (player2[NUM_CATEGORIES - 1] != -1) ? player2[NUM_CATEGORIES - 1] : 0;

    printf("+----------------------------+----------+----------+\n");
    printf("| Total                      | %8d | %8d |\n", p1, p2);
    printf("+----------------------------+----------+----------+\n");
}

void printDice(int* dice) {
    // Print top row of all dice
    for (int i = 0; i < NUM_DICE; i++) {
        printf("┌─────┐ ");
    }
    printf("\n");

    // Print first line of each die
    for (int i = 0; i < NUM_DICE; i++) {
        switch (dice[i]) {
            case 1:
                printf("│     │ ");
                break;
            case 2:
            case 3:
                printf("│    o│ ");
                break;
            case 4:
            case 5:
            case 6:
                printf("│o   o│ ");
                break;
            default:
                printf("       ");
                break;
        }
    }
    printf("\n");

    // Print middle line of each die
    for (int i = 0; i < NUM_DICE; i++) {
        switch (dice[i]) {
            case 1:
                printf("│  o  │ ");
                break;
            case 2:
                printf("│     │ ");
                break;
            case 3:
                printf("│  o  │ ");
                break;
            case 4:
                printf("│     │ ");
                break;
            case 5:
                printf("│  o  │ ");
                break;
            case 6:
                printf("│o   o│ ");
                break;
            default:
                printf("       ");
                break;
        }
    }
    printf("\n");

    // Print bottom line of each die
    for (int i = 0; i < NUM_DICE; i++) {
        switch (dice[i]) {
            case 1:
                printf("│     │ ");
                break;
            case 2:
            case 3:
                printf("│o    │ ");
                break;
            case 4:
            case 5:
            case 6:
                printf("│o   o│ ");
                break;
            default:
                printf("       ");
                break;
        }
    }
    printf("\n");

    // Print bottom row of all dice
    for (int i = 0; i < NUM_DICE; i++) {
        printf("└─────┘ ");
    }
    printf("\n");
    for (int i = 0; i < NUM_DICE; i++) {
        printf(" Pos. %d ", i + 1);
    }
    printf("\n");
}

void rollDice(int dice[NUM_DICE]) {
    for (int i = 0; i < NUM_DICE; i++) {
        if (dice[i] == 0) dice[i] = rand() % 6 + 1;
    }
}

int makeMove(int* player, int* dice, int category) {
    int sum = 0;
    switch (category) {
        case 1: {  // Ones
            if (player[0] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 1) {
                    sum += 1;
                }
            }
            player[0] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 2: {  // Twos
            if (player[1] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 2) {
                    sum += 2;
                }
            }
            player[1] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 3: {  // Threes
            if (player[2] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 3) {
                    sum += 3;
                }
            }
            player[2] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 4: {  // Fours
            if (player[3] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 4) {
                    sum += 4;
                }
            }
            player[3] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 5: {  // Fives
            if (player[4] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 5) {
                    sum += 5;
                }
            }
            player[4] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 6: {  // Sixes
            if (player[5] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                if (dice[i] == 6) {
                    sum += 6;
                }
            }
            player[5] = sum;
            if (player[6] == -1) {
                player[6] = sum;
                if ((player[6] >= 63) && (player[7] == -1)) {
                    player[7] = 50;
                }
                break;
            }
            player[6] += sum;
            if ((player[6] >= 63) && (player[7] == -1)) {
                player[7] = 50;
            }
            break;
        }
        case 7: {  // Three of a kind
            if (player[8] != -1) return 0;
            int counts[7] = {0};
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                sum += dice[i];
                counts[dice[i]]++;
            }
            int is_three_of_a_kind = 0;
            for (int i = 1; i <= 6; i++) {
                if (counts[i] >= 3) {
                    is_three_of_a_kind += 1;
                    break;
                }
            }
            if (is_three_of_a_kind == 0) {
                player[8] = 0;
                break;
            }
            player[8] = sum;
            break;
        }
        case 8: {  // Four of a kind
            if (player[9] != -1) return 0;
            int counts[7] = {0};
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                sum += dice[i];
                counts[dice[i]]++;
            }
            int is_four_of_a_kind = 0;
            for (int i = 1; i <= 6; i++) {
                if (counts[i] >= 4) {
                    is_four_of_a_kind += 1;
                    break;
                }
            }
            if (is_four_of_a_kind == 0) {
                player[9] = 0;
                break;
            }
            player[9] = sum;
            break;
        }
        case 9: {  // Full House
            if (player[10] != -1) return 0;
            int counts[7] = {0};
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i]]++;
            }
            int is_three_of_a_kind = 0;
            int is_two_of_a_kind = 0;
            for (int i = 1; i <= 6; i++) {
                if (counts[i] == 3) {
                    is_three_of_a_kind += 1;
                }
                if (counts[i] == 2) {
                    is_two_of_a_kind += 1;
                }
            }
            if ((is_three_of_a_kind == 0) || (is_two_of_a_kind == 0)) {
                player[10] = 0;
                break;
            }
            player[10] = 25;
            break;
        }
        case 10: {  // Small Straight
            if (player[11] != -1) return 0;
            int counts[7] = {0};
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i]]++;
            }
            int is_small_straight = 0;
            if ((counts[1] >= 1 && counts[2] >= 1 && counts[3] >= 1 &&
                 counts[4] >= 1) ||
                (counts[2] >= 1 && counts[3] >= 1 && counts[4] >= 1 &&
                 counts[5] >= 1) ||
                (counts[3] >= 1 && counts[4] >= 1 && counts[5] >= 1 &&
                 counts[6] >= 1)) {
                is_small_straight += 1;
            }
            if (is_small_straight == 0) {
                player[11] = 0;
                break;
            }
            player[11] = 30;
            break;
        }
        case 11: {  // Large Straight
            if (player[12] != -1) return 0;
            int counts[7] = {0};
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i]]++;
            }
            int is_large_straight = 0;
            if ((counts[1] == 1 && counts[2] == 1 && counts[3] == 1 &&
                 counts[4] == 1 && counts[5] == 1) ||
                (counts[2] == 1 && counts[3] == 1 && counts[4] == 1 &&
                 counts[5] == 1 && counts[6] == 1)) {
                is_large_straight += 1;
            }
            if (is_large_straight == 0) {
                player[12] = 0;
                break;
            }
            player[12] = 40;
            break;
        }
        case 12: {  // Yatzy
            if (player[13] != -1) return 0;
            int counts[7] = {0};
            for (int i = 0; i < NUM_DICE; i++) {
                counts[dice[i]]++;
            }
            int is_yatzy = 0;
            for (int i = 1; i <= 6; i++) {
                if (counts[i] == 5) {
                    is_yatzy += 1;
                    break;
                }
            }
            if (is_yatzy == 0) {
                player[13] = 0;
                break;
            }
            player[13] = 50;
            break;
        }
        case 13: {  // Choice
            if (player[14] != -1) return 0;
            sum = 0;
            for (int i = 0; i < NUM_DICE; i++) {
                sum += dice[i];
            }
            player[14] = sum;
            break;
        }
        default: {
            return 0;
            break;
        }
    }
    sum = 0;
    for (int i = 0; i < 15; i++) {
        if ((player[i] != -1) && (i != 6)) {
            sum += player[i];
        }
    }
    player[15] = sum;
    return 1;
}
