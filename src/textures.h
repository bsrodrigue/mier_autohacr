#include <raylib.h>

#ifndef TEXTURES_H
#define TEXTURES_H

#define NUM_TEXTURES 12

typedef struct {
  const char *path;
  Texture2D *texture;
} TextureInfo;

extern Texture2D player_texture;
extern Texture2D ubwall_texture;
extern Texture2D bwall_texture;
extern Texture2D floor_texture;
extern Texture2D target_texture;
extern Texture2D sentinel_texture;
extern Texture2D warpzone_texture;
extern Texture2D projectile_texture;
extern Texture2D sentinel_head_texture;
extern Texture2D sentinel_barrel_texture;
extern Texture2D healing_chip_texture;
extern Texture2D gate_key_texture;

void load_textures();

#endif
