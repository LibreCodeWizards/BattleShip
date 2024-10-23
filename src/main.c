#include <stdio.h>
#include "asst/asst.h"

int main()
{
    Player *player[2] = {initialize_player(), initialize_player()};

    int current_player = 0;

    // // here ask players for their configuration of ships
    // int current_player = 0;
    // while (1)
    // { // game loop
    //     int other_player = 1 - current_player;
    //     print_grid(current_player, other_player, 0); // print other player's grid
    //     char move_type[10];
    //     char square[4];
    //     scanf("%s %s", move_type, square); // take move input from current player
    //     // logic here (break the loop if game is done)
    //     current_player = other_player; // switch to the other player
    // }
    // return 0;
}