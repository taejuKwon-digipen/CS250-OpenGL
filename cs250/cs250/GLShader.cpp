#include "GLShader.h"
#include <iostream>

GLShader::GLShader()
{}

void GLShader::shaderFileLoad(GLenum shaderType, const char* fileName)
{
	std::ifstream infile(fileName);
	if (infile.good() == GL_FALSE)
	{
		std::cout << "File not found." << std::endl;
	}
	
	std::ifstream shaderFile(fileName, std::ifstream::in);
	if (!shaderFile)
	{
		std::cout << "Error in opening files." << fileName;
	}
	std::stringstream buffer;
	buffer << shaderFile.rdbuf();
	shaderFile.close();

	const std::string& tmp = buffer.str();
	const char* shaderSrc = tmp.c_str();

	GLint currShader = glCreateShader(shaderType);
	glShaderSource(currShader, 1, &shaderSrc, NULL);
	glCompileShader(currShader);

	GLchar infoLog[512];
	GLint success;
	glGetShaderiv(currShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(currShader, 512, NULL, infoLog);
		std::cout << fileName << " Shader compilation failed." << std::endl;
	}
	shaders.push_back(currShader);
}

void GLShader::Link()
{
	glLinkProgram(ShaderProgram);

	GLchar infoLog[512];
	GLint success;
	glGetProgramiv(ShaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ShaderProgram, 512, NULL, infoLog);
		std::cout << "Shader and program linking error." << std::endl;
	}
}

void GLShader::makeProgram(const char* vert, const char* frag)
{
	if (!shaders.empty())
	{
		DeleteShader();
		shaders.clear();
	}
	shaderFileLoad(VERTEX_SHADER, vert);
	shaderFileLoad(FRAGMENT_SHADER, frag);
	ShaderProgram = glCreateProgram();
	
	for (auto shad : shaders) {
		glAttachShader(ShaderProgram, shad);
	}
}

void GLShader::makeProgram(const char* vert, const char* tcs, const char* tes, const char* geo, const char* frag)
{
	if (!shaders.empty())
	{
		DeleteShader();
		shaders.clear();
	}
	shaderFileLoad(VERTEX_SHADER, vert);
	shaderFileLoad(TESSELLATION_CONTROL, tcs);
	shaderFileLoad(TESSELLATION_EVALUATION, tes);
	shaderFileLoad(GEOMETRY_SHADER, geo);
	shaderFileLoad(FRAGMENT_SHADER, frag);
	ShaderProgram = glCreateProgram();

	for (auto shad : shaders)
	{
		glAttachShader(ShaderProgram, shad);
	}
}

void GLShader::DeleteShader()
{
	for (auto shad : shaders) {
		glDeleteProgram(shad);
	}
}

void GLShader::Use()
{
	if (ShaderProgram > 0)
	{
		glUseProgram(ShaderProgram);
	}
}

void GLShader::Unuse()
{
	glUseProgram(0);
}

void GLShader::setMat4(const std::string& name, const Mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
