#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player Player;

Player* create_player();

double get_player_plane_x(Player* player);
double get_player_plane_y(Player* player);

double get_player_pos_x(Player* player);
double get_player_pos_y(Player* player);

double get_player_angle(Player* player);


void set_player_plane_x(Player* player, double plane_x);
void set_player_plane_y(Player* player, double plane_y);

void set_player_pos_x(Player* player, double player_pos_x);
void set_player_pos_y(Player* player, double player_pos_y);

void set_player_angle(Player* player, double player_angle);

void add_player_angle(Player* player, double value);
#endif
