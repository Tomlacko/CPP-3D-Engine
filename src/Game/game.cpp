#include "game.hpp"
#include "../application.hpp"
#include <GLFW/glfw3.h>

static constexpr char const* INITIAL_MAP = "map01.map";


Game::Game(Application* application, int w, int h)
:app(application), width(w), height(h), map(INITIAL_MAP), player(map.getPlayerSpawnPoint(), map.getPlayerSpawnRot()), camera(w, h), prerenderingFramebuffer(w, h) {
    glEnable(GL_CULL_FACE);
}


void Game::tick(float deltaTime) {
    currentTime += deltaTime;

    //control player movement
    if(!guiMode) {
        glm::vec3 moveDir;
        if(keyHeld(GLFW_KEY_W)) moveDir.z+=1;
        if(keyHeld(GLFW_KEY_S)) moveDir.z-=1;
        if(keyHeld(GLFW_KEY_A)) moveDir.x-=1;
        if(keyHeld(GLFW_KEY_D)) moveDir.x+=1;
        if(keyHeld(GLFW_KEY_SPACE)) moveDir.y+=1;
        if(keyHeld(GLFW_KEY_LEFT_CONTROL)) moveDir.y-=1;
        player.move(moveDir);
    }

    //tick entities
    map.tick(deltaTime);
    player.tick(deltaTime);

    setCameraToPlayer();
}

void Game::render() const {
    prerenderingFramebuffer.use();
    glEnable(GL_DEPTH_TEST);
    camera.use();
    progDrawTextured.use();
    map.render(currentTime);


    progPostprocess.use();

    //bind back the default framebuffer and use prerendered's texture
    Framebuffer::useDefault(&prerenderingFramebuffer);

    //depth test not needed for direct drawing
    glDisable(GL_DEPTH_TEST);

    //font_texture.use(2);

    Framebuffer::drawScreenCoveringTriangle();

    /*
    progUI.use();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    */
}

void Game::setGuiMode(bool enable) {
    if(enable==guiMode) return;
    guiMode = enable;
    app->lockCursor(!enable);
}

void Game::mouseMove(float dx, float dy) {
    if(!guiMode) player.changeRotBy(glm::radians(dx)*mouseLookSensitivity, glm::radians(-dy)*mouseLookSensitivity);
}

void Game::mouseClick(float x, float y, int button, bool down) {
    if(button==GLFW_MOUSE_BUTTON_LEFT) {
        if(down) {
            setGuiMode(false);
        }
    }
}

void Game::mouseScroll(float amount) {
    //+amount == up/forward
    //-amount == down/backward
    //map.objects.at("mansion_wall").changeRotBy(glm::radians(22.5*amount), 0);
    player.flyMode(amount>0);
}

void Game::handleKey(int key, bool down) {
    if(down) {
        //DEBUG_LOG("Key pressed: ");
        //DEBUG_LOG_LN(key);

        switch(key) {
            case 256: //ESC
                stopGame = true;
                break;
            case 340:
                player.setRunning(true);
                break;
        }
    }
    else {
        //DEBUG_LOG("Key unpressed: ");
        //DEBUG_LOG_LN(key);

        switch(key) {
            case 340:
                player.setRunning(false);
                break;
        }
    }
}

//example: GLFW_KEY_W
bool Game::keyHeld(int keyName) const {
    return app->keyHeld(keyName);
}
//example: GLFW_MOUSE_BUTTON_LEFT
bool Game::mouseHeld(int buttonName) const {
    return app->mouseHeld(buttonName);
}

void Game::windowResized(int width, int height) {
    camera.setAspect(width, height);
    prerenderingFramebuffer.resize(width, height);
}