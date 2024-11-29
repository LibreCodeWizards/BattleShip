#include <stdio.h>
#include <stdlib.h>
#include "bot.h"
#include "asst.h"

/*
Requires: nothing
Effects: prints the heatmap of the bot (mainly used for debugging)
*/
void print_heatmap(double **hm)
{
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
            printf("%.2f\t", hm[i][j]);
        }
        printf("\n");
    }
}

/*
Requires: nothing
Effects: randomly places the 4 ships on the bot's board
*/
void bot_configure_ships(Player *bot)
{
    for (int ship = 2; ship <= 5; ++ship)
    {
        while (1)
        {
            int x = _rand(10);
            int y = _rand(10);
            int orientation = _rand(2);

            if (add_ship(bot, x, y, ship, orientation))
            {
                break;
            }
        }
    }
}

/*
Requires: nothing
Effects: gives modified values representing how good the configuration of ships is
*/
double activation(double x, double y)
{
    return ((x + 1) / (y + 1)) + (1 / (y - x + 1));
}

void mark_radar_miss(Player *dummy, const int x, const int y)
{
    for (int i = x; i < min(x + 2, GRID_SIZE); ++i)
    {
        for (int j = y; j < min(y + 2, GRID_SIZE); ++j)
        {
            dummy->grid[i][j] = 1;
        }
    }
}

/*
Requires: nothing
Effects: creates a probablity heatmap of squares that are most likely to have ships
*/
double **get_heat_map(Player *opponent, Player *dummy)
{
    // PLEASE DO NOT TRY TO REFORMAT THIS CODE

    int hit_count = 0;
    // Count the number of hit squares not including those that have been sunk
    for (int i = 0; i < NUM_SHIPS; i++)
    {
        if (opponent->ships[i] > 0)
        {
            hit_count += i + 2 - opponent->ships[i];
        }
    }

    double **hm = initialize_double_grid();
    // loop over opponent ship healths, if not 0, make corresponding index its size
    int ships[] = {-1, -1, -1, -1};
    for (int i = 0; i < NUM_SHIPS; ++i)
        if (opponent->grid[i] > 0)
            ships[i] = i + 2;

    // sort the ships array in descending order to make it easier to loop over
    for (int i = 0; i < NUM_SHIPS - 1; ++i)
        for (int j = i + 1; j < NUM_SHIPS; ++j)
            if (ships[i] < ships[j])
            {
                int temp = ships[i];
                ships[i] = ships[j];
                ships[j] = temp;
            }

    // If we have less than 4 ships, we can bruteforce everything
    if (ships[NUM_SHIPS - 1] == -1)
    {
        for (int i0 = 0; i0 < GRID_SIZE; ++i0)
        {
            for (int j0 = 0; j0 < GRID_SIZE; ++j0)
            {
                for (int dir0 = 0; dir0 < 2; dir0++)
                {
                    if (ships[1] == -1)
                    {
                        // there's only 1 ship
                        int result = add_ship(dummy, i0, j0, ships[0], dir0);
                        if (result)
                        {
                            int covered_hits = 0;
                            int covered_misses = 0;
                            if (dir0 == 0)
                            {
                                for (int dy = 0; dy < ships[0]; ++dy)
                                {
                                    if (opponent->grid[i0][j0 + dy] < -1)
                                        covered_hits++;
                                    if (opponent->grid[i0][j0 + dy] == -1)
                                        covered_misses++;
                                }
                            }
                            else
                            {
                                for (int dx = 0; dx < ships[0]; ++dx)
                                {
                                    if (opponent->grid[i0 + dx][j0] < -1)
                                        covered_hits++;
                                    if (opponent->grid[i0 + dx][j0] == -1)
                                        covered_misses++;
                                }
                            }
                            if (covered_hits == hit_count && covered_misses == 0)
                            {
                                if (dir0 == 0)
                                {
                                    for (int dy = 0; dy < ships[0]; ++dy)
                                    {
                                        hm[i0][j0 + dy]++;
                                    }
                                }
                                else
                                {
                                    for (int dx = 0; dx < ships[0]; ++dx)
                                    {
                                        hm[i0 + dx][j0]++;
                                    }
                                }
                            }
                        }
                        // remove the ship
                        remove_ship(dummy, i0, j0, ships[0], dir0);
                    }
                    else
                    {
                        for (int i1 = 0; i1 < GRID_SIZE; ++i1)
                        {
                            for (int j1 = 0; j1 < GRID_SIZE; ++j1)
                            {
                                for (int dir1 = 0; dir1 < 2; ++dir1)
                                {
                                    if (ships[2] == -1)
                                    {
                                        int result = add_ship(dummy, i0, j0, ships[0], dir0) + add_ship(dummy, i1, j1, ships[1], dir1);
                                        if (result == 2)
                                        {
                                            int covered_hits = 0;
                                            int covered_misses = 0;
                                            if (dir0 == 0)
                                            {
                                                for (int dy = 0; dy < ships[0]; ++dy)
                                                {
                                                    if (opponent->grid[i0][j0 + dy] < -1)
                                                        covered_hits++;
                                                    if (opponent->grid[i0][j0 + dy] == -1)
                                                        covered_misses++;
                                                }
                                            }
                                            else
                                            {
                                                for (int dx = 0; dx < ships[0]; ++dx)
                                                {
                                                    if (opponent->grid[i0 + dx][j0] < -1)
                                                        covered_hits++;
                                                    if (opponent->grid[i0 + dx][j0] == -1)
                                                        covered_misses++;
                                                }
                                            }

                                            if (dir1 == 0)
                                            {
                                                for (int dy = 0; dy < ships[1]; ++dy)
                                                {
                                                    if (opponent->grid[i1][j1 + dy] < -1)
                                                        covered_hits++;
                                                    if (opponent->grid[i1][j1 + dy] == -1)
                                                        covered_misses++;
                                                }
                                            }
                                            else
                                            {
                                                for (int dx = 0; dx < ships[1]; ++dx)
                                                {
                                                    if (opponent->grid[i1 + dx][j1] < -1)
                                                        covered_hits++;
                                                    if (opponent->grid[i1 + dx][j1] == -1)
                                                        covered_misses++;
                                                }
                                            }
                                            if (covered_hits == hit_count && covered_misses == 0)
                                            {
                                                if (dir0 == 0)
                                                {
                                                    for (int dy = 0; dy < ships[0]; ++dy)
                                                    {
                                                        hm[i0][j0 + dy]++;
                                                    }
                                                }
                                                else
                                                {
                                                    for (int dx = 0; dx < ships[0]; ++dx)
                                                    {
                                                        hm[i0 + dx][j0]++;
                                                    }
                                                }

                                                if (dir1 == 0)
                                                {
                                                    for (int dy = 0; dy < ships[1]; ++dy)
                                                    {
                                                        hm[i1][j1 + dy]++;
                                                    }
                                                }
                                                else
                                                {
                                                    for (int dx = 0; dx < ships[1]; ++dx)
                                                    {
                                                        hm[i1 + dx][j1]++;
                                                    }
                                                }
                                            }
                                        }
                                        // remove the ships
                                        remove_ship(dummy, i0, j0, ships[0], dir0);
                                        remove_ship(dummy, i1, j1, ships[1], dir1);
                                    }
                                    else
                                    {
                                        for (int i2 = 0; i2 < GRID_SIZE; ++i2)
                                        {
                                            for (int j2 = 0; j2 < GRID_SIZE; ++j2)
                                            {
                                                for (int dir2 = 0; dir2 < 2; ++dir2)
                                                {
                                                    int result = add_ship(dummy, i0, j0, ships[0], dir0) + add_ship(dummy, i1, j1, ships[1], dir1) + add_ship(dummy, i2, j2, ships[2], dir2);
                                                    if (result == 3)
                                                    {
                                                        int covered_hits = 0;
                                                        int covered_misses = 0;
                                                        if (dir0 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[0]; ++dy)
                                                            {
                                                                if (opponent->grid[i0][j0 + dy] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i0][j0 + dy] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[0]; ++dx)
                                                            {
                                                                if (opponent->grid[i0 + dx][j0] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i0 + dx][j0] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }

                                                        if (dir1 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[1]; ++dy)
                                                            {
                                                                if (opponent->grid[i1][j1 + dy] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i1][j1 + dy] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[1]; ++dx)
                                                            {
                                                                if (opponent->grid[i1 + dx][j1] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i1 + dx][j1] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }

                                                        if (dir2 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[2]; ++dy)
                                                            {
                                                                if (opponent->grid[i2][j2 + dy] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i2][j2 + dy] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[2]; ++dx)
                                                            {
                                                                if (opponent->grid[i2 + dx][j2] < -1)
                                                                    covered_hits++;
                                                                if (opponent->grid[i2 + dx][j2] == -1)
                                                                    covered_misses++;
                                                            }
                                                        }
                                                        if (covered_hits == hit_count && covered_misses == 0)
                                                        {
                                                            if (dir0 == 0)
                                                            {
                                                                for (int dy = 0; dy < ships[0]; ++dy)
                                                                {
                                                                    hm[i0][j0 + dy]++;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                for (int dx = 0; dx < ships[0]; ++dx)
                                                                {
                                                                    hm[i0 + dx][j0]++;
                                                                }
                                                            }

                                                            if (dir1 == 0)
                                                            {
                                                                for (int dy = 0; dy < ships[1]; ++dy)
                                                                {
                                                                    hm[i1][j1 + dy]++;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                for (int dx = 0; dx < ships[1]; ++dx)
                                                                {
                                                                    hm[i1 + dx][j1]++;
                                                                }
                                                            }

                                                            if (dir2 == 0)
                                                            {
                                                                for (int dy = 0; dy < ships[2]; ++dy)
                                                                {
                                                                    hm[i2][j2 + dy]++;
                                                                }
                                                            }
                                                            else
                                                            {
                                                                for (int dx = 0; dx < ships[2]; ++dx)
                                                                {
                                                                    hm[i2 + dx][j2]++;
                                                                }
                                                            }
                                                        }
                                                    }
                                                    // remove the ships
                                                    remove_ship(dummy, i0, j0, ships[0], dir0);
                                                    remove_ship(dummy, i1, j1, ships[1], dir1);
                                                    remove_ship(dummy, i2, j2, ships[2], dir2);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Case where opponent has 4 ships
    // We cant brute force it because it is too expensive so instead
    // Brute force ever possible combination of only 3 ships, alternating the removed ship each time
    // then add them all together. This gets a good enough approximation of true heatmap
    else
    {
        int triplet[4][3] = {{3, 4, 5}, {2, 4, 5}, {2, 3, 5}, {2, 3, 4}};
        for (int removed = 0; removed < NUM_SHIPS; ++removed)
        {
            int ships[3];
            for (int i = 0; i < 3; ++i)
                ships[i] = triplet[removed][i];

            for (int i0 = 0; i0 < GRID_SIZE; ++i0)
            {
                for (int j0 = 0; j0 < GRID_SIZE; ++j0)
                {
                    for (int dir0 = 0; dir0 < 2; ++dir0)
                    {
                        for (int i1 = 0; i1 < GRID_SIZE; ++i1)
                        {
                            for (int j1 = 0; j1 < GRID_SIZE; ++j1)
                            {
                                for (int dir1 = 0; dir1 < 2; ++dir1)
                                {
                                    for (int i2 = 0; i2 < GRID_SIZE; ++i2)
                                    {
                                        for (int j2 = 0; j2 < GRID_SIZE; ++j2)
                                        {
                                            for (int dir2 = 0; dir2 < 2; ++dir2)
                                            {
                                                int result = add_ship(dummy, i0, j0, ships[0], dir0);
                                                result += add_ship(dummy, i1, j1, ships[1], dir1);
                                                result += add_ship(dummy, i2, j2, ships[2], dir2);
                                                if (result == 3)
                                                {
                                                    int covered_hits = 0;
                                                    int covered_misses = 0;
                                                    if (dir0 == 0)
                                                    {
                                                        for (int dy = 0; dy < ships[0]; ++dy)
                                                        {
                                                            if (opponent->grid[i0][j0 + dy] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i0][j0 + dy] == -1)
                                                                covered_misses++;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        for (int dx = 0; dx < ships[0]; ++dx)
                                                        {
                                                            if (opponent->grid[i0 + dx][j0] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i0 + dx][j0] == -1)
                                                                covered_misses++;
                                                        }
                                                    }

                                                    if (dir1 == 0)
                                                    {
                                                        for (int dy = 0; dy < ships[1]; ++dy)
                                                        {
                                                            if (opponent->grid[i1][j1 + dy] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i1][j1 + dy] == -1)
                                                                covered_misses++;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        for (int dx = 0; dx < ships[1]; ++dx)
                                                        {
                                                            if (opponent->grid[i1 + dx][j1] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i1 + dx][j1] == -1)
                                                                covered_misses++;
                                                        }
                                                    }

                                                    if (dir2 == 0)
                                                    {
                                                        for (int dy = 0; dy < ships[2]; ++dy)
                                                        {
                                                            if (opponent->grid[i2][j2 + dy] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i2][j2 + dy] == -1)
                                                                covered_misses++;
                                                        }
                                                    }
                                                    else
                                                    {
                                                        for (int dx = 0; dx < ships[2]; ++dx)
                                                        {
                                                            if (opponent->grid[i2 + dx][j2] < -1)
                                                                covered_hits++;
                                                            if (opponent->grid[i2 + dx][j2] == -1)
                                                                covered_misses++;
                                                        }
                                                    }
                                                    if (covered_misses == 0)
                                                    {
                                                        if (dir0 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[0]; ++dy)
                                                            {
                                                                hm[i0][j0 + dy] += activation(covered_hits, hit_count);
                                                                // hm[i0][j0 + dy]++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[0]; ++dx)
                                                            {
                                                                hm[i0 + dx][j0] += activation(covered_hits, hit_count);
                                                                // hm[i0 + dx][j0]++;
                                                            }
                                                        }

                                                        if (dir1 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[1]; ++dy)
                                                            {
                                                                hm[i1][j1 + dy] += activation(covered_hits, hit_count);
                                                                // hm[i1][j1 + dy]++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[1]; ++dx)
                                                            {
                                                                hm[i1 + dx][j1] += activation(covered_hits, hit_count);
                                                                // hm[i1 + dx][j1]++;
                                                            }
                                                        }

                                                        if (dir2 == 0)
                                                        {
                                                            for (int dy = 0; dy < ships[2]; ++dy)
                                                            {
                                                                hm[i2][j2 + dy] += activation(covered_hits, hit_count);
                                                                // hm[i2][j2 + dy]++;
                                                            }
                                                        }
                                                        else
                                                        {
                                                            for (int dx = 0; dx < ships[2]; ++dx)
                                                            {
                                                                hm[i2 + dx][j2] += activation(covered_hits, hit_count);
                                                                // hm[i2 + dx][j2]++;
                                                            }
                                                        }
                                                    }
                                                }
                                                // remove the ships
                                                remove_ship(dummy, i0, j0, ships[0], dir0);
                                                remove_ship(dummy, i1, j1, ships[1], dir1);
                                                remove_ship(dummy, i2, j2, ships[2], dir2);
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    // Dont allow bot to choose cell that has already been hit
    for (int i = 0; i < GRID_SIZE; ++i)
    {
        for (int j = 0; j < GRID_SIZE; ++j)
        {
            if (opponent->grid[i][j] < 0)
            {
                hm[i][j] = -1;
            }
        }
    }
    return hm;
}

/*
Requires: latest_bot_radar_hit be an integer array of size 2
Effects: stores the bot's intended move in move_number, and its coordinates in x and y
*/
void get_bot_move(Player *bot, Player *opponent, Player *dummy, int *x, int *y, int *move_number, int turn, int *latest_bot_radar_hit)
{
    double **hm = get_heat_map(opponent, dummy); // heatmap
    print_heatmap(hm);
    int bot_has_all_ships = 1;
    for (int i = 0; i < 4; ++i)
    {
        if (bot->ships[i] == 0)
        {
            bot_has_all_ships = 0;
            break;
        }
    }

    if (bot->torpedo > 0)
    {
        int bestRow = 0;
        double bestRowSum = 0;
        for (int i = 0; i < 10; ++i)
        {
            int rowSum = 0;
            for (int j = 0; j < 10; ++j)
                rowSum += hm[i][j];
            if (rowSum > bestRowSum)
            {
                bestRowSum = rowSum;
                bestRow = i;
            }
        }
        int bestCol = 0;
        double bestColSum = 0;
        for (int i = 0; i < 10; ++i)
        {
            int colSum = 0;
            for (int j = 0; j < 10; ++j)
                colSum += hm[j][i];
            if (colSum > bestColSum)
            {
                bestColSum = colSum;
                bestCol = i;
            }
        }

        if (bestRowSum >= bestColSum)
        {
            *y = 69;
            *x = bestRow;
        }
        else
        {
            *x = 10;
            *y = bestCol;
        }
        *move_number = 4;

        return;
    }
    else if (bot->artillery > 0)
    {
        int bestX = 0, bestY = 0;
        double bestSum = hm[0][0] + hm[0][1] + hm[1][0] + hm[1][1];
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
            {
                int sum = hm[i][j] + hm[i + 1][j] + hm[i][j + 1] + hm[i + 1][j + 1];
                if (sum > bestSum)
                {
                    bestSum = sum;
                    bestX = i;
                    bestY = j;
                }
            }
        // [bestX,bestY] is the best 2x2 subsquare to sweep
        *x = bestX;
        *y = bestY;

        // If we have artillary, choose it over radar sweep
        *move_number = 3;
        return;
    }
    else if (bot->radar_sweep > 0 && bot_has_all_ships && latest_bot_radar_hit[0] == -1)
    {
        *x = _rand(9);
        *y = _rand(9);
        *move_number = 1;
        return;
    }
    // if latest_bot_radar_hit isn't {-1,-1}
    else if (latest_bot_radar_hit[0] != -1)
    {
        int dx[4] = {0, 0, 1, 1};
        int dy[4] = {0, 1, 0, 1};
        for (int i = 0; i < 4; ++i)
        {
            // Note: >= 0 means that this cell has not been fired at yet
            if (opponent->grid[latest_bot_radar_hit[0] + dx[i]][latest_bot_radar_hit[1] + dy[i]] >= 0)
            {
                *x = latest_bot_radar_hit[0] + dx[i];
                *y = latest_bot_radar_hit[1] + dy[i];
                *move_number = 0;
                return;
            }
        }
    }
    else
    {
        // just do a normal hit
        int bestX = 0, bestY = 0;
        double bestSum = hm[0][0];
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                if (hm[i][j] > bestSum)
                {
                    bestSum = hm[i][j];
                    bestX = i;
                    bestY = j;
                }
        *x = bestX;
        *y = bestY;
        *move_number = 0;

        return;
    }

    return;
}