#define STB_IMAGE_IMPLEMENTATION
#include"Texture.hpp"

// Create 2D Image Texture
void Texture::create2DTexture(const std::string textureImageFilePath) {
    // Create A 2D Texture Object
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_2D, this->texture);

    // Flip Texture Images Vertically Upon Loading Them
    stbi_set_flip_vertically_on_load(true);

    // Load Texture Image File
    int textureWidth, textureHeight, textureNrChannels;
    unsigned char* textureImage = stbi_load(textureImageFilePath.c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0);
    if(textureImage) {
        // Figure Out Texture Image File Format
        GLenum textureFormat{};
        switch(textureNrChannels) {
            case 3:
                textureFormat = GL_RGB;
                break;
            case 4:
                textureFormat = GL_RGBA;
                break;
        }

        // Load Texture Image Data Into 2D Texture Object
        glTexImage2D(GL_TEXTURE_2D, 0, textureFormat, textureWidth, textureHeight, 0, textureFormat, GL_UNSIGNED_BYTE, textureImage);

        // Generate Texture Mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        // Delete Texture Object and Display Error Message
        glDeleteTextures(1, &this->texture);
        std::cerr << "Failed to Load Texture Image File! Check File Path and Try Again.\n";
    }

    // Free Texture Image Data From Memory
    stbi_image_free(textureImage);

    // Unbind 2D Texture Object
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Create A Cubemap
void Texture::createCubemap(const std::vector<std::string> textureImagesFilePaths) {
    // Create A Cubemap Texture Object
    glGenTextures(1, &this->texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);

    // Don't Flip Texture Images Vertically Upon Loading Them
    stbi_set_flip_vertically_on_load(false);

    // Load Cubemap Texture Files
    int textureWidth, textureHeight, textureNrChannels;
    for(unsigned int i = 0; i < textureImagesFilePaths.size(); i++) {
        // Open Cubemap Texture File
        unsigned char* textureImage = stbi_load(textureImagesFilePaths[i].c_str(), &textureWidth, &textureHeight, &textureNrChannels, 0);

        // Check if Texture Image File Was Loaded Successfully or Not
        if(textureImage) {
            // Figure Out Texture Image File's Format
            GLenum textureFormat{};
            switch(textureNrChannels) {
                case 3:
                    textureFormat = GL_RGB;
                    break;
                case 4:
                    textureFormat = GL_RGBA;
                    break;
            }

            // Load Texture Image Data Into Cubemap Texture Object
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, textureFormat, textureWidth, textureHeight, 0, textureFormat, GL_UNSIGNED_BYTE, textureImage);
        } else {
            // Display Error Message and Delete Cubemap Texture Object
            glDeleteTextures(1, &this->texture);
            std::cerr << "Failed to Load All Cubemap Texture Images! Check Filepaths and Try Again.\n";
        }
        
        // Free Texture Image Data From Memory
        stbi_image_free(textureImage);
    }

    // Unbind Cubemap Texture Object
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
