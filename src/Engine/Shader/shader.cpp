#include "Shader.hpp"

// Constructor - Create Shaders
Shader::Shader(const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath) {
    // Shaders
    GLuint vertexShader{this->readShaderSrcFile(vertexShaderSourceFilePath, GL_VERTEX_SHADER)};
    GLuint fragmentShader{this->readShaderSrcFile(fragmentShaderSourceFilePath, GL_FRAGMENT_SHADER)};

    // Attach Shaders to Shader Program and Link Shader Program
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glLinkProgram(this->shaderProgram);

    // Check For Shader Program Link Errors
    int success;
    char infoLog[1024];
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        // Get Shader Program Info Log
        glGetProgramInfoLog(this->shaderProgram, 1024, nullptr, infoLog);

        // Display Error Message and Info Log
        std::cerr << "Shader Program Link Error!\n";
        std::cerr << "Shader Program Info Log: " << infoLog << "\n\n";
    }

    // Detach Shaders From Shader Program and Delete Them
    glDetachShader(this->shaderProgram, vertexShader);
    glDetachShader(this->shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
Shader::Shader(const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath, const std::string geometryShaderSourceFilePath) {
    // Shaders
    GLuint vertexShader{this->readShaderSrcFile(vertexShaderSourceFilePath, GL_VERTEX_SHADER)};
    GLuint fragmentShader{this->readShaderSrcFile(fragmentShaderSourceFilePath, GL_FRAGMENT_SHADER)};
    GLuint geometryShader{this->readShaderSrcFile(geometryShaderSourceFilePath, GL_GEOMETRY_SHADER)};

    // Attach Shaders to Shader Program and Link Shader Program
    this->shaderProgram = glCreateProgram();
    glAttachShader(this->shaderProgram, vertexShader);
    glAttachShader(this->shaderProgram, fragmentShader);
    glAttachShader(this->shaderProgram, geometryShader);
    glLinkProgram(this->shaderProgram);

    // Check For Shader Program Link Errors
    int success;
    char infoLog[1024];
    glGetProgramiv(this->shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        // Get Shader Program Info Log
        glGetProgramInfoLog(this->shaderProgram, 1024, nullptr, infoLog);

        // Display Error Message and Info Log
        std::cerr << "Shader Program Link Error!\n";
        std::cerr << "Shader Program Info Log: " << infoLog << "\n\n";
    }

    // Detach Shaders From Shader Program and Delete Them
    glDetachShader(this->shaderProgram, vertexShader);
    glDetachShader(this->shaderProgram, fragmentShader);
    glDetachShader(this->shaderProgram, geometryShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(geometryShader);
}

// Create A Shader Using A Shader Source File
GLuint Shader::readShaderSrcFile(const std::string &shaderSourceFilePath, GLenum shaderType) {
    // Variables
    std::ifstream shaderSourceFile;
    std::string shaderCppSource;

    // Set Shader Source File Exceptions
    shaderSourceFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    // Open and Read A Shader Source File - Catch Any File Exceptions
    try {
        // Open Shader Source File
        shaderSourceFile.open(shaderSourceFilePath, std::ios::in | std::ios::binary);

        // Read Shader Source File Into C++ String
        shaderSourceFile.seekg(0, std::ios::end);
        shaderCppSource.resize(shaderSourceFile.tellg());
        shaderSourceFile.seekg(0, std::ios::beg);
        shaderSourceFile.read(&shaderCppSource[0], shaderCppSource.size());

        // Close Shader Source File
        shaderSourceFile.close();
    } catch(std::ifstream::failure &error) {
        std::cerr << "Failed to Open and Read Shader Source File!\n";
        std::cerr << "Error Message: " << error.what() << "\n\n";
    }

    // Convert Shader Source in C++ String to C String
    const char* shaderSource = shaderCppSource.c_str();

    // Shader
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    // Check For Shader Compilation Errors
    int success;
    char infoLog[1024];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        // Get Shader Info Log
        glGetShaderInfoLog(shader, 1024, nullptr, infoLog);

        // Display Error Message and Shader Info Log
        std::cerr << "Failed to Compile Shader!\n";
        std::cerr << "Shader Info Log: " << infoLog << "\n\n";
    }

    // Return Shader
    return shader;
}
