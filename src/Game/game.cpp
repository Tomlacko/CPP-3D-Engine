#include "game.hpp"
#include "../application.hpp"
#include <GLFW/glfw3.h>

#include "hitbox.hpp"

int pieces = 0;



static constexpr char const* INITIAL_MAP = "map01.map";


Game::Game(Application* application, int w, int h)
: app(application), screenWidth(w), screenHeight(h), map(INITIAL_MAP),
  player(map.getPlayerSpawnPoint(), map.getPlayerSpawnRot()),
  camera(w, h), prerenderingFramebuffer(w, h), ui(w, h) {
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    ui.addImageElement("blackscreen", {0, 0}, {32, 0, 48, 16}, {0, 0}, {100, 100});
    ui.addImageElement("crosshair", {0, 0}, {0, 0, 16, 16}, {0, 0});

    ui.addTextElement("start", "Click inside window to start the game!", {-1, 1}, {-0.9, 0.9}, {2, 2});
    ui.addTextElement("paused", "Game paused", {-1, 1}, {-0.9, 0.9}, {3, 3});
    ui.getTextElement("paused")->hide();

    ui.addTextElement("status", "Collected orb pieces: 0", {-1, -1}, {-1, -1}, {3, 3});
}


void Game::tick(float deltaTime) {
    if(!paused || tickCount==0) {
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
        map.tick(deltaTime, currentTime, player);
        //player.tick(deltaTime);

        Hitbox a = player.getHitbox();

        for(auto& [name, obj] : map.objects) {
            if(obj.tag=="orb_piece") {
                Hitbox b = obj.getHitbox();
                if(a.intersecting(b)) {
                    obj.changePosBy(0, 1000, 0);
                    pieces++;
                    DEBUG_LOG_LN("collected piece");
                }
            }
        }

        setCameraToPlayer();

        ++tickCount;
    }
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

    Framebuffer::drawScreenCoveringTriangle();


    ui.render(currentTime);
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
            if(paused && tickCount<=1) setPaused(false);
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
                setPaused(!paused);
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
    if(width<1 || height<1) return;
    screenWidth = width;
    screenHeight = height;

    camera.setAspect(width, height);
    prerenderingFramebuffer.resize(width, height);
    ui.resize(width, height);
}

void Game::setPaused(bool state) {
    if(state) {
        setGuiMode(true);
        paused = true;
        ui.getImageElement("blackscreen")->show();
        ui.getTextElement("paused")->show();
    }
    else {
        setGuiMode(false);
        paused = false;
        ui.getImageElement("blackscreen")->hide();
        ui.getTextElement("start")->hide();
        ui.getTextElement("paused")->hide();
    }
}

void Game::quitGame() {
    stopGame = true;
}