#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
class Shader {
public:
	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);
	~Shader();

	unsigned int ID;
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setMat4(const std::string &name, glm::mat4 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
private:

	std::string ReadShaderFile(const GLchar* path);
	unsigned int CompileShader(unsigned int shaderType, const char* source);
	void CheckShaderErrors(unsigned int shaderType,unsigned int shaderId);
	void CheckShaderProgramErrors();

};

#endif