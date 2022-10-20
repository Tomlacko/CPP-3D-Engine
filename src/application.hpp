#pragma once


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


    void lockCursor(bool lock);

    bool keyHeld(int keyName) const;
    bool mouseHeld(int buttonName) const;

    // ----------------------------------------------------------------------------
    // INPUT EVENTS
    // ----------------------------------------------------------------------------
    void on_resize(GLFWwindow* window, int width, int height);
    void on_mouse_move(GLFWwindow* window, double x, double y);
    void on_mouse_pressed(GLFWwindow* window, int button, int action, int mods);
    void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
    void on_mouse_scroll(GLFWwindow* window, double dx, double dy);
};