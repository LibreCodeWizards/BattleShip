#ifndef BOT_H
#define BOT_H
#include "asst.h"

void bot_configure_ships(Player *bot);

void get_bot_move(Player *bot, Player *opponent, Player *dummy, int *x, int *y, int *move_number, int turn, int *latest_bot_radar_hit);

void mark_radar_miss(Player *dummy, const int x, const int y);

void print_heatmap(double **hm);

double **get_heat_map(Player *opponent, Player *dummy);

double activation(double x, double y);

#endif