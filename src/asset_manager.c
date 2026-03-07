#include "asset_manager.h"
#include <SDL_image.h>
#include <stdlib.h>

struct AssetManager{
    int loaded_asset_count;
    int asset_capacity;

    Asset* loaded_assets;
};

struct Asset{
    int id;
    SDL_Surface* surface;
};

void init_sdl_image_lib(){
    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
    }
}

AssetManager* create_asset_manager(int asset_capacity){
    AssetManager* asset_manager = malloc(sizeof(AssetManager));
    asset_manager->loaded_asset_count = 0;
    asset_manager->asset_capacity = asset_capacity;
    asset_manager->loaded_assets = malloc(sizeof(Asset)*asset_manager->asset_capacity);

    return asset_manager;
}

// Asset* load_asset(AssetManager* asset_manager, const char *path){
//     Asset* asset = malloc(sizeof(Asset));
//     asset_manager->loaded_asset_count++;
//     asset->id = asset_manager->loaded_asset_count;
//
//     // asset->surface = IMG_Load(path);
//
//     SDL_Surface* temp = IMG_Load(path);
//     if (!temp) {
//         // This will tell you EXACTLY why it failed (usually "File not found")
//         printf("IMG_Load Error for [%s]: %s\n", path, IMG_GetError());
//         return NULL; 
//     }
//
//     SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA32, 0);
//     SDL_FreeSurface(temp);
//
//     if (!surface) {
//         printf("Surface Conversion Error: %s\n", SDL_GetError());
//         return NULL;
//     }
//
//     asset->surface = surface;
//     printf("\"%s\" loaded successfully\n", path);
//     return asset;
// }

Asset* load_asset(AssetManager* asset_manager, const char *path){
    // Write directly into the pre-allocated array slot
    int index = asset_manager->loaded_asset_count;
    Asset* asset = &asset_manager->loaded_assets[index];
    
    SDL_Surface* temp = IMG_Load(path);
    if (!temp) {
        printf("IMG_Load Error for [%s]: %s\n", path, IMG_GetError());
        return NULL; 
    }
    SDL_Surface* surface = SDL_ConvertSurfaceFormat(temp, SDL_PIXELFORMAT_RGBA32, 0);
    SDL_FreeSurface(temp);
    if (!surface) {
        printf("Surface Conversion Error: %s\n", SDL_GetError());
        return NULL;
    }
    asset->surface = surface;
    asset->id = index;
    asset_manager->loaded_asset_count++;  // increment AFTER, so id matches index
    
    printf("\"%s\" loaded successfully\n", path);
    return asset;
}

Asset* get_asset(AssetManager* asset_manager, int asset_id){ if(asset_id >= 0 && asset_id < asset_manager->loaded_asset_count)return &asset_manager->loaded_assets[asset_id]; else return NULL;}


int get_asset_id(Asset* asset){return asset->id;}
SDL_Surface* get_asset_surface(Asset* asset){return asset->surface;}
