#include "config.h"
#include <raylib.h>

void draw_cell(float x, float y, Color color) {
  DrawRectangle(CELL_OFFSET(x), CELL_OFFSET(y), CELL_SIZE - CELL_PADDING,
                CELL_SIZE - CELL_PADDING, color);
}

void draw_hover_cell(float x, float y, Color color) {
  DrawRectangle(x / CELL_SIZE, y / CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}
