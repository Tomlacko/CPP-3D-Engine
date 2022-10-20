#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "application.hpp"


void on_resize(GLFWwindow* window, int width, int height);
void on_mouse_move(GLFWwindow* window, double x, double y);
void on_mouse_pressed(GLFWwindow* window, int button, int action, int mods);
void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods);
void on_mouse_scroll(GLFWwindow* window, double dx, double dy);

// Callback function to be called when OpenGL emits an error
void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* user_parameter);



int main() {
    const size_t initial_width = 1280;
    const size_t initial_height = 720;

    // Initialize GLFW
    if(!glfwInit()) {
        std::cerr << "Could not initialize GLFW!" << std::endl;
        return -1;
    }
    std::cout << "--Window init--" << std::endl;

    // Before creating the Window, request OpenGL 4.5 Core context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

    // Create window
    GLFWwindow* window = glfwCreateWindow(initial_width, initial_height, "PV112 TermProject Game", nullptr, nullptr);
    if(!window) {
        std::cerr << "Could not create a window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make window's context current
    glfwMakeContextCurrent(window);

    // Load OpenGL functions
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Could not initialize OpenGL context!" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(opengl_debug_callback, nullptr);
/*
    // switch to full screen
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    int fullscreenWidth = mode->width;
    int fullscreenHeight = mode->height;
    glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, fullscreenWidth, fullscreenHeight, 0 );
*/
    {
        // Initialize our Application
        Application application(window, initial_width, initial_height);
        //Application application(window, fullscreenWidth, fullscreenHeight);

        // Set window's callbacks
        glfwSetWindowUserPointer(window, &application);
        glfwSetWindowSizeCallback(window, on_resize);
        glfwSetCursorPosCallback(window, on_mouse_move);
        glfwSetMouseButtonCallback(window, on_mouse_pressed);
        glfwSetKeyCallback(window, on_key_pressed);
        glfwSetScrollCallback(window, on_mouse_scroll);

        int w, h;
        float lastTime = 0;
        float currentTime = 0;

        // Loop
        while(!glfwWindowShouldClose(window)) {

            //stop rendering when window is minimized
            glfwGetWindowSize(window, &w, &h);
            if(w>0 && h>0) {
                //process window content
                currentTime = glfwGetTime();
                application.render(window, currentTime-lastTime);
                lastTime = currentTime;

                // Swap front and back buffers
                glfwSwapBuffers(window);
            }
            else {
                //still keep track of time to prevent time jumps
                lastTime = glfwGetTime();
            }

            // Poll for and process events
            glfwPollEvents();
        }

        // Free the entire application before terminating glfw. If this were done in the wrong order
        // application may crash on calling OpenGL (Delete*) calls after destruction of a context.
        // Freeing is done implicitly by enclosing this part of code in block {}.
    }

    // Free every allocated resource associated with GLFW
    glfwTerminate();
    return 0;
}

void on_resize(GLFWwindow* window, int width, int height) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);
    application->on_resize(window, width, height);
}

void on_mouse_move(GLFWwindow* window, double x, double y) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);
    application->on_mouse_move(window, x, y);
}

void on_mouse_pressed(GLFWwindow* window, int button, int action, int mods) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);
    application->on_mouse_pressed(window, button, action, mods);
}

void on_key_pressed(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);
    application->on_key_pressed(window, key, scancode, action, mods);
}

void on_mouse_scroll(GLFWwindow* window, double dx, double dy) {
    Application* application = (Application*)glfwGetWindowUserPointer(window);
    application->on_mouse_scroll(window, dx, dy);
}

void APIENTRY opengl_debug_callback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const char* message, const void* user_parameter) {
    switch(type) {
        case GL_DEBUG_TYPE_ERROR:
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            std::cerr << message << std::endl;
            return;
        default:
            return;
    }
}