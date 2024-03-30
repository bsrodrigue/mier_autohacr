#include "level_editor.h"

LevelEditor::LevelEditor() {}
void LevelEditor::save_level() {}
void LevelEditor::load_level_data() {}

void LevelEditor::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x].type = EMPTY;
      this->grid[y][x].index = 0;
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

void LevelEditor::place_entity(EditorGridCell *cell, EntityType type) {
  int index;

  switch (type) {
  case EMPTY:
    index = this->get_free_editor_entity(walls);

    break;
  case UBWALL:
    index = this->get_free_editor_entity(walls);
    walls[index].free = false;
    walls[index].type = UNBREAKABLE;
    break;
  case BWALL:
    index = this->get_free_editor_entity(walls);
    walls[index].free = false;
    walls[index].type = BREAKABLE;
    break;
  case PLAYER:
    index = this->get_free_editor_entity(walls);
    break;
  case BASE_ENEMY:
    index = this->get_free_editor_entity(enemies);
    break;
  case SENTRY_A_ENEMY:
    index = this->get_free_editor_entity(enemies);
    break;
  case WARPZONE:
    index = this->get_free_editor_entity(walls);
    break;
  }

  cell->index = index;
  cell->type = type;
}
