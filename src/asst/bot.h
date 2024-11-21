#ifndef BOT_H
#define BOT_H
#include "asst.h"

void bot_configure_ships(Player *bot);

void get_bot_move(Player *bot, Player *opponent, int *x, int *y, int *move_number, int turn, int *latest_bot_radar_hit);

#endif