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

Renderer* create_renderer(int screen_width, int screen_height, int renderer_width, int renderer_height){
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->render_distance = 5;

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

void renderer_render(Renderer* renderer, double camera_pos_x, double camera_pos_y, double camera_angle){
    for(int i = 0;i < renderer->pixel_count;i++) renderer->pixels[i] = get_rgb(0,0,0);

    double FOV = M_PI / 3; // 60 degree
    double start_angle = camera_angle - (FOV / 2);
    for(int x = 0; x < renderer->renderer_width; x++) {
        // Her sütun için açıyı hesapla
        double ray_angle = start_angle + ((double)x / renderer->renderer_width) * FOV;
        
        double psin = sin(ray_angle);
        double pcos = cos(ray_angle);
        
        double dist = 0;
        int hit = 0;

        // Işını ileriye doğru gönder (s = mesafe)
        for(double s = 0; s < renderer->render_distance; s += 0.1) { // 0.1 hassasiyet için
            int checkX = (int)(camera_pos_x + pcos * s);
            int checkY = (int)(camera_pos_y + psin * s);

            // Harita sınırları ve çarpışma kontrolü
            if(checkX < 0 || checkX >= MAP_W || checkY < 0 || checkY >= MAP_H) {
                dist = renderer->render_distance;
                break;
            }

            if(world_map[checkX][checkY] == 1) {
                // Balıkgözü (fisheye) etkisini düzeltmek için:
                dist = s * cos(ray_angle - camera_angle);
                hit = 1;
                break;
            }
        }

        if(hit) {
            // Duvar yüksekliğini mesafeye göre hesapla
            int wall_height = (int)(renderer->renderer_height / dist);
            
            // Sütunu çiz (Y ekseninde)
            int startY = (renderer->renderer_height / 2) - (wall_height / 2);
            int endY = (renderer->renderer_height / 2) + (wall_height / 2);

            // Ekran sınırlarını aşma
            if(startY < 0) startY = 0;
            if(endY >= renderer->renderer_height) endY = renderer->renderer_height - 1;

            for(int y = startY; y < endY; y++) {
                // int pixel_id = y * renderer->renderer_width + x;
                // int depth = 256 - ((int)(dist*renderer->render_distance) % 256);
                // renderer->pixels[pixel_id] = get_rgb(depth, depth, depth); // Gri duvar
                //                                                            //

                int pixel_id = y * renderer->renderer_width + x;
                double distance_ratio = dist / renderer->render_distance;
                if (distance_ratio > 1.0) distance_ratio = 1.0;
                int depth = (int)(255 * (1.0 - distance_ratio));

                renderer->pixels[pixel_id] = get_rgb(depth, depth, depth);
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

    SDL_RenderCopy(renderer->renderer, renderer->buffer_texture, NULL, NULL); // Tüm ekranı kaplar
    SDL_RenderPresent(renderer->renderer);

}
