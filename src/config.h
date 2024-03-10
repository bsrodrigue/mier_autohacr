#ifndef CONFIG_H
#define CONFIG_H

#define FPS 60

// TODO: Handle heterogenous screen sizes
#define LEVEL_WIDTH 2500
#define LEVEL_HEIGHT 2500
#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

#define TEXT_POS(x) x * 5

#define CELL_SIZE 25
// TODO: Make Level Cell Count independent of Cell Size
#define CELL_COUNT (int)(LEVEL_WIDTH / CELL_SIZE)
#define CELL_PADDING 0

#define CELL_OFFSET(x) (x * (CELL_SIZE))
#define MOUSE_TO_GRID(x) (int)(x / CELL_SIZE)
#define MOUSE_TO_CIRCLE(x) (float)(x * CELL_SIZE + (int)(CELL_SIZE / 2))

#define BREAKABLE_WALL_HEALTH 1

#endif
