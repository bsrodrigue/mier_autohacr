#include "textures.h"
#include <raylib.h>

Texture2D Sentinel_Head_texture;
Texture2D sentinel_texture;
Texture2D Sentinel_Barrel_texture;
Texture2D target_texture;
Texture2D ship_texture;
Texture2D floor_1_texture;
Texture2D floor_2_texture;
Texture2D bak_floor_texture;
Texture2D bwall_texture;
Texture2D wall1_texture;
Texture2D floor_texture;
Texture2D wall_texture;
Texture2D CHESS_texture;
Texture2D Warpzone_texture;
Texture2D Projectile_texture;
Texture2D gate_key_texture;
Texture2D healing_chip_texture;
Texture2D OFFICEDOOR_texture;
Texture2D SPOOKYDOOR_texture;
Texture2D CREAKYDOOR_texture;

TextureInfo textures[20] = {
    {"assets/textures/actors/Sentinel_Head.png", &Sentinel_Head_texture},
    {"assets/textures/actors/sentinel.png", &sentinel_texture},
    {"assets/textures/actors/Sentinel_Barrel.png", &Sentinel_Barrel_texture},
    {"assets/textures/actors/target.png", &target_texture},
    {"assets/textures/actors/ship.png", &ship_texture},
    {"assets/textures/environment/floor_1.png", &floor_1_texture},
    {"assets/textures/environment/floor_2.png", &floor_2_texture},
    {"assets/textures/environment/bak_floor.png", &bak_floor_texture},
    {"assets/textures/environment/bwall.png", &bwall_texture},
    {"assets/textures/environment/wall1.png", &wall1_texture},
    {"assets/textures/environment/floor.png", &floor_texture},
    {"assets/textures/environment/wall.png", &wall_texture},
    {"assets/textures/environment/CHESS.png", &CHESS_texture},
    {"assets/textures/transport/Warpzone.png", &Warpzone_texture},
    {"assets/textures/projectiles/Projectile.png", &Projectile_texture},
    {"assets/textures/items/gate_key.png", &gate_key_texture},
    {"assets/textures/items/healing_chip.png", &healing_chip_texture},
    {"assets/textures/gates/OFFICEDOOR.png", &OFFICEDOOR_texture},
    {"assets/textures/gates/SPOOKYDOOR.png", &SPOOKYDOOR_texture},
    {"assets/textures/gates/CREAKYDOOR.png", &CREAKYDOOR_texture}
};

void load_textures() {
  for (int i = 0; i < NUM_TEXTURES; i++) {
    *textures[i].texture = LoadTexture(textures[i].path);
  }
}
