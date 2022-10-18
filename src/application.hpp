#pragma once

// ----------------------------------------------------------------------------
// LOW-LEVEL LIBRARIES
// ----------------------------------------------------------------------------
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>

//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>

// ----------------------------------------------------------------------------
// FRAMEWORK
// ----------------------------------------------------------------------------
//#include "mesh.hpp"
//#include "program.hpp"
//#include "utilities.hpp"
//#include "texture.hpp"

#include "Game/game.hpp"



class GLFWwindow;

class Application {

    GLFWwindow* window;

    size_t width;
    size_t height;

    float lastMouseX = -1;
    float lastMouseY = -1;

    Game game;

public:
    Application(GLFWwindow* win, size_t initial_width, size_t initial_height);
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    void render(GLFWwindow* window, float deltaTime);

    // ----------------------------------------------------------------------------
    // INPUT EVENTS
    // ----------------------------------------------------------------------------
    void on_resize(GLFWwindow* window, int width, int height);
    void on_mouse_move(GLFWwindow* window, double x, double y);
    void on_mouse_pressed(GLFWwindow* window, int button, int action, int mods);
    void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void on_mouse_scroll(GLFWwindow* window, double dx, double dy);

    bool keyHeld(int keyName) const;
    bool mouseHeld(int buttonName) const;

    void lockCursor(bool lock);

private:




    // ----------------------------------------------------------------------------
    // PROGRAMS
    // ----------------------------------------------------------------------------
/*    GLuint draw_lights_program = create_program("shaders/draw_lights.vert", "shaders/draw_lights.frag");
    GLuint draw_object_program = create_program("shaders/draw_object.vert", "shaders/draw_object.frag");
    GLuint draw_object_textured_program = create_program("shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag");
    GLuint postprocess_program = create_program("shaders/postprocess.vert", "shaders/postprocess.frag");
*/
    // ----------------------------------------------------------------------------
    // SCENES/OBJECTS
    // ----------------------------------------------------------------------------
/*    Mesh cube = Mesh::cube();

    std::vector<std::unique_ptr<Mesh>> obj_test_scene = Mesh::from_file("objects/test_scene.obj");
*/
    // This is the default material that can be used when nothing else is.
    // You might want to create material UBOs and textures inside the Mesh class.
/*    ObjectUBO default_object;
    GLuint default_object_buffer = 0;

    Texture default_texture {"images/default.png", false};

    ObjectUBO floor_object;
    GLuint floor_object_buffer = 0;
*/
    //Texture font_texture {"images/font.png", false};

    // ----------------------------------------------------------------------------
    // LIGHTS
    // ----------------------------------------------------------------------------
    /*std::vector<LightUBO> lights;
    GLuint lights_buffer = 0;*/

    // ----------------------------------------------------------------------------
    // POST-PROCESS FRAMEBUFFER
    // ----------------------------------------------------------------------------
    /*GLuint postprocess_framebuffer = 0;
    GLuint postprocess_framebuffer_color = 0;
    GLuint postprocess_framebuffer_depth = 0;*/
};