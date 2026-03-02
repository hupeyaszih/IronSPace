#ifndef RENDERER_H
#define RENDERER_H

#include "player.h"

typedef struct Renderer Renderer;

int start();

Renderer* create_renderer(int screen_width, int screen_height);
void renderer_render(Renderer* renderer, Player* player);


#endif
