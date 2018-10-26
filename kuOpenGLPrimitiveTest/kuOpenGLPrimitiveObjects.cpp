#include "kuOpenGLPrimitiveObjects.h"

#define pi 3.1415926

#pragma region // Cylinder object //
kuCylinderObject::kuCylinderObject()
{

}

kuCylinderObject::kuCylinderObject(float radius, float length)
{
	SetParameters(radius, length);
}

kuCylinderObject::~kuCylinderObject()
{

}

void kuCylinderObject::SetParameters(float radius, float length)
{
	m_Radius = radius;
	m_Length = length;
	m_VerticesNum = 2 * (m_DivisionNum + 1);

	CreateVertices();
	CreateIndices();
	CreateRenderBuffers();
}

void kuCylinderObject::Draw(kuShaderHandler shader)
{
	shader.Use();

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_VerticesNum, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, (GLvoid*)((m_VerticesNum) * sizeof(int)));
	//glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, (GLvoid*)((m_VerticesNum + m_DivisionNum + 2) * sizeof(int)));
	glBindVertexArray(0);
}

void kuCylinderObject::CreateVertices()
{
	if (m_Vertices.size() != 0)
		m_Vertices.clear();

	#pragma region // Setup top and bottom vertices //
	std::vector<GLfloat> verticesTop;
	std::vector<GLfloat> verticesBottom;
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		float	theta = (float)i * 360.0f / (float)m_DivisionNum;
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		// Vertex positions
		verticesTop.push_back(m_Radius * cosVal);
		verticesTop.push_back(0.5f * m_Length);
		verticesTop.push_back(m_Radius * sinVal);

		// Bottom
		// Vertex positions
		verticesBottom.push_back(m_Radius * cosVal);
		verticesBottom.push_back(-0.5f * m_Length);
		verticesBottom.push_back(m_Radius * sinVal);
	}
	#pragma endregion

	#pragma region // Vertices for side strip //
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(verticesTop[3 * i + 1]);
		m_Vertices.push_back(verticesTop[3 * i + 2]);

		//// Normal
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(0);
		m_Vertices.push_back(verticesTop[3 * i + 2]);

		m_Vertices.push_back(verticesBottom[3 * i]);
		m_Vertices.push_back(verticesBottom[3 * i + 1]);
		m_Vertices.push_back(verticesBottom[3 * i + 2]);

		//// Normal
		m_Vertices.push_back(verticesBottom[3 * i]);
		m_Vertices.push_back(0);
		m_Vertices.push_back(verticesTop[3 * i + 2]);
	}
	#pragma endregion

	#pragma region // Vertices for top fan //
	// Top center
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(0.5f * m_Length);
	m_Vertices.push_back(0.0f);
	// Top center normal
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(1.0f);
	m_Vertices.push_back(0.0f);

	for (int i = 0; i <= m_DivisionNum; i++)
	{
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(verticesTop[3 * i + 1]);
		m_Vertices.push_back(verticesTop[3 * i + 2]);

		//// Normal
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(1.0f);
		m_Vertices.push_back(0.0f);
	}
	#pragma endregion

	#pragma region // Vertices for bottom fan //
	// Bottom center
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(0.5f * m_Length);
	m_Vertices.push_back(0.0f);
	// Bottom center normal
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(-1.0f);
	m_Vertices.push_back(0.0f);

	for (int i = 0; i <= m_DivisionNum; i++)
	{
		m_Vertices.push_back(verticesBottom[3 * i]);
		m_Vertices.push_back(verticesBottom[3 * i + 1]);
		m_Vertices.push_back(verticesBottom[3 * i + 2]);

		//// Normal
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(-1.0f);
		m_Vertices.push_back(0.0f);
	}
	#pragma endregion
}

void kuCylinderObject::CreateIndices()
{
	if (m_Indices.size() != 0)
		m_Indices.size();

	int vertexNumSide = 2 * (m_DivisionNum + 1);
	for (int i = 0; i < vertexNumSide; i++)
	{
		m_Indices.push_back(i);
	}

	int vertexNumTop = m_DivisionNum + 1;				// divisionNum + center
	m_Indices.push_back(2 * (m_DivisionNum + 1));		// Center
	for (int i = 0; i < m_DivisionNum; i++)
	{
		m_Indices.push_back(2 * i);
	}
	m_Indices.push_back(0);								// Last = first

	m_Indices.push_back(2 * (m_DivisionNum + 1) + 1);	// Center
	for (int i = 0; i < m_DivisionNum; i++)
	{
		m_Indices.push_back(2 * i + 1);
	}
	m_Indices.push_back(1);								// Last = first
}

void kuCylinderObject::CreateRenderBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), &m_Vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), &m_Indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
#pragma endregion

#pragma region // kuCubeObject //
kuCubeObject::kuCubeObject()
{

}

kuCubeObject::kuCubeObject(float size)
{
	SetParameters(size);
}

kuCubeObject::~kuCubeObject()
{

}

void kuCubeObject::SetParameters(float size)
{
	m_Size = size;
	m_VerticesNum = 8;

	CreateVertices();
	CreateIndices();
	CreateRenderBuffers();
}

void kuCubeObject::Draw(kuShaderHandler shader)
{

}

void kuCubeObject::CreateVertices()
{

}

void kuCubeObject::CreateIndices()
{

}

void kuCubeObject::CreateRenderBuffers()
{

}
#pragma endregion

#pragma region // kuConeObject //
kuConeObject::kuConeObject()
{

}

kuConeObject::kuConeObject(float radius, float length)
{
	SetParameters(radius, length);
}

kuConeObject::~kuConeObject()
{

}

void kuConeObject::SetParameters(float radius, float length)
{
	m_Radius = radius;
	m_Length = length;
	m_VerticesNum = 2 * (m_DivisionNum + 1) + 2;		// First 1: circle first = last, + 2: Tip and bottom center

	CreateVertices();
	CreateIndices();
	CreateRenderBuffers();
}

void kuConeObject::Draw(kuShaderHandler shader)
{
	shader.Use();

	glBindVertexArray(m_VAO);
	// Render sides
	glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, 0);
	// Render bottom
	glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, (GLvoid*)((m_DivisionNum + 2) * sizeof(int)));
	glBindVertexArray(0);
}

void kuConeObject::CreateVertices()
{
#pragma region // Assign sides vertices
	m_Vertices.push_back(0.0f);								// Top X
	m_Vertices.push_back(m_Length);							// Top Y
	m_Vertices.push_back(0.0f);								// Top Z
															// Normal
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(1.0f);
	m_Vertices.push_back(0.0f);

	// Circle vertices
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		float	theta = (float)i * 360.0f / (float)m_DivisionNum;
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		m_Vertices.push_back(m_Radius * cosVal);			// X
		m_Vertices.push_back(0.0f);							// Y
		m_Vertices.push_back(m_Radius * sinVal);			// Z

															// Normal
		m_Vertices.push_back(cosVal);
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(sinVal);
	}
#pragma region

	// Assign bottom center vertex
	m_Vertices.push_back(0.0f);								// X
	m_Vertices.push_back(0.0f);								// Y
	m_Vertices.push_back(0.0f);								// Z
															// Normal
	m_Vertices.push_back(0.0f);								// X
	m_Vertices.push_back(-1.0f);							// Y
	m_Vertices.push_back(0.0f);								// Z
}

void kuConeObject::CreateIndices()
{
	m_Indices.push_back(0);
	for (int i = 1; i <= m_DivisionNum + 1; i++)
	{
		m_Indices.push_back(i);
	}
	m_Indices.push_back(m_VerticesNum - 1);
	for (int i = 1; i <= m_DivisionNum + 1; i++)
	{
		m_Indices.push_back(i);
	}
}

void kuConeObject::CreateRenderBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), &m_Vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), &m_Indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
#pragma endregion