#include "player.h"
#include "math.h"
#include <stdlib.h>

struct Player{
    double posX; 
    double posY; 

    double planeX; 
    double planeY; // FOV ~66 derece

    double angle;
};

Player* create_player(){
    Player* player = malloc(sizeof(Player));

    player->posX = 3.5;
    player->posY = 3.5;
    player->angle = 0;
    player->planeX = 0;
    player->planeY = 0.66; // FOV ~66 derece
    return player;
}

/// GETTER FUNCTIONS ///
double get_player_angle(Player* player){
    return player->angle;
}

int get_player_plane_x(Player* player){
    return player->planeX;
}

int get_player_plane_y(Player* player){
    return player->planeY;
}

int get_player_pos_x(Player* player){
    return player->posX;
}

int get_player_pos_y(Player* player){
    return player->posY;
}

/// SETTER FUNCTIONS ///
void set_player_plane_x(Player* player, int plane_x){
    player->planeX = plane_x;
}
void set_player_plane_y(Player* player, int plane_y){
    player->planeY = plane_y;
}

void set_player_pos_x(Player* player, int player_pos_x){
    player->posX = player_pos_x;
}
void set_player_pos_y(Player* player, int player_pos_y){
    player->posY = player_pos_y;
}

void set_player_angle(Player* player, double player_angle){
    player->angle = player_angle;
    if (player->angle < 0) player->angle += 2 * M_PI;
    if (player->angle > 2 * M_PI) player->angle -= 2 * M_PI;
}
