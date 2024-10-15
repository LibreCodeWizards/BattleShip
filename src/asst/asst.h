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
    int* ships;
    int radar_sweep;
    int smoke_screen;
    int artillery;
    int torpedo;
    int** grid;
} Player;

void print_grid(int** grid);

int update_torpedo(Player* attacker, Player* defender, int is_sunk);

int is_sunk(Player* p, int ship_number);

int fire(Player* p, int x, int y);

int artillery(Player* attacker, Player* defender, int x, int y);

int** initialize_grid();

Player* initialize_player();

#endif //ASST_H
