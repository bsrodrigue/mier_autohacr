#include "entities.h"

#ifndef PLAYER_H
#define PLAYER_H
void handle_player_movement(GameObject *player, float velocity,
                            std::vector<Wall> walls);
#endif
