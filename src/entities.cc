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
  case WARPZONE:
    return "Warp Zone";
  case ITEM:
    return "Item";
  case GATE:
    return "GATE";
  default:
    return "";
  }
}
