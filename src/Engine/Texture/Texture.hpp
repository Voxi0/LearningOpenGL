#pragma once

// Standard Headers
#include<iostream>
#include<vector>

// GLAD OpenGL Loader
#include<glad/glad.h>

// STB Image
#include<stb_image.h>

// Texture Class
class Texture {
    public:
        // Create 2D Image Texture
        void create2DTexture(const std::string textureImageFilePath);

        // Create A Cubemap
        void createCubemap(const std::vector<std::string> textureImagesFilePaths);

        // Getters
        GLuint getTextureObject(void) {return this->texture;}

        // Binders
        void bind2DTexture(void) {glBindTexture(GL_TEXTURE_2D, this->texture);}
        void bindCubemap(void) {glBindTexture(GL_TEXTURE_CUBE_MAP, this->texture);}

        // Delete Texture Object
        void destroy(void) {glDeleteTextures(1, &this->texture);}
    private:
        // Texture Object
        GLuint texture;
};
