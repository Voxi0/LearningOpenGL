#include"Mesh.hpp"

// Setup Mesh
void Mesh::setupMesh() {
    // Vertex Array Object
    glGenVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    // Vertex Buffer Object
    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(VertexStruct), &this->vertices[0], GL_STATIC_DRAW);

    // Element/Index Buffer Object
    glGenBuffers(1, &this->EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(unsigned int), &this->indices[0], GL_STATIC_DRAW);

    // Vertex Attributes
    // Vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal Vectors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void*)offsetof(VertexStruct, normalVec));
    glEnableVertexAttribArray(1);

    // Texture Coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void*)offsetof(VertexStruct, texCords));
    glEnableVertexAttribArray(2);

    // Unbind Vertex Array Object
    glBindVertexArray(0);
}

// Render Mesh
void Mesh::render(Shader &shader) {
    // Iterate Through All The Textures And Seperate Diffusion and Specular Textures
    unsigned int diffuseNr{1}, specularNr{1};
    for(unsigned int i = 0; i < this->textures.size(); i++) {
        // Count The Number of Diffusion and Specular Textures
        std::string number, name{this->textures[i].type};
        if(name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
        } if(name == "texture_specular") {
            number = std::to_string(specularNr++);
        }

        // Set Material Textures
        glActiveTexture(GL_TEXTURE0 + i);
        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // Render Mesh
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
