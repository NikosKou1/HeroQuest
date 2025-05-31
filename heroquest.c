/*
Evangelos Trogiannos Kavvadas
AM : 2022201900225
dit19225@uop.gr
Nikolaos Koustenis
AM : 2022202000105
dit20105@uop.gr
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#define MAX_HEROES 4
#define BARBARIAN 'B'
#define DWARF 'D'
#define ELF 'E'
#define WIZARD 'W'
#define WALL '#'
#define EMPTY '.'
#define FURNITURE '@'



typedef struct {
    char type;
    int health;
} Game;

Game **board;
int N, M;

char heroes[] = {BARBARIAN, DWARF, ELF, WIZARD};
int maxHeroes[MAX_HEROES];
int HeroesNow = 0;

void allocateBoard() {
    board = malloc(N * sizeof(Game*));
    for (int i = 0; i < N; i++) {
        board[i] = malloc(M * sizeof(Game));
    }
}

void freeBoard() {
    for (int i = 0; i < N; i++) {
        free(board[i]);
    }
    free(board);
}

void initializeBoard(int difficulty) {
    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            board[i][j].type = EMPTY;
            board[i][j].health = 0;
        }
    }
    int level;
    if ( level > 1) {
    for (int i = 0; i < (N * M) / 10; i++) {
        int x = rand() % N;
        int y = rand() % M;
        board[x][y].type = WALL;
        }
    }

    for (int i = 0; i < (N * M) / 20; i++) {
        int x = rand() % N;
        int y = rand() % M;
        if (board[x][y].type == EMPTY) {
            board[x][y].type = FURNITURE;
        }
    }

    int MaxMonsters, MinHealth, MaxHealth;
    switch (difficulty) {
        case 1: MaxMonsters = 2; MinHealth = 1; MaxHealth = 3; break;
        case 2: MaxMonsters = 4; MinHealth = 4; MaxHealth = 6; break;
        case 3: MaxMonsters = 6; MinHealth = 7; MaxHealth = 9; break;
        default: MaxMonsters = 2; MinHealth = 1; MaxHealth = 3;
    }

    int temp = 0;
    while (temp < MaxMonsters) {
        int x = rand() % N;
        int y = rand() % M;
        if (board[x][y].type == EMPTY) {
            board[x][y].health = MinHealth + rand() % (MaxHealth - MinHealth + 1);
            board[x][y].type = '0' + board[x][y].health;
            temp++;
        }
    }

    for (int i = 0; i < HeroesNow; i++) {
        while (1) {
            int x = rand() % N;
            int y = rand() % M;
            if (board[x][y].type == EMPTY) {
                board[x][y].type = maxHeroes[i];
                switch (maxHeroes[i]) {
                    case BARBARIAN: board[x][y].health = 8; break;
                    case DWARF: board[x][y].health = 7; break;
                    case ELF: board[x][y].health = 6; break;
                    case WIZARD: board[x][y].health = 4; break;
                }
                break;
            }
        }
    }
}

void printBoard() {
    printf("   ");
    for (int j = 0; j < M; j++) {
        printf(" %2d", j + 1);
    }
    printf("\n");
    printf("   ");
    for (int j = 0; j < M; j++) {
        printf("----");
    }
    printf("\n");
    for (int i = 0; i < N; i++) {
        printf("%2d|", i + 1);
        for (int j = 0; j < M; j++) {
            printf("  %c", board[i][j].type);
        }
        printf("\n");
    }
}
void heroHealth() {
    printf("\nhealth hrwwn \n");
    for (int i = 0; i < HeroesNow; i++) {
        char hero = maxHeroes[i];
        int found = 0;
        for (int j = 0; j < N && !found; j++) {
            for (int k = 0; k < M && !found; k++) {
                if (board[j][k].type == hero || board[j][k].type == hero + 32) {
                    printf("  %c: %d Health\n", hero, board[j][k].health);
                    found = 1;
                }
            }
        }
        if (!found) {
            printf("  %c: DEAD\n", hero);
        }
    }
}
int getMaxMoves(char hero) {
    switch (hero) {
        case BARBARIAN: return 8;
        case DWARF: return 6;
        case ELF: return 12;
        case WIZARD: return 10;
        default: return 0;
    }
}

int checkBoard(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < M;
}


int startCombat(int attackerHero, int *defenderHealth);
int isWalkable(int x, int y, int hero) {
    char t = board[x][y].type;
    return (t == EMPTY || (t >= '1' && t <= '9'));
}


void moveHero(char hero) {
    int found = 0, x = -1, y = -1;
    for (int i = 0; i < N && !found; i++) {
        for (int j = 0; j < M && !found; j++) {
            if (board[i][j].type == hero || board[i][j].type == hero + 32) {
                x = i;
                y = j;
                found = 1;

            }
        }
    }

    if (!found) {
        printf("O hrwas %c den einai sto tamplo\n", hero);
        return;
    }

    int maxMoves = getMaxMoves(hero);
    char command[50];
    printf("Dwse entolh gia ton hrwa %c (p.x. %c>U2R1): ", hero, hero);
    scanf("%s", command);

    if (command[0] != hero || command[1] != '>') {
        printf("Lathos entolh.\n");
        return;
    }

    int totalMoves = 0, i = 2;

    while (command[i] != '\0') {
        char direction = command[i++];
        int vhmata = 0;

        if (command[i] < '0' || command[i] > '9') {
            printf("Lathos entolh vhmatwn\n");
            return;
        }

        while (command[i] >= '0' && command[i] <= '9') {
            vhmata = vhmata * 10 + (command[i++] - '0');
        }

        totalMoves += vhmata;
        if (totalMoves > maxMoves) {
            printf("Ypervash diathesimwn kinhsewn (%d) gia ton hrwa %c.\n", maxMoves, hero);
            return;
        }

        for (int j = 0; j < vhmata; j++) {
            int xx = x, yy = y;
            switch (direction) {
                case 'U': case 'u': xx--; break;
                case 'D': case 'd': xx++; break;
                case 'L': case 'l': yy--; break;
                case 'R': case 'r': yy++; break;
                default:
                    printf("Lathos kateythinsh %c.\n", direction);
                    return;
            }

            if (!checkBoard(xx, yy)) {
                printf("H kinhsh einai ektos oriwn tou tamplou\n");
                return;
            }
            if (board[xx][yy].type >= '1' && board[xx][yy].type <= '9') {
                printf("O hrwas %c epitithete se teras (%d, %d)\n", hero, xx + 1, yy + 1);
                int last = startCombat(1, &board[xx][yy].health);

                if (board[xx][yy].health <= 0) {
                    printf("To teras skotwthike\n");
                    board[xx][yy] = board[x][y];
                    board[x][y].type = EMPTY;
                    board[x][y].health = 0;
                    x = xx;
                    y = yy;
                } else {
                    printf("To teras epezhse\n");
                    return;
                }
            }
            else if (isWalkable(xx, yy, hero)) {
                board[xx][yy].type = board[x][y].type;
                board[xx][yy].health = board[x][y].health;
                board[x][y].type = EMPTY;
                board[x][y].health = 0;
                x = xx;
                y = yy;
            }
            else {
                printf("Yparxei empodio h allos hrwas%c)\n", board[xx][yy].type);
                return;
            }
        }
    }
}
void epiloghHrwwn() {
    int picked[4] = {0};
    printf("Mporeite na epileksete ews kai 4 hwres ( 1 apo kathe klash)\n");
    printf("Diathesimoi hrwes: B (Barbarian), D (Dwarf), E (Elf), W (Wizard)\n");

    while (HeroesNow < MAX_HEROES) {
        char input[10];
        printf("Epilsekste hrwas #%d h Enter gia stamathmo ", HeroesNow + 1);
        fgets(input, sizeof(input), stdin);
        if (input[0] == '\n') break;

        char hero = toupper(input[0]);

        int val = 0;
        for (int i = 0; i < 4; i++) {
            if (hero == heroes[i] && !picked[i]) {
                maxHeroes[HeroesNow++] = hero;
                picked[i] = 1;
                val = 1;
                break;
            }
        }

        if (!val) {
            printf("Mh egkyrh h epilegmenh klash. Epilekste pali\n");
        }
    }
}
const char* rollDice() {
    int random = rand() % 6;
    if (random < 3) return "kranio";      
    else if (random == 3) return "mayrh";    
    else return "leykh";              
}

void rollTwoDice(const char **dice1, const char **dice2) {
    *dice1 = rollDice();
    *dice2 = rollDice();
}
int rollHits(int numDice) {
    int hits = 0;
    for (int i = 0; i < numDice; i++) {
        const char *face = rollDice();
        if (strcmp(face, "kranio") == 0) hits++;
    }
    return hits;
}

int rollBlocks(int numDice, int defenderIsHero) {
    int blocks = 0;
    const char *needShield = defenderIsHero ? "leykh" : "mayrh";

    for (int i = 0; i < numDice; i++) {
        const char *face = rollDice();
        if (strcmp(face, needShield) == 0) blocks++;
    }
    return blocks;
}

int startCombat(int attackerHero, int *defenderHealth) {
    int attDice = attackerHero ? 3 : 2;
    int defDice = 2;

    int hits = rollHits(attDice);
    int blocks = rollBlocks(defDice, !attackerHero);

    int dmg = hits - blocks;
    if (dmg < 0) dmg = 0;

    *defenderHealth -= dmg;

    printf("Epithesh :  %d xtyphmata / Amyna : %d apokrouseis -> zhmia epitheshs %d\n", hits, blocks, dmg);

    return *defenderHealth <= 0;
}
void doAttack(int attackerHero, int *defenderHealth) {
    printf("Epithesi apo %s:\n", attackerHero ? "hrwa" : "teras");
    int last = startCombat(attackerHero, defenderHealth);
    printf("Zwh antipalou meta thn epithesi %d\n", *defenderHealth);
}
void moveMonsters() {
    int xxx[] = {-1, 1, 0, 0};
    int yyy[] = {0, 0, -1, 1};

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char type = board[i][j].type;
            if (type >= '1' && type <= '9') {
                int monsterHealth = board[i][j].health;
                int attacked = 0;
                for (int k = 0; k < 4; k++) {
                    int ii = i + xxx[k], jj = j + yyy[k];
                    if (checkBoard(ii, jj)) {
                        char target = board[ii][jj].type;
                        if (target == BARBARIAN || target == DWARF || target == ELF || target == WIZARD) {
                            printf("Teras sto (%d,%d) epitithetai ston %c sto (%d,%d)\n", i+1, j+1, target, ii+1, jj+1);
                            startCombat(0, &board[ii][jj].health);
                            if (board[ii][jj].health <= 0) {
                                printf("O hrwas %c pethane\n", target);
                                board[ii][jj].type = EMPTY;
                                board[ii][jj].health = 0;
                            }
                            attacked = 1;
                            break;
                        }
                    }
                }

                if (!attacked) {
                    for (int k = 0; k < 4; k++) {
                        int ii = i + xxx[k], jj = j + yyy[k];
                        if (checkBoard(ii, jj) && board[ii][jj].type == EMPTY) {
                            board[ii][jj].type = type;
                            board[ii][jj].health = monsterHealth;
                            board[i][j].type = EMPTY;
                            board[i][j].health = 0;
                            break;
                        }
                    }
                }
            }
        }
    }
}

int main() {

do {
    printf("Dwste tis diastaseis tou tamplo (N , M )");
    scanf("%d %d", &N, &M);
    getchar();

    if (N < 8 || M < 8 || N > 25 || M > 25) {
        printf("Mh egkyres diastaseis, prepei na einai apo 8 ews 25.\n");
    }
} while (N < 8 || M < 8 || N > 25 || M > 25);
    allocateBoard();

    epiloghHrwwn();

    int difficulty;
    printf("Epilsekste epipedo duskolias (1:Eukolo, 2:Metrio , 3:Duskolo ");
    scanf("%d", &difficulty);
    if (difficulty < 1 || difficulty > 3) {
        printf("Mh egkuros arithmos\n");
        freeBoard();
        return 1;
    }

    initializeBoard(difficulty);
    printBoard();
    heroHealth();
char again;
do {
    char heroMove;
    printf("\nDwse hrwa gia kinhsh (B, D, E, W) h 'x' gia eksodo ");
    scanf("   %c", &heroMove);
    if (heroMove == 'x' || heroMove == 'X') break;

    moveHero(heroMove);
    printBoard();
    moveMonsters();
    printBoard();
    heroHealth();

    printf("\nTheleis na dwseis allh entolh ?  (y/n): ");
    scanf(" %c", &again);
    again = tolower(again);
    if (again != 'y' && again != 'n') {
        printf("Mh egkyrh entolh. Grapse 'y' h 'n'.\n");
    }
} while (again == 'y');



    freeBoard();
    return 0;
}