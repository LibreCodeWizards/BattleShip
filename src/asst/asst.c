//
// Created by ahmad on 10/12/24.
//
#include <stdlib.h>
#include <stdio.h>
#include "asst.h"

void print_grid(Player *attacker, Player *defender, int difficulty)
{
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        printf("%c\t", 'A' + i);
    }

    printf("\n");

    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        printf("%d\t", i + 1);
        for (size_t j = 0; j < GRID_SIZE; j++)
        {
            if (defender->grid[i][j] < -1)
            {
                int ship_number = abs(defender->grid[i][j]);

                if (defender->ships[ship_number - 2] == 0)
                {
                    printf("%d\t", ship_number);
                }
                else
                {
                    printf("*\t");
                }
            }
            else if (defender->grid[i][j] == -1)
            {
                if (difficulty == 0)
                {
                    printf("o\t");
                }
                else
                {
                    printf("~\t");
                }
            }
            else
            {
                printf("~\t");
            }
        }
        printf("\n");
    }
}

int update_torpedo(Player *attacker, Player *defender, int is_sunk)
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

int is_sunk(Player *p, int ship_number)
{
    // Thing hit is not a ship
    if (ship_number < 0)
    {
        return 0;
    }
    // NOTE: this considers that the first ship on the grid will be 1, the second 2...
    // If we want to have the ships represented by their size, i.e. ship 1 represented by 2 and so on...
    // we would need to change the -1 to -2
    return p->ships[ship_number - 2] == 0;
}

int fire(Player *attacker, Player *defender, int x, int y)
{
    // Checks if a grid at this index contains a ship
    // Decrements the ship HP
    // Returns the number at the grid[x][y]
    int item_hit = defender->grid[x][y];
    if (item_hit > 0)
    {
        // NOTE: this considers that the first ship on the grid will be 1, the second 2...
        // If we want to have the ships represented by their size, i.e. ship 1 represented by 2 and so on...
        // we would need to change the -1 to -2
        defender->ships[item_hit - 2]--;
        defender->grid[x][y] *= -1;
    }
    if (is_sunk(defender, item_hit) == 1)
    {
        attacker->smoke_screen += 1;
    }
    return item_hit;
}

int artillery(Player *attacker, Player *defender, const int x, const int y)
{
    // Maintain booleans to ensure that player gets his abilities if any ship is sunk
    int got_artillery = 0;
    int got_torpedo = 0;
    int got_hit = 0;
    for (int i = x; i < min(GRID_SIZE, x + 2); ++i)
    {
        for (int j = y; j < min(GRID_SIZE, y + 2); ++j)
        {
            int ship_hit = fire(attacker, defender, i, j);
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

int torpedo(Player *attacker, Player *defender, const int pos, int oreintation)
{
    // orientation 0 for row, 1 for col

    int hit = 0;
    for (size_t i = 0; i < GRID_SIZE; i++)
    {
        int square_hit;
        if (oreintation == 0)
        {
            int square_hit = fire(attacker, defender, pos, i);
            // Can tell the user if he sunk a ship later using the is_sunk function if we need to
        }
        else
        {
            int square_hit = fire(attacker, defender, i, pos);
        }

        if (square_hit > 0)
        {
            hit = 1;
        }
    }

    return hit;
}

int radar_sweep(Player *defender, int x, int y)
{
    for (int i = x; i < min(GRID_SIZE, x + 2); ++i)
    {
        for (int j = y; j < min(GRID_SIZE, y + 2); ++j)
        {
            if (defender->grid[i][j] > 0 && defender->visible_grid[i][j] > 0)
            {
                return 1;
            }
        }
    }

    return 0;
}

void smoke_screen(Player *p, int x, int y)
{
    for (int i = x; i < min(GRID_SIZE, x + 2); ++i)
    {
        for (int j = y; j < min(GRID_SIZE, y + 2); ++j)
        {
            p->visible_grid[i][j] = 0;
        }
    }
}

int add_ship(Player *p, int x, int y, int ship_size, int orientation)
{
    // making sure ships fit in this orientation (0 for ho)izontal, 1 for vertical)
    if ((orientation == 0 && x + ship_size > GRID_SIZE) || (orientation == 1 && y + ship_size > GRID_SIZE))
        return 0;

    // making sure this orientation doesnt overlap with existing ship
    for (size_t i = 0; i < ship_size; i++)
    {
        if (orientation == 0)
        {
            if (p->grid[x][y + i] > 0)
            {
                return 0;
            }
        }
        else
        {
            if (p->grid[x + i][y] > 0)
            {
                return 0;
            }
        }
    }

    // No overlap or out of bound, so we can place
    for (size_t i = 0; i < ship_size; i++)
    {
        if (orientation == 0)
        {
            p->grid[x][y + i] = ship_size;
        }
        else
        {
            p->grid[x + i][y] = ship_size;
        }
    }

    return 1;
}

int is_game_over(Player *defender)
{
    for (size_t i = 0; i < NUM_SHIPS; i++)
    {
        if (defender->ships[i] > 0)
        {
            return 0;
        }
    }

    return 1;
}

// Initializes a 2D matrix
int **initialize_grid()
{
    // First dimension have 10 pointers.
    int **grid = (int **)calloc(GRID_SIZE, sizeof(int *));

    for (int i = 0; i < GRID_SIZE; i++)
    {
        // Second dimension have 10 elements.
        grid[i] = (int *)calloc(GRID_SIZE, sizeof(int));
    }

    return grid;
}

Player *initialize_player()
{
    Player *p = (Player *)malloc(sizeof(Player));

    // Setting HP for each ship
    p->ships = (int *)calloc(NUM_SHIPS, sizeof(int));
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
    p->visible_grid = initialize_grid();

    return p;
}

// turns square input into coorinates on the grid
int get_column(char square[4])
{
    return square[0] - 'A';
}

int get_row(char square[4])
{
    return square[1] != '1' ? square[1] - '1' : (square[2] == '0' ? 9 : 0);
}

int is_valid_square(char square[4])
{
    return 'A' <= square[0] & 'J' >= square[0] & '1' <= square[1] & '9' >= square[1] & (square[1] != '1' || (square[2] == '0' || square[2] == '\0'));
}