//
// Created by ahmad on 10/12/24.
//

#include <stdlib.h>
#include <stdio.h>
#include "asst.h"

const char *ORIENTATION[2] =
    {
        "vertical",
        "horizontal"};

const char *SHIP_NAMES[4] =
    {
        "Submarine",
        "Destroyer",
        "Battleship",
        "Carrier"};

const char *MOVE_LIST[5] =
    {
        "Fire",
        "Radar",
        "Smoke",
        "Artillery",
        "Torpedo"};
/*
 * Requires: Nothing
 * Effects: Clears the screen for the next round
 */
void clear_screen()
{
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
}

/*
 * Requires: Nothing
 * Effects: Prints the player grid
 */
void print_configuration(const Player *p)
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

/*
 * Requires: Nothing
 * Effects: Prints the defender's grid during the game to show the attacker their opponent grid
 *          if the difficulty is set to 0 (easy) the hit squares are displayed as 'o', otherwise
 *          they are displayed as '~'.
 */
void print_grid(Player *attacker, const Player *defender, const int difficulty)
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

/*
 * Requires: str should end with '\0' if it has more than 0 character
 * Effects: Returns the length of the string without the '\0.
 */
int _strlen(const char *str)
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

/*
 * Requires: nothing
 * Effects: Gives the player a torpedo if he sunk a ship and the ship was the third one he sunk and returns 1, else
 *          resets the player's torpedos to 0 and returns 0
 */
int update_torpedo(Player *attacker, const Player *defender, const int is_sunk)
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

/*
 * Requires: Nothing
 * Effects: Returns an integer as boolean by checking if the ship health is 0 (it is sunk returns 1) or not (returns 0).
 */
int is_sunk(const Player *defender, const int ship_number)
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

/*
Requires: x and y be within the bounds of the grid
Effects: decrements the hp of the hit ship in the defender, updates the grid to mark the hit or the miss, gives the attack smokescreen and artillary if the ship is sunk, and prints if the ship is sunk
Returns the number found at grid[x][y] before it was updated

This function does a lot of things because it is used as the main helper function in all other functions
*/
int fire(Player *attacker, const Player *defender, const int x, const int y)
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
    else if (item_hit == 0)
    {
        // Mark as miss
        defender->grid[x][y] = -1;
    }

    return item_hit;
}

/*
Requires: x and y be within the bounds of the grid
Effects: Fires at the 2x2 area starting at xy and updates the grid to mark hits or misses and gives the attacker the correct powerups. Prints the sunk ships if there is one or more.
*/
int artillery(Player *attacker, const Player *defender, const int x, const int y)
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

/*
Requires: Pos be within the range of the grid, orientation be 0 (horizontal) or 1 (vertical)
Effects: Fires at the specified row or column, prints the sunk ships if there are 1 or more, and gives the player the correct powerups

Note: Doesn't handle giving the attacking player a torpedo since if he is using a torpedo, then he already sunk 3/4 ships, meaning he cannot get another one.
*/
int torpedo(Player *attacker, const Player *defender, const int pos, const int orientation)
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

/*
Requires: x and y be within the bounds of the grid
Effects: returns 1 if there are any enemy ships within the 2x2 area of the opponent's visible grid (the one that has the radar sweeps applied) starting from xy, else 0
*/
int radar_sweep(const Player *defender, const int x, const int y)
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

/*
Requires: x and y be within the bounds of the grid
Effects: updates the player's visible grid to make the 2x2 area starting at xy invisible (0) to enemy radar sweeps
 */
void smoke_screen(const Player *p, const int x, const int y)
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
Requires: Orientation is 0 (horizontal) or 1 (vertical)
Effects: returns 1 if ships can fit at grid[x][y] without going out of bounds or colliding with another ship, else 0
 */
int can_fit(const Player *p, const int x, const int y, const int ship_size, const int orientation)
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
Requires: Orientation is 0 (horizontal) or 1 (vertical)
Effects: Marks the place of the ship with its corresponding number in the player's grid and with 1 in the player's visible grid and returns 1 if the ship can fit, else returns 0
 */
int add_ship(const Player *p, const int x, const int y, const int ship_size, const int orientation)
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

void remove_ship(const Player *p, int x, int y, const int ship_size, const int orientation)
{
    if (!p || p == NULL || (orientation == 1 && (x + ship_size) - 1 >= GRID_SIZE) || (orientation == 0 && (y + ship_size) - 1 >= GRID_SIZE))
    {
        return;
    }

    if (orientation == 0)
    {
        for (int i = y; i < y + ship_size; i++)
        {
            if (p->grid[x][i] > 1)
            {
                p->grid[x][i] = 0;
                p->visible_grid[x][i] = 0;
            }
        }
    }
    else
    {
        for (int i = x; i < x + ship_size; i++)
        {
            if (p->grid[i][y] > 1)
            {
                p->grid[i][y] = 0;
                p->visible_grid[i][y] = 0;
            }
        }
    }
}

/*
Requires: nothing
Effects: returns 1 if all the defender's ships have 0 hp, else 0
*/
int is_game_over(const Player *defender)
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

/*
Requires: nothing
Effects: Creates a GRID_SIZExGRIZ_SIZE grid of integers on the heap and returns a pointer to it
*/
int **initialize_grid()
{
    // First dimension have 10 pointers.
    int **grid = (int **)calloc(GRID_SIZE, sizeof(int *));

    for (int i = 0; i < GRID_SIZE; ++i)
    {
        // Second dimension have 10 elements.
        grid[i] = (int *)calloc(GRID_SIZE, sizeof(int));
    }

    return grid;
}

/*
Requires: nothing
Effects: creates a GRID_SIZExGRID_SIZE grid of doubles on the heap and returns the pointer to it
*/
double **initialize_double_grid()
{
    double **grid = (double **)malloc(GRID_SIZE * sizeof(double *));
    for (int i = 0; i < GRID_SIZE; i++)
    {
        grid[i] = (double *)malloc(GRID_SIZE * sizeof(double));
    }

    return grid;
}

/*
Requires: nothing
Effects: creates a player pointer on the heap and initializes the ship's hp array with the corresponding ship size, initializes both normal and visible grid, gives starting powerups, and returns the player pointer
*/
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

/*
Requires: nothing
Effects: returns the column as an integer ('A' - square)
*/
int get_column(const char square[4])
{
    return square[0] - 'A';
}

/*
Requires: nothing
Effects: returns the column as an integer
*/
int get_row(const char square[4])
{
    return square[1] != '1' ? square[1] - '1' : (square[2] == '0' ? 9 : 0);
}

/*
Requires: square contains a valid torpedo coordinate (passed through is_valid_torped_row)
Effects: returns the torpedo row as an integer
*/
int get_torpedo_row(const char square[4])
{
    return square[0] != '1' ? square[0] - '1' : (square[1] == '0' ? 9 : 0);
}

/*
Requires: nothing
Effects: returns true if the column of the square entered is valid and within bounds, else false
*/
int is_valid_column(const char square[4])
{
    if (!square)
        return -1;
    return 'A' <= square[0] && 'J' >= square[0];
}

/*
Requires: nothing
Effects: returns true if the row of the square entered is valid and within bounds, else false
*/
int is_valid_row(const char square[4])
{
    if (!square)
        return -1;
    return '1' <= square[1] && '9' >= square[1] &&
           (square[1] != '1' ||
            (square[2] == '0' ||
             square[2] == '\0'));
}

/*
Requires: nothing
Effects: returns true if the row of the square entered is valid and within bounds, else false
*/
int is_valid_torpedo_row(const char square[4])
{
    if (!square)
        return -1;
    return '1' <= square[0] && '9' >= square[0] &&
           (square[0] != '1' ||
            (square[1] == '0' ||
             square[1] == '\0'));
}

/*
Requires: nothing
Effects: returns true if the square has a valid row and column, else false
*/
int is_valid_square_input(const char square[4])
{
    return is_valid_row(square) && is_valid_column(square);
}

/*
Requires: nothing
Effects: returns the number of the move if move contains a valid move (as defined by MOVE_LIST), else -1
*/
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

/*
Requires: nothing
Effects: returns the number of the orientation if orientation contains a valid orientation (as defined in ORIENTATION), else -1
*/
int get_orientation(const char orientation[11])
{
    if (orientation == NULL)
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

/*
Requires: range is positive
Effects: returns a random integer in [0, range)
*/
int _rand(const int range)
{
    // Since we are not allowed to use any libraries we will generate
    // a random number by allocating a random memory address in the
    // memory using malloc, we will use the garbage value as a rand.

    void *temp = malloc(1);
    int res = (((int)temp) / 7) % range;
    free(temp);
    return res;
}

/*
Requires: nothing
Effects: prints the number of availabe moves for each move of the player
*/
void print_available_moves(const Player *p)
{
    printf("Available moves:\n");
    printf("Fire: ∞\n");
    printf("Torpedo: %d\n", p->torpedo);
    printf("Artillery: %d\n", p->artillery);
    printf("Radar Sweeps: %d\n", p->radar_sweep);
    printf("Smoke Screens: %d\n", p->smoke_screen);
}