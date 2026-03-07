#include <math.h>
#include "renderer.h"
#include "asset_manager.h"
#include "game_map.h"
#include <SDL2/SDL.h>
#include <stdio.h>

typedef struct {
    Uint8 r, g, b, a;
} ColorRGBA;

static inline uint32_t rgb_color(uint8_t r, uint8_t g, uint8_t b) {
    return (255<<24) + (r<<16) + (g<<8) + b;
}
static inline ColorRGBA get_rgb(SDL_Surface* surface, int x, int y){
    ColorRGBA color = {0, 0, 0, 0};

    // 1. Boundary check (Crucial: prevents segmentation faults!)
    if (x < 0 || x >= surface->w || y < 0 || y >= surface->h) {
        return color; 
    }

    // 2. Lock if necessary
    if (SDL_MUSTLOCK(surface)) SDL_LockSurface(surface);

    // 3. Get the pixel address
    // We use the 'pitch' (row length in bytes) to find the right row
    Uint8* pixel_ptr = (Uint8*)surface->pixels + (y * surface->pitch) + (x * surface->format->BytesPerPixel);
    Uint32 pixel_data;

    // 4. Handle different bit-depths (8, 16, 24, or 32-bit)
    switch (surface->format->BytesPerPixel) {
        case 1: pixel_data = *pixel_ptr; break;
        case 2: pixel_data = *(Uint16*)pixel_ptr; break;
        case 3: 
            // 24-bit is tricky, depends on Endianness
            #if SDL_BYTEORDER == SDL_BIG_ENDIAN
                pixel_data = pixel_ptr[0] << 16 | pixel_ptr[1] << 8 | pixel_ptr[2];
            #else
                pixel_data = pixel_ptr[0] | pixel_ptr[1] << 8 | pixel_ptr[2] << 16;
            #endif
            break;
        case 4: pixel_data = *(Uint32*)pixel_ptr; break;
        default: pixel_data = 0; break;
    }

    // 5. Extract the actual RGBA components
    SDL_GetRGBA(pixel_data, surface->format, &color.r, &color.g, &color.b, &color.a);

    if (SDL_MUSTLOCK(surface)) SDL_UnlockSurface(surface);
    return color;
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

void renderer_render(Renderer* renderer, AssetManager* texture_manager, double camera_pos_x, double camera_pos_y, double camera_angle, double step_size, double FOV){
    for(int i = 0;i < renderer->pixel_count;i++) renderer->pixels[i] = rgb_color(0,0,0);

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

            if(world_map[checkX][checkY].type != 0) {
                // to fix fisheye effect
                dist = s * cos(ray_angle - camera_angle);
                hit = 1;

                tile_x = checkX;
                tile_y = checkY;
                break;
            }
        }

        if(hit) {
            SDL_Surface* tex_surface = get_asset_surface(get_asset(texture_manager, world_map[tile_x][tile_y].texture_asset_id));
            int tex_w = tex_surface->w;
            int tex_h = tex_surface->h;

            double hit_x = camera_pos_x + pcos * dist;
            double hit_y = camera_pos_y + psin * dist;

            double hit_offset;
            if (tile_x != (int)(camera_pos_x + pcos * (dist - step_size))) {
                // Hit a wall on X axis — use Y fraction
                hit_offset = hit_y - floor(hit_y);
            } else {
                // Hit a wall on Y axis — use X fraction
                hit_offset = hit_x - floor(hit_x);
            }


            int tx = (int)(hit_offset * tex_w);
            if (tx < 0) tx = 0;
            if (tx >= tex_w) tx = tex_w - 1;
            
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

                double wall_fraction = (double)(y - ((renderer->renderer_height / 2) - (wall_height / 2))) / (double)wall_height;
                    int ty = (int)(wall_fraction * tex_h);
                    if (ty < 0) ty = 0;
                    if (ty >= tex_h) ty = tex_h - 1;

                    ColorRGBA colour = get_rgb(tex_surface, tx, ty);

                    // Apply depth shading
                    renderer->pixels[pixel_id] = rgb_color(
                        (uint8_t)(colour.r * depth),
                        (uint8_t)(colour.g * depth),
                        (uint8_t)(colour.b * depth)
                    );
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
