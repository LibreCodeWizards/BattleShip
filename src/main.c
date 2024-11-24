#include <stdio.h>
#include <stdlib.h>
#include "asst/asst.h"
#include "asst/bot.h"

int main()
{
    Player *player[2] = {initialize_player(), initialize_player()};

    int exit = 0;

    int difficulty;
    while (1)
    {
        printf("Choose your difficulty: (0 for Easy and 1 for Hard): ");
        scanf("%d", &difficulty);
        if (difficulty > 1 || difficulty < 0)
        {
            printf("Invalid difficulty!\n");
            continue;
        }
        break;
    }

    // 0 means single player, 1 is multiplaye
    int mode;
    while (1)
    {
        printf("Choose mode: (0 for Single player against bot and 1 for multiplayer): ");
        scanf("%d", &mode);
        if (mode != 1 && mode != 0)
        {
            printf("Invalid mode!\n");
            continue;
        }
        break;
    }

    for (int p = 0; p < 2; p++)
    {
        if (mode == 1)
        {
            if (p == 0)
                printf("Player 1, please configure your ships\n");
            else
                printf("Player 2, please configure your ships\n");
        }
        else
        {
            printf("Human Player, please configure your ships\n");
        }
        // prints the empty grid
        print_configuration(player[1]);

        for (int ship_size = 2; ship_size <= 5; ++ship_size)
        {
            while (1)
            {
                printf("Enter coordinates of %s (Example: A2 vertical or C4 horizontal): ", SHIP_NAMES[ship_size - 2]);

                char square[4];
                char orientation[11];

                scanf("%s %s", square, orientation);

                if (!is_valid_square_input(square) || get_orientation(orientation) == -1)
                {
                    printf("Invalid Input!\n");
                    continue;
                }

                const int x = get_row(square);
                const int y = get_column(square);

                if (!add_ship(player[p], x, y, ship_size, get_orientation(orientation)))
                {
                    printf("You can't put your ship there!\n");
                    continue;
                }

                break;
            }
            print_configuration(player[p]);
            printf("AFTER WE PRINT CONFIGURATION");
        }
        // WTF IS GOING ON HERE????????
        printf("BEFORE CLEAR SCREEN");
        clear_screen();
        printf("AFTER CLEAR SCREEN");

        if (mode == 0)
        {
            // get bot's configuration
            // NOTE: bot is always player[1], human is always player[0]
            bot_configure_ships(player[1]);
            printf("CONFIGURED BOT SHIPS");
            break;
        }
    }

    // Random number generator
    int current_player = _rand(2);
    int turn = 0;

    int latest_radar_bot_hit[2] = {-1, -1};
    while (!exit)
    {
        printf("ENTERED GAME LOOP");
        turn++;
        int opponent = current_player ^ 1;
        int move_number;
        int x, y;

        if (mode == 0)
        {

            if (current_player == 0)
            {
                printf("Human Player, it's your turn: \n");

                printf("The bot's current grid is:\n");
                print_grid(player[current_player], player[opponent], difficulty);

                printf("Your available moves are:\n");
                print_available_moves(player[current_player]);
            }
        }
        else
        {
            printf("Player %d, it's your turn: \n", current_player + 1);

            printf("Your opponent's current grid is:\n");
            print_grid(player[current_player], player[opponent], difficulty);

            printf("Your available moves are:\n");
            print_available_moves(player[current_player]);
        }
        if (mode == 1 || current_player == 0)
        {
            while (1)
            {
                printf("Enter your move: ");

                char move[10];
                char square[4];
                scanf("%s %s", move, square);

                move_number = get_move(move);

                if (move_number == -1)
                {
                    printf("That is not a legal move!\n");
                    continue;
                }

                // If player doesnt have artillary or torpedo and he tries to play them, make him choose again
                if (move_number == 3 && player[current_player]->artillery <= 0)
                {
                    printf("You dont have artilleries!\n");
                    continue;
                }

                if (move_number == 4 && player[current_player]->torpedo <= 0)
                {
                    printf("You dont have a torpedo!\n");
                    continue;
                }

                if (move_number == 4 && !is_valid_torpedo_row(square) && !is_valid_column(square))
                {
                    x = y = -1;
                    break;
                }
                if (move_number == 4 && is_valid_torpedo_row(square))
                {
                    x = get_torpedo_row(square);
                    y = 10;
                    break;
                }
                if (move_number == 4 && is_valid_column(square))
                {
                    x = 10;
                    y = get_column(square);
                    break;
                }

                if (!is_valid_square_input(square) && move_number != 4)
                {
                    x = y = -1;
                    break;
                }

                x = get_row(square);
                y = get_column(square);

                break;
            }
        }
        else
        {
            // TODO: get bot move
            printf("ENTERED CONDITION WHERE BOT PLAYs");
            get_bot_move(player[current_player], player[opponent], &x, &y, &move_number, turn, latest_radar_bot_hit);

            const char *move_name = MOVE_LIST[move_number];
            char col = 'A' + y;
            char row = x + 1;

            printf("The Bot Played: %s %c%d\n", move_name, col, row);
        }

        if (x == -1)
        {
            // Player inputted invalid coordinates so they lose their turn
            printf("Invalid coordinates, you have lost your turn.");
            continue;
        }

        // Remove the artillery if the player did not use it
        // NOTE: this is done before checking to see if the move is allowed
        if (player[current_player]->artillery > 0 && move_number != 3)
        {
            player[current_player]->artillery = 0;
        }

        // if player doesnt have radar sweep or smokescreen and he tries to play them, he loses his turn
        if (move_number == 1 && player[current_player]->radar_sweep <= 0)
        {
            printf("You don't have a radar sweep! You have lost your turn!");
            continue;
        }
        if (move_number == 2 && player[current_player]->smoke_screen <= 0)
        {
            printf("You don't have a smoke screen! You have lost your turn!");
            continue;
        }

        if (move_number == 0)
        {
            const int hit = fire(player[current_player], player[opponent], x, y);
            if (mode == 0 && current_player == 1 && hit)
            {
                latest_radar_bot_hit[0] = latest_radar_bot_hit[1] = -1;
            }
            if (hit > 0)
            {
                printf("Hit!");
                int sunk = is_sunk(player[opponent], hit);
                update_torpedo(player[current_player], player[opponent], sunk);
            }
            else
            {
                printf("Miss!");
            }
        }
        else if (move_number == 1)
        {
            player[current_player]->radar_sweep -= 1;

            if (mode == 1 || current_player == 0)
            {
                const int found = radar_sweep(player[opponent], x, y);
                if (found)
                {
                    printf("Enemy ships found!");
                }
                else
                {
                    printf("No enemy ships found!");
                }
            }
            else
            {
                // Do Bot Radar
                const int found = radar_sweep(player[opponent], x, y);
                if (found)
                {
                    latest_radar_bot_hit[0] = x;
                    latest_radar_bot_hit[1] = y;
                }
            }
        }
        else if (move_number == 2)
        {
            player[current_player]->smoke_screen -= 1;
            smoke_screen(player[current_player], x, y);
            clear_screen();
        }
        else if (move_number == 3)
        {
            player[current_player]->artillery = 0;
            int hit = artillery(player[current_player], player[opponent], x, y);
            if (hit > 0)
            {
                printf("Hit!");
            }
            else
            {
                printf("Miss!");
            }
        }
        else
        {
            player[current_player]->torpedo = 0;
            // x == 10 means its horizontal
            // if x 10, we use y as the coordinate, else use x as the coordinate
            int hit = torpedo(player[current_player], player[opponent], x == 10 ? y : x, x == 10);
            if (hit > 0)
            {
                printf("Hit!");
            }
            else
            {
                printf("Miss!");
            }
        }

        printf("\n");
        printf("BIG MEOW");

        // Check game over
        if (is_game_over(player[opponent]))
        {
            printf("Player %d Wins!", current_player + 1);
            break;
        }

        // Turn mechanic
        printf("CHANGED TURN SUCCESSFULLY!");
        current_player = current_player ^ 1;
    }

    // Free members of structs to avoid dangling pointers
    free(player[0]->grid);
    free(player[0]->visible_grid);
    free(player[0]->ships);
    free(player[1]->grid);
    free(player[1]->visible_grid);
    free(player[1]->ships);

    // Free player pointers themselves
    free(player[0]);
    free(player[1]);
}