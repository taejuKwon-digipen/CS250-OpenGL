#pragma once
#include <GL/glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "math.hpp"

class GLShader
{
public:
	enum ShaderType
	{
		VERTEX_SHADER = GL_VERTEX_SHADER,
		FRAGMENT_SHADER = GL_FRAGMENT_SHADER,
		TESSELLATION_CONTROL = GL_TESS_CONTROL_SHADER,
		TESSELLATION_EVALUATION = GL_TESS_EVALUATION_SHADER,
		GEOMETRY_SHADER = GL_GEOMETRY_SHADER,
	};

	GLShader();
	void shaderFileLoad(GLenum shaderType, const char* fileName);
	void Link();
	void makeProgram(const char* vert, const char* frag);
	void makeProgram(const char* vert, const char* tcs, const char* tes, const char* geo, const char* frag);
	void DeleteShader();
	void Use();
	void Unuse();
	void setMat4(const std::string& name, const Mat4& mat) const;
	
	GLint ShaderProgram;

private:
	std::vector<GLint> shaders;

};
