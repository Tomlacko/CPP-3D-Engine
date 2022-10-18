#include "rotate_camera.hpp"

#include <GLFW/glfw3.h>

const float RotateCamera::min_elevation = -1.5f;
const float RotateCamera::max_elevation = 1.5f;
const float RotateCamera::min_distance = 1.0f;
const float RotateCamera::angle_sensitivity = 0.008f;
const float RotateCamera::zoom_sensitivity = 0.003f;

RotateCamera::RotateCamera()
    : angle_direction(0.0f), angle_elevation(0.785398163f), distance(15.0f), last_x(0), last_y(0), is_rotating(false),
      is_zooming(false) {
  update_eye_pos();
}

void RotateCamera::update_eye_pos() {
  eye_position.x = distance * cosf(angle_elevation) * -sinf(angle_direction);
  eye_position.y = distance * sinf(angle_elevation);
  eye_position.z = distance * cosf(angle_elevation) * cosf(angle_direction);
}

void RotateCamera::on_mouse_button(int button, int action, int mods) {
  // Left mouse button affects the angles
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    if (action == GLFW_PRESS) {
      is_rotating = true;
    } else {
      is_rotating = false;
    }
  }
  // Right mouse button affects the zoom
  if (button == GLFW_MOUSE_BUTTON_RIGHT) {
    if (action == GLFW_PRESS) {
      is_zooming = true;
    } else {
      is_zooming = false;
    }
  }
}

void RotateCamera::on_mouse_move(double x, double y) {
  float dx = float(x - last_x);
  float dy = float(y - last_y);
  last_x = static_cast<int>(x);
  last_y = static_cast<int>(y);

  if (is_rotating) {
    angle_direction += dx * angle_sensitivity;
    angle_elevation += dy * angle_sensitivity;

    // Clamp the results
    if (angle_elevation > max_elevation)
      angle_elevation = max_elevation;
    if (angle_elevation < min_elevation)
      angle_elevation = min_elevation;
  }
  if (is_zooming) {
    distance *= (1.0f + dy * zoom_sensitivity);

    // Clamp the results
    if (distance < min_distance)
      distance = min_distance;
  }
  update_eye_pos();
}

glm::vec3 RotateCamera::get_eye_position() const { return eye_position; }
