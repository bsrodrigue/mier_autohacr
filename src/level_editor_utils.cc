#include "level_editor.h"
#include "save.h"

LevelEditor::LevelEditor() {}

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

void LevelEditor::next_type() {
  if (!this->can_change_entity) {
    // TODO: Handle this situation.
    return;
  }

  bool reached_end = current_entity_index + 1 >= TYPE_COUNT;
  current_entity_index = reached_end ? 0 : (current_entity_index + 1);
}

template <typename T> int LevelEditor::get_free_editor_entity(T entities[100]) {
  for (int i = 0; i < 100; i++) {
    if (entities[i].free)
      return i;
  }

  return -1;
}

void LevelEditor::place_entity(Vector2 mouse) {
  // Out of bounds
  if (MOUSE_TO_GRID(mouse.x) >= CELL_COUNT ||
      MOUSE_TO_GRID(mouse.y) >= CELL_COUNT) {
    return;
  }

  EntityType type = this->types[this->current_entity_index];

  if (type == PLAYER_ENTITY) {
    Vector2 previous_pos = this->get_player_position();

    if (previous_pos.x != -1) {
      this->grid[(int)previous_pos.y / CELL_SIZE]
                [(int)previous_pos.x / CELL_SIZE]
                    .type = EMPTY_ENTITY;
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
  case PLAYER_ENTITY:
  case WARPZONE_ENTITY:
  case ITEM_ENTITY:
  case GATE_ENTITY:
    break;
  case UBWALL_ENTITY:
    cell->wall.type = UNBREAKABLE_WALL;
    break;
  case BWALL_ENTITY:
    cell->wall.type = BREAKABLE_WALL;
    break;
  case BASE_ENEMY_ENTITY:
    cell->enemy.type = BASE_ENEMY;
    break;
  }
}
