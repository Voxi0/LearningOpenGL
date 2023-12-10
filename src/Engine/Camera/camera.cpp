#include"Camera.hpp"

// Constructor - Initialize The Camera
Camera::Camera(const glm::vec3 &camPos, const float &moveSpeed, const float &mouseSensitivity, const float &camFOV, const float &camMinFOV, const float &camMaxFOV) {
    // Set Camera Position
    this->camPos = camPos;

    // Set Movement Speed and Look Speed (Mouse Sensitivity)
    this->moveSpeed = moveSpeed;
    this->lookSpeed = mouseSensitivity;

    // Set FOV Values
    this->camFOV = camFOV;
    this->camMinFOV = camMinFOV;
    this->camMaxFOV = camMaxFOV;
}

// Process Keyboard Input - Camera Movement
void Camera::processKeyboard(GLFWwindow* win, const float &deltaTime) {
    // Camera Movement Code
    const float camSpeed = this->moveSpeed * deltaTime;
    if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        this->camPos += camSpeed * this->camFront;
    } if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        this->camPos -= camSpeed * this->camFront;
    } if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
        this->camPos -= glm::normalize(glm::cross(this->camFront, this->camUp)) * camSpeed;
    } if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
        this->camPos += glm::normalize(glm::cross(this->camFront, this->camUp)) * camSpeed;
    }
}

// Process Mouse Movement - Calculate Camera Look Direction
void Camera::processMouseMovement(float mousePosX, float mousePosY) {
    // Check if Mouse Entered Window For The First Time
    if(this->firstMouse) {
        // Set Last X and Y Position
        this->lastX = mousePosX;
        this->lastY = mousePosY;

        // Mouse Has Now Entered Window For The First Time
        this->firstMouse = false;
    }

    // Calculate Mouse Offset Values
    float xOffset{mousePosX - this->lastX};
    float yOffset{this->lastY - mousePosY};

    // Update Mouse Last X and Y Position Values
    this->lastX = mousePosX;
    this->lastY = mousePosY;

    // Apply Mouse Sensitivity to Mouse Offset Values
    xOffset *= this->lookSpeed;
    yOffset *= this->lookSpeed;

    // Calculate Camera Yaw and Pitch Values
    this->camYaw += xOffset;
    this->camPitch += yOffset;

    // Constrain Camera Pitch Values so it Doesn't Flip Vertically
    if(this->camPitch < -89.0f) {
        this->camPitch = -89.0f;
    } if(this->camPitch > 89.0f) {
        this->camPitch = 89.0f;
    }

    // Calculate New Camera Direction
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(this->camYaw)) * glm::cos(glm::radians(this->camPitch));
    direction.y = glm::sin(glm::radians(this->camPitch));
    direction.z = glm::sin(glm::radians(this->camYaw)) * glm::cos(glm::radians(this->camPitch));

    // Set New Camera Direction
    this->camFront = glm::normalize(direction);
}

// Process Mouse Scroll
void Camera::processMouseScroll(const double scrollPosY) {
    this->camFOV -= (float)scrollPosY;
    if(this->camFOV > this->camMaxFOV) {
        this->camFOV = this->camMaxFOV;
    } if(this->camFOV < this->camMinFOV) {
        this->camFOV = this->camMinFOV;
    }
}
