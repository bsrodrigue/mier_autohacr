#include "config.h"

#ifndef SAVE_H
#define SAVE_H

//======== File Format =======//
#define FORMAT_VERSION 0.1
#define LEVEL_NAME_OFFSET 0
#define LEVEL_NAME_SIZE 256
#define LEVEL_DATA_OFFSET (LEVEL_NAME_SIZE)
#define LEVEL_DATA_SIZE (CELL_COUNT * CELL_COUNT) * (sizeof(int))
#define LEVEL_FILE_SIZE (LEVEL_NAME_SIZE + LEVEL_DATA_SIZE)

void load_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]);
void save_level_file(const char *name, int level[CELL_COUNT][CELL_COUNT]);

#endif
