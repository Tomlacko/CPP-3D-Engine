#pragma once

#include <vector>
#include <unordered_map>
#include <string>

#include "object.hpp"
#include "entity.hpp"
#include "light.hpp"
#include "area.hpp"



class Map {
    Area spawnPoint;

    std::unordered_map<std::string, std::unique_ptr<Texture>> textures;
    std::unordered_map<std::string, std::unique_ptr<TextureGroup>> textureGroups;
    std::unordered_map<std::string, std::unique_ptr<Meshes>> meshes;


    GLuint lightsBufferID = 0;


public:

    std::unordered_map<std::string, Object> objects;
    std::unordered_map<std::string, Entity> entities;

    std::vector<LightUBO> lights;

    std::unordered_map<std::string, std::string> definitions_string;
    std::unordered_map<std::string, int> definitions_int;
    std::unordered_map<std::string, float> definitions_float;


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