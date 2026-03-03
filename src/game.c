#include "game.h"

#include "SDL2/SDL.h"

#include "player.h"
#include "renderer.h"
#include <stdio.h>

int run(){
    Player* player = create_player(); 

    Renderer* renderer = create_renderer(600, 600, 500, 500);
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
        }

        renderer_render(renderer, get_player_pos_x(player), get_player_pos_y(player), get_player_angle(player));

        double angle = get_player_angle(player) + 0.03;
        set_player_angle(player, angle);

        SDL_Delay(17);
    }

    return 0;
}
