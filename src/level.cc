#include "level.h"
#include "entities.h"
#include "save.h"

// TODO: Create Separate loaders for the game and the level editor
Level::Level() { this->filename = NULL; }

Level::Level(const char *filename) { this->filename = filename; }

// void Level::load_level_data() { load_level_file(this->filename, grid); }
void Level::load_level_data() {}

void Level::create_level_data() {
  for (int y = 0; y < CELL_COUNT; y++) {
    for (int x = 0; x < CELL_COUNT; x++) {
      this->grid[y][x] = EMPTY;
    }
  }

  // save_level_file(this->filename, this->grid);
  create_level_file(this->filename, "New Level", "Sawcekeeper");
}

void Level::save_level() {}
