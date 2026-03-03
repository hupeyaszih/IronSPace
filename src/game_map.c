#include "game_map.h"

int world_map[MAP_H][MAP_W] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,1,0,0,0,1},
    {1,0,0,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1},
};
int get_grid_type(int x, int y){
    if(x < 0 || x > MAP_W || y < 0 || y > MAP_H) return -1;
    return world_map[x][y];
}
