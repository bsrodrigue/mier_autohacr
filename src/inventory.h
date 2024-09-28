#include "entities.h"
#include "gate.h"
#include "item.h"
#include "projectiles.h"
#include "wall.h"
#include <raylib.h>

#ifndef INVENTORY_H
#define INVENTORY_H

#define MAX_INVENTORY_ITEMS 10

typedef struct {
  BaseItem items[MAX_INVENTORY_ITEMS];
  int item_count;
  int current_item_index;
} Inventory;

int inventory_add_item(Inventory *inventory, BaseItem item);

int find_iventory_item_by_effect(Inventory inventory, ItemEffect effect);

BaseItem get_item(Inventory *inventory, int index);

void init_inventory(Inventory *inventory);

#endif
