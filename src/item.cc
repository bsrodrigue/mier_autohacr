#include "item.h"
#include <raylib.h>

BaseItem create_base_item(ItemEffect effect, ItemUsage usage,
                          Vector2 position) {
  BaseItem item;

  item.effect = effect;
  item.usage = usage;

  item.position = position;
  item.picked = false;

  return item;
}
