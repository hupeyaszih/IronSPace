#ifndef RENDERER_H
#define RENDERER_H

#include "asset_manager.h"

typedef struct Renderer Renderer;

Renderer* create_renderer(int screen_width, int screen_height, int renderer_width, int renderer_height, int renderer_distance);
void renderer_render(Renderer* renderer, AssetManager* texture_manager, double camera_pos_x, double camera_pos_y, double camera_angle, double step_size, double FOV);

void set_renderer_render_distance(Renderer* renderer, int new_distance);

#endif
