#include "item_drop.h"

BaseItem drop(ItemDrop item_drop, Vector2 position) {
  return create_base_item(item_drop.item_effect, item_drop.item_usage,
                          position);
}
