//
// Created by ahmad on 10/12/24.
//
#include <stdlib.h>
#include "asst.h"

// Initializes a 2D matrix
int** initialize_grid()
{
    // First dimension have 10 pointers.
    int** grid = (int**)calloc(GRID_SIZE, sizeof(int*));

    for (int i = 0; i < GRID_SIZE; i++)
    {
        // Second dimension have 10 elements.
        grid[i] = (int*)calloc(GRID_SIZE, sizeof(int));
    }

    return grid;
}

Player* initialize_player()
{
    Player* p = (Player*)malloc(sizeof(Player));

    // Setting HP for each ship
    p->ships.ship1 = 1;
    p->ships.ship2 = 2;
    p->ships.ship3 = 3;
    p->ships.ship4 = 4;

    // Setting the abilities
    p->radar_sweep = 3;
    p->smoke_screen = 0;
    p->artillery = 0;
    p->torpedo = 0;

    p->grid = initialize_grid();

    return p;
}
