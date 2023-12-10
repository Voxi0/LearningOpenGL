// Standard Headers
#include<iostream>
#include<cstdlib>

// GLAD OpenGL Loader
#include<glad/glad.h>

// GLFW
#include<GLFW/glfw3.h>

// OpenGL Mathematics (GLM)
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

// Assimp (Open Asset Importer)
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/postprocess.h>

// Custom Engine Headers
#include"Engine/Shader/Shader.hpp"
#include"Engine/Camera/Camera.hpp"
#include"Engine/Texture/Texture.hpp"
#include"Engine/Framebuffer/Framebuffer.hpp"
#include"Engine/Model/Model.hpp"

// Function Prototypes
// Process User Input
void processInput(GLFWwindow* win);

// GLFW Callbacks
void ErrorCallback(int errorCode, const char* errorDesc) {
    // Display GLFW Error Code and Description
    std::cerr << "GLFW Error " << errorCode << ": " << errorDesc << "\n";
}
void FramebufferSizeCallback(GLFWwindow*, int newWindowWidth, int newWindowHeight) {
    // Resize OpenGL Render Viewport Whenever Main Window is Resized
    glViewport(0, 0, newWindowWidth, newWindowHeight);
}
void MouseCallback(GLFWwindow*, double xPos, double yPos);
void ScrollCallback(GLFWwindow*, double xPos, double yPos);

// Variables
// Game Config
const float moveSpeed{2.5f}, mouseSensitivity{0.4f};
const bool vsyncEnabled{true}, msaaEnabled{true};
const unsigned int msaaSamples{4};

// Main Window Config
const std::string windowTitle = "LearningOpenGL";
unsigned int windowWidth{800}, windowHeight{600};
const bool windowFullscreen{true};

// Delta Time
float lastTime, currentTime, deltaTime;

// Perspective Camera
Camera perspectiveCamera(glm::vec3(0.0f, 0.0f, 5.0f), moveSpeed, mouseSensitivity, 70.0f, 0.1f, 120.0f);

// Test Object Config
const float testObjectShininess{256.0f};

// Spotlight Config
const float lightInnerCutOff{15.0f}, lightOuterCutOff{17.0f};
const glm::vec3 lightAmbientIntensity{glm::vec3(0.2f)}, lightDiffusionIntensity{glm::vec3(0.5f)}, lightSpecularIntensity{glm::vec3(1.0f)};
const float lightAttenuationConstant{1.0f}, lightAttenuationLinear{0.045f}, lightAttenuationQuadratic{0.0075f};

// Depth Map (For Shadows) Config
const unsigned int shadowMapWidth{1024}, shadowMapHeight{1024};

// Main
int main(void) {
    // Set GLFW Error Callback
    glfwSetErrorCallback(ErrorCallback);

    // Initialize GLFW
    if(!glfwInit()) return EXIT_FAILURE;

    // Set GLFW Window Hints
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);

    // Main Window
    GLFWwindow* win = nullptr;
    switch(windowFullscreen) {
        case true:
            // Set Window Dimensions to Primary Monitor's Max Dimensions
            windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
            windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

            // Create Main Window (Borderless Fullscreen)
            win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);

            // Break
            break;
        default:
            // Create Main Window (Bordered Windowed)
            win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);

            // Break
            break;
    }

    // Check if Main Window Was Created Successfully or Not
    if(win == nullptr) {
        // Terminate GLFW and Return Error Exit Code
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set Current OpenGL Render Context
    glfwMakeContextCurrent(win);

    // Initialize GLAD OpenGL Loader
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        // Display Error Message
        std::cerr << "Failed to Initialize GLAD OpenGL Loader!\n";

        // Destroy Main Window and Terminate GLFW
        glfwDestroyWindow(win);
        glfwTerminate();

        // Return Error Exit Code
        return EXIT_FAILURE;
    }

    // Set GLFW Callbacks
    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);
    glfwSetCursorPosCallback(win, MouseCallback);
    glfwSetScrollCallback(win, ScrollCallback);
    
    // Set GLFW Input Mode
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable Vertical Synchronization (Vsync)
    vsyncEnabled ? glfwSwapInterval(1) : glfwSwapInterval(0);

    // Enable/Disable OpenGL Functions
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Quad Data
    const std::vector<GLfloat> quadData {
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f
    };
    const std::vector<GLuint> quadIndices {
        0, 2, 1,
        2, 3, 1,
    };

    // Objects
    // Vertex Array Object - Quad Object
    GLuint quadObject;
    glGenVertexArrays(1, &quadObject);
    glBindVertexArray(quadObject);

    // Vertex Buffer Object - Quad VBO
    GLuint quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, quadData.size() * sizeof(GLfloat), &quadData[0], GL_STATIC_DRAW);

    // Element/Index Buffer Object - Quad EBO
    GLuint quadEBO;
    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, quadIndices.size() * sizeof(GLuint), &quadIndices[0], GL_STATIC_DRAW);

    // Vertex Attributes
    // Vertex Position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // Framebuffers
    // Multisampled Framebuffer
    Framebuffer msaaFramebuffer(windowWidth, windowHeight, true, 4);
    msaaFramebuffer.createColorBufferTexture();
    msaaFramebuffer.createRenderbuffer(GL_DEPTH24_STENCIL8);

    // Post-Processing Framebuffer
    Framebuffer postProcessingFramebuffer(windowWidth, windowHeight, false, 0);
    postProcessingFramebuffer.createColorBufferTexture();

    // Depth Map (For Shadows)
    Framebuffer depthMap(shadowMapWidth, shadowMapHeight, false, 0);
    depthMap.createShadowMap();

    // Models
    Model testObject("../assets/models/survivalBackpack/backpack.obj", aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);
    Model cubeObject("../assets/models/cube.obj", aiProcess_FlipUVs | aiProcess_PreTransformVertices | aiProcess_JoinIdenticalVertices | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph);

    // Shaders
    Shader framebufferShaders("../src/shaders/framebufferShaders/vertexShader.glsl", "../src/shaders/framebufferShaders/fragmentShader.glsl");
    Shader shadowMapShaders("../src/shaders/shadowMapShaders/vertexShader.glsl", "../src/shaders/shadowMapShaders/fragmentShader.glsl");
    Shader skyboxShaders("../src/shaders/skyboxShaders/vertexShader.glsl", "../src/shaders/skyboxShaders/fragmentShader.glsl");
    Shader testShaders("../src/shaders/testShaders/vertexShader.glsl", "../src/shaders/testShaders/fragmentShader.glsl", "../src/shaders/testShaders/geometryShader.glsl");

    // Textures
    // Skybox Texture
    const std::vector<std::string> skyboxTextureFilePaths{
        "../assets/textures/skybox/right.jpg",
        "../assets/textures/skybox/left.jpg",
        "../assets/textures/skybox/top.jpg",
        "../assets/textures/skybox/bottom.jpg",
        "../assets/textures/skybox/front.jpg",
        "../assets/textures/skybox/back.jpg",
    };
    Texture skyboxTexture;
    skyboxTexture.createCubemap(skyboxTextureFilePaths);
    skyboxTexture.bindCubemap();
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    // Model and Projection Matrices
    glm::mat4 modelMatrix, projectionMatrix;

    // Main Loop
    while(!glfwWindowShouldClose(win)) {
        // Swap Buffers
        glfwSwapBuffers(win);

        // Projection Matrix
        projectionMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::perspective(glm::radians(perspectiveCamera.getCamFOV()), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);

        // Enable Depth Testing
        glEnable(GL_DEPTH_TEST);

        // Bind Depth Map
        glViewport(0, 0, shadowMapWidth, shadowMapHeight);
        shadowMapShaders.use();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, depthMap.getFramebufferObject());

        // Clear Depth Buffer Bit
        glClear(GL_DEPTH_BUFFER_BIT);

        // Send Light Model, View and Projection Matrices to Vertex Shader

        // Unbind Framebuffer Object
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // Bind Multisampled Framebuffer Object
        glViewport(0, 0, windowWidth, windowHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, msaaFramebuffer.getFramebufferObject());

        // Clear Screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Prepare to Render Test Object
        testShaders.use();

        // Test Object's Model Matrix
        modelMatrix = glm::mat4(1.0f);

        // Send Model Matrix and Product of Projection, View and Model Matrix (In Respective Order) to Vertex Shader
        testShaders.setMat4("modelMatrix", modelMatrix);
        testShaders.setMat4("pvm", projectionMatrix * perspectiveCamera.getViewMatrix() * modelMatrix);

        // Send Test Object's Material Properties to Fragment Shader
        testShaders.setFloat("material.shininess", 256.0f);

        // Send Spotlight Properties to Test Object's Fragment Shader
        // Position and Direction
        testShaders.setVec3("light.position", perspectiveCamera.getCamPos());
        testShaders.setVec3("light.direction", -perspectiveCamera.getCamFront());

        // Inner and Outer Cut Off (Light Radius/Size)
        testShaders.setFloat("light.innerCutOff", glm::cos(glm::radians(lightInnerCutOff)));
        testShaders.setFloat("light.outerCutOff", glm::cos(glm::radians(lightOuterCutOff)));

        // Intensity Values
        testShaders.setVec3("light.ambientIntensity", lightAmbientIntensity);
        testShaders.setVec3("light.diffusionIntensity", lightDiffusionIntensity);
        testShaders.setVec3("light.specularIntensity", lightSpecularIntensity);

        // Light Attenuation Properties
        testShaders.setFloat("light.attenuationConstant", lightAttenuationConstant);
        testShaders.setFloat("light.attenuationLinear", lightAttenuationLinear);
        testShaders.setFloat("light.attenuationQuadratic", lightAttenuationQuadratic);

        // Render Test Object
        testObject.render(testShaders);

        // Prepare to Render Skybox Using Cube Object
        glDepthFunc(GL_LEQUAL);
        skyboxShaders.use();

        // Set Skybox Object Texture
        glActiveTexture(GL_TEXTURE0);
        skyboxTexture.bindCubemap();

        // Send Product of Projection and View Matrices to Vertex Shader (In Respective Order)
        skyboxShaders.setMat4("pv", projectionMatrix * glm::mat4(glm::mat3(perspectiveCamera.getViewMatrix())));

        // Render Skybox Texture Object
        cubeObject.render(skyboxShaders);
        glDepthFunc(GL_LESS);

        // Copy Everything From MSAA Framebuffer to Intermediary Framebuffer (Downscale)
        glBindFramebuffer(GL_READ_FRAMEBUFFER, msaaFramebuffer.getFramebufferObject());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFramebuffer.getFramebufferObject());
        glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        // Unbind Framebuffer Object and Disable Depth Testing
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        // Clear Default Framebuffer
        glClear(GL_COLOR_BUFFER_BIT);

        // Prepare to Render Quad Object
        glBindVertexArray(quadObject);
        framebufferShaders.use();

        // Bind Screen Texture to Quad Object
        glActiveTexture(GL_TEXTURE0);
        postProcessingFramebuffer.bindColorBufferTexture();

        // Render Quad Object
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind Vertex Array Object
        glBindVertexArray(0);

        // Calculate Delta Time
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Process User Input
        processInput(win);

        // Poll Events
        glfwPollEvents();
    }

    // Terminate Program
    // Delete Vertex Array Objects
    glDeleteVertexArrays(1, &quadObject);

    // Delete Vertex Buffer Objects
    glDeleteBuffers(1, &quadVBO);

    // Delete Element/Index Buffer Objects
    glDeleteBuffers(1, &quadEBO);

    // Delete Framebuffers
    msaaFramebuffer.destroy(true);
    postProcessingFramebuffer.destroy(false);

    // Delete Models
    testObject.destroy();
    cubeObject.destroy();

    // Delete Shaders
    framebufferShaders.destroy();
    skyboxShaders.destroy();
    testShaders.destroy();

    // Delete Textures
    skyboxTexture.destroy();

    // Destroy Main Window and Terminate GLFW
    glfwDestroyWindow(win);
    win = nullptr;
    glfwTerminate();
    return EXIT_SUCCESS;
}

// Process User Input
void processInput(GLFWwindow* win) {
    // Escape Key Pressed (Quit)
    if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, true);
    }

    // Process Camera Movement
    perspectiveCamera.processKeyboard(win, deltaTime);
}

// Process Mouse Movement
void MouseCallback(GLFWwindow*, double xPos, double yPos) {
    perspectiveCamera.processMouseMovement(xPos, yPos);
}

// Process Mouse Scroll
void ScrollCallback(GLFWwindow*, double, double yPos) {
    perspectiveCamera.processMouseScroll(yPos);
}
