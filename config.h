#ifndef CONFIG_H
#define CONFIG_H

#define FPS 60

// TODO: Handle heterogenous screen sizes
#define WIN_WIDTH 800
#define WIN_HEIGHT 800

#define TEXT_POS(x) x * 5

#define CELL_COUNT 25
#define CELL_SIZE (int)(WIN_HEIGHT / CELL_COUNT)
#define CELL_PADDING 5

#define CELL_OFFSET(x) (x * (CELL_SIZE))
#define MOUSE_TO_GRID(x) (int)(x / CELL_SIZE)

#define BREAKABLE_WALL_HEALTH 5

typedef enum {
  UNKNOWN = -1,
  MENU = 0,
  GAME,
  LEVEL_EDITOR,
} Screen;

#endif
