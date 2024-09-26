#include "level_editor.h"
#include "camera.h"
#include "common.h"
#include "config.h"
#include "draw.h"
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

Vector2 get_player_position(EditorGridCell (*grid)[100]) {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      EntityType type = grid[y][x].type;
      if (type == PLAYER_ENTITY)
        return {(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)};
    }
  }

  return {-1, -1};
}

LevelEditor level_editor;

void load_level_editor(const char *filename) {
  level_editor.filename = filename;

  // TODO: Allow nesting
  if (!FileExists(filename)) {
    level_editor.create_level_data();
  }

  level_editor.load_level_data();
}

void render_entities() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_editor.grid[y][x].type;
      draw_editor_entity((EntityType)type, {(float)x, (float)y});
    }
  }
}

void handle_editor_actions(Camera2D *camera, int pressed_key) {
  switch (pressed_key) {
  case KEY_M:
    level_editor.save_level();
    break;
  case KEY_SPACE:
    level_editor.next_type();
    break;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse = get_world_mouse(*camera);
    level_editor.place_entity(mouse);
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = get_world_mouse(*camera);
    level_editor.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)].type =
        EMPTY_ENTITY;
  }
}

// TODO: Setup a generic Key Manipulation Handler
void handle_editor_input(Camera2D *camera, int pressed_key) {
  handle_editor_actions(camera, pressed_key);
  handle_camera_movement(camera);
}

void render_mouse_position(Camera2D camera, Vector2 mouse) {
  Vector2 hud_position = camera.target;

  const char *message = TextFormat("CELL: (%d,%d)", MOUSE_TO_GRID(mouse.x),
                                   MOUSE_TO_GRID(mouse.y));

  DrawText(message, hud_position.x, hud_position.y, 14, RED);
}

void render_current_entity_type(Camera2D camera) {
  Vector2 hud_position = camera.target;
  float offset = 20;

  const char *message =
      TextFormat("ENTITY TYPE: %s",
                 get_entity_type_name(
                     level_editor.types[level_editor.current_entity_index]));

  DrawText(message, hud_position.x, hud_position.y + offset, 14, RED);
}

void render_hud(Camera2D *camera, Vector2 mouse) {
  render_mouse_position(*camera, mouse);
  render_current_entity_type(*camera);
}

void render_level_editor(Camera2D *camera) {
  Vector2 mouse = get_world_mouse(*camera);

  render_entities();
  render_mouse_hover_grid(
      mouse, level_editor.types[level_editor.current_entity_index]);
  render_hud(camera, mouse);
}
