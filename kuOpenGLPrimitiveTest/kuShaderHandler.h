#ifndef KUSHADERHANDLER_H
#define KUSHADERHANDLER_H

#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <GLEW/glew.h>

class kuShaderHandler
{
public:
	kuShaderHandler();
	kuShaderHandler(const char * VSPathName, const char * FSPathName);		// Vertex Shader, Fragment Shader
	~kuShaderHandler();

	void	Load(const char * VSPathName, const char * FSPathName);
	void	Use();
	GLuint	GetShaderProgramID();

	kuShaderHandler& operator=(kuShaderHandler &rhs);

private:

	GLuint	m_ShaderProgramID;
	bool	m_fShaderCreated;
};

#endif