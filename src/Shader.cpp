#include "Shader.h"


Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath) {

	//Vertex Shader.
	std::string strVertexShaderCode = ReadShaderFile(vertexPath);
	const char* vShaderCode = strVertexShaderCode.c_str();

	unsigned int vertexShader;
	vertexShader = CompileShader(GL_VERTEX_SHADER,vShaderCode);
	CheckShaderErrors(GL_VERTEX_SHADER, vertexShader);

	//Fragment Shader.
	std::string strFragmentShaderCode = ReadShaderFile(fragmentPath);
	const char* fShaderCode = strFragmentShaderCode.c_str();

	unsigned int fragmentShader;
	fragmentShader = CompileShader(GL_FRAGMENT_SHADER,fShaderCode);
	CheckShaderErrors(GL_FRAGMENT_SHADER, fragmentShader);

	//Create the shader program.
	ID = glCreateProgram();
	glAttachShader(ID,vertexShader);
	glAttachShader(ID, fragmentShader);
	glLinkProgram(ID);

	CheckShaderProgramErrors();

	//We don't need these anymore.Every call will use the program object.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

std::string Shader::ReadShaderFile(const GLchar * path) {

	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try{
		shaderFile.open(path);

		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		return shaderStream.str();

	}catch(std::ifstream::failure e) {
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	return nullptr;
}

unsigned int Shader::CompileShader(unsigned int shaderType, const char* source) {

	unsigned int shaderID = glCreateShader(shaderType);
	glShaderSource(shaderID, 1, &source, NULL);
	glCompileShader(shaderID);
	return shaderID;
}

void Shader::CheckShaderErrors(unsigned int shaderType, unsigned int shaderId) {
	int success;
	//Get the compile status of the shader.
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);

	if (success == GL_FALSE) {
		char infoLog[512];
		//Get infoLog.
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);

		char* shaderTypeText;	
		switch(shaderType){
		case GL_VERTEX_SHADER:
			shaderTypeText = (char*)"VERTEX";
			break;
		case GL_FRAGMENT_SHADER:
			shaderTypeText = (char*)"FRAGMENT";
			break;
		default:
			shaderTypeText = (char*)"UNKNOWN";
			break;
		}
		
		//Print error with infolog.
		std::cout << "ERROR::SHADER::" << shaderTypeText << "::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void Shader::CheckShaderProgramErrors() {
	int success;
	//Check if the program was created correctly.
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		char infoLog[512];
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::CREATION_FAILED\n" << infoLog << std::endl;
	}

}



Shader::~Shader() {
	glDeleteProgram(ID);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::setBool(const std::string & name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()),(int)value);
}

void Shader::setInt(const std::string & name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()),value);
}

void Shader::setFloat(const std::string & name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string & name, glm::mat4 &value) const{
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()),1,GL_FALSE,&value[0][0]);
}

void Shader::setVec3(const std::string &name,float x,float y,float z)const{
	glUniform3f(glGetUniformLocation(ID, name.c_str()),x,y,z);
}


