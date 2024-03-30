#include "save.h"
#include "common.h"
#include "config.h"
#include "enemy.h"
#include "entities.h"
#include <raylib.h>
#include <string.h>

void create_level_file(const char *filename, const char *level_name,
                       const char *author_name) {

  unsigned char *file = (unsigned char *)MemAlloc(LEVEL_FILE_SIZE);

  strncpy((char *)(file + HEADER_NAME_OFFSET), level_name, HEADER_NAME_LEN);
  strncpy((char *)(file + HEADER_AUTHOR_OFFSET), author_name,
          HEADER_AUTHOR_LEN);

  EditorGridCell *cells = (EditorGridCell *)(file + DATA_OFFSET);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      cells->type = EMPTY;
      cells++;
    }
  }

  bool saved = SaveFileData(filename, file, LEVEL_FILE_SIZE);

  if (!saved) {
    TraceLog(LOG_ERROR, "An error occured while creating fresh level file");
  }

  TraceLog(LOG_INFO, "Level File Created");
  MemFree(file);
}

void load_level_file(const char *filename, EditorGridCell (*grid)[100]) {
  int data_size = LEVEL_FILE_SIZE;

  unsigned char *loaded_data = LoadFileData(filename, &data_size);

  if (loaded_data == NULL) {
    return;
  }

  EditorGridCell *cells = (EditorGridCell *)(loaded_data + DATA_OFFSET);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      grid[i][j] = *cells;
      cells++;
    }
  }
}

void save_level_file(const char *filename, EditorGridCell (*grid)[100]) {
  unsigned char *file = (unsigned char *)MemAlloc(LEVEL_FILE_SIZE);

  EditorGridCell *cells = (EditorGridCell *)(file + DATA_OFFSET);

  for (int i = 0; i < CELL_COUNT; i++) {
    for (int j = 0; j < CELL_COUNT; j++) {
      memcpy(cells, &grid[i][j], sizeof(EditorGridCell));
      cells++;
    }
  }

  bool saved = SaveFileData(filename, file, LEVEL_FILE_SIZE);

  if (!saved) {
    TraceLog(LOG_ERROR, "An error occured while saving level file");
  }

  TraceLog(LOG_INFO, "Level File Saved");
  MemFree(file);
}
