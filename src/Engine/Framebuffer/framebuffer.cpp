#include"Framebuffer.hpp"

// Constructor - Create A New Framebuffer Object
Framebuffer::Framebuffer(const unsigned int &windowWidth, const unsigned int &windowHeight, const bool &msaaEnabled, const unsigned int &msaaSamples) {
    // Set Window Properties
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    // Set MSAA Properties
    this->msaaEnabled = msaaEnabled;
    this->msaaSamples = msaaSamples;

    // Create A New Framebuffer Object
    glGenFramebuffers(1, &this->framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    // Unbind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Create Color Buffer Texture
void Framebuffer::createColorBufferTexture() {
    // Bind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    // Create A Color Buffer Texture and Attach it to Framebuffer Object
    glGenTextures(1, &this->colorBufferTexture);
    switch(msaaEnabled) {
        case true:
            // Set MSAA Properties
            this->msaaEnabled = true;
            this->msaaSamples = msaaSamples;
            
            // Create an Empty Multisampled Color Buffer Texture
            glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, this->colorBufferTexture);
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, this->msaaSamples, GL_RGB, this->windowWidth, this->windowHeight, GL_TRUE);

            // Texture Parameters
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Attach Color Buffer Texture to Framebuffer Object
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, this->colorBufferTexture, 0);
            break;
        default:
            // Set MSAA Properties
            this->msaaEnabled = false;

            // Create an Empty Color Buffer Texture
            glBindTexture(GL_TEXTURE_2D, this->colorBufferTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->windowWidth, this->windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

            // Texture Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

            // Attach Color Buffer Texture to Framebuffer Object
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->colorBufferTexture, 0);
            break;
    }

    // Unbind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Create A Renderbuffer Object (Depth and Stencil Buffer)
void Framebuffer::createRenderbuffer(const GLenum &renderbufferType) {
    // Bind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);

    // Create A Renderbuffer Object
    glGenRenderbuffers(1, &this->renderbufferObject);
    glBindRenderbuffer(GL_RENDERBUFFER, this->renderbufferObject);
    if(this->msaaEnabled) {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, this->msaaSamples, renderbufferType, this->windowWidth, this->windowHeight);
    } else {
        glRenderbufferStorage(GL_RENDERBUFFER, renderbufferType, this->windowWidth, this->windowHeight);
    }

    // Attach Renderbuffer Object to Framebuffer
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->renderbufferObject);

    // Check if Framebuffer is Complete or Not
    if(glCheckFramebufferStatus(this->framebuffer) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to Create Framebuffer!\n";
        std::cerr << "Status: " << glCheckFramebufferStatus(this->framebuffer);
    }

    // Unbind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Create Depth Buffer Texture
void Framebuffer::createShadowMap() {
    // Generate A Depth Map Texture Object
    glGenTextures(1, &this->depthBufferTexture);
    glBindTexture(GL_TEXTURE_2D, this->depthBufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, this->windowWidth, this->windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

    // Texture Parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Attach Depth Map Texture to Framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->depthBufferTexture, 0);
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);

    // Check if Framebuffer is Complete or Not
    if(glCheckFramebufferStatus(this->framebuffer) != GL_FRAMEBUFFER_COMPLETE) {
        std::cerr << "Failed to Create Framebuffer!\n";
        std::cerr << "Status: " << glCheckFramebufferStatus(this->framebuffer);
    }

    // Unbind Framebuffer Object
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Destroy
void Framebuffer::destroy(const bool &usingRenderbuffer) {
    // Delete Framebuffer Object
    glDeleteFramebuffers(1, &this->framebuffer);

    // Delete Color Buffer Texture
    glDeleteTextures(1, &this->colorBufferTexture);

    // Delete Renderbuffer Object (If Using it)
    if(usingRenderbuffer) {
        glDeleteRenderbuffers(1, &this->renderbufferObject);
    } else {
        glDeleteTextures(1, &this->depthBufferTexture);
    }
}
