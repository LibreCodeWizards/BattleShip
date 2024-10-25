#include <stdio.h>
#include <stdlib.h>
#include "asst/asst.c"

int main()
{
    Player *player[2] = {initialize_player(), initialize_player()};

    int exit = 0;

    // cursed random number generator
    int current_player = get_random(2);
    printf("THE PLAYER IS: %d\n", current_player);

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

    for (int p = 0; p < 2; ++p)
    {
        if (p == 0)
            printf("Player 1, please configure your ships\n");
        else
            printf("Player 2, please configure your ships\n");
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
                int x = get_row(square);
                int y = get_column(square);
                if (!add_ship(player[p], x, y, ship_size, get_orientation(orientation)))
                {
                    printf("You can't put your ship there!\n");
                    continue;
                }
                break;
            }
            print_configuration(player[p]);
        }
        clear_screen();
    }

    while (!exit)
    {
        int opponent = current_player ^ 1;
        printf("Player %d, make your move: \n", current_player + 1);
        printf("Your opponent's current grid is:\n");
        print_grid(player[current_player], player[opponent], difficulty);
        printf("Your available moves are:\n");
        print_available_moves(player[current_player]);

        int move_number;
        int x, y;
        while (1)
        {
            printf("Enter your move: ");
            char move[10];
            char square[4];
            scanf("%s %s", move, square);

            move_number = get_move(move);
            if (move_number == -1)
            {
                printf("That is not a legal move!");
                continue;
            }

            if (!is_valid_square_input(square))
            {
                x = y = -1;
                break;
            }

            x = get_row(square);
            y = get_column(square);
            break;
        }
        if (x == -1)
        {
            // player inputted invalid coordinates so they lose their turn
            printf("Invalid coordinates, you have lost your turn.");
            continue;
        }

        // Remove the artillary if the player did not use it
        // NOTE: this is done before checking to see if the move is allowed
        if (player[current_player]->artillery > 0 && move_number != 3)
        {
            player[current_player]->artillery = 0;
        }

        // TODO: check if it's an allowed move (we can just continue after switching players)

        if (move_number == 0)
        {
            int hit = fire(player[current_player], player[opponent], x, y);
            if (hit > 0)
            {
                printf("Hit!");
                int sunk = is_sunk(player[opponent], hit);
                update_torpedo(player[current_player], player[opponent], hit);
            }
            else
            {
                printf("Miss!");
            }
        }
        else if (move_number == 1)
        {
            player[current_player] -= 1;
            int found = radar_sweep(player[opponent], x, y);
            if (found)
            {
                printf("Enemy ships found!");
            }
            else
            {
                printf("No enemy ships found!");
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
            // TODO: implement logic to take the input of the torpedo, which should be
            // either a character or a number, and determine the orientation from that
            // torpedo(player[current_player], player[opponent], )
        }

        printf("\n");

        // Check game over
        if (is_game_over(player[opponent]))
        {
            printf("Player %d Wins!", current_player + 1);
            break;
        }

        // Turn mechanic
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
