#pragma once


#include <vector>
#include <string>
#include <memory>

#include <glad/glad.h>


/*
struct MeshAttribs {
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> tex_coords;
    std::vector<uint32_t> indices;
};*/


class Mesh;
typedef std::vector<std::unique_ptr<Mesh>> Meshes;


class Mesh {

    GLuint vao = 0;


    size_t vertices_count = 0;

    GLuint vertices_buffer = 0;
    GLuint normals_buffer = 0;
    GLuint tex_coords_buffer = 0;


    size_t indices_count = 0;
    GLuint indices_buffer = 0;


    GLint position_location = -1;
    GLint normal_location = -1;
    GLint tex_coord_location = -1;


    GLenum mode = GL_TRIANGLES;


public:

    bool rightHanded = true;


    Mesh(std::vector<float> vertices, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2)
    :Mesh(std::move(vertices), std::vector<float>(), std::vector<float>(), std::move(indices), mode, position_location, normal_location, tex_coord_location) {
    }

    Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2)
    :Mesh(std::move(vertices), std::move(normals), std::vector<float>(), std::move(indices), mode, position_location, normal_location, tex_coord_location) {
    }

    //MAIN CONSTRUCTOR
    Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex_coords, std::vector<uint32_t> indices, GLenum mode = GL_TRIANGLES, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

    Mesh(const Mesh& other);

    ~Mesh();

    void recreate_vao(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);


    [[nodiscard]] GLuint get_vao() const {return this->vao;}
    [[nodiscard]] GLenum get_mode() const {return this->mode;}

    [[nodiscard]] size_t get_vertices_count() const {return this->vertices_count;}
    [[nodiscard]] size_t get_indices_count() const {return this->indices_count;}

    [[nodiscard]] GLuint get_vertices_buffer() const {return this->vertices_buffer;}
    [[nodiscard]] GLuint get_normals_buffer() const {return this->normals_buffer;}
    [[nodiscard]] GLuint get_tex_coords_buffer() const {return this->tex_coords_buffer;}
    [[nodiscard]] GLuint get_indices_buffer() const {return this->indices_buffer;}


    void draw(int handednessLocation = 1) const;


    static std::unique_ptr<Meshes> from_file(const std::string &file_name, bool rightHanded = true, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

    static std::unique_ptr<Meshes> from_interleaved(std::vector<float> interleaved_vertices, std::vector<uint32_t> indices, bool rightHanded = true, GLenum mode = GL_TRIANGLES, GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);

    static std::unique_ptr<Meshes> plane(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
    static std::unique_ptr<Meshes> cube(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
    static std::unique_ptr<Meshes> sphere(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
    static std::unique_ptr<Meshes> teapot(GLint position_location = 0, GLint normal_location = 1, GLint tex_coord_location = 2);
};