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

    int render_distance;

    uint32_t* pixels;
    uint32_t pixel_count;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer_texture;
};

Renderer* create_renderer(int screen_width, int screen_height){
    Renderer* renderer = malloc(sizeof(Renderer));
    renderer->render_distance = 50;

    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        printf("error initializing SDL: %s\n", SDL_GetError());
    }
    renderer->screen_width = screen_width;
    renderer->screen_height = screen_height;

    renderer->window = SDL_CreateWindow("IronSPace",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,renderer->screen_width,renderer->screen_height,0);
    

    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    renderer->renderer = SDL_CreateRenderer(renderer->window, -1, render_flags);


    renderer->buffer_texture = SDL_CreateTexture(
        renderer->renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        renderer->screen_width, renderer->screen_height
    );

    renderer->pixel_count = renderer->screen_width*renderer->screen_height;
    renderer->pixels = malloc(sizeof(uint32_t)*renderer->pixel_count);

    return renderer;
}

void renderer_render(Renderer* renderer, Player* player){
    // for(int y = 0;y < renderer->screen_height;y++){
    //     int dist = 0;
    //     int height = 0;
    //     double angle = (double)y*M_PI / 180;
    //
    //     for(int s = 0;s < renderer->render_distance;s++){
    //         double psin = sin(angle);
    //         double pcos = cos(angle);
    //         int posY = (int) (psin*s);
    //         int posX = (int) (pcos*s);
    //
    //         if(posY < 0 || posX < 0 || posY > MAP_H || posX > MAP_W) break;
    //
    //         printf("%d ",posX);
    //         if(world_map[posX][posY] == 1){
    //             dist = s;
    //             break;
    //         }
    //
    //     }
    //
    //     if(dist > 0)height = (int)(renderer->screen_height / dist);
    //
    //     for(int x = height/2;x < renderer->screen_width - height/2;x++){
    //         int pixel_id = y * renderer->screen_width + x;
    //
    //         renderer->pixels[pixel_id] = get_rgb(dist,dist,dist);
    //     }
    // }

        double FOV = M_PI / 3; // 60 degree
        double start_angle = get_player_angle(player) - (FOV / 2);


    for(int x = 0; x < renderer->screen_width; x++) {
        // Her sütun için açıyı hesapla
        double ray_angle = start_angle + ((double)x / renderer->screen_width) * FOV;
        
        double psin = sin(ray_angle);
        double pcos = cos(ray_angle);
        
        double dist = 0;
        int hit = 0;

        // Işını ileriye doğru gönder (s = mesafe)
        for(double s = 0; s < renderer->render_distance; s += 0.1) { // 0.1 hassasiyet için
            int checkX = (int)(get_player_pos_x(player) + pcos * s);
            int checkY = (int)(get_player_pos_y(player) + psin * s);

            // Harita sınırları ve çarpışma kontrolü
            if(checkX < 0 || checkX >= MAP_W || checkY < 0 || checkY >= MAP_H) {
                dist = renderer->render_distance;
                break;
            }

            if(world_map[checkX][checkY] == 1) {
                // Balıkgözü (fisheye) etkisini düzeltmek için:
                dist = s * cos(ray_angle - get_player_angle(player));
                hit = 1;
                break;
            }
        }

        if(hit) {
            // Duvar yüksekliğini mesafeye göre hesapla
            int wall_height = (int)(renderer->screen_height / dist);
            
            // Sütunu çiz (Y ekseninde)
            int startY = (renderer->screen_height / 2) - (wall_height / 2);
            int endY = (renderer->screen_height / 2) + (wall_height / 2);

            // Ekran sınırlarını aşma
            if(startY < 0) startY = 0;
            if(endY >= renderer->screen_height) endY = renderer->screen_height - 1;

            for(int y = startY; y < endY; y++) {
                int pixel_id = y * renderer->screen_width + x;
                renderer->pixels[pixel_id] = get_rgb(150, 150, 150); // Gri duvar
            }
        }
    }


    SDL_RenderClear(renderer->renderer);

    void* pixel_ptr;
    int pitch;
    if (SDL_LockTexture(renderer->buffer_texture, NULL, &pixel_ptr, &pitch) == 0) {
        memcpy(pixel_ptr, renderer->pixels, renderer->screen_width * renderer->screen_height * sizeof(uint32_t));
        SDL_UnlockTexture(renderer->buffer_texture);
    }

    SDL_RenderCopy(renderer->renderer, renderer->buffer_texture, NULL, NULL); // Tüm ekranı kaplar
    SDL_RenderPresent(renderer->renderer);

}

int start(){
    Player* player = create_player(); 

    Renderer* renderer = create_renderer(730, 510);
    int close = 0;

    while(close == 0){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_QUIT:
                close = 1;
                break;
            }
        }

        renderer_render(renderer, player);
    }

    return 0;
}
