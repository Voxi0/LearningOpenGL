#version 460 core

// Vertex Attributes
layout(location = 0) in vec3 vertexPos;

// Uniform Variables
// Product of Light Model, View and Projection Matrices )In Respective Order)
uniform mat4 lightPVM;

// Main
void main(void) {
    // Calculate and Set Final Vertex Position
    gl_Position = lightPVM * vec4(vertexPos, 1.0f);
}
