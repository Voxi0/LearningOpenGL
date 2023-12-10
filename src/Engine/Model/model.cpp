#include"Model.hpp"

 // Load Model File
void Model::loadModel(const std::string modelFilePath, const unsigned int processFlags) {
    // Model Importer
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(modelFilePath, processFlags);

    // Check if Model Was Successfully Imported From Model File or Not
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        // Display Error Message
        std::cerr << "Failed to Load Model File!\n";
        std::cerr << "Error Desc: " << importer.GetErrorString() << "\n\n";
        return;
    }

    // Figure Out The Full Directory of Where The Model File is Located
    this->directory = modelFilePath.substr(0, modelFilePath.find_last_of('/'));

    // Process All The Nodes in The Model
    this->processNode(scene->mRootNode, scene);
}

// Process All The Nodes in A Model
void Model::processNode(const aiNode* node, const aiScene* scene) {
    // Process All The Node's Meshes (If Any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        this->meshes.push_back(processMesh(mesh, scene));
    }

    // Process All The Children Nodes (If Any)
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        this->processNode(node->mChildren[i], scene);
    }
}

// Process All The Meshes in A Model
Mesh Model::processMesh(const aiMesh* mesh, const aiScene* scene) {
    // Mesh Data
    std::vector<VertexStruct> vertices;
    std::vector<unsigned int> indices;
    std::vector<TextureStruct> textures;

    // Process All The Vertices
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        VertexStruct vertex;
        glm::vec3 vec;

        // Vertex Positions
        vec.x = mesh->mVertices[i].x;
        vec.y = mesh->mVertices[i].y;
        vec.z = mesh->mVertices[i].z;
        vertex.vertexPos = vec;

        // Normal Vectors
        if(mesh->HasNormals()) {
            vec.x = mesh->mNormals[i].x;
            vec.y = mesh->mNormals[i].y;
            vec.z = mesh->mNormals[i].z;
            vertex.normalVec = vec;
        }

        // Texture Coordinates
        if(mesh->mTextureCoords[0]) {
            glm::vec2 texVec;
            texVec.x = mesh->mTextureCoords[0][i].x;
            texVec.y = mesh->mTextureCoords[0][i].y;
            vertex.texCords = texVec;
        } else {
            vertex.texCords = glm::vec2(0.0f);
        }

        vertices.push_back(vertex);
    }

    // Process All Indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // Process All Materials
    if(mesh->mMaterialIndex > 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        std::vector<TextureStruct> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<TextureStruct> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    }

    // Return Mesh
    return Mesh(vertices, indices, textures);
}

// Load Material Textures
std::vector<TextureStruct> Model::loadMaterialTextures(aiMaterial *material, aiTextureType textureType, std::string textureTypeName) {
    std::vector<TextureStruct> textures;
    for(unsigned int i = 0; i < material->GetTextureCount(textureType); i++) {
        aiString str;
        material->GetTexture(textureType, i, &str);
        bool skip = false;
        for(unsigned int j = 0; j < this->loadedTextures.size(); j++) {
            if(std::strcmp(this->loadedTextures[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(this->loadedTextures[j]);
                skip = true; 
                break;
            }
        }
        if(!skip) {
            // If texture Hasn't Been Loaded Already, Load it
            TextureStruct texture;
            Texture textureImage;
            textureImage.create2DTexture(this->directory + '/' + str.C_Str());
            textureImage.bind2DTexture();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            texture.id = textureImage.getTextureObject();
            texture.type = textureTypeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            this->loadedTextures.push_back(texture); // add to loaded textures
        }
    }

    return textures;
}
