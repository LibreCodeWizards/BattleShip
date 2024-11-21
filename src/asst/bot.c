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