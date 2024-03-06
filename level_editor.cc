#include "common.h"
#include "config.h"
#include "entities.h"
#include "save.h"
#include <raylib.h>
#include <raymath.h>

// TODO: Define flexible level structure and editing
// TODO: Provide simple GUI to switch between entity types

Vector2 get_world_mouse(Camera2D camera) {
  Vector2 mouse = GetMousePosition();
  return GetScreenToWorld2D(mouse, camera);
}

int current_entity_index = 1;
EntityType types[6] = {EMPTY,  BWALL,      UBWALL,
                       PLAYER, BASE_ENEMY, SENTRY_A_ENEMY};
EntityType current_entity_type = types[current_entity_index];

void next_type() {
  if (current_entity_index + 1 >= 6) {
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
  default:
    return "";
  }
}

static int level_grid[CELL_COUNT][CELL_COUNT];

void empty_level() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      level_grid[y][x] = EMPTY;
    }
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

void load_level_editor() {
  // empty_level();
  load_level_file("level.hacc", level_grid);
}

void draw_grid() {
  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(CELL_OFFSET(i), 0, CELL_OFFSET(i), LEVEL_HEIGHT, WHITE);
  }

  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(0, CELL_OFFSET(i), LEVEL_WIDTH, CELL_OFFSET(i), WHITE);
  }
}

void render_entity(EntityType type, Vector2 position) {
  switch (type) {
  case EMPTY:
    break;
  case UBWALL:
    draw_cell(position.x, position.y, WHITE);
    break;
  case BWALL:
    draw_cell(position.x, position.y, RED);
    break;
  case PLAYER:
    draw_cell(position.x, position.y, VIOLET);
    break;
  case BASE_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(RED, 1));
    break;
  case SENTRY_A_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE(position.x), MOUSE_TO_CIRCLE(position.y)}, 10,
                ColorAlpha(PURPLE, 1));
    break;
  }
}

void render_entities() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      int type = level_grid[y][x];
      render_entity((EntityType)type, {(float)x, (float)y});
    }
  }
}

void handle_level_input(Camera2D *camera, int pressed_key) {
  Vector2 camera_movement = {0, 0};
  float camera_velocity = 10;

  switch (pressed_key) {
  case KEY_S:
    save_level_file("level.hacc", level_grid);
    break;
  case KEY_SPACE:
    next_type();
    current_entity_type = types[current_entity_index];
    break;
  }

  // TODO: Allow multi-input camera movement
  if (IsKeyDown(KEY_UP)) {
    camera_movement = {0, -camera_velocity};
  } else if (IsKeyDown(KEY_DOWN)) {
    camera_movement = {0, camera_velocity};
  }

  if (IsKeyDown(KEY_LEFT)) {
    camera_movement = {-camera_velocity, 0};
  } else if (IsKeyDown(KEY_RIGHT)) {
    camera_movement = {camera_velocity, 0};
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse = get_world_mouse(*camera);

    if (current_entity_type == PLAYER) {
      Vector2 previous_pos = get_player_position(level_grid);

      if (previous_pos.x != -1) {
        level_grid[(int)previous_pos.y][(int)previous_pos.x] = EMPTY;
      }
    }

    level_grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] =
        current_entity_type;
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = get_world_mouse(*camera);
    level_grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] = EMPTY;
  }

  camera->target = Vector2Add(camera->target, camera_movement);
}

void render_mouse_hover_grid(Vector2 mouse) {
  switch (current_entity_type) {
  case UBWALL:
    draw_cell(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y),
              ColorAlpha(WHITE, 0.5));
    break;
  case BWALL:
    draw_cell(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y),
              ColorAlpha(RED, 0.5));
    break;
  case PLAYER:
    draw_cell(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y),
              ColorAlpha(VIOLET, 0.5));
    break;
  case BASE_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE((int)(mouse.x / CELL_SIZE)),
                 MOUSE_TO_CIRCLE((int)(mouse.y / CELL_SIZE))},
                10, ColorAlpha(RED, 0.5));
    break;
  case EMPTY:
    break;
  case SENTRY_A_ENEMY:
    DrawCircleV({MOUSE_TO_CIRCLE((int)(mouse.x / CELL_SIZE)),
                 MOUSE_TO_CIRCLE((int)(mouse.y / CELL_SIZE))},
                10, ColorAlpha(PURPLE, 0.5));
    break;
  }
}

void render_mouse_position(Vector2 mouse) {
  const char *message = TextFormat("MOUSE: (%d,%d)", MOUSE_TO_GRID(mouse.x),
                                   MOUSE_TO_GRID(mouse.y));
  DrawText(message, TEXT_POS(1), TEXT_POS(5), 14, RED);
}

void render_current_entity_type() {
  const char *message =
      TextFormat("ENTITY TYPE: %s", get_entity_type_name(current_entity_type));
  DrawText(message, TEXT_POS(1), TEXT_POS(10), 14, RED);
}

void render_hud(Vector2 mouse) {
  render_mouse_position(mouse);
  render_current_entity_type();
}

void render_level_editor(Camera2D *camera) {
  Vector2 mouse = get_world_mouse(*camera);
  draw_grid();
  render_entities();
  render_mouse_hover_grid(mouse);
  render_hud(mouse);
}
