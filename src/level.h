#include "config.h"
#include "entities.h"
#include "level_editor.h"
#include "save.h"
#include <vector>

#ifndef LEVEL_H
#define LEVEL_H

class Level {
public:
  EditorGridCell grid[CELL_COUNT][CELL_COUNT];
  const char *filename;

  Level();
  Level(const char *filename);

  void load_level_data();
  void save_level();
};

#endif
