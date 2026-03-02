#include "player.h"
#include <stdlib.h>

struct Player{
    double posX; 
    double posY; 

    double dirX; 
    double dirY; 

    double planeX; 
    double planeY; // FOV ~66 derece
};

Player* create_player(){
    Player* player = malloc(sizeof(Player));

    player->posX = 3.5;
    player->posY = 3.5;
    player->dirX = -1;
    player->dirY = 0;
    player->planeX = 0;
    player->planeY = 0.66; // FOV ~66 derece
    return player;
}

int get_player_dir_x(Player* player){
    return player->dirX;
}

int get_player_dir_y(Player* player){
    return player->dirY;
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
