#include <stdio.h>
#include "asst/asst.h"

int main()
{
    Player *player[2] = {initialize_player(), initialize_player()};
    char *ship_name[4] = {"Submarine",
                          "Destroyer",
                          "Battleship",
                          "Carrier"};

    int exit = 0;
    int current_player = 0;

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
            printf("Attacker player, please configure your ships\n");
        else
            printf("Defender player, please confugure your ships\n");
        // prints the empty grid
        print_configuration(player[1]);

        for (int ship_size = 2; ship_size <= 5; ++ship_size)
        {
            while (1)
            {
                printf("Enter coordinates of %s (Example: A2 vertical or C4 horizontal): ", ship_name[ship_size - 2]);
                char square[4];
                char orientation[11];
                scanf("%s %s", square, orientation);
                if (!is_valid_square(square) || get_orientation(orientation) == -1)
                {
                    printf("Invalid Input!\n");
                    continue;
                }
                int x = get_row(square);
                int y = get_column(square);
                add_ship(player[p], x, y, ship_size, get_orientation(orientation));
                break;
            }
            print_configuration(player[p]);
        }
        clear_screen();
    }

    while (!exit)
    {
    }
}