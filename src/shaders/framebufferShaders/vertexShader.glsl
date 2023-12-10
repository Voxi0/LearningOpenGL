#version 460 core

// Vertex Attributes
layout(location = 0) in vec2 vertexPos;
layout(location = 1) in vec2 textureCords;

// Output
out vec2 texCords;

// Main
void main(void) {
    // Calculate and Set Final Vertex Position
    gl_Position = vec4(vertexPos.x, vertexPos.y, 0.0f, 1.0f);

    // Send Texture Coordinates to Fragment Shader
    texCords = textureCords;
}
