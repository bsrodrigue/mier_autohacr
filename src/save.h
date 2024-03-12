#include "enemy.h"
#include "wall.h"
#include <raylib.h>

#ifndef SAVE_H
#define SAVE_H

//======== File Format =======//
#define FORMAT_VERSION 0.2
#define LEN_SIZE 4
#define OFFSET_SIZE 4

typedef struct {
  Vector2 position;
} FilePlayer;

typedef struct {
  Vector2 position;
  EnemyType type;
} FileEnemy;

typedef struct {
  Vector2 position;
  WallType type;
} FileWall;

typedef struct {
  Vector2 position;
  Vector2 destination;
} FileWarpzone;

#define MAX_FPLAYERS 1
#define MAX_FENEMIES 100
#define MAX_FWALLS 10000
#define MAX_FWARPZONES 100

#define FTYPE_COUNT 4

// HEADER
#define HEADER_NAME_OFFSET 0
#define HEADER_NAME_LEN 256

#define HEADER_AUTHOR_OFFSET 256
#define HEADER_AUTHOR_LEN 256

#define HEADER_SIZE ((256 * 2) + (sizeof(int) * FTYPE_COUNT))
#define HEADER_TAB_OFFSET (256 * 2)

#define DATA_OFFSET HEADER_SIZE

// HEADER TABLE
// HEADER TABLE -> ACTORS
#define HEADER_TAB_PLAYER_OFFSET (DATA_OFFSET + 0)
#define HEADER_TAB_PLAYER_LEN LEN_SIZE

#define HEADER_TAB_ENEMIES_OFFSET                                              \
  (HEADER_TAB_PLAYER_OFFSET + (sizeof(FilePlayer) * MAX_FPLAYERS))
#define HEADER_TAB_ENEMIES_LEN LEN_SIZE

// HEADER TABLE -> OBSTACLES
#define HEADER_TAB_WALLS_OFFSET                                                \
  (HEADER_TAB_ENEMIES_OFFSET + (sizeof(FileEnemy) * MAX_FENEMIES))
#define HEADER_TAB_WALLS_LEN LEN_SIZE

// HEADER TABLE -> SPECIAL
#define HEADER_TAB_WARPZONES_OFFSET                                            \
  (HEADER_TAB_WALLS_OFFSET + (sizeof(FileWall) * MAX_FWALLS))
#define HEADER_TAB_WARPZONES_LEN LEN_SIZE

// Legacy
void load_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]);
void save_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]);

// New
void create_level_file(const char *filename, const char *level_name,
                       const char *author_name);

#endif
