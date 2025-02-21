#include "inventory.h"

void init_inventory(Inventory *inventory) {
  inventory->item_count = 0;
  inventory->current_item_index = 0;

  for (int i = 0; i < MAX_INVENTORY_ITEMS; i++) {
    inventory->items[i] =
        create_base_item(NO_EFFECT, NO_USAGE, NO_TEXTURE, {0, 0});
  }
}

int inventory_add_item(Inventory *inventory, BaseItem item) {
  if (inventory->item_count + 1 > MAX_INVENTORY_ITEMS) {
    // TODO: Handle full inventory
    return -1;
  }

  inventory->items[inventory->item_count++] = item;

  return inventory->item_count;
}

int find_iventory_item_by_effect(Inventory inventory, ItemEffect effect) {
  for (int i = 0; i < inventory.item_count; i++) {
    if (inventory.items[i].effect == effect)
      return i;
  }

  return -1;
}
