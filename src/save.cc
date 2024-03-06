#include "save.h"
#include "common.h"
#include "config.h"
#include <cstdlib>
#include <raylib.h>
#include <string.h>

void load_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]) {
  int data_size = LEVEL_DATA_SIZE;
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
  char *data = (char *)malloc(LEVEL_FILE_SIZE);

  strncpy((data + LEVEL_NAME_OFFSET), name, LEVEL_NAME_SIZE);

  int *data_section = (int *)(data + LEVEL_DATA_OFFSET);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      *data_section = level[i][j];
      data_section++;
    }
  }

  if (!SaveFileData(name, data, LEVEL_FILE_SIZE)) {
    TraceLog(LOG_ERROR, "Error while saving level data");
    exit(EXIT_FAILURE);
  }

  TraceLog(LOG_INFO, "Level data saved with success");

  free(data);
}
