#version 460 core

// Vertex Attributes
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec3 normalVec;
layout(location = 2) in vec2 textureCords;

// Uniform Variables
uniform mat4 modelMatrix;

// Product of Projection, View and Model Matrix (In Respective Order)
uniform mat4 pvm;

// Output to Geometry Shader
out vertexShaderOUT {
    vec2 textureCoordinates;
    vec3 normalVector, fragmentPosition;
} vertex_shader_out;

// Main
void main(void) {
    // Calculate and Set Final Vertex Position
    gl_Position = pvm * vec4(vertexPos, 1.0f);

    // Send Texture Coordinates, Normal Vector and Fragment Position to Geometry Shader
    vertex_shader_out.textureCoordinates = textureCords;
    vertex_shader_out.normalVector = normalVec;
    vertex_shader_out.fragmentPosition = vec3(modelMatrix * vec4(vertexPos, 1.0f));
}
