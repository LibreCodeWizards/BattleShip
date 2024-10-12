#include <stdio.h>
#include "asst.h"

int main()
{
    Player* p1;
    for (int i = 0; i < 80; ++i)
    {
        p1 = initialize_player();
    }
    printf("%d", p1->ships.ship4);
    return 0;
}