#ifndef PLAYER_H
#define PLAYER_H

typedef struct Player Player;

Player* create_player();

int get_player_dir_x(Player* player);
int get_player_dir_y(Player* player);
int get_player_plane_x(Player* player);
int get_player_plane_y(Player* player);

int get_player_pos_x(Player* player);
int get_player_pos_y(Player* player);

#endif
