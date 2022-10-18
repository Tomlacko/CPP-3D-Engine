#include "map.hpp"



Map::Map(const std::string& mapName) {
    try {
        std::ifstream file("maps/"+mapName);

        std::string line;
        while(std::getline(file, line)) {
            std::istringstream linestream(line);

            std::string command;
            linestream >> std::ws >> command;

            if(command.empty() || command=="#") {
                //comments and empty lines = pass
            }
            else if(command=="spawnpoint") { //set map spawnpoint for player
                float x, y, z, a, b;
                if(!(linestream >> x >> y >> z >> a >> b)) throw std::runtime_error("Map parsing error - spawnpoint - invalid values");
                spawnPoint.setPos(x, y, z);
                spawnPoint.setRot(glm::radians(a), glm::radians(b));
            }
            else if(command=="mesh") {
                //load mesh under a named identifier
            }
            else if(command=="object") { //load object
                //in future, make object use already loaded mesh

                //std::vector<std::unique_ptr<Mesh>> obj_test_scene = Mesh::from_file("objects/test_scene.obj");
            }
            else if(command=="entity") {

            }
            else if(command=="light") {
                //type
                //coords xyz (w set to type, 1 is point light, 0 is directional)
                //color rgb (w unused, intensity in the future), replicate for ambient_color, diffuse_color, specular_color
            }
        }
    }
    catch(...) {
        std::cerr << "Map failed to load!" << std::endl;

        return;
    }



    textures.emplace(std::piecewise_construct, std::forward_as_tuple("default"), std::forward_as_tuple("images/default.png", false));

    objects.emplace(std::piecewise_construct, std::forward_as_tuple("floor"), std::forward_as_tuple(
        glm::vec3(0,0,0),
        glm::vec2(0,0),
        glm::vec3(1,1,1),
        textures.at("default")
    )).first->second.setSpecular({1,1,1,20});

    objects["floor"].setGeometry(Mesh::from_file("objects/mansion_wall.obj"));


    //setup some random directional light (REMOVE, move to custom Light object)
    LightUBO directional_light;
    directional_light.position = glm::vec4(100.0f, 100.0f, 50.0f, 0.0f);
    directional_light.ambient_color = glm::vec4(1, 1, 1, 0);
    directional_light.diffuse_color = glm::vec4(1, 1, 1, 0);
    directional_light.specular_color = glm::vec4(1, 1,1,0);
    lights.push_back(directional_light);

    //create grid of lights (as uniforms), added to global list of lights
    for(int x=-8; x<8; x+=2) {
        for(int y=-8; y<8; y+=2) {
            //variable color depending on position
            glm::vec3 color = (glm::vec3(x, y, 0.0f) + 8.0f) / 16.0f;

            lights.push_back({
                glm::vec4(x, 0.40f, y, 1.0f), //position
                glm::vec4(0.0),               //ambient color
                glm::vec4(color, 1.0f),       //diffuse color
                glm::vec4(0.0f)               //specular color
            });
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

    glUniform1f(0, time); //location, value

    for(const auto& [name, obj]: objects) {
        obj.render(time);
    }
    for(const auto& [name, ent] : entities) {
        ent.render(time);
    }
}