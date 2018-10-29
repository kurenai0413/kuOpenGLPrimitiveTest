#pragma once
#include <iostream>
#include <vector>

#include "kuShaderHandler.h"

// Abstract class
class kuGLPrimitiveObject
{
public:
	virtual void			Draw(kuShaderHandler shader) = 0;

protected:
	std::vector<GLfloat>	m_Vertices;
	std::vector<int>		m_Indices;

	GLuint					m_VAO;							// Vertex array object
	GLuint					m_VBO;							// Vertex buffer object
	GLuint					m_EBO;							// Element buffer object

	GLfloat					m_XPos				  = 0;
	GLfloat					m_YPos				  = 0;
	GLfloat					m_ZPos				  = 0;

	int						m_VerticesNum;

	virtual void			CreateModel()		  = 0;
	void					CreateRenderBuffers();
};

class kuCylinderObject : kuGLPrimitiveObject
{
public:
	kuCylinderObject();
	kuCylinderObject(float radius, float length);
	~kuCylinderObject();

	void					SetParameters(float radius, float length);
	void					Draw(kuShaderHandler shader);
	
protected:
	const int				m_DivisionNum = 36;
	float					m_Radius;
	float					m_Length;

	void					CreateModel();
};

class kuCubeObject : kuGLPrimitiveObject
{
public:
	kuCubeObject();
	kuCubeObject(float size);
	~kuCubeObject();

	void					SetParameters(float size);
	void					Draw(kuShaderHandler shader);

protected:
	float					m_Size;

	void					CreateVertices();
	void					CreateIndices();
	void					CreateRenderBuffers();
};

class kuCuboidObject : kuGLPrimitiveObject
{
public:
	kuCuboidObject();
	kuCuboidObject(float width, float height, float length);
	~kuCuboidObject();

	void					SetParameters(float width, float height, float length);
	void					Draw(kuShaderHandler shader);

protected:
	float					m_Width;
	float					m_Height;
	float					m_Length;
};

class kuConeObject : kuGLPrimitiveObject
{
public:
	kuConeObject();
	kuConeObject(float radius, float length);
	~kuConeObject();

	void					SetParameters(float radius, float length);
	void					Draw(kuShaderHandler shader);

protected:
	const int				m_DivisionNum = 36;
	float					m_Radius;
	float					m_Length;

	void					CreateModel();
};

class kuSphereObject : kuGLPrimitiveObject
{
public:
	kuSphereObject();
	kuSphereObject(float radius);
	~kuSphereObject();

	void					SetParameters(float radius);
	void					Draw(kuShaderHandler shader);

protected:
	const int				m_DivisionNum = 180;
	const int				m_Level		  = 15;

	float					m_Radius;
	
	void					CreateModel();
};