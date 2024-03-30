#include "level_editor.h"

LevelEditor::LevelEditor() {}
void LevelEditor::save_level() {}
void LevelEditor::load_level_data() {}

void LevelEditor::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x].type = EMPTY;
    }
  }

  create_level_file(this->filename, "New Level", "Sawcekeeper");
}

Vector2 LevelEditor::get_player_position() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      EntityType type = this->grid[y][x].type;
      if (type == PLAYER)
        return {(float)(x * CELL_SIZE), (float)(y * CELL_SIZE)};
    }
  }

  return {-1, -1};
}

void LevelEditor::next_type() {
  if (current_entity_index + 1 >= TYPE_COUNT) {
    current_entity_index = 0;
  }

  else {

    current_entity_index++;
  }
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

  if (this->types[this->current_entity_index] == PLAYER) {
    Vector2 previous_pos = this->get_player_position();

    if (previous_pos.x != -1) {
      this->grid[(int)previous_pos.y / CELL_SIZE]
                [(int)previous_pos.x / CELL_SIZE]
                    .type = EMPTY;
    }
  }

  EditorGridCell *cell =
      &this->grid[MOUSE_TO_GRID(mouse.y)][MOUSE_TO_GRID(mouse.x)];

  cell->type = this->types[this->current_entity_index];

  switch (cell->type) {
  case EMPTY:
  case PLAYER:
    break;
  case UBWALL:
    cell->wall.type = UNBREAKABLE;
    break;
  case BWALL:
    cell->wall.type = BREAKABLE;
    break;
  case BASE_ENEMY:
    cell->enemy.type = BASE;
    break;
  case SENTRY_A_ENEMY:
    cell->enemy.type = SENTRY_A;
    break;
  case WARPZONE:
    // TODO: Handle warpzone
    break;
  }
}
