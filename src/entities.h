#include <raylib.h>
#include <vector>

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum {
  EMPTY_ENTITY,
  UBWALL_ENTITY,
  BWALL_ENTITY,
  PLAYER_ENTITY,
  BASE_ENEMY_ENTITY,
  WARPZONE_ENTITY,
  ITEM_ENTITY,
  GATE_ENTITY,
} EntityType;

const char *get_entity_type_name(EntityType type);

#endif
