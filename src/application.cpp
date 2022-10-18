#include "application.hpp"
#include <GLFW/glfw3.h>



Application::Application(GLFWwindow* win, size_t initial_width, size_t initial_height)
:window(win), width(initial_width), height(initial_height), game(this, initial_width, initial_height) {
}


void Application::render(GLFWwindow* window, float deltaTime) {
    game.tick(deltaTime);

    if(game.shouldEnd()) {
        glfwSetWindowShouldClose(window, 1);
        return;
    }

    game.render();
}


void Application::on_resize(GLFWwindow* window, int width, int height) {
    this->width = width;
    this->height = height;

    game.windowResized(width, height);
}


Application::~Application() {
}




void Application::on_mouse_move(GLFWwindow* window, double x, double y) {
    if(lastMouseX==-1) lastMouseX = x;
    if(lastMouseY==-1) lastMouseY = y;

    game.mouseMove(x-lastMouseX, y-lastMouseY);

    lastMouseX = x;
    lastMouseY = y;
}


void Application::on_mouse_pressed(GLFWwindow* window, int button, int action, int mods) {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    game.mouseClick(x, y, button, action == GLFW_PRESS);
}


void Application::on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(action==GLFW_PRESS) game.handleKey(key, true);
    else if(action==GLFW_RELEASE) game.handleKey(key, false);
    //GLFW_REPEAT
}

void Application::on_mouse_scroll(GLFWwindow* window, double dx, double dy) {
    if(dy!=0) game.mouseScroll(dy);
}


bool Application::keyHeld(int keyName) const {
    return glfwGetKey(window, keyName) == GLFW_PRESS;
}

bool Application::mouseHeld(int buttonName) const {
    return glfwGetMouseButton(window, buttonName) == GLFW_PRESS;
}


void Application::lockCursor(bool lock) {
    glfwSetInputMode(window, GLFW_CURSOR, lock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}