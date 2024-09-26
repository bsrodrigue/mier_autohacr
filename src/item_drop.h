#include "item.h"

#ifndef ITEM_DROP_H
#define ITEM_DROP_H

typedef struct {
  ItemEffect item_effect;
  ItemUsage item_usage;
  int count;

} ItemDrop;

BaseItem drop(ItemDrop item_drop, Vector2 position);

#endif // !ITEM_DROP_H
