//
// Created by ahmad on 10/12/24.
//

#ifndef ASST_H
#define ASST_H
#define GRID_SIZE 10

typedef struct
{
    int ship1;
    int ship2;
    int ship3;
    int ship4;
} Ships;

typedef struct
{
    Ships ships;
    int radar_sweep;
    int smoke_screen;
    int artillery;
    int torpedo;
    int** grid;
} Player;

int** initialize_grid();

Player* initialize_player();

#endif //ASST_H
