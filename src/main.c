#include <stdio.h>
#include "asst/asst.h"

int main()
{
    // what used to be here
    // Player* p1;
    // for (int i = 0; i < 80; ++i)
    // {
    //     p1 = initialize_player();
    // }
    // printf("%d", p1->ships.ship4);
    // return 0;

    Player* player[2] = {initialize_player(), initialize_player()};
    // here ask players for their configuration of ships
    int current_player = 0;
    while(1){ // game loop
        int other_player = 1 - current_player;
        print_grid(player[other_player]); // print other player's grid
        char move_type[10];
        char square[3];
        scanf("%s %s",move_type,square); //take move input from current player
        // logic here (break the loop if game is done)
        current_player = other_player; // switch to the other player
    }
    return 0;
}