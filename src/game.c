#include "game.h"
#include "renderer.h"
#include "SDL2/SDL.h"
#include "game_map.h"
#include "player.h"

int run(){
    Player* player = create_player(); 

    Renderer* renderer = create_renderer(1280, 800, 640, 400);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    int close = 0;

    set_player_pos_x(player, 4);

    while(close == 0){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch (event.type) {
            case SDL_QUIT:
                close = 1;
                break;

            }


            if (event.type == SDL_MOUSEMOTION) {
                float sensitivity = 0.003f;
                add_player_angle(player, event.motion.xrel * sensitivity);
            }

        }


        {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        double speed = 0.01;
        double angle = get_player_angle(player);
        double cur_x = get_player_pos_x(player);
        double cur_y = get_player_pos_y(player);
        
        // Temp variables (To calculate new position)
        double next_x = cur_x;
        double next_y = cur_y;

        if (state[SDL_SCANCODE_W]) {
            next_x += cos(angle) * speed;
            next_y += sin(angle) * speed;
        }
        if (state[SDL_SCANCODE_S]) {
            next_x -= cos(angle) * speed;
            next_y -= sin(angle) * speed;
        }
        if (state[SDL_SCANCODE_A]) {
            next_x += cos(angle - M_PI/2) * speed;
            next_y += sin(angle - M_PI/2) * speed;
        }
        if (state[SDL_SCANCODE_D]) {
            next_x += cos(angle + M_PI/2) * speed;
            next_y += sin(angle + M_PI/2) * speed;
        }

        // Basit Çarpışma Kontrolü
        if (get_grid_type((int)next_x, (int)next_y) == 0) {
            set_player_pos_x(player, next_x);
            set_player_pos_y(player, next_y);
        }
        }

        renderer_render(renderer, get_player_pos_x(player), get_player_pos_y(player), get_player_angle(player), 0.03, M_PI/3);
        SDL_Delay(17);
    }

    return 0;
}
