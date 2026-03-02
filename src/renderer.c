#include "renderer.h"
#include <SDL2/SDL.h>

static inline int get_rgb(int r, int g, int b){
    return (0xFF << 24) | (r << 16) | (g << 8) | b;
}

struct Renderer{
    int screen_width;
    int screen_height;

    uint32_t* pixels;
    uint32_t pixel_count;

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* buffer_texture;
};

Renderer* create_renderer(int screen_width, int screen_height){
    Renderer* renderer = malloc(sizeof(Renderer));

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
    for(int y = 0;y < renderer->screen_height;y++){
        for(int x = 0;x < renderer->screen_width;x++){
            renderer->pixels[y*renderer->screen_width+x] = get_rgb(x%256,y%256,0);
        }
    }

    return renderer;
}

void renderer_render(Renderer* renderer, Player* player){
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
