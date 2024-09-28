#include "level_editor.h"
#include "save.h"

LevelEditor::LevelEditor() = default;

void LevelEditor::save_level() { save_level_file(this->filename, this->grid); }

void LevelEditor::load_level_data() {
  load_level_file(this->filename, this->grid);
}

void LevelEditor::create_level_data() {
  // TODO: Get user input for level name and author.
  create_level_file(this->filename, "New Level", "Sawcekeeper");
}

Vector2 LevelEditor::get_player_position() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      EntityType type = this->grid[y][x].type;
      if (type == PLAYER_ENTITY)
        return {(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)};
    }
  }

  return {-1, -1};
}

void LevelEditor::place_entity(Vector2 mouse) {
  if (!placing_mode)
    return;

  // Check out of bounds
  if (MOUSE_TO_GRID(mouse.x) < 0 || MOUSE_TO_GRID(mouse.x) >= CELL_COUNT ||
      MOUSE_TO_GRID(mouse.y) < 0 || MOUSE_TO_GRID(mouse.y) >= CELL_COUNT) {
    return;
  }

  EntityType type = this->current_entity;

  if (type == PLAYER_ENTITY) {
    Vector2 previous_pos = this->get_player_position();

    if (previous_pos.x != -1) {
      EditorGridCell *previous_cell =
          &this->grid[(int)previous_pos.y / CELL_SIZE]
                     [(int)previous_pos.x / CELL_SIZE];

      previous_cell->type = EMPTY_ENTITY;
      previous_cell->entity = EditorVoid{};
    }
  }

  if (type == WARPZONE_ENTITY && !this->origin_warpzone_placed) {
    this->origin_warpzone_placed = true;
    this->can_change_entity = false;
  }

  if (type == WARPZONE_ENTITY && this->origin_warpzone_placed) {
    this->origin_warpzone_placed = false;
    this->can_change_entity = true;
  }

  EditorGridCell *cell =
      &this->grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)];

  cell->type = type;

  switch (cell->type) {
  case EMPTY_ENTITY:
    cell->entity = EditorVoid{};
    break;
  case PLAYER_ENTITY:
    cell->entity = EditorPlayer{};
    break;
  case WARPZONE_ENTITY:
    cell->entity = EditorWarpzone{mouse};
    break;
  case ITEM_ENTITY:
    cell->entity = item_params;
  case GATE_ENTITY:
    break;
  case UBWALL_ENTITY:
    cell->entity = EditorWall{UNBREAKABLE_WALL};
    break;
  case BWALL_ENTITY:
    cell->entity = EditorWall{BREAKABLE_WALL};
    break;
  case BASE_ENEMY_ENTITY:
    cell->entity = EditorEnemy{BASE_ENEMY};
    break;
  }
}
