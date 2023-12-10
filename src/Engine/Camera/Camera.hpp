#pragma once

// GLFW
#include<GLFW/glfw3.h>

// OpenGL Mathematics (GLM)
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

// Camera Class
class Camera {
    public:
        // Constructor - Initialize The Camera
        Camera(const glm::vec3 &camPos, const float &moveSpeed, const float &mouseSensitivity, const float &camFOV, const float &camMinFOV, const float &camMaxFOV);
    
        // Process Keyboard Input - Camera Movement
        void processKeyboard(GLFWwindow* win, const float &deltaTime);

        // Process Mouse Movement - Calculate Camera Look Direction
        void processMouseMovement(float mousePosX, float mousePosY);

        // Process Mouse Scroll
        void processMouseScroll(const double scrollPosY);

        // Getters
        glm::vec3 getCamPos(void) {return this->camPos;}
        glm::vec3 getCamFront(void) {return this->camFront;}
        glm::mat4 getViewMatrix(void) {return glm::lookAt(this->camPos, this->camPos + this->camFront, this->camUp);}
        float &getCamFOV(void) {return this->camFOV;}
    private:
        // Movement and Look Speed (Mouse Sensitivity)
        float moveSpeed, lookSpeed;

        // Camera Position, Front and Up Vectors
        glm::vec3 camPos, camFront{0.0f, 0.0f, -1.0f}, camUp{0.0f, 1.0f, 0.0f};

        // Camera FOV
        float camFOV, camMinFOV, camMaxFOV;

        // Mouse Just Entered Window
        bool firstMouse{true};

        // Camera Yaw and Pitch
        float camYaw{-90.0f}, camPitch;

        // Camera Last X and Y Values
        float lastX, lastY;
};
