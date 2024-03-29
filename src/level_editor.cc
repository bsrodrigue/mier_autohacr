#include "common.h"
#include "config.h"
#include "entities.h"
#include "level.h"
#include "save.h"
#include "textures.h"
#include "wall.h"
#include <raylib.h>
#include <raymath.h>

// TODO: Define flexible level structure and editing
// TODO: Provide simple GUI to switch between entity types
// TODO: Add undo action

#define TYPE_COUNT 7

static Level level;

Vector2 get_world_mouse(Camera2D camera) {
  Vector2 mouse = GetMousePosition();
  return GetScreenToWorld2D(mouse, camera);
}

int current_entity_index = 1;
EntityType types[TYPE_COUNT] = {EMPTY,      BWALL,          UBWALL,  PLAYER,
                                BASE_ENEMY, SENTRY_A_ENEMY, WARPZONE};
EntityType current_entity_type = types[current_entity_index];

void next_type() {
  if (current_entity_index + 1 >= TYPE_COUNT) {
    current_entity_index = 0;
    return;
  }

  current_entity_index++;
}

const char *get_entity_type_name(EntityType type) {
  switch (type) {
  case EMPTY:
    return "Empty";
  case UBWALL:
    return "Unbreakable Wall";
  case BWALL:
    return "Breakable Wall";
  case PLAYER:
    return "Player";
  case BASE_ENEMY:
    return "Base Enemy";
  case SENTRY_A_ENEMY:
    return "Sentry A Enemy";
  case WARPZONE:
    return "Warp Zone";
  default:
    return "";
  }
}

Vector2 get_player_position(int level_grid[CELL_COUNT][CELL_COUNT]) {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      EntityType type = (EntityType)level_grid[y][x];
      if (type == PLAYER)
        return {(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)};
    }
  }

  return {-1, -1};
}

void load_level_editor(const char *filename) {
  level.filename = filename;

  if (!FileExists(filename)) {
    level.create_level_data();
  }

  level.load_level_data();
}

void draw_grid() {
  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(CELL_OFFSET(i), 0, CELL_OFFSET(i), LEVEL_HEIGHT, WHITE);
  }

  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(0, CELL_OFFSET(i), LEVEL_WIDTH, CELL_OFFSET(i), WHITE);
  }
}

void draw_ubwall(Vector2 position) { draw_wall(position, ubwall_texture); }
void draw_bwall(Vector2 position) { draw_wall(position, bwall_texture); }

void draw_warpzone(Vector2 position) {
  DrawTexturePro(warzone_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = (position.x * CELL_SIZE),
                  .y = (position.y * CELL_SIZE),
                  .width = 25,
                  .height = 25},
                 {0, 0}, 0, WHITE);
}

void draw_ship(Vector2 position) {
  DrawTexturePro(player_texture, {.x = 0, .y = 0, .width = 32, .height = 32},
                 {.x = (position.x * CELL_SIZE),
                  .y = (position.y * CELL_SIZE),
                  .width = 25,
                  .height = 25},
                 {0, 0}, 0, WHITE);
}

void render_entity(EntityType type, Vector2 position) {
  draw_wall(position, floor_texture);
  switch (type) {
  case EMPTY:
    break;
  case UBWALL:
    draw_wall(position, ubwall_texture);
    break;
  case BWALL:
    draw_wall(position, bwall_texture);
    break;
  case PLAYER:
    draw_ship(position);
    break;
  case BASE_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(RED, 1));
    break;
  case SENTRY_A_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(PURPLE, 1));
    break;
  case WARPZONE:
    draw_warpzone(position);
    break;
  }
}

void render_entities() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level.grid[y][x];
      render_entity((EntityType)type, {(float)x, (float)y});
    }
  }
}

// TODO: Setup a generic Key Manipulation Handler
void handle_level_input(Camera2D *camera, int pressed_key) {
  Vector2 camera_movement = {0, 0};
  Vector2 camera_vertical_movement = {0, 0};
  Vector2 camera_horizontal_movement = {0, 0};
  float camera_velocity = 10;

  switch (pressed_key) {
  case KEY_M:
    level.save_level();
    break;
  case KEY_SPACE:
    next_type();
    current_entity_type = types[current_entity_index];
    break;
  }

  if (IsKeyDown(KEY_W)) {
    camera_vertical_movement = {0, -camera_velocity};
  } else if (IsKeyDown(KEY_S)) {
    camera_vertical_movement = {0, camera_velocity};
  }

  if (IsKeyDown(KEY_A)) {
    camera_horizontal_movement = {-camera_velocity, 0};
  } else if (IsKeyDown(KEY_D)) {
    camera_horizontal_movement = {camera_velocity, 0};
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse = get_world_mouse(*camera);

    if (MOUSE_TO_GRID(mouse.x) >= CELL_COUNT ||
        MOUSE_TO_GRID(mouse.y) >= CELL_COUNT) {
      return;
    }

    if (current_entity_type == PLAYER) {
      Vector2 previous_pos = get_player_position(level.grid);

      if (previous_pos.x != -1) {
        level.grid[(int)previous_pos.y / CELL_SIZE]
                  [(int)previous_pos.x / CELL_SIZE] = EMPTY;
      }
    }

    level.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] =
        current_entity_type;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = get_world_mouse(*camera);
    level.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] = EMPTY;
  }

  camera->target =
      Vector2Add(camera->target, Vector2Add(camera_horizontal_movement,
                                            camera_vertical_movement));
}

void render_mouse_hover_grid(Vector2 mouse) {
  switch (current_entity_type) {
  case UBWALL:
    draw_wall({(float)MOUSE_TO_GRID(mouse.x), (float)MOUSE_TO_GRID(mouse.y)},
              ubwall_texture);
    break;
  case BWALL:
    draw_wall({(float)MOUSE_TO_GRID(mouse.x), (float)MOUSE_TO_GRID(mouse.y)},
              bwall_texture);
    break;
  case PLAYER:
    draw_ship({(float)MOUSE_TO_GRID(mouse.x), (float)MOUSE_TO_GRID(mouse.y)});
    break;
  case BASE_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE((int)(mouse.x / CELL_SIZE)),
                 MOUSE_TO_CIRCLE((int)(mouse.y / CELL_SIZE))},
                10, ColorAlpha(RED, 0.5));
    break;
  case EMPTY:
    draw_wall({(float)MOUSE_TO_GRID(mouse.x), (float)MOUSE_TO_GRID(mouse.y)},
              floor_texture);
    break;
  case SENTRY_A_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE((int)(mouse.x / CELL_SIZE)),
                 MOUSE_TO_CIRCLE((int)(mouse.y / CELL_SIZE))},
                10, ColorAlpha(PURPLE, 0.5));
    break;
  case WARPZONE:
    draw_warpzone(
        {(float)MOUSE_TO_GRID(mouse.x), (float)MOUSE_TO_GRID(mouse.y)});
    break;
  }
}

void render_mouse_position(Camera2D camera, Vector2 mouse) {
  Vector2 hud_position = camera.target;
  const char *message = TextFormat("MOUSE: (%d,%d)", MOUSE_TO_GRID(mouse.x),
                                   MOUSE_TO_GRID(mouse.y));
  DrawText(message, hud_position.x, hud_position.y, 14, RED);
}

void render_current_entity_type(Camera2D camera) {
  const char *message =
      TextFormat("ENTITY TYPE: %s", get_entity_type_name(current_entity_type));
  DrawText(message, TEXT_POS(1), TEXT_POS(10), 14, RED);
}

void render_hud(Camera2D *camera, Vector2 mouse) {
  render_mouse_position(*camera, mouse);
  render_current_entity_type(*camera);
}

void render_level_editor(Camera2D *camera) {
  Vector2 mouse = get_world_mouse(*camera);
  draw_grid();
  render_entities();
  render_mouse_hover_grid(mouse);
  render_hud(camera, mouse);
}
