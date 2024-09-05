#include "item.h"
#include <raylib.h>

BaseItem create_base_item(Vector2 position, ItemEffect effect) {
  BaseItem item;

  item.effect = effect;
  item.position = position;
  item.picked = false;

  return item;
}
