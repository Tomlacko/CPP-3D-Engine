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
    Framebuffer prerenderingFramebuffer;

    GLProgram progDrawTextured {"shaders/draw_object_textured.vert", "shaders/draw_object_textured.frag"};
    //GLProgram progDrawColored {"shaders/draw_object.vert", "shaders/draw_object.frag"};
    //GLProgram progLightPosVisualization {"shaders/draw_lights.vert", "shaders/draw_lights.frag"};
    GLProgram progPostprocess {"shaders/postprocess.vert", "shaders/postprocess.frag"};

    Texture font {"images/font.png", false};

public:

    Game(Application* application, int w, int h, const std::string& mapName);

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