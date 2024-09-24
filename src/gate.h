#include "config.h"
#include "entities.h"
#include <raylib.h>
#include <vector>

#ifndef GATE_H
#define GATE_H

#define MAX_GATES 50

typedef enum { BASE_GATE } GateType;

typedef struct {
  GateType type;
  bool opened;
  Vector2 position;
} Gate;
#endif
