#include "entities.h"

const char *get_entity_type_name(EntityType type) {
  switch (type) {
  case EMPTY:
    return "Empty";
  case UBWALL:
    return "Unbreakable Wall";
  case BWALL:
    return "Breakable Wall";
  case PLAYER:
    return "Player";
  case BASE_ENEMY:
    return "Base Enemy";
  case SENTRY_A_ENEMY:
    return "Sentry A Enemy";
  case WARPZONE:
    return "Warp Zone";
  case ITEM:
    return "Item";
  default:
    return "";
  }
}
