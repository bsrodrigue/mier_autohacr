#include <raylib.h>

#ifndef ITEM_H
#define ITEM_H

#define MAX_ITEMS 50

typedef enum {
  HEALING,
  SPECIAL

} ItemEffect;

typedef struct {
  ItemEffect effect;
  Vector2 position;
  bool picked;
} BaseItem;

BaseItem create_base_item(Vector2 position, ItemEffect effect);

#endif
