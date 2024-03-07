#include <raylib.h>

#ifndef TEXTURES_H
#define TEXTURES_H

// ACTORS
#define PLAYER_TEXTURE_SRC "./assets/textures/actors/ship.png"
#define TARGET_TEXTURE_SRC "./assets/textures/actors/target.png"
#define SENTINEL_TEXTURE_SRC "./assets/textures/actors/sentinel.png"

// ENVIRONMENT
#define UBWALL_TEXTURE_SRC "./assets/textures/environment/wall.png"
#define BWALL_TEXTURE_SRC "./assets/textures/environment/bwall.png"
#define FLOOR_TEXTURE_SRC "./assets/textures/environment/floor.png"

extern Texture2D player_texture;
extern Texture2D ubwall_texture;
extern Texture2D bwall_texture;
extern Texture2D floor_texture;
extern Texture2D target_texture;
extern Texture2D sentinel_texture;

void load_textures();

#endif
