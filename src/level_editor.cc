#include "level_editor.h"
#include "camera.h"
#include "common.h"
#include "config.h"
#include "draw.h"
#include "entities.h"
#include "level.h"
#include "raygui.h"
#include "save.h"
#include "textures.h"
#include "wall.h"
#include <raylib.h>
#include <raymath.h>

// TODO: Define flexible level structure and editing
// TODO: Add undo action

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

      // Fill entire space with floor texture beforehand
      draw_floor(get_offset_position(x, y));

      EditorGridCell cell = level_editor.grid[y][x];
      draw_editor_entity(cell, get_offset_position(x, y));
    }
  }
}

void handle_editor_actions(Camera2D *camera, int pressed_key) {
  switch (pressed_key) {
  case KEY_ESCAPE:
    level_editor.placing_mode = false;
    level_editor.current_entity = EMPTY_ENTITY;
    break;
  case KEY_M:
    level_editor.save_level();
    break;
  }

  if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) ||
      (IsKeyDown(KEY_LEFT_SHIFT) && IsMouseButtonDown(MOUSE_BUTTON_LEFT))) {
    Vector2 mouse = get_world_mouse(*camera);
    level_editor.place_entity(mouse);
  }

  if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
    Vector2 mouse = get_world_mouse(*camera);

    EditorGridCell *cell =
        &level_editor.grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)];

    cell->entity = EditorVoid{};
    cell->type = EMPTY_ENTITY;
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

void render_hud(Camera2D *camera, Vector2 mouse) {
  render_mouse_position(*camera, mouse);
}

std::string entity_types_str = get_enum_string();
std::string item_effect_str = get_item_effect_enum_string();

const char *dropdown_items = entity_types_str.c_str();
const char *item_effect_dropdown_items = item_effect_str.c_str();

bool item_effect_dropdown_is_open = false;

void render_entity_dropdown() {
  if (!level_editor.can_change_entity)
    return;

  float width = ENTITY_DROPDOWN_WIDTH;
  float height = ENTITY_DROPDOWN_HEIGHT;
  float x = WIN_WIDTH - width;

  if (GuiDropdownBox((Rectangle){x, 0, width, height}, dropdown_items,
                     (int *)&level_editor.current_entity,
                     level_editor.entity_dropdown_is_open)) {

    level_editor.entity_dropdown_is_open =
        !level_editor.entity_dropdown_is_open;

    // Only Item Entity can have sub-dropdown
    if (!level_editor.entity_dropdown_is_open &&
        (level_editor.current_entity != ITEM_ENTITY)) {
      level_editor.placing_mode = true;
    }
  }

  // Sub-dropdowns
  if (level_editor.current_entity == ITEM_ENTITY) {

    float item_effect_height =
        (level_editor.entity_dropdown_is_open) ? (height * (8 + 2)) : (height);

    if (GuiDropdownBox((Rectangle){x, item_effect_height, width, height},
                       item_effect_dropdown_items,
                       (int *)&level_editor.item_params.effect,
                       item_effect_dropdown_is_open)) {

      item_effect_dropdown_is_open = !item_effect_dropdown_is_open;

      if (!level_editor.entity_dropdown_is_open) {
        level_editor.placing_mode = true;
      }
    }
  }
}

void render_level_editor(Camera2D *camera) {
  Vector2 mouse = get_world_mouse(*camera);

  render_entities();
  render_mouse_hover_grid(level_editor.current_entity, mouse);

  render_hud(camera, mouse);
}
