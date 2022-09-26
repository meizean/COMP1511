// CS Explorer
// cs_explorer.c
//
// This program was written by Zean Mei (z5434651)
// on 06/07/2022
//
// Version 1.0.0 (2022-03-08): Assignment Released.
//
// TODO: Description of your program.
// a 2D game that allows the player to destroy all the monsters hiding in the building by moving around the map, whilst also regenerating health by consuming health potions.

#include <stdio.h>

// Additional libraries here


// Provided constants 
#define SIZE 8
#define PLAYER_STARTING_ROW (SIZE - 1)
#define PLAYER_STARTING_COL 0
#define EMPTY_POINTS 0
#define EMPTY_TYPE 'E'
#define PLAYER_TYPE 'P'
#define MONSTER_TYPE 'M'
#define HEALING_TYPE 'H'
#define BOULDER_TYPE 'B'

// Your constants here

// Provided struct
struct location {
    char occupier;
    int points;
};

// Your structs here

// Provided functions use for game setup
// You do not need to use these functions yourself.
void init_map(struct location map[SIZE][SIZE]);
void place_player_on_starting_location(struct location map[SIZE][SIZE]);

// You will need to use these functions for stage 1.
void print_cheat_map(struct location map[SIZE][SIZE]);
void print_game_play_map(struct location map[SIZE][SIZE]);

// Your functions prototypes here
int moveup_down(char direction, int i, int j, struct location map[SIZE][SIZE]);
int moveleft_right(char direction, int i, int j, struct location map[SIZE][SIZE]);
int clamprow_col(int i);
int clampsize(int row, int col, int size);
int count_sq(int row, int col, int size, char type, struct location map[SIZE][SIZE]);
int checkwon(struct location map[SIZE][SIZE]);
int checklost(int i, int j, struct location map[SIZE][SIZE]);
int count_rec(int start_row, int start_col, int end_row, int end_col, char type, struct location map[SIZE][SIZE]);


int main(void) {

    struct location map[SIZE][SIZE];
    init_map(map);

    printf("Welcome Explorer!!\n");
    
    printf("How many game pieces are on the map?\n");
    
    // TODO: Add code to scan in the number of game pieces here.
    int game_pieces;
    scanf("%d", &game_pieces);
    // TODO: Add code to scan in the details of each game piece and place them
    //       on the map
    printf("Enter the details of game pieces:\n");
    int n = 0;
    int points, rows, cols;
    while (n < game_pieces) {
        scanf("%d %d %d", &points, &rows, &cols);
        if (points < 10 && points > -10) {
            if (rows < SIZE && cols < SIZE && rows >= 0 && cols >= 0) {
                if (map[rows][cols].occupier != PLAYER_TYPE) {
                    map[rows][cols].points = points;
                    if (points > 0) {
                        map[rows][cols].occupier = HEALING_TYPE;
                    } else if (points < 0) {
                        map[rows][cols].occupier = MONSTER_TYPE;
                    } else {
                        map[rows][cols].occupier = BOULDER_TYPE;
                    }
                }
            }
        }
        n++;
    }
            
    // After the game pieces have been added to the map print out the map.
    print_game_play_map(map);
    printf("\nEXPLORE!\n");
    printf("Enter command: ");

    // TODO: keep scanning in commands from the user until the user presses
    // ctrl + d
    char command;
    int i = PLAYER_STARTING_ROW;
    int j = PLAYER_STARTING_COL;
    map[i][j].points = 10;
    while (scanf("%c", &command) == 1) {
        if (command == 'c') {
            //print cheat map command
            print_cheat_map(map);
            print_game_play_map(map);
            //check if the game is lost
            int lose = checklost(i, j, map);   
            if (lose == 1) {
                printf("GAME LOST!\n");
                return 0;
            }
            //check if the game is won
            int won = checkwon(map);
            if (won == 0) {
                printf("GAME WON!\n");
                return 0;
            }
            printf("Enter command: ");
        } else if (command == 'q') {
            //Exiting program command
            printf("Exiting Program!\n");
            return 0;
        } else if (command == 'm') {
            //Moving the player command
            map[i][j].occupier = EMPTY_TYPE;
            char direction;
            scanf(" %c", &direction);
            if (direction == 'u' || direction == 'd') {
                i = moveup_down(direction, i, j, map);
                map[i][j].occupier = PLAYER_TYPE;
            } else if (direction == 'l' || direction == 'r') {
                j = moveleft_right(direction, i, j, map);
                map[i][j].occupier = PLAYER_TYPE;
            } else {
                printf("Invalid direction input\n");
                printf("Enter command: ");
                continue;
            }
            print_game_play_map(map);
            //check if the game is lost
            int lose = checklost(i, j, map);   
            if (lose == 1) {
                printf("GAME LOST!\n");
                return 0;
            }
            //check if the game is won
            int won = checkwon(map);
            if (won == 0) {
                printf("GAME WON!\n");
                return 0;
            }
            printf("Enter command: ");
        } else if (command == 'h') {
            //Print health command
            int health = map[i][j].points;
            printf("Your player is at (%d, %d) with a health of %d.\n", i, j, health);
            print_game_play_map(map);
            printf("Enter command: ");
        } else if (command == 's') {
            //Count points in a square command
            int row, col, size;
            char type;
            if (scanf(" %d %d %d %c", &row, &col, &size, &type) == 4) {
                //clamping row, col and size
                row = clamprow_col(row);
                col = clamprow_col(col);
                size = clampsize(row, col, size);
                int totalpoints = count_sq(row, col, size, type, map);
                if (type == 'M') {
                    printf("Monsters in this section could apply %d health points.\n", totalpoints);
                } else if (type == 'H') {
                    printf("Healing potions in this section could apply %d health points.\n", totalpoints);
                }
                print_game_play_map(map);
                printf("Enter command: ");
            } else {
                printf("Not enough variables.\n");
                printf("Enter command: ");
                continue;
            }
        } else if (command == 'b') {
            //Falling boulders commands
            int row, col;
            scanf(" %d %d", &row, &col);
            row = clamprow_col(row);
            col = clamprow_col(col);
            if (map[row][col].occupier != PLAYER_TYPE) {
                map[row][col].occupier = BOULDER_TYPE;
                map[row][col].points = EMPTY_POINTS;
            } else {
                print_game_play_map(map);
                printf("GAME LOST!\n");
                return 0;
            }
            print_game_play_map(map);
            //check if the game is lost
            int lose = checklost(i, j, map);   
            if (lose == 1) {
                printf("GAME LOST!\n");
                return 0;
            }
            //check if the game is won
            int won = checkwon(map);
            if (won == 0) {
                printf("GAME WON!\n");
                return 0;
            }
            printf("Enter command: ");
        } else if (command == 'r') {
            //Counting points in rectangle command
            int start_row, start_col, end_row, end_col, totalpoints;
            char type;
            if (scanf(" %d %d %d %d %c", &start_row, &start_col, &end_row, &end_col, &type) == 5) {
                //clamping row, col
                start_row = clamprow_col(start_row);
                start_col = clamprow_col(start_col);
                end_row = clamprow_col(end_row);
                end_col = clamprow_col(end_col);
                totalpoints = count_rec(start_row, start_col, end_row, end_col, type, map);
                if (type == 'M') {
                    printf("Monsters in this section could apply %d health points.\n", totalpoints);
                } else if (type == 'H') {
                    printf("Healing Potions in this section could apply %d health points.\n", totalpoints);
                }
                print_game_play_map(map);
                printf("Enter command: ");
            } else {
                printf("Not enough variables.\n");
                printf("Enter command: ");
            }
        }
            
    }
    
    return 0;
}


////////////////////////////////////////////////////////////////////////////////
///////////////////////////// ADDITIONAL FUNCTIONS /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// TODO: you may need to add additional functions here
//function for moving up or down
int moveup_down(char direction, int i, int j, struct location map[SIZE][SIZE]) {
    if (direction == 'u') {
        if (i - 1 >= 0 && map[i - 1][j].occupier != BOULDER_TYPE) {
            int point = map[i][j].points;
            i--;
            map[i][j].points = map[i][j].points + point;
            return i;
        }    
    } else if (direction == 'd') {
        if (i + 1 < SIZE && map[i + 1][j].occupier != BOULDER_TYPE) {  
            int point = map[i][j].points;
            i++;
            map[i][j].points = map[i][j].points + point;
            return i;
        }
    }
    return i;
}
//function for moving left or right
int moveleft_right(char direction, int i, int j, struct location map[SIZE][SIZE]) {  
    if (direction == 'l') {
        if (j - 1 >= 0 && map[i][j - 1].occupier != BOULDER_TYPE) {
            int point = map[i][j].points;
            j--;
            map[i][j].points = map[i][j].points + point;
            return j;
        }
    } else if (direction == 'r') {
        if (j + 1 < SIZE && map[i][j + 1].occupier != BOULDER_TYPE) {
            int point = map[i][j].points;
            j++;
            map[i][j].points = map[i][j].points + point;
            return j;
        }
    }
    return j;   
}
//function for clamping row and col
int clamprow_col(int i) {
    if (i < 0) {
        i = 0;
        return i;
    } else if (i >= SIZE) {
        i = SIZE - 1;
        return i;
    }
    return i;
}

//function for clamping size
int clampsize(int row, int col, int size) {
    if (size + row > SIZE) {
        size = SIZE - row;
        if (size + col > SIZE) {
            size = SIZE - col;
            return size;
        }
        return size;
    } else if (size + col > SIZE) {
        size = SIZE -col;
        if (size + row > SIZE) {
            size = SIZE - row;
            return size;
        }
        return size;
    }
    return size;
} 
//function for counting points in a square
int count_sq(int row, int col, int size, char type, struct location map[SIZE][SIZE]) {
    int totalpoints = 0;
    if (type == 'M') {
        int i = row;
        while (i >= row && i < row + size) {
            int j = col;
            while (j >= col && j < col + size) {
                if (map[i][j].occupier == MONSTER_TYPE) {
                    totalpoints = totalpoints + map[i][j].points;
                }
                j++;
            }
            i++;
        }
        return totalpoints;
    } else if (type == 'H') {
        int i = row;
        while (i >= row && i < row + size) {
            int j = col;
            while (j >= col && j < col + size) {
                if (map[i][j].occupier == HEALING_TYPE) {
                    totalpoints = totalpoints + map[i][j].points;
                }
                j++;
            }
            i++;
        }
        return totalpoints;
    } else {
        return totalpoints;
    }
}
//function for checking if the game is won
int checkwon(struct location map[SIZE][SIZE]) {
    int won = 0;
    int x = 0;
    while (x < SIZE) {
        int y = 0;
        while (y < SIZE) {
            if (map[x][y].occupier == MONSTER_TYPE) {
                won++;
            }
            y++;
        }
        x++;
    }
    return won;
}
//function for checking if the game is lost
int checklost(int i, int j, struct location map[SIZE][SIZE]) {
    int lost = 0;
    if (map[i][j].points <= 0) {
        lost = 1;
    } else if (i - 1 < 0 || map[i - 1][j].occupier == BOULDER_TYPE) {
        if (i + 1 >= SIZE || map[i + 1][j].occupier == BOULDER_TYPE) {
            if (j - 1 < 0 || map[i][j - 1].occupier == BOULDER_TYPE) {
                if (j + 1 >= SIZE || map[i][j + 1].occupier == BOULDER_TYPE) {
                    lost = 1;
                }
            }
        }
    }
    return lost;
} 
//function for counting points in a rectangle
int count_rec(int start_row, int start_col, int end_row, int end_col, char type, struct location map[SIZE][SIZE]) {
    int totalpoints = 0;
    int i, j, x, y;
    //organize row in right order
    if (start_row <= end_row) {
        i = start_row;
        x = end_row;
    } else if (start_row > end_row) {
        i = end_row;
        x = start_row;
    }
    if (type == 'M') {
        while (i <= x) {
            //organize col in right order
            if (start_col <= end_col) {
                j = start_col;
                y = end_col;
            } else if ( start_col > end_col) {
                j = end_col;
                y = start_col;
            }
            while (j <= y) {
                if (map[i][j].occupier == MONSTER_TYPE) {
                    totalpoints = totalpoints + map[i][j].points;
                }
                j++;
            }
            i++;
        }
        return totalpoints;
    } else if (type == 'H') {
        while (i <= x) {
            if (start_col <= end_col) {
                j = start_col;
                y = end_col;
            } else if (start_col > end_col) {
                j = end_col;
                y = start_col;
            }
            while (j <= y) {
                if (map[i][j].occupier == HEALING_TYPE) {
                    totalpoints = totalpoints + map[i][j].points;
                }
                j++;
            }
            i++;
        }
        return totalpoints;
    } else {
        return totalpoints;
    }
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////// PROVIDED FUNCTIONS //////////////////////////////
/////////////////////////// (DO NOT EDIT BELOW HERE) ///////////////////////////
////////////////////////////////////////////////////////////////////////////////

// Provided Function
// Initalises all elements on the map to be empty to prevent access errors.
void init_map(struct location map[SIZE][SIZE]) {
    int row = 0;
    while (row < SIZE) {
        int col = 0;
        while (col < SIZE) {
            struct location curr_loc;
            curr_loc.occupier = EMPTY_TYPE;
            curr_loc.points = EMPTY_POINTS;
            map[row][col] = curr_loc;
            col++;
        }
        row++;
    }

    place_player_on_starting_location(map);
}

// Provided Function
// Places the player in the bottom left most location.
void place_player_on_starting_location(struct location map[SIZE][SIZE]) {
    map[PLAYER_STARTING_ROW][PLAYER_STARTING_COL].occupier = PLAYER_TYPE;
}

// Provided Function
// Prints out map with alphabetic values. Monsters are represented with 'M',
// healing potions in 'H', boulders with 'B' and the player with 'P'.
void print_game_play_map(struct location map[SIZE][SIZE]) {
    printf(" -----------------\n");
    int row = 0;
    while (row < SIZE) {
        printf("| ");
        int col = 0;
        while (col < SIZE) {
            if (map[row][col].occupier == EMPTY_TYPE) {
                printf("- ");
            } else {
                printf("%c ", map[row][col].occupier);
            }
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(" -----------------\n");
}

// Provided Function
// Prints out map with numerical values. Monsters are represented in red,
// healing potions in blue, boulder in green and the player in yellow.
//
// We use some functionality you have not been taught to make sure that
// colours do not appear during testing.
void print_cheat_map(struct location map[SIZE][SIZE]) {

    printf(" -----------------\n");
    int row = 0;
    while (row < SIZE) {
        printf("| ");
        int col = 0;
        while (col < SIZE) {
            if (map[row][col].occupier == PLAYER_TYPE) {
                // print the player in purple
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;35m");
                #endif
                // ----------------------------------------
                printf("%c ", PLAYER_TYPE);
            } else if (map[row][col].occupier == HEALING_TYPE) {
                // print healing potion in green
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;32m");
                #endif
                // ----------------------------------------
                printf("%d ", map[row][col].points);
            } else if (map[row][col].occupier == MONSTER_TYPE) {
                // print monsters in red
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;31m");
                #endif
                // ----------------------------------------
                printf("%d ", -map[row][col].points);
            } else if (map[row][col].occupier == BOULDER_TYPE) {
                // print boulder in blue
                // ----------------------------------------
                // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
                #ifndef NO_COLORS
                printf("\033[1;34m");
                #endif
                // ----------------------------------------
                printf("%d ", map[row][col].points);
            } else {
                // print empty squares in the default colour
                printf("- ");
            }
            // ----------------------------------------
            // YOU DO NOT NEED TO UNDERSTAND THIS CODE.
            // reset the colour back to default
            #ifndef NO_COLORS
            printf("\033[0m");
            #endif
            // ----------------------------------------
            col++;
        }
        printf("|\n");
        row++;
    }
    printf(" -----------------\n");
}