#pragma once
#include <raylib.h>

#define NUM_TEXTURES 20

typedef struct {
  const char *path;
  Texture2D *texture;
} TextureInfo;

extern Texture2D Sentinel_Head_texture;
extern Texture2D sentinel_texture;
extern Texture2D Sentinel_Barrel_texture;
extern Texture2D target_texture;
extern Texture2D ship_texture;
extern Texture2D floor_1_texture;
extern Texture2D floor_2_texture;
extern Texture2D bak_floor_texture;
extern Texture2D bwall_texture;
extern Texture2D wall1_texture;
extern Texture2D floor_texture;
extern Texture2D wall_texture;
extern Texture2D CHESS_texture;
extern Texture2D Warpzone_texture;
extern Texture2D Projectile_texture;
extern Texture2D gate_key_texture;
extern Texture2D healing_chip_texture;
extern Texture2D OFFICEDOOR_texture;
extern Texture2D SPOOKYDOOR_texture;
extern Texture2D CREAKYDOOR_texture;

void load_textures();
