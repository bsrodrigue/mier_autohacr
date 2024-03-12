#include "level.h"
#include "entities.h"
#include "save.h"

Level::Level() { this->filename = NULL; }

Level::Level(const char *filename) { this->filename = filename; }

void Level::load_level_data() { load_level_file(this->filename, grid); }

void Level::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x] = EMPTY;
    }
  }

  // save_level_file(this->filename, this->grid);
  create_level_file(this->filename, "New Level", "Sawcekeeper");
}

void Level::save_level() { save_level_file(this->filename, this->grid); }

void LevelEditor::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x].type = EMPTY;
      this->grid[y][x].index = 0;
    }
  }

  create_level_file(this->filename, "New Level", "Sawcekeeper");
}
