
#include "textures.h"
#include <raylib.h>

void load_actor_textures() { player_texture = LoadTexture(PLAYER_TEXTURE_SRC); }

void load_wall_textures() {
  ubwall_texture = LoadTexture(UBWALL_TEXTURE_SRC);
  bwall_texture = LoadTexture(BWALL_TEXTURE_SRC);
}

void load_floor_textures() { floor_texture = LoadTexture(FLOOR_TEXTURE_SRC); }

void load_target_textures() {
  target_texture = LoadTexture(TARGET_TEXTURE_SRC);
}

void load_enemies_textures() {
  sentinel_texture = LoadTexture(SENTINEL_TEXTURE_SRC);
  sentinel_head_texture = LoadTexture(SENTINEL_HEAD_TEXTURE_SRC);
  sentinel_barrel_texture = LoadTexture(SENTINEL_BARREL_TEXTURE_SRC);
}

void load_transport_textures() {
  warzone_texture = LoadTexture(WARPZONE_TEXTURE_SRC);
}

void load_projectile_textures() {
  projectile_texture = LoadTexture(PROJECTILE_TEXTURE_SRC);
}

void load_textures() {
  load_actor_textures();
  load_target_textures();
  load_wall_textures();
  load_floor_textures();
  load_enemies_textures();
  load_transport_textures();
  load_projectile_textures();
}
