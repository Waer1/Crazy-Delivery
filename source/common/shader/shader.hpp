#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle (OpenGL object name)
        GLuint program;

    public:
        ShaderProgram(){
            //TODO: (Req 1) Create A shader program
						program = glCreateProgram();
        }
        ~ShaderProgram(){
            //TODO: (Req 1) Delete a shader program
						glDeleteProgram(program);
        }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            glUseProgram(program);
        }

        GLuint getUniformLocation(const std::string &name) {
            // TODO: (Req 1) Return the location of the uniform with the given name
            // Use the openGL function to get the uniform location of name which is passed in to the function
            // and then return this obtained location
						GLint location = glGetUniformLocation(program, name.c_str());
						return location;
        }

        void set(const std::string &uniform, GLfloat value) {
            // TODO: (Req 1) Send the given float value to the given uniform
            // 1f for the float value & then use the getUniformLocation function we just wrote for
            // retrieving the location and passing it to the glUniform1f function along with the value
            // we want to send
						glUniform1f(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLuint value) {
            // TODO: (Req 1) Send the given unsigned integer value to the given uniform
            // Same steps as the set function for float values but instead we use the glUniform1ui
            // function fo sending an unsigned integer
						glUniform1ui(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, GLint value) {
            // TODO: (Req 1) Send the given integer value to the given uniform
            // 1i for an integer value & pass the location with the given value as explained
						glUniform1i(getUniformLocation(uniform), value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            // TODO: (Req 1) Send the given 2D vector value to the given uniform
            // Now the value parameter is 2-dimensional so we send it using the
            // function glUniform2f where 2f represents a vec2 float. Then, we
            // pass the x & y coordinates along with the uniform location
						glUniform2f(getUniformLocation(uniform), value.x, value.y);
        }

        void set(const std::string &uniform, glm::vec3 value) {
            // TODO: (Req 1) Send the given 3D vector value to the given uniform
            // 3-dimensional value is sent using the glUniform3f and now we
            // send an extra coordinate z with x & y
						glUniform3f(getUniformLocation(uniform), value.x, value.y, value.z);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            // TODO: (Req 1) Send the given 4D vector value to the given uniform
            // 4-dimensional value so we use glUniform4f & send another coordinate w
						glUniform4f(getUniformLocation(uniform), value.x, value.y, value.z, value.w);
        }

        void set(const std::string &uniform, glm::mat4 matrix) {
            // TODO: (Req 1) Send the given matrix 4x4 value to the given uniform
            // Sending a 4x4 matrix is done by using the function glUniformMatrix4fv where we
            // pass the uniform location as usual and then the count whic is set to 1. The false
            // parameter is for transposing the matrix which is not needed here and finally the
            // 4th argument is the matrix reference itself starting from the very first element
            // with coordinates [0][0]
						glUniformMatrix4fv(getUniformLocation(uniform), 1, false, &matrix[0][0]);
        }

        // TODO: (Req 1) Delete the copy constructor and assignment operator.
        // Delete the shader program copy constructor
				ShaderProgram (const ShaderProgram&) = delete;

        // Delete the shader program assignment operator
				ShaderProgram &operator=(const ShaderProgram&) = delete;

        // Question: Why do we delete the copy constructor and assignment operator?
				// Answer: To prevent implicit-declaration and copying the object
    };
}

#endif