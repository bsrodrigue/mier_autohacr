#include "camera.h"
#include <raymath.h>

void handle_camera_movement(Camera2D *camera) {
  Vector2 camera_vertical_movement = {0, 0};
  Vector2 camera_horizontal_movement = {0, 0};

  if (IsKeyDown(KEY_W)) {
    camera_vertical_movement = {0, -CAMERA_VELOCITY};
  } else if (IsKeyDown(KEY_S)) {
    camera_vertical_movement = {0, CAMERA_VELOCITY};
  }

  if (IsKeyDown(KEY_A)) {
    camera_horizontal_movement = {-CAMERA_VELOCITY, 0};
  } else if (IsKeyDown(KEY_D)) {
    camera_horizontal_movement = {CAMERA_VELOCITY, 0};
  }

  camera->target =
      Vector2Add(camera->target, Vector2Add(camera_horizontal_movement,
                                            camera_vertical_movement));
}
