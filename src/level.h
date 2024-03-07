#include "config.h"

#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
  int grid[CELL_COUNT][CELL_COUNT];
  Level();
  Level(const char *filename);

  void load_level_data(const char *filename);
};

#endif
