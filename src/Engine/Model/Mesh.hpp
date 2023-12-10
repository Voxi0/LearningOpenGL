#pragma once

// Standard Headers
#include<vector>

// GLAD OpenGL Loader
#include<glad/glad.h>

// OpenGL Mathematics (GLM)
#include<glm/glm.hpp>

// Custom Engine Headers
#include"../Shader/Shader.hpp"

// Vertex Struct
struct VertexStruct {
    glm::vec3 vertexPos;
    glm::vec3 normalVec;
    glm::vec2 texCords;
};

// Texture Struct
struct TextureStruct {
    unsigned int id;
    std::string type, path;
};

// Mesh Class
class Mesh {
    public:
        // Mesh Data
        std::vector<VertexStruct> vertices;
        std::vector<unsigned int> indices;
        std::vector<TextureStruct> textures;

        // Constructor - Create A New Mesh
        Mesh(std::vector<VertexStruct> vertices, std::vector<unsigned int> indices, std::vector<TextureStruct> textures) {
            // Initialize Mesh
            this->vertices = vertices;
            this->indices = indices;
            this->textures = textures;

            // Setup Mesh
            this->setupMesh();
        }

        // Render Mesh
        void render(Shader &shader);

        // Destroy Mesh
        void destroy(void) {
            // Delete Vertex Array, Vertex Buffer and Element/Index Buffer Object
            glDeleteVertexArrays(1, &this->VAO);
            glDeleteBuffers(1, &this->VBO);
            glDeleteBuffers(1, &this->EBO);
        }
    private:
        // Render Data - Vertex Array Object, Vertex Buffer Object and Element/Index Buffer Object
        GLuint VAO, VBO, EBO;

        // Setup Mesh
        void setupMesh();
};
