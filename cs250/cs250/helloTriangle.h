#pragma once
#include "GLShader.h"

class triangle
{
public:
	void init();
	void draw();

private:
	GLShader shader;
	unsigned int VAO, VBO;
};