#include "entities.h"

const char *get_entity_type_name(EntityType type) {
  switch (type) {
  case EMPTY_ENTITY:
    return "Empty";
  case UBWALL_ENTITY:
    return "Unbreakable Wall";
  case BWALL_ENTITY:
    return "Breakable Wall";
  case PLAYER_ENTITY:
    return "Player";
  case BASE_ENEMY_ENTITY:
    return "Base Enemy";
  case WARPZONE_ENTITY:
    return "Warp Zone";
  case ITEM_ENTITY:
    return "Item";
  case GATE_ENTITY:
    return "GATE";
  default:
    return "";
  }
}
