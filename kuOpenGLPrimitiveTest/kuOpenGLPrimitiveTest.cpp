#include <iostream>
#include <vector>
#include <windows.h>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "kuShaderHandler.h"
#include "kuOpenGLPrimitiveObjects.h"

#define pi			3.1415926
#define WND_WIDTH	1024
#define WND_HEIGHT	768

#define cylinderNum 100000

bool						keyPressArray[1024];

GLFWwindow				*	kuGLInit(const char * title, int xRes, int yRes);
void						key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void						mouse_callback(GLFWwindow * window, double xPos, double yPos);
void						createCylinderModel(std::vector<GLfloat> &cylinderVertices, std::vector<int> &cylinderIndices, float radius, int divisionNum, float length);
void						createCylinderVertices(std::vector<GLfloat> &cylinderVertices, float radius, const unsigned int divisionNum, float length);
void						createCylinderIndices(std::vector<int> &cylinderIndices,const unsigned int divisionNum);

void main()
{
	GLFWwindow			*	window = kuGLInit("kuOpenGLTest", WND_WIDTH, WND_HEIGHT);

	kuShaderHandler			objectShader;
	objectShader.Load("VertexShader.vert", "FragmentShader.frag");

	kuCylinderObject		cylinderObj(0.05f, 1.0f);
	kuConeObject			coneObj(0.15f, 0.5f);

	glm::vec3				cameraPos		 = glm::vec3(0.0f, 1.5f, -1.5f);
	glm::vec3				cameraTarget	 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				cameraFront		 = glm::normalize(cameraPos - cameraTarget);
	glm::vec3				worldUp			 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3				cameraRight		 = glm::normalize(glm::cross(worldUp, cameraFront));
	glm::vec3				cameraUp		 = glm::cross(cameraFront, cameraRight);

	glm::mat4				modelMat;
	glm::mat4				projectionMat	 = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH/(float)WND_HEIGHT, 0.1f, 100.0f);
	glm::mat4				viewMat			 = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	objectShader.Use();

	GLuint modelMatLoc, viewMatLoc, projectionMatLoc;
	GLuint cameraPosLoc;
	modelMatLoc = glGetUniformLocation(objectShader.GetShaderProgramID(), "model");
	glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, glm::value_ptr(modelMat));
	viewMatLoc = glGetUniformLocation(objectShader.GetShaderProgramID(), "view");
	glUniformMatrix4fv(viewMatLoc, 1, GL_FALSE, glm::value_ptr(viewMat));
	projectionMatLoc = glGetUniformLocation(objectShader.GetShaderProgramID(), "projection");
	glUniformMatrix4fv(projectionMatLoc, 1, GL_FALSE, glm::value_ptr(projectionMat));

	cameraPosLoc = glGetUniformLocation(objectShader.GetShaderProgramID(), "cameraPos");
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		cylinderObj.Draw(objectShader);
		//coneObj.Draw(objectShader);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	system("pause");
}

GLFWwindow * kuGLInit(const char * title, int xRes, int yRes)
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed." << std::endl;
		return NULL;
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow * window = glfwCreateWindow(xRes, yRes, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "GLFW window creation failed." << std::endl;

		return NULL;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);					// 顧名思義...大概只有位置資訊而沒有button事件資訊吧

	// need to create OpenGL window before glew initialization.
	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		// Problem: glewInit failed, something is seriously wrong.
		std::cout << "glewInit failed, aborting." << std::endl;
	}

	// Define the viewport dimensions
	glViewport(0, 0, xRes, yRes);

	// Setup OpenGL options (z-buffer)
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	
	//glEnable(GL_MULTISAMPLE);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	// get renderer string (graphic card)
	const GLubyte* version = glGetString(GL_VERSION);
	// version as a string (OpenGL supported version and graphic card driver version)
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;

	return window;
}

void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keyPressArray[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keyPressArray[key] = false;
		}
	}

	if (key == GLFW_KEY_C)
	{
		//std::cout << "CameraPos: (" << CameraPos.x << ", " << CameraPos.y << ", " << CameraPos.z << ")" << endl;
	}
}

void mouse_callback(GLFWwindow * window, double xPos, double yPos)
{
}

void createCylinderModel(std::vector<GLfloat>& cylinderVertices, std::vector<int>& cylinderIndices, float radius, int divisionNum, float length)
{
	createCylinderVertices(cylinderVertices, radius, divisionNum, length);
	createCylinderIndices(cylinderIndices, divisionNum);
}

void createCylinderVertices(std::vector<GLfloat> &cylinderVertices, float radius, const unsigned int divisionNum, float length)
{
	if (cylinderVertices.size() != 0)
		cylinderVertices.clear();

	// Create cylinder top
	int		  vertexNum = 2 * (divisionNum + 1);

	for (int i = 0; i <= divisionNum; i++)
	{
		float	theta = (float)i * 360.0f / (float)divisionNum;
		float	cosVal = cos(theta * pi / 180);
		float	sinVal = sin(theta * pi / 180);

		// Top
		// Vertex positions
		cylinderVertices.push_back(radius * cosVal);
		cylinderVertices.push_back(0.5f * length);
		cylinderVertices.push_back(radius * sinVal);
		// Normal
		cylinderVertices.push_back(cosVal);
		cylinderVertices.push_back(0);
		cylinderVertices.push_back(sinVal);
		
		// Bottom
		// Vertex positions
		cylinderVertices.push_back(radius * cosVal);
		cylinderVertices.push_back(-0.5f * length);
		cylinderVertices.push_back(radius * sinVal);
		// Normal
		cylinderVertices.push_back(cosVal);
		cylinderVertices.push_back(0);
		cylinderVertices.push_back(sinVal);
	}

	// Top center
	cylinderVertices.push_back(0.0f);
	cylinderVertices.push_back(0.5f * length);
	cylinderVertices.push_back(0.0f);
	// Top center normal
	cylinderVertices.push_back(0.0f);
	cylinderVertices.push_back(1.0f);
	cylinderVertices.push_back(0.0f);

	// Bottom center
	cylinderVertices.push_back(0.0f);
	cylinderVertices.push_back(-0.5f * length);
	cylinderVertices.push_back(0.0f);
	// Bottom center normal
	cylinderVertices.push_back(0.0f);
	cylinderVertices.push_back(-1.0f);
	cylinderVertices.push_back(0.0f);
}

void createCylinderIndices(std::vector<int>& cylinderIndices, const unsigned int divisionNum)
{
	int vertexNumSide = 2 * (divisionNum + 1);				// in (divisionNum + 1): last = first
	for (int i = 0; i < vertexNumSide; i++)
	{
		cylinderIndices.push_back(i);
	}

	int vertexNumTop = divisionNum + 1;						// divisionNum + center
	cylinderIndices.push_back(2 * (divisionNum + 1));		// Center
	for (int i = 0; i < divisionNum; i++)
	{
		cylinderIndices.push_back(2 * i);
	}
	cylinderIndices.push_back(0);							// Last = first

	cylinderIndices.push_back(2 * (divisionNum + 1) + 1); // Center
	for (int i = 0; i < divisionNum; i++)
	{
		cylinderIndices.push_back(2 * i + 1);
	}
	cylinderIndices.push_back(1);							// Last = first
}
