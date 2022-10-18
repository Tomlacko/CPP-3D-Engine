#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "object.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "area.hpp"
#include <texture.hpp>



class Map {
    Area spawnPoint;

    std::unordered_map<std::string, Texture> textures;

    std::unordered_map<std::string, Object> objects;
    std::unordered_map<std::string, Entity> entities;

    std::vector<LightUBO> lights;

    GLuint lightsBufferID = 0;


public:

    Map() = default;
    explicit Map(const std::string& mapName);

    ~Map();

    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;


    glm::vec3 getPlayerSpawnPoint() const {return spawnPoint.getPos();}
    glm::vec2 getPlayerSpawnRot() const {return spawnPoint.getRot();}

    void tick(float deltaTime);
    void render(float time) const;

};