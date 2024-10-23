//
// Created by ahmad on 10/12/24.
//

#ifndef ASST_H
#define ASST_H
#define GRID_SIZE 10
#define NUM_SHIPS 4
#define min(a, b) ((a) < (b) ? (a) : (b))

typedef struct
{
    int *ships;
    int radar_sweep;
    int smoke_screen;
    int artillery;
    int torpedo;
    int **grid;
    int **visible_grid;
} Player;

void print_configuration(Player *p);

void print_grid(Player *attacker, Player *defender, int difficulty);

int update_torpedo(Player *attacker, Player *defender, int is_sunk);

int is_sunk(Player *p, int ship_number);

int fire(Player *attacker, Player *defender, int x, int y);

int artillery(Player *attacker, Player *defender, int x, int y);

int radar_sweep(Player *defender, int x, int y);

int add_ship(Player *p, int x, int y, int ship_size, int orientation);

int is_game_over(Player *defender);

int **initialize_grid();

int get_column(char square[4]);

int get_row(char square[4]);

int is_valid_square(char square[4]);

int get_orientation(char orientation[11]);

void clear_screen();

Player *initialize_player();

#endif // ASST_H
