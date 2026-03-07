#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAP_W 16
#define MAP_H 16

#define WHITE_TILE 1
#define BLACK_TILE 2
#define RED_TILE 3
#define BLUE_TILE 4
#define GREEN_TILE 5

struct Grid{
    int type;
    int texture_asset_id;
};
extern struct Grid world_map[MAP_H][MAP_W];

int get_grid_type(int x, int y);
int get_grid_texture_asset_id(int x, int y);

#endif
