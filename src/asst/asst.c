//
// Created by ahmad on 10/12/24.
//
#include <stdlib.h>
#include "asst.h"

void print_grid(int** grid)
{
    //prints the grid
}

int update_torpedo(Player* attacker, Player* defender, int is_sunk)
{
    attacker->torpedo = 0;

    if (is_sunk)
    {
        int count = 0;
        for (int i = 0; i < NUM_SHIPS; ++i)
        {
            count += defender->ships[i] == 0;
        }
        if (count == 3)
        {
            attacker->torpedo = 1;
        }
        return attacker->torpedo == 1;
    }
    return 0;
}

int is_sunk(Player* p, int ship_number)
{
    // returns 1 if i-th ship of the player is sunk i.e shipHP= 0
    // otherwise 0
}

int fire(Player* p, int x, int y)
{
    // Checks if a grid at this index contains a ship
    // Decrements the ship HP
    // Returns the number at the grid[x][y]
    return 1;
}

int artillery(Player* attacker, Player* defender, const int x, const int y)
{
    int got_artillery = 0;
    int got_torpedo = 0;
    int got_hit = 0;
    for (int i = x; i < min(GRID_SIZE, x + 2); ++i)
    {
        for (int j = y; j < min(GRID_SIZE, y+2); ++j)
        {
            int ship_hit = fire(defender, i, j);
            if (ship_hit > 0)
            {
                got_hit = 1;
                int ship_sunk = is_sunk(defender, ship_hit);
                if (!got_artillery && ship_sunk)
                {
                    got_artillery = 1;
                    attacker->artillery = 1;
                }
                // Short circuits so update_torpedo won't be called each time
                if (!got_torpedo && update_torpedo(attacker, defender, ship_sunk))
                {
                    got_torpedo = 1;
                }
            }
        }
    }

    return got_hit;
}

int torpedo(Player* attacker, Player* defender, const int x, const int y, int oreintation)
{
    //TODO
}


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
    p->ships = (int*)calloc(NUM_SHIPS, sizeof(int));
    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        p->ships[i] = i + 2;
    }

    // Setting the abilities
    p->radar_sweep = 3;
    p->smoke_screen = 0;
    p->artillery = 0;
    p->torpedo = 0;

    p->grid = initialize_grid();

    return p;
}

// turns square input into coordinates on the grid
int column(char square[3])
{
    return square[0]-'A';
}

int row(char square[3])
{
    return square[1]-'1';
}