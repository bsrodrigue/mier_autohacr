#include "level.h"
#include "save.h"

Level::Level(const char *filename) { load_level_file(filename, grid); }
Level::Level() {}
void Level::load_level_data(const char *filename) {
  load_level_file(filename, grid);
}
