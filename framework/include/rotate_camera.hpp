#pragma once

#include <glm/glm.hpp>

/// This is a VERY SIMPLE class that allows to very simply move with the camera.
/// It is not a perfect, brilliant, smart, or whatever implementation of a camera,
/// but it is sufficient for PV112 lectures.
///
/// Use left mouse button to change the point of view.
/// Use right mouse button to zoom in and zoom out.
class RotateCamera {
private:
  /// Constants that defines the behaviour of the camera
  ///		- Minimum elevation in radians
  static const float min_elevation;
  ///		- Maximum elevation in radians
  static const float max_elevation;
  ///		- Minimum distance from the point of interest
  static const float min_distance;
  ///		- Sensitivity of the mouse when changing elevation or direction angles
  static const float angle_sensitivity;
  ///		- Sensitivity of the mouse when changing zoom
  static const float zoom_sensitivity;

  /// angle_direction is an angle in which determines into which direction in xz plane I look.
  ///		- 0 degrees .. I look in -z direction
  ///		- 90 degrees .. I look in -x direction
  ///		- 180 degrees .. I look in +z direction
  ///		- 270 degrees .. I look in +x direction
  float angle_direction;

  /// angle_direction is an angle in which determines from which "height" I look.
  ///		- positive elevation .. I look from above the xz plane
  ///		- negative elevation .. I look from below the xz plane
  float angle_elevation;

  /// Distance from (0,0,0), the point at which I look
  float distance;

  /// Final position of the eye in world space coordinates, for LookAt or shaders
  glm::vec3 eye_position;

  /// Last X and Y coordinates of the mouse cursor
  int last_x, last_y;

  /// True or false if moused buttons are pressed and the user rotates/zooms the camera
  bool is_rotating, is_zooming;

  /// Recomputes 'eye_position' from 'angle_direction', 'angle_elevation', and 'distance'
  void update_eye_pos();

public:
  RotateCamera();

  /// Call when the user presses or releases a mouse button (see glfwSetMouseButtonCallback)
  void on_mouse_button(int button, int action, int mods);

  /// Call when the user moves with the mouse cursor (see glfwSetCursorPosCallback)
  void on_mouse_move(double x, double y);

  /// Returns the position of the eye in world space coordinates
  glm::vec3 get_eye_position() const;
};
