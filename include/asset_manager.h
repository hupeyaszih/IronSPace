#ifndef ASSET_MANAGER
#define ASSET_MANAGER

#include <SDL_surface.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct AssetManager AssetManager;
typedef struct Asset Asset;

void init_sdl_image_lib();

AssetManager* create_asset_manager(int asset_capacity);

Asset* load_asset(AssetManager* asset_manager, const char *path); //Load asset from file, returns pointer
Asset* get_asset(AssetManager* asset_manager, int asset_id); //returns pointer from loaded assets list by id

int get_asset_id(Asset* asset);
SDL_Surface* get_asset_surface(Asset* asset);

#endif
