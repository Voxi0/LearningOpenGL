#version 460 core

// Input Primitive
layout(triangles) in;

// Output Primitive
layout(triangle_strip, max_vertices = 3) out;

// Input From Vertex Shader
in vertexShaderOUT {
    vec2 textureCoordinates;
    vec3 normalVector, fragmentPosition;
} geometry_shader_in[];

// Output to Fragment Shader
out vec2 texCords;
out vec3 normalVec, fragPos;

// Main
void main(void) {
    // Loop Through All The Vertices
    for(int i = 0; i != 3; i++) {
        gl_Position = gl_in[i].gl_Position;
        normalVec = geometry_shader_in[i].normalVector;
        texCords = geometry_shader_in[i].textureCoordinates;
        fragPos = geometry_shader_in[i].fragmentPosition;
        EmitVertex();
    }

    // End Primitive
    EndPrimitive();
}
