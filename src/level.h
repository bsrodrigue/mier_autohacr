#include "config.h"

#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
  int grid[CELL_COUNT][CELL_COUNT];
  const char *filename;
  Level();
  Level(const char *filename);

  void load_level_data();
  void create_level_data();
  void save_level();
};

#endif
