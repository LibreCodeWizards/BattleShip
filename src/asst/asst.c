//
// Created by ahmad on 10/12/24.
//

#include "asst.h"
#include <stdlib.h>
#include <stdio.h>

const char* ORIENTATION[2] =
{
    "vertical",
    "horizontal"
};

const char* SHIP_NAMES[4] =
{
    "Submarine",
    "Destroyer",
    "Battleship",
    "Carrier"
};

const char* MOVE_LIST[5] =
{
    "Fire",
    "Radar",
    "Smoke",
    "Artillery",
    "Torpedo"
};

void clear_screen()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

void print_configuration(const Player* p)
{
    if (!p)
        return;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        printf("\t%c", 'A' + i);
    }
    printf("\n");

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        printf("%d\t", i + 1);
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (p->grid[i][j] > 0)
                printf("%i\t", p->grid[i][j]);
            else
                printf("~\t");
        }
        printf("\n");
    }
}

void print_grid(Player* attacker, const Player* defender, const int difficulty)
{
    if (!defender)
        return;

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        printf("\t%c", 'A' + i);
    }

    printf("\n");

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        printf("%d\t", i + 1);
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (defender->grid[i][j] < -1)
            {
                const int ship_number = abs(defender->grid[i][j]);

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

int _strlen(const char* str)
{
    if (!str)
        return -1;

    int len = 0;
    int i = 0;

    while (str[i++] != '\0')
    {
        len++;
    }
    return len;
}

int update_torpedo(Player* attacker, const Player* defender, const int is_sunk)
{
    attacker->torpedo = 0;

    if (is_sunk)
    {
        int count = 0;
        // Counts the number of currently sunk ships
        for (int i = 0; i < NUM_SHIPS; ++i)
        {
            count += defender->ships[i] == 0;
        }

        if (count == 3)
        {
            attacker->torpedo = 1;
        }

        return attacker->torpedo;
    }
    return 0;
}

int is_sunk(const Player* defender, const int ship_number)
{
    // Thing hit is not a ship
    if (ship_number < 0)
    {
        return 0;
    }
    // NOTE: this considers that the first ship on the grid will be 1, the second 2...
    // If we want to have the ships represented by their size, i.e. ship 1 represented by 2 and so on...
    // we would need to change the -1 to -2
    return defender->ships[ship_number - 2] == 0;
}

int fire(Player* attacker, const Player* defender, const int x, const int y)
{
    // Checks if a grid at this index contains a ship
    // Decrements the ship HP
    // Returns the number at the grid[x][y]
    const int item_hit = defender->grid[x][y];
    if (item_hit > 0)
    {
        // NOTE: this considers that the first ship on the grid will be 1, the second 2...
        // If we want to have the ships represented by their size, i.e. ship 1 represented by 2 and so on...
        // we would need to change the -1 to -2

        defender->ships[item_hit - 2]--;
        defender->grid[x][y] *= -1;

        if (is_sunk(defender, item_hit))
        {
            printf("You have sunk your opponent's %s\n", SHIP_NAMES[item_hit - 2]);
            attacker->smoke_screen += 1;

            // Gives the player an artillery when he sinks a ship
            if (attacker->artillery == 0)
            {
                attacker->artillery = 1;
            }
        }
    }

    return item_hit;
}

int artillery(Player* attacker, const Player* defender, const int x, const int y)
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

int torpedo(Player* attacker, const Player* defender, const int pos, const int orientation)
{
    // orientation 0 for row, 1 for col

    int hit = 0;
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        int square_hit;

        if (orientation == 0)
        {
            square_hit = fire(attacker, defender, pos, i);
            // Can tell the user if he sunk a ship later using the is_sunk function if we need to
        }
        else
        {
            square_hit = fire(attacker, defender, i, pos);
        }

        if (square_hit > 0)
        {
            hit = 1;
        }
    }

    return hit;
}

int radar_sweep(const Player* defender, const int x, const int y)
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

void smoke_screen(const Player* p, const int x, const int y)
{
    for (int i = x; i < min(GRID_SIZE, x + 2); ++i)
    {
        for (int j = y; j < min(GRID_SIZE, y + 2); ++j)
        {
            p->visible_grid[i][j] = 0;
        }
    }
}

/*
 * Checks if a ship can fit in a certain place in the grid.
 * returns a boolean.
 */
int can_fit(const Player* p, const int x, const int y, const int ship_size, const int orientation)
{
    // Make sure ships fit in this orientation (0 for horizontal, 1 for vertical)
    if ((orientation == 1 && (x + ship_size) - 1 >= GRID_SIZE) || (orientation == 0 && (y + ship_size) - 1 >=
        GRID_SIZE))
        return 0;

    // Make sure the ship can fit in this orientation and doesn't overlap with another
    if (orientation == 0)
    {
        for (int i = 0; i < ship_size; ++i)
        {
            if (p->grid[x][y + i] > 0)
                return 0;
        }
    }

    else
    {
        for (int i = 0; i < ship_size; ++i)
        {
            if (p->grid[x + i][y] > 0)
                return 0;
        }
    }

    // If not return 1
    return 1;
}

/*
 * Adds a ship to the grid.
 * Returns a boolean to tell if the ship has been placed successfully.
 */
int add_ship(const Player* p, const int x, const int y, const int ship_size, const int orientation)
{
    if (!can_fit(p, x, y, ship_size, orientation))
        return 0;

    // No overlap or out of bound, so we can place

    if (orientation == 0)
    {
        for (int i = 0; i < ship_size; ++i)
        {
            p->grid[x][y + i] = ship_size;

            // Also add the ship to the visible grid
            p->visible_grid[x][y + i] = 1;
        }
    }

    else
    {
        for (int i = 0; i < ship_size; ++i)
        {
            p->grid[x + i][y] = ship_size;

            // Also add the ship to the visible grid
            p->visible_grid[x + i][y] = 1;
        }
    }

    return 1;
}

int is_game_over(const Player* defender)
{
    for (int i = 0; i < NUM_SHIPS; ++i)
    {
        if (defender->ships[i] > 0)
        {
            return 0;
        }
    }

    return 1;
}

// Initializes a 2D matrix
int** initialize_grid()
{
    // First dimension have 10 pointers.
    int** grid = (int**)calloc(GRID_SIZE, sizeof(int*));

    for (int i = 0; i < GRID_SIZE; ++i)
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
    p->visible_grid = initialize_grid();

    return p;
}


// Turns square input into coordinates on the grid.
int get_column(const char square[4])
{
    return square[0] - 'A';
}

int get_row(const char square[4])
{
    return square[1] != '1' ? square[1] - '1' : (square[2] == '0' ? 9 : 0);
}

int get_torpedo_row(const char square[4])
{
    return square[0] != '1' ? square[0] - '1' : (square[1] == '0' ? 9 : 0);
}

int is_valid_column(const char square[4])
{
    if (!square)
        return -1;
    return 'A' <= square[0] && 'J' >= square[0];
}

int is_valid_row(const char square[4])
{
    if (!square)
        return -1;
    return '1' <= square[1] && '9' >= square[1] &&
    (
        square[1] != '1' ||
        (
            square[2] == '0' ||
            square[2] == '\0'
        )
    );
}

int is_valid_torpedo_row(const char square[4])
{
    if (!square)
        return -1;
    return '1' <= square[0] && '9' >= square[0] &&
    (
        square[0] != '1' ||
        (
            square[1] == '0' ||
            square[1] == '\0'
        )
    );
}

// Changed bitwise AND to logical AND, this saves time when executed
// Once one condition is not met it short circuits instead of evaluating all other operations
int is_valid_square_input(const char square[4])
{
    return is_valid_column(square) && is_valid_column(square);
}

int get_move(const char move[10])
{
    if (!move)
        return -1;

    const int len = _strlen(move);

    switch (len)
    {
    case 4:
        if (move[0] == 'F' | move[0] == 'f')
        {
            int is_fire = 1;
            for (int i = 1; i < 4; ++i)
            {
                if (move[i] != MOVE_LIST[0][i])
                {
                    is_fire = 0;
                    break;
                }
            }
            return is_fire ? 0 : -1;
        }
        break;
    case 5:
        if (move[0] == 'R' | move[0] == 'r')
        {
            int is_radar = 1;
            for (int i = 1; i < 5; ++i)
            {
                if (move[i] != MOVE_LIST[1][i])
                {
                    is_radar = 0;
                    break;
                }
            }
            return is_radar ? 1 : -1;
        }
        if (move[0] == 'S' | move[0] == 's')
        {
            int is_smoke = 1;
            for (int i = 1; i < 5; ++i)
            {
                if (move[i] != MOVE_LIST[2][i])
                {
                    is_smoke = 0;
                    break;
                }
            }
            return is_smoke ? 2 : -1;
        }
        break;
    case 9:
        if (move[0] == 'A' | move[0] == 'a')
        {
            int is_artillery = 1;
            for (int i = 1; i < 9; ++i)
            {
                if (move[i] != MOVE_LIST[3][i])
                {
                    is_artillery = 0;
                    break;
                }
            }
            return is_artillery ? 3 : -1;
        }
        break;
    case 7:
        if (move[0] == 'T' | move[0] == 't')
        {
            int is_torpedo = 1;
            for (int i = 1; i < 7; ++i)
            {
                if (move[i] != MOVE_LIST[4][i])
                {
                    is_torpedo = 0;
                    break;
                }
            }
            return is_torpedo ? 4 : -1;
        }
        break;
    default:
        return -1;
    }

    return -1;
}

int get_orientation(const char orientation[11])
{
    if (orientation == nullptr)
        return -1;

    int len = _strlen(orientation);

    switch (len)
    {
    case 8:
        if (orientation[0] == 'V' | orientation[0] == 'v')
        {
            for (int i = 1; i < 9; ++i)
            {
                if (orientation[i] != ORIENTATION[0][i])
                    return -1;
            }
            return 1;
        }
        break;
    case 10:
        if (orientation[0] == 'H' | orientation[0] == 'h')
        {
            for (int i = 1; i < 10; ++i)
            {
                if (orientation[i] != ORIENTATION[1][i])
                    return -1;
            }
            return 0;
        }
        break;
    default:
        return -1;
    }

    return -1;
}

// Generates a random number
int _rand(const int range)
{
    // Since we are not allowed to use any libraries we will generate
    // a random number by allocating a random memory address in the
    // memory using malloc, we will use the garbage value as a rand.

    int* rand = (int*)malloc(sizeof(int));

    int res = (rand[0] / 7) % range;

    free(rand);

    return res;

    /*srand48(time(nullptr));
    return rand() % range;*/
}

void print_available_moves(const Player* p)
{
    printf("Available moves:\n");
    printf("Fire: ∞\n");
    printf("Torpedo: %d\n", p->torpedo);
    printf("Artillery: %d\n", p->artillery);
    printf("Radar Sweeps: %d\n", p->radar_sweep);
    printf("Smoke Screens: %d\n", p->smoke_screen);
}
