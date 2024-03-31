#include "level.h"
#include "entities.h"
#include "save.h"

// TODO: Create Separate loaders for the game and the level editor
Level::Level() { this->filename = NULL; }

Level::Level(const char *filename) { this->filename = filename; }

void Level::load_level_data() { load_level_file(this->filename, this->grid); }

void Level::save_level() {}
