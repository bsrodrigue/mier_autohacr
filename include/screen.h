#ifndef SCREEN_H
#define SCREEN_H

typedef enum {
  UNKNOWN = -1,
  MENU = 0,
  GAME,
  LEVEL_EDITOR,
} Screen;

#define DEFAULT_SCREEN GAME

class ScreenManager {
public:
  Screen active_screen;
  Screen previous_screen;

  ScreenManager();

  bool screen_changed();

  void init_game_screen();
  void init_level_editor_screen(const char *filename);

  void handle_screen_change();

  void set_screen(Screen screen);
};

#endif
