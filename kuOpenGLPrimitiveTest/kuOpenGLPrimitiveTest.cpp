#include <iostream>
#include <vector>
#include <windows.h>

//#include <GL/glew.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>

#include "kuShaderHandler.h"
#include "kuOpenGLPrimitiveObjects.h"

#define pi			3.1415926
#define WND_WIDTH	1024
#define WND_HEIGHT	768

bool						keyPressArray[1024];

GLFWwindow				*	kuGLInit(const char * title, int xRes, int yRes);
void						key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
void						mouse_callback(GLFWwindow * window, double xPos, double yPos);

using namespace kuGLModule;

void main()
{
	GLFWwindow			*	window = kuGLInit("kuOpenGLTest", WND_WIDTH, WND_HEIGHT);

	glm::vec3				cameraPos		 = glm::vec3(-3.0f, 3.0f, 5.0f);		// 0.0f, 1.5f, -5.0f
	glm::vec3				cameraTarget	 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3				cameraFront		 = glm::normalize(cameraPos - cameraTarget);
	glm::vec3				worldUp			 = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3				cameraRight		 = glm::normalize(glm::cross(worldUp, cameraFront));
	glm::vec3				cameraUp		 = glm::cross(cameraFront, cameraRight);

	glm::mat4				projectionMat	 = glm::perspective(glm::radians(45.0f), (float)WND_WIDTH/(float)WND_HEIGHT, 0.1f, 100.0f);
	glm::mat4				viewMat			 = glm::lookAt(cameraPos, cameraTarget, cameraUp);

	glm::vec3				entryPt			 = glm::vec3(1.0f, 1.0f, -1.0f);
	glm::vec3				targetPt		 = glm::vec3(0.0f, 0.0f, 0.0f);
	float					pathLength		 = (float)sqrt(glm::vec3(targetPt - entryPt).length());

	//kuCylinderObject		pathObj(0.025f, pathLength);
	kuConeObject			coneObj(0.1f, 0.25f);

	kuSphereObject			entryPtObj(0.03f);
	kuSphereObject			targetPtObj(0.06f);

	kuArrawObject			arrawObj(0.04f, pathLength);

	kuCoordinateAxesObject	coordinateAxesObj(1.0f);

	//pathObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);
	coneObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);
	targetPtObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);
	entryPtObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);
	arrawObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);
	coordinateAxesObj.SetCameraConfiguration(projectionMat, viewMat, cameraPos);

	//pathObj.SetPosition(glm::vec3(0.5f * (entryPt + targetPt)));
	//pathObj.RotateToVec(glm::vec3(targetPt - entryPt));
	//pathObj.SetColor(0.0f, 0.0f, 1.0f, 0.85f);
		
	coneObj.SetPosition(glm::vec3(0.0f, 0.5f, 0.0f));
	coneObj.SetColor(0.0f, 1.0f, 0.0f, 0.5f);
	
	entryPtObj.SetPosition(entryPt);
	entryPtObj.SetColor(1.0f, 0.0f, 0.0f, 0.4f);
	targetPtObj.SetPosition(targetPt);
	targetPtObj.SetColor(1.0f, 0.0f, 0.0f, 0.4f);

	arrawObj.SetColor(0.0f, 0.0f, 1.0f, 1.0f);
	arrawObj.SetPosition(glm::vec3(0.5f * (entryPt + targetPt)));
	arrawObj.RotateToVec(glm::vec3(targetPt - entryPt));

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		coordinateAxesObj.Draw();

		arrawObj.Draw();
		targetPtObj.Draw();
		entryPtObj.Draw();
	
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
	//glfwSetCursorPosCallback(window, mouse_callback);					// �U�W��q...�j���u����m��T�ӨS��button�ƥ��T�a

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		//return -1;
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