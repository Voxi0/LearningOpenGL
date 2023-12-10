#pragma once

// Standard Headers
#include<iostream>
#include<vector>

// Assimp (Open Asset Importer)
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

// OpenGL Mathematics (GLM)
#include<glm/glm.hpp>

// Custom Engine Headers
#include"Mesh.hpp"
#include"../Shader/Shader.hpp"
#include"../Texture/Texture.hpp"

// Model Class
class Model {
    public:
        // Constructor - Load 3D Model File
        Model(const std::string modelFilePath, const unsigned int processFlags) {
            // Load Model File
            loadModel(modelFilePath, processFlags);
        }

        // Render Model
        void render(Shader &shader) {
            // Render All The Meshes of The Model
            for(unsigned int i = 0; i < meshes.size(); i++) {
                meshes[i].render(shader);
            }
        }
    
        // Destroy Model
        void destroy(void) {
            // Loop Through All The Meshes and Destroy Them One by One
            for(unsigned int i = 0; i < this->meshes.size(); i++) {
                this->meshes[i].destroy();
            }
        }
    private:
        // Model Data
        std::vector<Mesh> meshes;
        std::vector<TextureStruct> loadedTextures;
        std::string directory;

        // Load Model File
        void loadModel(const std::string modelFilePath, const unsigned int processFlags);

        // Process All The Nodes in A Model
        void processNode(const aiNode* node, const aiScene* scene);

        // Process All The Meshes in A Model
        Mesh processMesh(const aiMesh* mesh, const aiScene* scene);

        // Load Model Material Textures
        std::vector<TextureStruct> loadMaterialTextures(aiMaterial* material, aiTextureType textureType, std::string textureTypeName);
};
