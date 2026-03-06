#include <math.h>
#include "renderer.h"
#include "game_map.h"
#include <SDL2/SDL.h>

static inline int get_rgb(int r, int g, int b){
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

struct Renderer{
    int screen_width;
    int screen_height;

    int renderer_width;
    int renderer_height;

    int render_distance;
 
    uint32_t* pixels;
    uint32_t pixel_count;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer_texture;
};

Renderer* create_renderer(int screen_width, int screen_height, int renderer_width, int renderer_height, int renderer_distance){
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->render_distance = renderer_distance;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    renderer->screen_width = screen_width;
    renderer->screen_height = screen_height;

    renderer->renderer_width = renderer_width;
    renderer->renderer_height = renderer_height;

    renderer->window = SDL_CreateWindow("IronSPace",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,renderer->screen_width,renderer->screen_height,0);
    

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, render_flags);


    renderer->buffer_texture = SDL_CreateTexture(
        renderer->renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        renderer->renderer_width , renderer->renderer_height
    );

    renderer->pixel_count = renderer->renderer_width*renderer->renderer_height;
    renderer->pixels = malloc(sizeof(uint32_t)*renderer->pixel_count);

    return renderer;
}

void renderer_render(Renderer* renderer, double camera_pos_x, double camera_pos_y, double camera_angle, double step_size, double FOV){
    for(int i = 0;i < renderer->pixel_count;i++) renderer->pixels[i] = get_rgb(0,0,0);

    double start_angle = camera_angle - (FOV / 2);
    for(int x = 0; x < renderer->renderer_width; x++) {
        // Calculate ray angle
        double ray_angle = start_angle + ((double)x / renderer->renderer_width) * FOV;
        
        double psin = sin(ray_angle);
        double pcos = cos(ray_angle);
        
        double dist = 0;
        int hit = 0;

        int tile_x = -1;
        int tile_y = -1;


        // Move light forward (s = step size)
        for(double s = 0; s < renderer->render_distance; s += step_size) { 
            int checkX = (int)(camera_pos_x + pcos * s);
            int checkY = (int)(camera_pos_y + psin * s);

            // Border Check
            if(checkX < 0 || checkX >= MAP_W || checkY < 0 || checkY >= MAP_H) {
                dist = renderer->render_distance;
                break;
            }

            if(world_map[checkX][checkY] != 0) {
                // to fix fisheye effect
                dist = s * cos(ray_angle - camera_angle);
                hit = 1;

                tile_x = checkX;
                tile_y = checkY;
                break;
            }
        }

        if(hit) {
            // Calculate wall height
            int wall_height = (int)(renderer->renderer_height / dist);
            
            // Draw column
            int startY = (renderer->renderer_height / 2) - (wall_height / 2);
            int endY = (renderer->renderer_height / 2) + (wall_height / 2);

            // Border check
            if(startY < 0) startY = 0;
            if(endY >= renderer->renderer_height) endY = renderer->renderer_height - 1;

            for(int y = startY; y < endY; y++) {
                int pixel_id = y * renderer->renderer_width + x;
                double distance_ratio = dist / renderer->render_distance;
                if (distance_ratio > 1.0) distance_ratio = 1.0;
                double depth = 1.0 - distance_ratio;

                int r = 0;
                int g = 0;
                int b = 0;

                switch (world_map[tile_x][tile_y]) {
                    case WHITE_TILE:
                        r = 255;
                        g = 255;
                        b = 255;
                        break;
                    case BLACK_TILE:
                        r = 50;
                        g = 50;
                        b = 50;
                        break;
                    case BLUE_TILE: 
                        b = 255;
                        break;
                    case GREEN_TILE:
                        g = 255;
                        break;
                    case RED_TILE:
                        r = 255;
                        break;
                }
                r *= depth;
                g *= depth;
                b *= depth;

                renderer->pixels[pixel_id] = get_rgb(r, g, b);
            }
        }
    }


    SDL_RenderClear(renderer->renderer);

    void* pixel_ptr;
    int pitch;
    if (SDL_LockTexture(renderer->buffer_texture, NULL, &pixel_ptr, &pitch) == 0) {
        memcpy(pixel_ptr, renderer->pixels, renderer->renderer_width * renderer->renderer_height * sizeof(uint32_t));
        SDL_UnlockTexture(renderer->buffer_texture);
    }

    SDL_RenderCopy(renderer->renderer, renderer->buffer_texture, NULL, NULL);
    SDL_RenderPresent(renderer->renderer);

}


void set_renderer_render_distance(Renderer* renderer, int new_distance){
    renderer->render_distance = new_distance;
}
