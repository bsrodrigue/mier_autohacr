#include "common.h"
#include "config.h"
#include "entities.h"
#include "save.h"
#include <raylib.h>
#include <raymath.h>

EntityType current_entity_type = BWALL;

static int level_grid[CELL_COUNT][CELL_COUNT];

void empty_level() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      level_grid[y][x] = EMPTY;
    }
  }
}

void load_level_editor() { load_level_file("level.hacc", level_grid); }

void draw_grid() {
  for (int i = 0; i < CELL_COUNT + 1; i++) {
    DrawLine(CELL_OFFSET(i), 0, CELL_OFFSET(i), WIN_HEIGHT, WHITE);
  }

  for (int i = 0; i < CELL_COUNT; i++) {
    DrawLine(0, CELL_OFFSET(i), WIN_WIDTH, CELL_OFFSET(i), WHITE);
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
    break;
  case ENEMY:
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

void handle_level_input(int pressed_key) {
  switch (pressed_key) {
  case KEY_S:
    save_level_file("level.hacc", level_grid);
    break;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    Vector2 mouse = GetMousePosition();
    level_grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] =
        current_entity_type;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = GetMousePosition();
    level_grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)] = EMPTY;
  }
}

void render_mouse_hover_grid(Vector2 mouse) {
  switch (current_entity_type) {
  case UBWALL:
    draw_cell(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y), WHITE);
    break;
  case BWALL:
    draw_cell(MOUSE_TO_GRID(mouse.x), MOUSE_TO_GRID(mouse.y), RED);
    break;
  case PLAYER:
    break;
  case ENEMY:
    break;
  case EMPTY:
    break;
  }
}

void render_mouse_position(Vector2 mouse) {
  const char *message = TextFormat("MOUSE: (%d,%d)", MOUSE_TO_GRID(mouse.x),
                                   MOUSE_TO_GRID(mouse.y));
  DrawText(message, TEXT_POS(1), TEXT_POS(5), 14, RED);
}

void render_current_entity_type() {
  const char *message = TextFormat("ENTITY TYPE: UBWALL");
  DrawText(message, TEXT_POS(1), TEXT_POS(10), 14, RED);
}

void render_hud(Vector2 mouse) {
  render_mouse_position(mouse);
  render_current_entity_type();
}

void render_level_editor() {
  Vector2 mouse = GetMousePosition();
  draw_grid();
  render_entities();
  render_mouse_hover_grid(mouse);

  render_hud(mouse);
}
