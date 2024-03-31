#include <raylib.h>
#include <vector>

#ifndef ENTITIES_H
#define ENTITIES_H

typedef enum {
  EMPTY,
  UBWALL,
  BWALL,
  PLAYER,
  BASE_ENEMY,
  SENTRY_A_ENEMY,
  WARPZONE
} EntityType;

const char *get_entity_type_name(EntityType type);

#endif
