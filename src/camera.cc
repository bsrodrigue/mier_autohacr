#include "camera.h"
#include <raymath.h>

void handle_camera_movement(Camera2D *camera) {
  Vector2 camera_movement = {0, 0};

  if (IsKeyDown(KEY_W)) {
    camera_movement.y -= CAMERA_VELOCITY;
  }

  if (IsKeyDown(KEY_S)) {
    camera_movement.y += CAMERA_VELOCITY;
  }

  if (IsKeyDown(KEY_A)) {
    camera_movement.x -= CAMERA_VELOCITY;
  }

  if (IsKeyDown(KEY_D)) {
    camera_movement.x += CAMERA_VELOCITY;
  }

  camera->target = Vector2Add(camera->target, camera_movement);
}
