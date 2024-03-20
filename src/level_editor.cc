#include "level_editor.h"
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

LevelEditor level_editor;

void load_level_editor(const char *filename) {
  level_editor.filename = filename;

  if (!FileExists(filename)) {
    level_editor.create_level_data();
  }

  level_editor.load_level_data();
}

void render_entities() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_editor.grid[y][x].type;
      draw_entity((EntityType)type, {(float)x, (float)y});
    }
  }
}

// TODO: Setup a generic Key Manipulation Handler
void handle_editor_input(Camera2D *camera, int pressed_key) {
  Vector2 camera_movement = {0, 0};
  Vector2 camera_vertical_movement = {0, 0};
  Vector2 camera_horizontal_movement = {0, 0};
  float camera_velocity = 10;

  switch (pressed_key) {
  case KEY_M:
    level_editor.save_level();
    break;
  case KEY_SPACE:
    level_editor.next_type();
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

    if (level_editor.current_entity_type == PLAYER) {
      Vector2 previous_pos = level_editor.get_player_position();

      if (previous_pos.x != -1) {
        level_editor
            .grid[(int)previous_pos.y / CELL_SIZE]
                 [(int)previous_pos.x / CELL_SIZE]
            .type = EMPTY;
      }
    }

    EditorGridCell *cell =
        &level_editor.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)];

    cell->type = level_editor.current_entity_type;

    switch (level_editor.current_entity_type) {
    case EMPTY:
    case UBWALL:
    case BWALL: {
      int index = level_editor.get_free_editor_entity(level_editor.walls);
      level_editor.walls[index].free = false;
      level_editor.walls[index].type = BREAKABLE;
      cell->index = index;
      break;
    }
    case PLAYER:
    case BASE_ENEMY:
    case SENTRY_A_ENEMY:
    case WARPZONE:
      break;
    }
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = get_world_mouse(*camera);
    level_editor.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)].type =
        EMPTY;
  }

  camera->target =
      Vector2Add(camera->target, Vector2Add(camera_horizontal_movement,
                                            camera_vertical_movement));
}

void render_mouse_hover_grid(Vector2 mouse) {
  switch (level_editor.current_entity_type) {
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
      TextFormat("ENTITY TYPE: %s",
                 get_entity_type_name(level_editor.current_entity_type));
  DrawText(message, TEXT_POS(1), TEXT_POS(10), 14, RED);
}

void render_hud(Camera2D *camera, Vector2 mouse) {
  render_mouse_position(*camera, mouse);
  render_current_entity_type(*camera);
}

void render_level_editor(Camera2D *camera) {
  Vector2 mouse = get_world_mouse(*camera);

  render_entities();
  render_mouse_hover_grid(mouse);
  render_hud(camera, mouse);
}
