#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>
#include <utility>

#include <mesh.hpp>

//#include <texture.hpp>
class Texture;


/*
 * rotation {0,0} is looking towards +Z (south)
*/

static const float PI = 3.14159265358979323846;
static const float PI2 = 2*PI;


struct ObjectUBO {
    glm::mat4 model_matrix;  // [  0 -  64) bytes
    glm::vec4 ambient_color {0.0f}; // [ 64 -  80) bytes
    glm::vec4 diffuse_color {1.0f}; // [ 80 -  96) bytes

    // Contains shininess in .w element
    glm::vec4 specular_color {0.0f}; // [ 96 - 112) bytes
};



class Object {


protected:
    glm::vec3 position;
    glm::vec2 rotation;
    glm::vec3 size;
    glm::vec3 scale {1, 1, 1};
    bool centeredOnBottom = true;
    bool rightHanded = true;

    std::vector<std::unique_ptr<Mesh>> geometry;

    mutable bool modelMatrixOutdated = true;
    mutable bool uboOutdated = true;
    mutable ObjectUBO ubo;
    GLuint bufferID = 0;

    Texture* texture = nullptr;

    void init();

    void updateModelMatrix() const;

    Mesh cube = Mesh::cube();

public:
    Object() {
        init();
    }

    Object(const glm::vec3& pos, const glm::vec2& rot, const glm::vec3& sc, Texture& tex, bool rHanded = true)
    : position(pos), rotation(rot), scale(sc), rightHanded(rHanded), texture(&tex) {
        init();
    }

    Object(const glm::vec3& pos, const glm::vec2& rot, const glm::vec3& sc, bool rHanded = true)
    : position(pos), rotation(rot), scale(sc), rightHanded(rHanded) {
        init();
    }

    Object(const glm::vec3& pos, const glm::vec2& rot)
    : position(pos), rotation(rot) {
        init();
    }

    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    virtual ~Object();

    void setGeometry(std::vector<std::unique_ptr<Mesh>> geom) {geometry = std::move(geom);}
    void setTexture(Texture& tex) {texture = &tex;}

    void setSpecular(glm::vec4 spec) {ubo.specular_color = spec; uboOutdated = true;}

    //change to be used to cache AABB box or to create hitbox
    void setSize(const glm::vec3& sz) {size = sz;}

    virtual void changeRotBy(float x, float y);
    virtual void setRot(float x, float y);
    virtual void setRot(const glm::vec2& rot);
    [[nodiscard]] const glm::vec2& getRot() const {return rotation;}

    void changePosBy(float x, float y, float z) {position += glm::vec3(x, y, z); modelMatrixOutdated = true;}
    void changePosBy(const glm::vec3& pos) {position += pos; modelMatrixOutdated = true;}
    void setPos(float x, float y, float z) {position = {x, y, z}; modelMatrixOutdated = true;}
    void setPos(const glm::vec3& pos) {position = pos; modelMatrixOutdated = true;}
    [[nodiscard]] const glm::vec3& getPos() const {return position;}

    void changeScaleBy(float x, float y, float z) {scale += glm::vec3(x, y, z); modelMatrixOutdated = true;}
    void changeScaleBy(const glm::vec3& s) {scale += s; modelMatrixOutdated = true;}
    void setScale(float x, float y, float z) {scale = {x, y, z}; modelMatrixOutdated = true;}
    void setScale(const glm::vec3& s) {scale = s; modelMatrixOutdated = true;}
    [[nodiscard]] const glm::vec3& getScale() const {return scale;}

    //axis-aligned bounding box, disregarding rotation
    [[nodiscard]] std::pair<glm::vec3, glm::vec3> getAABB() const;


    void render(float time) const;
};