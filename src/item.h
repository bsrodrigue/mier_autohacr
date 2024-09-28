#include <raylib.h>
#include <string>

#ifndef ITEM_H
#define ITEM_H

#define MAX_ITEMS 50

// Premade (Static) Items
typedef enum {
  NO_TYPE_ITEM,
  // Healing
  BASE_HEALING_ITEM,
  MEDIUM_HEAING_ITEM,
  ADVANCED_HEALING_ITEM,

  // Key
  BASE_GATE_KEY_ITEM,
} ItemType;

typedef enum {
  NO_TEXTURE,
  HEALING_CHIP_TEXTURE,
  KEY_TEXTURE,
} ItemTexture;

typedef enum {
  NO_EFFECT,
  HEALING_EFFECT,
  PROJECTILE_BOOST_EFFECT,
  KEY_EFFECT,
  SPECIAL_EFFECT,
} ItemEffect;

typedef enum {
  NO_USAGE,
  INSTANT_USAGE,
  INVENTORY_USAGE,
  GATE_UNLOCKING_USAGE,
  SPECIAL_USAGE,
} ItemUsage;

typedef struct {
  ItemEffect effect;
  ItemUsage usage;
  ItemTexture texture;
  Vector2 position;
  bool picked;
  bool used;
} BaseItem;

BaseItem create_base_item(ItemEffect effect, ItemUsage usage,
                          ItemTexture texture, Vector2 position);

BaseItem create_static_item(ItemType type, Vector2 position);

std::string get_item_effect_enum_string(const char delimiter = ';');
#endif
