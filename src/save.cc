#include "save.h"
#include "common.h"
#include "config.h"
#include "entities.h"
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <string.h>

typedef struct {
  char name[256];
  char author[256];

  size_t wall_count;
  size_t enemy_count;
  size_t warpzone_count;
  size_t player_count;
  size_t item_count;

} LevelHeader;

typedef struct {
  WallType type;
  Vector2 position;
} WallData;

typedef struct {
  WallData walls_data[256];
} LevelData;

class LevelFileManager {
public:
  const char *filename;
  LevelHeader level_header;

  LevelFileManager(const char *filename) { this->filename = filename; }
};

void load_level_header(const char *filename, LevelHeader *header) {
  int data_size = LEVEL_HEADER_SIZE;
  unsigned char *loaded_data = LoadFileData(filename, &data_size);

  if (loaded_data == NULL) {
    return;
  }

  char *level_name = (char *)(loaded_data + LEVEL_HEADER_NAME_OFFSET);
  char *level_author = (char *)(loaded_data + LEVEL_HEADER_AUTHOR_OFFSET);

  int *walls = (int *)(loaded_data + LEVEL_HEADER_WALLS_OFFSET);
  int *enemies = (int *)(loaded_data + LEVEL_HEADER_ENEMIES_OFFSET);
  int *warpzones = (int *)(loaded_data + LEVEL_HEADER_WARPZONES_OFFSET);
  int *player = (int *)(loaded_data + LEVEL_HEADER_PLAYER_OFFSET);
  int *items = (int *)(loaded_data + LEVEL_HEADER_ITEMS_OFFSET);

  // Fill Level Header Structure
  strncpy(header->name, level_name, 256);
  strncpy(header->author, level_author, 256);

  header->wall_count = walls[1];
  header->enemy_count = enemies[1];
  header->warpzone_count = warpzones[1];
  header->player_count = player[1];
  header->item_count = items[1];
}

void save_level_header(const char *filename, LevelHeader *header) {
  unsigned char *file_header = (unsigned char *)MemAlloc(LEVEL_HEADER_SIZE);

  if (file_header == NULL) {
    return;
  }

  strncpy((char *)(file_header + LEVEL_HEADER_NAME_OFFSET), header->name, 256);
  strncpy((char *)(file_header + LEVEL_HEADER_AUTHOR_OFFSET), header->author,
          256);

  int *file_header_metadata = (int *)(file_header + LEVEL_HEADER_WALLS_OFFSET);
  (file_header_metadata + 0)[1] = header->wall_count;
  (file_header_metadata + 2)[1] = header->enemy_count;
  (file_header_metadata + 4)[1] = header->warpzone_count;
  (file_header_metadata + 6)[1] = header->player_count;
  (file_header_metadata + 8)[1] = header->item_count;
}

void load_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]) {
  int data_size = 100 * 100 * 4;
  unsigned char *loaded_data = LoadFileData(name, &data_size);

  if (loaded_data == NULL) {
    return;
  }

  TraceLog(LOG_INFO, "Level data loaded with success");

  int *data_section = (int *)(loaded_data + LEVEL_DATA_OFFSET);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      level[i][j] = *data_section;
      data_section++;
    }
  }
}

void save_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]) {
  char *data = (char *)malloc((256) + (100 * 100 * 4));

  strncpy((data + 0), name, 256);

  int *data_section = (int *)(data + 256);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      *data_section = level[i][j];
      data_section++;
    }
  }

  if (!SaveFileData(name, data, 256 + (100 * 100 * 4))) {
    TraceLog(LOG_ERROR, "Error while saving level data");
    exit(EXIT_FAILURE);
  }

  TraceLog(LOG_INFO, "Level data saved with success");

  free(data);
}
