#pragma once

// Standard Headers
#include<iostream>

// GLAD OpenGL Loader
#include<glad/glad.h>

// Custom Engine Headers
#include"../Texture/Texture.hpp"

// Framebuffer Class
class Framebuffer {
    public:
        // Constructor - Create A New Framebuffer Object
        Framebuffer(const unsigned int &windowWidth, const unsigned int &windowHeight, const bool &msaaEnabled, const unsigned int &msaaSamples);

        // Create Color Buffer Texture
        void createColorBufferTexture();

        // Create A Renderbuffer Object (Depth and Stencil Buffer)
        void createRenderbuffer(const GLenum &renderbufferType);

        // Create Shadow Map
        void createShadowMap();

        // Bind Color Buffer Texture
        void bindColorBufferTexture(void) {
            this->msaaEnabled ? glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->colorBufferTexture) : glBindTexture(GL_TEXTURE_2D, this->colorBufferTexture);
        }

        // Getters
        GLuint getFramebufferObject(void) {return this->framebuffer;}

        // Destroy
        void destroy(const bool &usingRenderbuffer);
    private:
        // Window Properties
        unsigned int windowWidth, windowHeight;

        // MSAA
        bool msaaEnabled;
        unsigned int msaaSamples;

        // Framebuffer Object
        GLuint framebuffer;

        // Color Buffer Texture
        GLuint colorBufferTexture;

        // Renderbuffer Object (Depth and Stencil Buffer)
        GLuint renderbufferObject;

        // Depth Buffer Texture
        GLuint depthBufferTexture;
};
