#include "textures.h"
#include <raylib.h>

Texture2D player_texture;
Texture2D ubwall_texture;
Texture2D bwall_texture;
Texture2D floor_texture;
Texture2D target_texture;
Texture2D sentinel_texture;
Texture2D warpzone_texture;
Texture2D projectile_texture;
Texture2D sentinel_head_texture;
Texture2D sentinel_barrel_texture;
Texture2D healing_chip_texture;
Texture2D gate_key_texture;

TextureInfo textures[NUM_TEXTURES] = {
    {"./assets/textures/environment/wall.png", &ubwall_texture},
    {"./assets/textures/environment/bwall.png", &bwall_texture},
    {"./assets/textures/environment/floor.png", &floor_texture},

    {"./assets/textures/transport/Warpzone.png", &warpzone_texture},

    {"./assets/textures/projectiles/Projectile.png", &projectile_texture},

    {"./assets/textures/actors/ship.png", &player_texture},
    {"./assets/textures/actors/target.png", &target_texture},
    {"./assets/textures/actors/sentinel.png", &sentinel_texture},
    {"./assets/textures/actors/Sentinel_Head.png", &sentinel_head_texture},
    {"./assets/textures/actors/Sentinel_Barrel.png", &sentinel_barrel_texture},

    {"./assets/textures/items/healing_chip.png", &healing_chip_texture},
    {"./assets/textures/items/gate_key.png", &gate_key_texture}};

void load_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    *textures[i].texture = LoadTexture(textures[i].path);
  }
}
