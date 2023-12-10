#version 460 core

// Vertex Attributes
layout(location = 0) in vec3 vertexPos;

// Uniform Variables
// Model of Projection and View Matrices (In Respective Order)
uniform mat4 pv;

// Output
out vec3 texCords;

// Main
void main(void) {
    // Calculate and Set Final Vertex Position
    vec4 pos = pv * vec4(vertexPos, 1.0f);
    gl_Position = pos.xyww;

    // Send Texture Coordinates to Fragment Shader
    texCords = vertexPos;
}
