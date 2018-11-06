#pragma once
#include <iostream>
#include <vector>

#include <GLM/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/common.hpp>

#include "kuShaderHandler.h"

namespace kuGLModule
{
	// Abstract class
	class kuGLPrimitiveObject
	{
	public:
		virtual void			Draw() = 0;
		void					SetCameraConfiguration(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 cameraPos);

		// Transform
		void					SetPosition(glm::vec3 pos);
		void					RotateToVec(glm::vec3 newUpVec);
		// Color
		void					SetColor(float R, float G, float B, float alpha);

		GLuint					GetShaderProgramID();

	protected:
		std::vector<GLfloat>	m_Vertices;
		std::vector<int>		m_Indices;

		glm::mat4				m_ModelMat;
		kuShaderHandler			m_Shader;

		GLuint					m_VAO;							// Vertex array object
		GLuint					m_VBO;							// Vertex buffer object
		GLuint					m_EBO;							// Element buffer object

		GLfloat					m_XPos = 0;
		GLfloat					m_YPos = 0;
		GLfloat					m_ZPos = 0;

		glm::vec3				m_UpVector;

		int						m_VerticesNum;

		virtual void			CreateModel() = 0;
		void					CreateRenderBuffers();

		void					UpdateModelMat();
	};
	// Interface
	class IkuGLPrimitiveComboObject
	{
	public:
		virtual void			Draw() = 0;
		virtual void			SetCameraConfiguration(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 cameraPos) = 0;

		virtual void			SetPosition(glm::vec3 pos)		= 0;
		virtual void			RotateToVec(glm::vec3 newUpVec) = 0;
	};

	class kuCylinderObject : public kuGLPrimitiveObject
	{
	public:
		kuCylinderObject();
		kuCylinderObject(float radius, float length);
		~kuCylinderObject();

		void					SetParameters(float radius, float length);
		void					Draw();

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

	class kuConeObject : public kuGLPrimitiveObject
	{
	public:
		kuConeObject();
		kuConeObject(float radius, float length);
		~kuConeObject();

		void					SetParameters(float radius, float length);
		void					Draw();

	protected:
		const int				m_DivisionNum = 36;
		float					m_Radius;
		float					m_Length;

		void					CreateModel();
	};

	class kuSphereObject : public kuGLPrimitiveObject
	{
	public:
		kuSphereObject();
		kuSphereObject(float radius);
		~kuSphereObject();

		void					SetParameters(float radius);
		void					Draw();

	protected:
		const int				m_DivisionNum = 180;
		const int				m_Level = 15;

		float					m_Radius;

		void					CreateModel();
	};

	class kuArrawObject : public IkuGLPrimitiveComboObject
	{
	public:
		kuArrawObject();
		kuArrawObject(float radius, float length);
		~kuArrawObject();

		void				SetCameraConfiguration(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 cameraPos);
		void				SetParameters(float radius, float length);
		
		// Transform
		void				SetPosition(glm::vec3 pos);
		void				RotateToVec(glm::vec3 newUpVec);
		//Color
		void				SetColor(float R, float G, float B, float alpha);

		void				Draw();

	protected:
		kuConeObject		m_ArrawTip;
		kuCylinderObject	m_ArrawBody;
	
		glm::vec3			m_TipPosition;
		glm::vec3			m_BodyPosition;

		glm::vec3			m_UpVector;
		glm::vec3			m_Position;

		float				m_Length;
		float				m_Radius;
	};

	class kuCoordinateAxesObject : public IkuGLPrimitiveComboObject
	{
	public:
		kuCoordinateAxesObject();
		kuCoordinateAxesObject(float scale);
		~kuCoordinateAxesObject();

		void				SetCameraConfiguration(glm::mat4 projectionMat, glm::mat4 viewMat, glm::vec3 cameraPos);
		void				SetParameters(float scale);

		// Transform
		void				SetPosition(glm::vec3 pos);
		void				RotateToVec(glm::vec3 newUpVec);
		
		void				Draw();

	protected:
		kuArrawObject		m_Axes[3];						// Coordinate axes.
		kuSphereObject		m_Sphere;						// Coordinate axes origin.
		glm::vec3			m_AxesPos[3];					// Position for each single axis.

		glm::vec3			m_Position;
	
		glm::vec3			m_UpVector;
		glm::vec3			m_RightVector;
		glm::vec3			m_FrontVector;

		float				m_Scale;

	private:
		float				m_AxesRadius   = 0.04f;			// Radius factor for coordinate axes (needs to be multiplied with the scale).
		float				m_SphereRadius = 0.05f;			// Radius factor for the sphere (needs to be multiplied with the scale).
		float				m_AxesLength   = 1.0f;			// Length for coordinate axes.				
	};
}