#include "draw.h"
#include "common.h"
#include "config.h"
#include "entities.h"
#include "level_editor.h"
#include "textures.h"
#include "wall.h"
#include <raylib.h>
#include <raymath.h>

void draw_texture_cell(Vector2 position, Texture2D texture,
                       float rotation = 0) {
  DrawTexturePro(
      texture,
      {.x = 0, .y = 0, .width = GAME_TEXTURE_SIZE, .height = GAME_TEXTURE_SIZE},
      {.x = (position.x),
       .y = (position.y),
       .width = CELL_SIZE,
       .height = CELL_SIZE},
      {0, 0}, rotation, WHITE);
}

void draw_game_texture_editor(Vector2 position, Texture2D texture,
                              float rotation, Vector2 origin) {

  draw_game_texture(position, texture, rotation, CELL_SIZE, CELL_SIZE, origin);
}

void draw_game_texture(Vector2 position, Texture2D texture, float rotation,
                       float width, float height, Vector2 origin) {
  DrawTexturePro(
      texture,
      {.x = 0, .y = 0, .width = GAME_TEXTURE_SIZE, .height = GAME_TEXTURE_SIZE},
      {.x = (position.x), .y = (position.y), .width = width, .height = height},
      origin, rotation, WHITE);
}

void draw_floor(Vector2 position) {
  draw_texture_cell(position, floor_texture);
}

void draw_target_cursor(Vector2 position, float rotation) {
  draw_texture_cell(position, target_texture, rotation);
}

void draw_wall(Vector2 position, WallType type) {
  switch (type) {
  case BREAKABLE_WALL:
    draw_texture_cell(position, bwall_texture);
    break;
  case UNBREAKABLE_WALL:
    draw_texture_cell(position, ubwall_texture);
    break;
  }
}

void draw_warpzone(Vector2 position) {
  draw_texture_cell(position, warpzone_texture);
}
void draw_gate_key(Vector2 position, float rotation) {
  draw_game_texture(position, gate_key_texture, rotation, 15, 15);
}

void draw_ship(Vector2 position, float rotation) {
  draw_game_texture(position, player_texture, rotation);
}

void draw_ship_editor(Vector2 position, float rotation) {
  draw_texture_cell(position, player_texture, rotation);
}

void draw_base_enemy(Vector2 position, float rotation = 0) {

  draw_game_texture(Vector2Add(position, {0, 0}), sentinel_barrel_texture,
                    rotation + 90, GAME_TEXTURE_SIZE, GAME_TEXTURE_SIZE,
                    {.x = (GAME_TEXTURE_SIZE / 2.0f), .y = GAME_TEXTURE_SIZE});

  draw_game_texture(position, sentinel_head_texture, rotation + 90);
}

void draw_projectile(Vector2 position, float rotation) {
  draw_game_texture(position, projectile_texture, rotation);
}

void draw_healing_chip(Vector2 position, float rotation) {
  draw_game_texture(position, healing_chip_texture, rotation, 15, 15);
}

void draw_editor_entity(EditorGridCell cell, Vector2 position) {
  EntityType type = cell.type;

  if (std::holds_alternative<EditorWall>(cell.entity)) {
    const EditorWall &wall = std::get<EditorWall>(cell.entity);

    draw_wall(position, wall.type);
  }

  else if (std::holds_alternative<EditorPlayer>(cell.entity)) {
    draw_ship_editor(position, 0);
  }

  else if (std::holds_alternative<EditorEnemy>(cell.entity)) {
    // const EditorEnemy &enemy = std::get<EditorEnemy>(cell.entity);

    Vector2 offset = {GAME_TEXTURE_SIZE / 2.0f, GAME_TEXTURE_SIZE / 2.0f};
    draw_base_enemy(Vector2Add(position, offset), 90.0f);
  }

  else if (std::holds_alternative<EditorWarpzone>(cell.entity)) {
    // const EditorEnemy &enemy = std::get<EditorEnemy>(cell.entity);

    draw_warpzone(position);
  }

  else if (std::holds_alternative<EditorItem>(cell.entity)) {
    const EditorItem &item = std::get<EditorItem>(cell.entity);

    if (item.effect == HEALING_EFFECT) {
      draw_healing_chip(position, 90.0f);
    }

    else if (item.effect == KEY_EFFECT) {
      draw_gate_key(position, 90.0f);
    }
  }

  else if (std::holds_alternative<EditorGate>(cell.entity)) {
    const EditorGate &gate = std::get<EditorGate>(cell.entity);
    draw_warpzone(position);
  }
}

void render_mouse_hover_grid(EntityType type, Vector2 mouse) {
  Vector2 position =
      get_offset_position(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y));

  switch (type) {
  case EMPTY_ENTITY:
    draw_floor(position);
    break;
  case UBWALL_ENTITY:
    draw_wall(position, UNBREAKABLE_WALL);
    break;
  case BWALL_ENTITY:
    draw_wall(position, BREAKABLE_WALL);
    break;
  case PLAYER_ENTITY:
    draw_texture_cell(position, player_texture);
    break;
  case BASE_ENEMY_ENTITY: {
    Vector2 offset = {GAME_TEXTURE_SIZE / 2.0f, GAME_TEXTURE_SIZE / 2.0f};
    draw_base_enemy(Vector2Add(position, offset), 90.0f);
  } break;
  case WARPZONE_ENTITY:
    draw_warpzone(position);
    break;
  case ITEM_ENTITY:
    draw_healing_chip(position, 90.0f);
    break;
  case GATE_ENTITY:
    draw_warpzone(position);
    break;
  }
}
