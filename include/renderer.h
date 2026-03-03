#ifndef RENDERER_H
#define RENDERER_H

typedef struct Renderer Renderer;

Renderer* create_renderer(int screen_width, int screen_height, int renderer_width, int renderer_height);
void renderer_render(Renderer* renderer, double camera_pos_x, double camera_pos_y, double camera_angle);

#endif
