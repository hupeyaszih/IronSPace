#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player Player;

Player* create_player();

int get_player_plane_x(Player* player);
int get_player_plane_y(Player* player);

int get_player_pos_x(Player* player);
int get_player_pos_y(Player* player);

double get_player_angle(Player* player);


void set_player_plane_x(Player* player, int plane_x);
void set_player_plane_y(Player* player, int plane_y);

void set_player_pos_x(Player* player, int player_pos_x);
void set_player_pos_y(Player* player, int player_pos_y);

void set_player_angle(Player* player, double player_angle);

#endif
