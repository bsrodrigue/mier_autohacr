#include "item.h"
#include <raylib.h>

BaseItem create_base_item(ItemEffect effect, ItemUsage usage,
                          ItemTexture texture, Vector2 position) {
  BaseItem item;

  item.effect = effect;
  item.usage = usage;
  item.texture = texture;

  item.position = position;
  item.picked = false;
  item.used = false;

  return item;
}
