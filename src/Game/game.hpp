#pragma once

#include <string>

#include "map.hpp"
#include "player.hpp"
#include "camera.hpp"

#include <framebuffer.hpp>
#include <program.hpp>


class Application;

class Game {
    Application* app;

    int width;
    int height;

    float mouseLookSensitivity = 0.5;
    bool guiMode = true;
    bool stopGame = false;

    float currentTime = 0;

    Map map;
    Player player;
    Camera camera;

    GLProgram progDrawTextured {"shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag"};
    GLProgram progPostprocess {"shaders/postprocess.vert", "shaders/postprocess.frag"};
    GLProgram progUI {"shaders/ui.vert", "shaders/ui.frag"};

    Texture font_texture {"textures/font.png", false};

    Framebuffer prerenderingFramebuffer;

public:

    Game(Application* application, int w, int h);

    ~Game() = default;

    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    void setCameraToPlayer() {
        camera.setPos(player.getEyePos());
        camera.setRot(player.getRot());
    }

    bool shouldEnd() const {return stopGame;}
    void setGuiMode(bool enable);

    void windowResized(int width, int height);

    void mouseMove(float dx, float dy);
    void mouseClick(float x, float y, int button, bool down);
    void mouseScroll(float amount);

    void handleKey(int code, bool down);
    bool keyHeld(int keyName) const;
    bool mouseHeld(int buttonName) const;

    void tick(float deltaTime);

    void render() const;
};