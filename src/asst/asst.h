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

extern const char *ORIENTATION[2];

extern const char *SHIP_NAMES[4];

extern const char *MOVE_LIST[5];

void clear_screen();

void print_configuration(const Player *p);

void print_grid(Player *attacker, const Player *defender, const int difficulty);

int _strlen(const char *str);

int update_torpedo(Player *attacker, const Player *defender, const int is_sunk);

int is_sunk(const Player *defender, const int ship_number);

int fire(Player *attacker, const Player *defender, const int x, const int y);

int artillery(Player *attacker, const Player *defender, const int x, const int y);

int torpedo(Player *attacker, const Player *defender, const int pos, const int orientation);

int radar_sweep(const Player *defender, const int x, const int y);

void smoke_screen(const Player *p, const int x, const int y);

int can_fit(const Player *p, const int x, const int y, const int ship_size, const int orientation);

int add_ship(const Player *p, const int x, const int y, const int ship_size, const int orientation);

int is_game_over(const Player *defender);

int **initialize_grid();

Player *initialize_player();

int get_column(const char square[4]);

int get_row(const char square[4]);

int get_torpedo_row(const char square[4]);

int is_valid_column(const char square[4]);

int is_valid_row(const char square[4]);

int is_valid_torpedo_row(const char square[4]);

int is_valid_square_input(const char square[4]);

int get_move(const char move[10]);

int get_orientation(const char orientation[11]);

void print_available_moves(const Player *p);

int _rand(int range);

#endif // ASST_H