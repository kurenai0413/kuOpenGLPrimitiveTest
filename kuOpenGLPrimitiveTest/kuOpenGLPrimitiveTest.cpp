#include <iostream>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "kuShaderHandler.h"

#define pi			3.1415926
#define WND_WIDTH	1024
#define WND_HEIGHT	768

bool		 keyPressArray[1024];

glm::vec3	 cameraPos   = glm::vec3(0.0f, 0.0f, 10.0f);
glm::vec3	 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3	 cameraUp	 = glm::vec3(0.0f, 1.0f, 0.0f);

GLFWwindow * kuGLInit(const char * title, int xRes, int yRes);
void		 key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void		 mouse_callback(GLFWwindow * window, double xPos, double yPos);

void main()
{
	GLFWwindow		*	window = kuGLInit("kuOpenGLTest", WND_WIDTH, WND_HEIGHT);

	kuShaderHandler		objectShader;
	objectShader.Load("VertexShader.vert", "FragmentShader.frag");

	// Create cylinder top
	float	  radius		 = 0.25f;
	int		  divisionNum	 = 36;
	int		  vertexNum		 = /*2 **/ (divisionNum + 1);
	GLfloat * cylinderVertex = new GLfloat [3 * vertexNum];

	for (int i = 0; i < vertexNum; i++)
	{
		float theta = (float)i * 360.0f / (float)divisionNum;
		float cosVal = cos(theta * pi / 180);
		float sinVal = sin(theta * pi / 180);

		cylinderVertex[3 * i]	  = radius * cosVal;
		cylinderVertex[3 * i + 1] = radius * sinVal;
		cylinderVertex[3 * i + 2] = 0;
	}

	GLuint cylinderVAO;
	glGenVertexArrays(1, &cylinderVAO);
	GLuint cylinderVBO;
	glGenBuffers(1, &cylinderVBO);

	glBindVertexArray(cylinderVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cylinderVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * vertexNum * sizeof(GLfloat), cylinderVertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(cylinderVAO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, vertexNum);
		glBindVertexArray(0);

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

	GLFWwindow * window = glfwCreateWindow(xRes, yRes, title, NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		std::cout << "GLFW window creation failed." << std::endl;

		return NULL;
	}

	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);				// 顧名思義...大概只有位置資訊而沒有button事件資訊吧

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
	//glEnable(GL_DEPTH_TEST);

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
