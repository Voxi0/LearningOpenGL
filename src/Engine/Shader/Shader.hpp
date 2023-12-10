#pragma once

// Standard Headers
#include<iostream>
#include<fstream>

// GLAD OpenGL Loader
#include<glad/glad.h>

// OpenGL Mathematics (GLM)
#include<glm/glm.hpp>

// Shader Class
class Shader {
    public:
        // Constructor - Create Shaders
        Shader(const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath);
        Shader(const std::string &vertexShaderSourceFilePath, const std::string &fragmentShaderSourceFilePath, const std::string geometryShaderSourceFilePath);

        // Use Shaders
        void use(void) {glUseProgram(this->shaderProgram);}

        // Uniform Variable Setters
        // Boolean
        void setBool(const std::string &uniformVariableName, const bool &value) {
            glUniform1i(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), (int)value);
        }

        // Integer
        void setInt(const std::string &uniformVariableName, const int &value) {
            glUniform1i(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), value);
        }

        // Float
        void setFloat(const std::string &uniformVariableName, const float &value) {
            glUniform1f(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), value);
        }

        // Double
        void setDouble(const std::string &uniformVariableName, const double &value) {
            glUniform1d(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), value);
        }

        // Vec2
        void setVec2(const std::string &uniformVariableName, const glm::vec2 &value) {
            glUniform2fv(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string &uniformVariableName, const float &a, const float &b) {
            glUniform2f(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), a, b);
        }

        // Vec3
        void setVec3(const std::string &uniformVariableName, const glm::vec3 &value) {
            glUniform3fv(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string &uniformVariableName, const float &a, const float &b, const float &c) {
            glUniform3f(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), a, b, c);
        }

        // Vec4
        void setVec4(const std::string &uniformVariableName, const glm::vec4 &value) {
            glUniform4fv(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string &uniformVariableName, const float &a, const float &b, const float &c, const float &d) {
            glUniform4f(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), a, b, c, d);
        }

        // Mat4
        void setMat4(const std::string &uniformVariableName, const glm::mat4 &value) {
            glUniformMatrix4fv(glGetUniformLocation(this->shaderProgram, uniformVariableName.c_str()), 1, GL_FALSE, &value[0][0]);
        }

        // Delete Shaders
        void destroy(void) {glDeleteProgram(this->shaderProgram);}
    private:
        // Shader Program
        GLuint shaderProgram{};

        // Create A Shader Using A Shader Source File
        GLuint readShaderSrcFile(const std::string &shaderSourceFilePath, GLenum shaderType);
};
