#include <raylib.h>

#ifndef TEXTURES_H
#define TEXTURES_H

#define PLAYER_TEXTURE_SRC "./assets/textures/actors/ship.png"
#define UBWALL_TEXTURE_SRC "./assets/textures/environment/wall.png"
#define BWALL_TEXTURE_SRC "./assets/textures/environment/bwall.png"
#define FLOOR_TEXTURE_SRC "./assets/textures/environment/floor.png"

extern Texture2D player_texture;
extern Texture2D ubwall_texture;
extern Texture2D bwall_texture;
extern Texture2D floor_texture;

void load_actor_textures();

void load_wall_textures();

void load_floor_textures();

void load_textures();

#endif
