#ifndef GAME_MAP_H
#define GAME_MAP_H

#define MAP_W 8
#define MAP_H 8

extern int world_map[MAP_H][MAP_W];

int get_grid_type(int x, int y);

#endif
