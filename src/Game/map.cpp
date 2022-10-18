#include "map.hpp"
#include <utilities.hpp>

#include <glad/glad.h>
#include <fstream>
#include <sstream>


Map::Map(const std::string& mapName) {

    //load primitive meshes
    meshes.insert(std::make_pair("PLANE", Mesh::plane()));
    meshes.insert(std::make_pair("CUBE", Mesh::cube()));
    meshes.insert(std::make_pair("SPHERE", Mesh::sphere()));


    //load map from file

    std::ifstream file("maps/"+mapName);

    std::string line;
    while(std::getline(file, line)) {
        std::istringstream linestream(line);

        std::string command;
        linestream >> std::ws >> command;


        if(command.empty() || command=="#") {
            //comments and empty lines = pass
        }


        //syntax: spawnpoint "x" "y" "z" "rx" "ry"
        else if(command=="spawnpoint") { //set map spawnpoint for player
            float x, y, z, rx, ry;

            if(!(linestream >> x >> y >> z >> rx >> ry))
                throw std::runtime_error("Map parsing error - spawnpoint - invalid values");

            spawnPoint.setPos(x, y, z);
            spawnPoint.setRot(glm::radians(rx), glm::radians(ry));
        }


        //syntax: texture "name" "useInterpolation"
        else if(command=="texture") {
            std::string name;
            boolstr useInterpolation;

            if(!(linestream >> name >> useInterpolation.str))
                throw std::runtime_error("Map parsing error - texture - invalid values");

            textures.insert(std::make_pair(name, std::make_unique<Texture>("textures/" + name + ".png", useInterpolation)));
        }


        //syntax: textures "name" "animationFrequency" "useInterpolation" "diffuseCount" "normalCount" "heightCount" "specularCount"
        else if(command=="textures") {
            std::string name;
            float animFreq;
            boolstr useInterpolation;
            unsigned int texCounts[TEXTURE_TYPES_COUNT];

            if(!(linestream >> name >> animFreq >> useInterpolation.str))
                throw std::runtime_error("Map parsing error - textures - invalid values");


            TextureGroup* texGrp = textureGroups.insert(std::make_pair(name, std::make_unique<TextureGroup>(animFreq))).first->second.get();

            for(unsigned int typeID=0; typeID<TEXTURE_TYPES_COUNT; typeID++) {
                if(!(linestream >> texCounts[typeID])) {
                    //at least some (the first) value needs to be specified
                    if(typeID==0) throw std::runtime_error("Map parsing error - textures - invalid values (texCounts)");
                    break;
                }

                //direct access
                std::vector<Texture>& frames = texGrp->type(typeID);

                //load all texture frames of the current type
                for(unsigned int i=0; i<texCounts[typeID]; i++) {
                    frames.emplace_back(
                        "textures/" + name + "/" + TEXTURE_TYPE_NAMES[typeID] + ((texCounts[typeID] == 1)?"":"_"+std::to_string(i)) + ".png",
                        typeID>0 || useInterpolation
                    );
                }
            }
            texGrp->init();
        }


        //syntax: mesh "mesh_name" "rightHanded"
        else if(command=="mesh") { //load mesh under a named identifier
            std::string name;
            boolstr rightHanded;

            if(!(linestream >> name >> rightHanded.str))
                throw std::runtime_error("Map parsing error - mesh - invalid value");

            meshes.insert(std::make_pair(name, Mesh::from_file("objects/"+name+".obj", rightHanded)));
        }


        //syntax: <object|entity> "unique_name" "x" "y" "z" "rx" "ry" "sx" "sy" "sz" "mesh_name" "texture_group_name"
        else if(command=="object" || command=="entity") { //load object/entity
            float x, y, z, rx, ry, sx, sy, sz;
            std::string name, meshName, texName; //name is in-game unique name of this object, name is shared global name of this type of object

            if(!(linestream >> name >> x >> y >> z >> rx >> ry >> sx >> sy >> sz))
                throw std::runtime_error("Map parsing error - object/entity - invalid values");

            glm::vec3 pos {x, y, z};
            glm::vec2 rot {glm::radians(rx), glm::radians(ry)};
            glm::vec3 scale {sx, sy, sz};

            Meshes* geom = nullptr;
            if(linestream >> meshName) geom = meshes.at(meshName).get();

            TextureGroup* texGrp = nullptr;
            if(linestream >> texName) texGrp = textureGroups.at(texName).get();

            Object* obj = nullptr;
            if(command=="entity")
                obj = &entities.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(pos, rot, scale, geom, texGrp)).first->second;
            else
                obj = &objects.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(pos, rot, scale, geom, texGrp)).first->second;
        }


        //old syntax: light <point|directional> "x" "y" "z" "r" "g" "b"
        //syntax: light "radius" "x" "y" "z" "r" "g" "b"
        else if(command=="light") {
            //type
            //coords xyz (w set to type, 1 is point light, 0 is directional)
            //color rgb (w unused, intensity in the future), replicate for ambient_color, diffuse_color, specular_color

            std::string type;
            float radius, x, y, z, r, g, b;
            if(!(linestream >> radius >> x >> y >> z >> r >> g >> b))
                throw std::runtime_error("Map parsing error - light - invalid values");

            //LightType lightType = (type=="directional") ? LightType::directional : LightType::point;

            lights.emplace_back(radius, x, y, z, r, g, b);
        }


        //syntax: define <string|int|float> "name" "value"
        else if(command=="define") {
            std::string type, name;
            if(!(linestream >> type >> name))
                throw std::runtime_error("Map parsing error - define - invalid type/name");

            if(type=="string") {
                std::string val;
                if(!(linestream >> val))
                    throw std::runtime_error("Map parsing error - define string - invalid value");
                definitions_string.insert(std::make_pair(name, val));
            }
            else if(type=="int") {
                int val;
                if(!(linestream >> val))
                    throw std::runtime_error("Map parsing error - define int - invalid value");
                definitions_int.insert(std::make_pair(name, val));
            }
            else if(type=="float") {
                float val;
                if(!(linestream >> val))
                    throw std::runtime_error("Map parsing error - define float - invalid value");
                definitions_float.insert(std::make_pair(name, val));
            }
        }
    }





    //create buffer for lights, but data not passed in until during rendering
    glCreateBuffers(1, &lightsBufferID);
    glNamedBufferStorage(lightsBufferID, lights.size()*sizeof(LightUBO), nullptr/*lights.data()*/, GL_DYNAMIC_STORAGE_BIT);
}

Map::~Map() {
    glDeleteBuffers(1, &lightsBufferID);
}



void Map::tick(float deltaTime) {
    for(auto& [name, ent] : entities) {
        ent.tick(deltaTime);
    }
}

void Map::render(float time) const {
    //load new lightUBOs into lights buffer
    glNamedBufferSubData(lightsBufferID, 0, lights.size()*sizeof(LightUBO), lights.data());

    //bind lights buffer into shader (1 = binding point)
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, lightsBufferID);

    glUniform1f(2, time); //location, value

    for(const auto& [name, obj]: objects) {
        obj.render(time);
    }
    for(const auto& [name, ent] : entities) {
        ent.render(time);
    }
}