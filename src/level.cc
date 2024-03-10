#include "level.h"
#include "entities.h"
#include "save.h"

Level::Level(const char *filename) { this->filename = filename; }
Level::Level() { this->filename = NULL; }

void Level::load_level_data() { load_level_file(this->filename, grid); }

void Level::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x] = EMPTY;
    }
  }

  save_level_file(this->filename, this->grid);
}

void Level::save_level() { save_level_file(this->filename, this->grid); }
