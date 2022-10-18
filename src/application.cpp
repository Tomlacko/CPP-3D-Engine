#include "application.hpp"

Application::Application(size_t initial_width, size_t initial_height) {
  this->width = initial_width;
  this->height = initial_height;

  // --------------------------------------------------------------------------
  // Initialize Data
  // --------------------------------------------------------------------------
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.projection = glm::perspective(glm::radians(45.0f), float(width) / float(height), 0.01f, 1000.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

  LightUBO directional_light;
  directional_light.position = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
  directional_light.ambient_color = glm::vec4(0.0f);
  directional_light.diffuse_color = glm::vec4(1.0f);
  directional_light.specular_color = glm::vec4(1.0f);
  lights.push_back(directional_light);

  default_object.model_matrix = glm::mat4(1.0f);
  default_object.ambient_color = glm::vec4(0.0f);
  default_object.diffuse_color = glm::vec4(1.0f);
  default_object.specular_color = glm::vec4(0.0f, 0.0f, 0.8f, 8.0f);

  floor_object.model_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(8.0, 0.01f, 8.0f));
  floor_object.ambient_color = glm::vec4(0.0f);
  floor_object.diffuse_color = glm::vec4(1.0f);
  floor_object.specular_color = glm::vec4(1.0f);

  // Scatter lights
  for (int x = -8; x < 8; x += 2) {
    for (int y = -8; y < 8; y += 2) {
      glm::vec3 color = (glm::vec3(x, y, 0.0f) + 8.0f) / 16.0f;
      lights.push_back({
          glm::vec4(x, 0.40f, y, 1.0f), // position
          glm::vec4(0.0),               // ambient
          glm::vec4(color, 1.0f),       // diffuse
          glm::vec4(0.0f)               // specular
      });
    }
  }

  glEnable(GL_CULL_FACE);

  // --------------------------------------------------------------------------
  // Create Buffers
  // --------------------------------------------------------------------------
  glCreateBuffers(1, &camera_buffer);
  glNamedBufferStorage(camera_buffer, sizeof(CameraUBO), &camera_ubo, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &lights_buffer);
  glNamedBufferStorage(lights_buffer, lights.size() * sizeof(LightUBO), lights.data(), GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &default_object_buffer);
  glNamedBufferStorage(default_object_buffer, sizeof(ObjectUBO), &default_object, GL_DYNAMIC_STORAGE_BIT);

  glCreateBuffers(1, &floor_object_buffer);
  glNamedBufferStorage(floor_object_buffer, sizeof(ObjectUBO), &floor_object, GL_DYNAMIC_STORAGE_BIT);

  // --------------------------------------------------------------------------
  // Create Framebuffer
  // --------------------------------------------------------------------------
  glCreateFramebuffers(1, &postprocess_framebuffer);

  // Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_color);
  glTextureStorage2D(postprocess_framebuffer_color, 1, GL_RGBA32F, width, height);

  // Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_depth);
  glTextureStorage2D(postprocess_framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(postprocess_framebuffer, GL_COLOR_ATTACHMENT0, postprocess_framebuffer_color, 0);
  glNamedFramebufferTexture(postprocess_framebuffer, GL_DEPTH_ATTACHMENT, postprocess_framebuffer_depth, 0);
}

Application::~Application() {
  glDeleteTextures(1, &postprocess_framebuffer_depth);
  glDeleteTextures(1, &postprocess_framebuffer_color);
  glDeleteTextures(1, &postprocess_framebuffer);

  glDeleteBuffers(1, &lights_buffer);
  glDeleteBuffers(1, &floor_object_buffer);
  glDeleteBuffers(1, &default_object_buffer);

  glDeleteTextures(1, &default_texture);

  glDeleteProgram(postprocess_program);
  glDeleteProgram(draw_object_textured_program);
  glDeleteProgram(draw_object_program);
  glDeleteProgram(draw_lights_program);

  glDeleteBuffers(1, &camera_buffer);
}

void Application::render() {
  // --------------------------------------------------------------------------
  // Update data
  // --------------------------------------------------------------------------

  // Camera
  camera_ubo.position = glm::vec4(camera.get_eye_position(), 1.0f);
  camera_ubo.view = glm::lookAt(camera.get_eye_position(), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  glNamedBufferSubData(camera_buffer, 0, sizeof(CameraUBO), &camera_ubo);

  // --------------------------------------------------------------------------
  // Draw the scene
  // --------------------------------------------------------------------------

  // Bind the Framebuffer
  glBindFramebuffer(GL_FRAMEBUFFER, postprocess_framebuffer);

  // Clear attachments
  glClearNamedFramebufferfv(postprocess_framebuffer, GL_COLOR, 0, clear_color);
  glClearNamedFramebufferfv(postprocess_framebuffer, GL_DEPTH, 0, clear_depth);

  // Configure fixed function pipeline
  glEnable(GL_DEPTH_TEST);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);

  // Draw floor using the built-in object
  glUseProgram(draw_object_textured_program);
  glBindBufferBase(GL_UNIFORM_BUFFER, 2, floor_object_buffer);
  glBindTextureUnit(0, default_texture);
  cube.draw();

  // Draw several objects loaded through the Mesh class
  glUseProgram(draw_object_program);
  for (auto &mesh : obj_test_scene) {
    glBindBufferBase(GL_UNIFORM_BUFFER, 2, default_object_buffer);
    mesh->draw();
  }

  // Draw lights using Instanced rendering
  glUseProgram(draw_lights_program);

  glBindBufferBase(GL_UNIFORM_BUFFER, 0, camera_buffer);
  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lights_buffer);

  glBindVertexArray(sphere.get_vao());
  glDrawElementsInstanced(sphere.get_mode(), sphere.get_indices_count(), GL_UNSIGNED_INT, nullptr, lights.size());

  // --------------------------------------------------------------------------
  // Apply post-process
  // --------------------------------------------------------------------------

  // Bind back the default framebuffer (0)
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // Clear color buffer
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set fixed function pipeline
  glDisable(GL_DEPTH_TEST); // Disable depth test - we do not need it
  glViewport(0, 0, this->width, this->height);

  // Use post-process program
  glUseProgram(postprocess_program);

  // Bind the output from previous program as input texture to the post-process program
  glBindTextureUnit(0, postprocess_framebuffer_color);
  glBindTextureUnit(1, postprocess_framebuffer_depth);

  // Draw the full-screen triangle
  glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
  this->width = width;
  this->height = height;

  glDeleteTextures(1, &postprocess_framebuffer_color);
  glDeleteTextures(1, &postprocess_framebuffer_depth);

  // Re-Initialize color output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_color);
  glTextureStorage2D(postprocess_framebuffer_color, 1, GL_RGBA32F, width, height);

  // Re-Initialize depth output texture
  glCreateTextures(GL_TEXTURE_2D, 1, &postprocess_framebuffer_depth);
  glTextureStorage2D(postprocess_framebuffer_depth, 1, GL_DEPTH_COMPONENT32F, width, height);

  // Re-Associate color and depth `attachments` with color and depth `textures`
  glNamedFramebufferTexture(postprocess_framebuffer, GL_COLOR_ATTACHMENT0, postprocess_framebuffer_color, 0);
  glNamedFramebufferTexture(postprocess_framebuffer, GL_DEPTH_ATTACHMENT, postprocess_framebuffer_depth, 0);
}

void Application::on_mouse_move(GLFWwindow *window, double x, double y) { camera.on_mouse_move(x, y); }
void Application::on_mouse_pressed(GLFWwindow *window, int button, int action, int mods) { camera.on_mouse_button(button, action, mods); }
void Application::on_key_pressed(GLFWwindow *window, int key, int scancode, int action, int mods) {}