#include "mesh.hpp"
//#include <utility>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "plane.inl"
#include "cube.inl"
#include "sphere.inl"
#include "teapot.inl"

#include <limits>


//MAIN CONSTRUCTOR
Mesh::Mesh(std::vector<float> vertices, std::vector<float> normals, std::vector<float> tex_coords, std::vector<uint32_t> indices, bool rightHanded, GLenum mode, GLint position_location, GLint normal_location, GLint tex_coord_location)
:vertices_count(vertices.size()/3), mode(mode), rightHanded(rightHanded) {

    // Create buffer for vertices
    glCreateBuffers(1, &vertices_buffer);
    glNamedBufferStorage(vertices_buffer, vertices.size()*sizeof(float), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    // Create buffer for normals
    if(!normals.empty()) {
        glCreateBuffers(1, &normals_buffer);
        glNamedBufferStorage(normals_buffer, normals.size()*sizeof(float), normals.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    // Create buffer for texture coordinates
    if(!tex_coords.empty()) {
        glCreateBuffers(1, &tex_coords_buffer);
        glNamedBufferStorage(tex_coords_buffer, tex_coords.size()*sizeof(float), tex_coords.data(), GL_DYNAMIC_STORAGE_BIT);
    }

    // If indices buffer is not empty => create buffer for indices
    if(!indices.empty()) {
        glCreateBuffers(1, &indices_buffer);
        glNamedBufferStorage(indices_buffer, indices.size()*sizeof(float), indices.data(), GL_DYNAMIC_STORAGE_BIT);

        indices_count = indices.size();
    }

    //calculate bounding region
    float ax = std::numeric_limits<float>::infinity();
    float ay = ax; float az = ax;
    float bx = -ax; float by = bx; float bz = bx;

    int verts = vertices.size()/3;
    for(int i = 0; i<verts; i++) {
        float x = vertices[i*3];
        float y = vertices[i*3 + 1];
        float z = vertices[i*3 + 2];

        if(x < ax) ax = x;
        if(y < ay) ay = y;
        if(z < az) az = z;

        if(x > bx) bx = x;
        if(y > by) by = y;
        if(z > bz) bz = z;
    }

    if(ax==bx) {
        ax-=0.05;
        bx+=0.05;
    }
    if(ay==by) {
        ay-=0.05;
        by+=0.05;
    }
    if(az==bz) {
        az-=0.05;
        bz+=0.05;
    }

    if(rightHanded) {
        minCorner = {ax, ay, -bz};
        maxCorner = {bx, by, -az};
    }
    else {
        minCorner = {ax, ay, az};
        maxCorner = {bx, by, bz};
    }

    recreate_vao(position_location, normal_location, tex_coord_location);
}

Mesh::Mesh(const Mesh& other)
:vertices_count(other.vertices_count), indices_count(other.indices_count), mode(other.mode), rightHanded(other.rightHanded), minCorner(other.minCorner), maxCorner(other.maxCorner) {
    // Copy vertices
    if(other.vertices_buffer != 0) {
        glCreateBuffers(1, &vertices_buffer);
        glNamedBufferStorage(vertices_buffer, vertices_count*sizeof(float)*3, nullptr, GL_DYNAMIC_STORAGE_BIT);

        glCopyNamedBufferSubData(other.vertices_buffer, vertices_buffer, 0, 0, vertices_count*sizeof(float)*3);
    }

    // Copy normals
    if(other.normals_buffer != 0) {
        glCreateBuffers(1, &normals_buffer);
        glNamedBufferStorage(normals_buffer, vertices_count*sizeof(float)*3, nullptr, GL_DYNAMIC_STORAGE_BIT);
        glCopyNamedBufferSubData(other.normals_buffer, normals_buffer, 0, 0, vertices_count*sizeof(float)*3);
    }

    // Copy texture coordinates
    if(other.tex_coords_buffer != 0) {
        glCreateBuffers(1, &tex_coords_buffer);
        glNamedBufferStorage(tex_coords_buffer, vertices_count*sizeof(float)*2, nullptr, GL_DYNAMIC_STORAGE_BIT);

        glCopyNamedBufferSubData(other.tex_coords_buffer, tex_coords_buffer, 0, 0, vertices_count*sizeof(float)*2);
    }

    // Copy indices buffer
    if(other.indices_buffer != 0) {
        glCreateBuffers(1, &indices_buffer);
        glNamedBufferStorage(indices_buffer, indices_count*sizeof(uint32_t), nullptr, GL_DYNAMIC_STORAGE_BIT);

        glCopyNamedBufferSubData(other.indices_buffer, indices_buffer, 0, 0, indices_count*sizeof(uint32_t));
    }

    recreate_vao(other.position_location, other.normal_location, other.tex_coord_location);
}

void Mesh::recreate_vao(GLint position_location /* = 0*/, GLint normal_location /* = 1*/, GLint tex_coord_location /* = 2*/) {
    // In case it was created before
    glDeleteVertexArrays(1, &vao);

    // Create a vertex array object for the geometry
    glCreateVertexArrays(1, &vao);

    if(position_location >= 0) {
        this->position_location = position_location;
        glVertexArrayVertexBuffer(vao, position_location, vertices_buffer, 0, 3*sizeof(float));

        glEnableVertexArrayAttrib(vao, position_location);
        glVertexArrayAttribFormat(vao, position_location, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, position_location, position_location);
    }

    if(normal_location >= 0) {
        this->normal_location = normal_location;

        glVertexArrayVertexBuffer(vao, normal_location, normals_buffer, 0, 3*sizeof(float));

        glEnableVertexArrayAttrib(vao, normal_location);
        glVertexArrayAttribFormat(vao, normal_location, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, normal_location, normal_location);
    }

    if(tex_coord_location >= 0) {
        this->tex_coord_location = tex_coord_location;

        glVertexArrayVertexBuffer(vao, tex_coord_location, tex_coords_buffer, 0, 2*sizeof(float));

        glEnableVertexArrayAttrib(vao, tex_coord_location);
        glVertexArrayAttribFormat(vao, tex_coord_location, 2, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vao, tex_coord_location, tex_coord_location);
    }

    if(indices_buffer != 0) {
        glVertexArrayElementBuffer(vao, indices_buffer);
    }
}


Mesh::~Mesh() {
    // Deallocate all resources on GPU
    // Everything can be deleted without checking because we initialize it
    // to 0 and specification ignores 0 in glDelete* calls
    glDeleteVertexArrays(1, &this->vao);

    glDeleteBuffers(1, &this->vertices_buffer);
    glDeleteBuffers(1, &this->normals_buffer);
    glDeleteBuffers(1, &this->tex_coords_buffer);
    glDeleteBuffers(1, &this->indices_buffer);
}


void Mesh::draw(int handednessLocation /* = 1*/) const {
    glUniform1i(handednessLocation, rightHanded?-1:1); //handedness
    glBindVertexArray(vao);

    if(indices_buffer > 0)
        glDrawElements(mode, static_cast<GLsizei>(indices_count), GL_UNSIGNED_INT, nullptr);
    else
        glDrawArrays(mode, 0, static_cast<GLsizei>(vertices_count));
}



std::unique_ptr<Meshes> Mesh::from_file(const std::string& file_name, bool rightHanded /* = true*/, GLint position_location /* = 0*/, GLint normal_location /* = 1*/, GLint tex_coord_location /* = 2*/) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, file_name.c_str())) {
        throw "Could not load object file.";
    }

    std::unique_ptr<Meshes> meshes_ptr = std::make_unique<Meshes>(); //vector
    std::vector<std::unique_ptr<Mesh>>& meshes = *meshes_ptr;

    // Loop over shapes
    for(auto& shape : shapes) {
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<float> tex_coords;

        for(size_t f=0; f < shape.mesh.num_face_vertices.size(); f++) {
            // Loop over vertices in the face/polygon
            // (because of triangulate true in LoadObj number of vertices
            // per face/polygon is always 3
            for(size_t v=0; v<3; v++) {
                // access to vertex
                tinyobj::index_t idx = shape.mesh.indices[f*3 + v];

                for(int i=0; i<3; i++) {
                    vertices.push_back(attrib.vertices[(size_t)idx.vertex_index*3 + i]);

                    if(!attrib.normals.empty()) {
                        normals.push_back(attrib.normals[(size_t)idx.normal_index*3 + i]);
                    }
                }

                if(!attrib.texcoords.empty()) {
                    tex_coords.push_back(attrib.texcoords[(size_t)idx.texcoord_index*2 + 0]);
                    tex_coords.push_back(attrib.texcoords[(size_t)idx.texcoord_index*2 + 1]);
                }
            }
        }

        meshes.push_back(std::make_unique<Mesh>(vertices, normals, tex_coords, std::vector<uint32_t>(), rightHanded));
        meshes[meshes.size()-1]->recreate_vao(position_location, normal_location, tex_coord_location);
    }

    return meshes_ptr;
}



std::unique_ptr<Meshes> Mesh::from_interleaved(std::vector<float> interleaved_vertices, std::vector<uint32_t> indices, bool rightHanded /* = true*/, GLenum mode /* = GL_TRIANGLES*/, GLint position_location /* = 0*/, GLint normal_location /* = 1*/, GLint tex_coord_location /* = 2*/) {
    // Deinterleave and then call deinterleaved constructor
    std::vector<float> vertices;
    std::vector<float> normals;
    std::vector<float> tex_coords;

    for(size_t vertex_offset = 0; vertex_offset < interleaved_vertices.size(); vertex_offset += 8) {
        for(int i=0; i<3; i++) {
            vertices.push_back(interleaved_vertices[vertex_offset+i]);
            normals.push_back(interleaved_vertices[vertex_offset+i+3]);
        }

        tex_coords.push_back(interleaved_vertices[vertex_offset+6]);
        tex_coords.push_back(interleaved_vertices[vertex_offset+7]);
    }

    std::unique_ptr<Meshes> meshes_ptr = std::make_unique<Meshes>(); //vector
    std::vector<std::unique_ptr<Mesh>>& meshes = *meshes_ptr;

    meshes.emplace_back(std::make_unique<Mesh>(std::move(vertices), std::move(normals), std::move(tex_coords), std::move(indices), rightHanded, mode));
    meshes[0]->recreate_vao(position_location, normal_location, tex_coord_location);
    return meshes_ptr;
}


std::unique_ptr<Meshes> Mesh::plane(GLint position_location, GLint normal_location, GLint tex_coord_location) {
    return Mesh::from_interleaved(
            std::vector<float>(plane_vertices, std::end(plane_vertices)),
            std::vector<uint32_t>(plane_indices, std::end(plane_indices)),
            false,
            GL_TRIANGLES,
            position_location,
            normal_location,
            tex_coord_location
    );
}

std::unique_ptr<Meshes> Mesh::cube(GLint position_location, GLint normal_location, GLint tex_coord_location) {
    return Mesh::from_interleaved(
            std::vector<float>(cube_vertices, std::end(cube_vertices)),
            std::vector<uint32_t>(cube_indices, std::end(cube_indices)),
            true,
            GL_TRIANGLES,
            position_location,
            normal_location,
            tex_coord_location
    );
}

std::unique_ptr<Meshes> Mesh::sphere(GLint position_location, GLint normal_location, GLint tex_coord_location) {
    return Mesh::from_interleaved(
            std::vector<float>(sphere_vertices, std::end(sphere_vertices)),
            std::vector<uint32_t>(sphere_indices, std::end(sphere_indices)),
            true,
            GL_TRIANGLE_STRIP,
            position_location,
            normal_location,
            tex_coord_location
    );
}

std::unique_ptr<Meshes> Mesh::teapot(GLint position_location, GLint normal_location, GLint tex_coord_location) {
    return Mesh::from_interleaved(
            std::vector<float>(teapot_vertices, std::end(teapot_vertices)),
            std::vector<uint32_t>(teapot_indices, std::end(teapot_indices)),
            true,
            GL_TRIANGLE_STRIP,
            position_location,
            normal_location,
            tex_coord_location
    );
}