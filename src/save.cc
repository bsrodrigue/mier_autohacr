#include "save.h"
#include "common.h"
#include "config.h"
#include "enemy.h"
#include "entities.h"
#include <cstddef>
#include <cstdlib>
#include <raylib.h>
#include <string.h>

void create_level_file(const char *filename, const char *level_name,
                       const char *author_name) {

  char *header = (char *)MemAlloc(HEADER_SIZE);

  strncpy((header + HEADER_NAME_OFFSET), level_name, HEADER_NAME_LEN);
  strncpy((header + HEADER_AUTHOR_OFFSET), author_name, HEADER_AUTHOR_LEN);

  int *entity_counts = (int *)(header + HEADER_TAB_OFFSET);

  for (int i = 0; i < FTYPE_COUNT; i++) {
    entity_counts[i] = 0;
  }

  bool saved = SaveFileData(filename, &header, HEADER_SIZE);

  if (!saved) {
    TraceLog(LOG_ERROR, "An error occured while creating empty level file");
  }
}

// Legacy
void load_level_file_legacy(const char *name,
                            int level[CELL_COUNT][CELL_COUNT]) {
  return;
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
