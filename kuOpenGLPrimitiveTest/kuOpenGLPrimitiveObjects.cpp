#include "kuOpenGLPrimitiveObjects.h"

#define pi			3.1415926
#define VertexSize	6			// 3 for position and 3 for normal

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

	CreateModel();
	CreateRenderBuffers();
}

void kuCylinderObject::Draw(kuShaderHandler shader)
{
	shader.Use();	

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLE_STRIP, m_VerticesNum, GL_UNSIGNED_INT, 0);
	glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, (GLvoid*)((m_VerticesNum) * sizeof(int)));
	glDrawElements(GL_TRIANGLE_FAN, m_DivisionNum + 2, GL_UNSIGNED_INT, (GLvoid*)((m_VerticesNum + m_DivisionNum + 2) * sizeof(int)));
	glBindVertexArray(0);
}

void kuCylinderObject::CreateModel()
{
	if (m_Vertices.size() != 0)
		m_Vertices.clear();

	if (m_Indices.size() != 0)
		m_Indices.clear();

	#pragma region // Generate top and bottom vertices //
	std::vector<GLfloat> verticesTop;
	std::vector<GLfloat> verticesBottom;

	for (int i = 0; i <= m_DivisionNum; i++)
	{
		float	theta = -((float)i * 360.0f / (float)m_DivisionNum);
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		// Top vertices positions
		verticesTop.push_back(m_Radius * cosVal);
		verticesTop.push_back(0.5f * m_Length);
		verticesTop.push_back(m_Radius * sinVal);

		// Bottom vertex positions
		verticesBottom.push_back(m_Radius * cosVal);
		verticesBottom.push_back(-0.5f * m_Length);
		verticesBottom.push_back(m_Radius * sinVal);
	}
	#pragma endregion

	#pragma region // Set side faces vertices and indices: 0 ~ (2 * (m_DivisionNum + 1) - 1) //
	// Side triangle strip vertices.
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		// Top circle vertex
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(verticesTop[3 * i + 1]);
		m_Vertices.push_back(verticesTop[3 * i + 2]);
		// Top circle normal
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(0);
		m_Vertices.push_back(verticesTop[3 * i + 2]);

		// Index
		m_Indices.push_back(m_Vertices.size() / VertexSize - 1);

		// Bottom circle vertex
		m_Vertices.push_back(verticesBottom[3 * i]);
		m_Vertices.push_back(verticesBottom[3 * i + 1]);
		m_Vertices.push_back(verticesBottom[3 * i + 2]);
		// Bottom circle normal
		m_Vertices.push_back(verticesBottom[3 * i]);
		m_Vertices.push_back(0);
		m_Vertices.push_back(verticesTop[3 * i + 2]);
	
		// Index
		m_Indices.push_back(m_Vertices.size() / VertexSize - 1);
	}
	#pragma endregion

	#pragma region // Set top face vertices and indices: 2 * (m_DivisionNum + 1) ~ 2 * (m_DivisionNum + 1) + (m_DivisionNum + 2) - 1 //
	// Top center
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(0.5f * m_Length);
	m_Vertices.push_back(0.0f);
	// Top center normal
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(1.0f);
	m_Vertices.push_back(0.0f);

	// Set top center index
	m_Indices.push_back(m_Vertices.size() / VertexSize - 1);

	int topStride = 2 * (m_DivisionNum + 1) + 1;
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		m_Vertices.push_back(verticesTop[3 * i]);
		m_Vertices.push_back(verticesTop[3 * i + 1]);
		m_Vertices.push_back(verticesTop[3 * i + 2]);

		// Top normal
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(1.0f);
		m_Vertices.push_back(0.0f);

		// Indices
		m_Indices.push_back(m_Vertices.size() / VertexSize - 1);
	}
	#pragma endregion

	#pragma region // Set bottom face vertices and indcies: 2 * (m_DivisionNum + 1) + (m_DivisionNum + 2) ~ 2 * (m_DivisionNum + 1) + 2 * (m_DivisionNum + 2) - 1 //
	// Bottom center
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(0.5f * m_Length);
	m_Vertices.push_back(0.0f);
	// Bottom center normal
	m_Vertices.push_back(0.0f);
	m_Vertices.push_back(-1.0f);
	m_Vertices.push_back(0.0f);

	// Set bottom center index
	m_Indices.push_back(m_Vertices.size() / VertexSize - 1);

	for (int i = 0; i <= m_DivisionNum; i++)
	{
		// Bottom vertex
		m_Vertices.push_back(verticesBottom[3 * (m_DivisionNum - i)]);
		m_Vertices.push_back(verticesBottom[3 * (m_DivisionNum - i) + 1]);
		m_Vertices.push_back(verticesBottom[3 * (m_DivisionNum - i) + 2]);

		// Bottom normal
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(-1.0f);
		m_Vertices.push_back(0.0f);
		
		// Index
		m_Indices.push_back(m_Vertices.size() / VertexSize - 1);
	}
	#pragma endregion
}

void kuCylinderObject::CreateRenderBuffers()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(GLfloat), &m_Vertices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexSize * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, VertexSize * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(int), &m_Indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
#pragma endregion

#pragma region // Cube object //
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

#pragma region // Cone object //
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

void kuConeObject::CreateModel()
{
}

void kuConeObject::CreateVertices()
{
	std::vector<GLfloat>	verticesBottom;
	#pragma region // Generate circle vertices //
	for (int i = 0; i <= m_DivisionNum; i++)
	{
		float	theta = (float)i * 360.0f / (float)m_DivisionNum;
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		verticesBottom.push_back(m_Radius * cosVal);			// X
		verticesBottom.push_back(0.0f);							// Y
		verticesBottom.push_back(m_Radius * sinVal);			// Z
	}
	#pragma endregion

	#pragma region // Assign sides vertices //
	m_Vertices.push_back(0.0f);									// Top X
	m_Vertices.push_back(m_Length);								// Top Y
	m_Vertices.push_back(0.0f);									// Top Z
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

		m_Vertices.push_back(verticesBottom[3 * i]);			// X
		m_Vertices.push_back(verticesBottom[3 * i + 1]);		// Y
		m_Vertices.push_back(verticesBottom[3 * i + 2]);		// Z
																// Normal
		m_Vertices.push_back(cosVal);
		m_Vertices.push_back(0.0f);
		m_Vertices.push_back(sinVal);
	}
	#pragma endregion

	// Assign bottom center vertex
	m_Vertices.push_back(0.0f);									// X
	m_Vertices.push_back(0.0f);									// Y
	m_Vertices.push_back(0.0f);									// Z
	// Normal
	m_Vertices.push_back(0.0f);									// X
	m_Vertices.push_back(-1.0f);								// Y
	m_Vertices.push_back(0.0f);									// Z

	for (int i = 0; i <= m_DivisionNum; i++)
	{
		float	theta  = (float)i * 360.0f / (float)m_DivisionNum;
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		m_Vertices.push_back(verticesBottom[3 * i]);			// X
		m_Vertices.push_back(verticesBottom[3 * i + 1]);		// Y
		m_Vertices.push_back(verticesBottom[3 * i + 2]);		// Z
		// Normal
		m_Vertices.push_back(cosVal);
		m_Vertices.push_back(-1.0f);
		m_Vertices.push_back(sinVal);
	}
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