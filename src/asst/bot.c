#include "bot.h"

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

// TODO: finish Later
int **get_heat_map(Player *opponent)
{

    int **heat_map = initialize_grid();
}

/*
stores the intended move and coordinates in x, y, move_number
*/
void get_bot_move(Player *bot, Player *opponent, int *x, int *y, int *move_number, int turn, int *latest_bot_radar_hit)
{
    int **hm = get_heat_map(opponent); // heatmap
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
        int bestRow = 0, bestRowSum = 0;
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
        int bestCol = 0, bestColSum = 0;
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
    else if (bot->artillery > 0 || (bot->radar_sweep > 0 && bot_has_all_ships && latest_bot_radar_hit[0] == -1))
    {
        int bestX = 0, bestY = 0, bestSum = hm[0][0] + hm[0][1] + hm[1][0] + hm[1][1];
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
        *move_number = bot->artillery > 0 ? 3 : 1;
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
        int bestX = 0, bestY = 0, bestSum = hm[0][0];
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