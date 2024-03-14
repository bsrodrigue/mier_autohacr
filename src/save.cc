#include "save.h"
#include "common.h"
#include "enemy.h"
#include "entities.h"
#include <raylib.h>
#include <string.h>

void create_level_file(const char *filename, const char *level_name,
                       const char *author_name) {

  unsigned char *header = (unsigned char *)MemAlloc(HEADER_SIZE);

  strncpy((char *)(header + HEADER_NAME_OFFSET), level_name, HEADER_NAME_LEN);
  strncpy((char *)(header + HEADER_AUTHOR_OFFSET), author_name,
          HEADER_AUTHOR_LEN);

  int *entity_counts = (int *)(header + HEADER_TAB_OFFSET);

  for (int i = 0; i < FTYPE_COUNT; i++) {
    entity_counts[i] = 0;
  }

  bool saved = SaveFileData(filename, header, HEADER_SIZE);

  if (!saved) {
    TraceLog(LOG_ERROR, "An error occured while creating empty level file");
  }

  TraceLog(LOG_INFO, "Level File Created");
  MemFree(header);
}
