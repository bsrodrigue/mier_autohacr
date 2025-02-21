#include "screen.h"

ScreenManager::ScreenManager() {
  this->active_screen = DEFAULT_SCREEN;
  this->previous_screen = UNKNOWN;
}

// This will always trigger on startup
bool ScreenManager::screen_changed() {
  return active_screen != previous_screen;
}

void ScreenManager::set_screen(Screen screen) { this->active_screen = screen; }
