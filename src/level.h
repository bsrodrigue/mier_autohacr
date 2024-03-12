#include "config.h"
#include "entities.h"
#include "save.h"
#include <vector>

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

typedef struct {
  EntityType type;
  int index;
} EditorGridCell;

class LevelEditor {
public:
  EditorGridCell grid[CELL_COUNT][CELL_COUNT];
  FilePlayer player;
  std::vector<FileEnemy> enemies;
  std::vector<FileWall> walls;
  std::vector<FileWarpzone> warpzones;

  const char *filename;
  LevelEditor();
  LevelEditor(const char *filename);

  void load_level_data();
  void create_level_data();
  void save_level();
};

#endif
