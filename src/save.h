#include "config.h"
#include "level_editor.h"
#include <raylib.h>

#ifndef SAVE_H
#define SAVE_H

//======== File Format =======//
#define FORMAT_VERSION 0.2
#define LEN_SIZE 4
#define OFFSET_SIZE 4

//-----------------------------------------
// NAME [256]
//-----------------------------------------
// AUTHOR [256]
//-----------------------------------------
// DATA [100][100] * sizeof(EditorGridCell)
//-----------------------------------------

// HEADER
#define HEADER_NAME_OFFSET 0
#define HEADER_NAME_LEN 256

#define HEADER_AUTHOR_OFFSET 256
#define HEADER_AUTHOR_LEN 256

#define HEADER_SIZE (256 * 2)

#define DATA_OFFSET HEADER_SIZE

#define LEVEL_FILE_SIZE                                                        \
  ((HEADER_SIZE) + (CELL_COUNT * CELL_COUNT) * sizeof(EditorGridCell))

//------------------------ Level Files
void load_level_file(const char *filename,
                     EditorGridCell grid[CELL_COUNT][CELL_COUNT]);

void save_level_file(const char *filename,
                     EditorGridCell grid[CELL_COUNT][CELL_COUNT]);

// New
void create_level_file(const char *filename, const char *level_name,
                       const char *author_name);

#endif
