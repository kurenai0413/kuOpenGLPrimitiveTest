#include "kuShaderHandler.h"



kuShaderHandler::kuShaderHandler()
{
}

kuShaderHandler::kuShaderHandler(const char * VSPathName, const char * FSPathName)
{
	this->Load(VSPathName, FSPathName);
}


kuShaderHandler::~kuShaderHandler()
{
}

void kuShaderHandler::Load(const char * VSPathName, const char * FSPathName)
{
	std::string		vertexCode;
	std::string		fragmentCode;
	std::ifstream	VSFile;
	std::ifstream	FSFile;

	VSFile.exceptions(std::ifstream::badbit);
	FSFile.exceptions(std::ifstream::badbit);

	try
	{
		VSFile.open(VSPathName);
		FSFile.open(FSPathName);

		std::stringstream VSStream; 
		std::stringstream FSStream;

		VSStream << VSFile.rdbuf();
		FSStream << FSFile.rdbuf();

		VSFile.close();
		FSFile.close();

		vertexCode   = VSStream.str();
		fragmentCode = FSStream.str();
	}
	catch(std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}

	const GLchar * VSCode = vertexCode.c_str();
	const GLchar * FSCode = fragmentCode.c_str();

	// 2. Compile shaders
	CompileShaders(VSCode, FSCode);
}

void kuShaderHandler::Use()
{
	if (m_fShaderCreated)
	{
		glUseProgram(this->m_ShaderProgramID);
	}
}

GLuint kuShaderHandler::GetShaderProgramID()
{
	return m_ShaderProgramID;
}

kuShaderHandler & kuShaderHandler::operator=(kuShaderHandler & rhs)
{
	// TODO: 於此處插入傳回陳述式
	this->m_ShaderProgramID = rhs.m_ShaderProgramID;
	this->m_fShaderCreated = rhs.m_fShaderCreated;
	
	return *this;
}

void kuShaderHandler::CompileShaders(const GLchar * vertexShaderCode, const GLchar * fragmentShaderCode)
{
	GLuint VertexShader, FragmentShader;
	GLint  success;
	GLchar infoLog[512];

	// Vertex Shader
	VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShader, 1, &vertexShaderCode, NULL);
	glCompileShader(VertexShader);
	// Print compile errors if any
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(VertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Similiar for Fragment Shader
	FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShader, 1, &fragmentShaderCode, NULL);
	glCompileShader(FragmentShader);
	// Print compile errors if any
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(FragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	this->m_ShaderProgramID = glCreateProgram();
	glAttachShader(this->m_ShaderProgramID, VertexShader);
	glAttachShader(this->m_ShaderProgramID, FragmentShader);
	glLinkProgram(this->m_ShaderProgramID);
	// Print linking errors if any
	glGetProgramiv(this->m_ShaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(this->m_ShaderProgramID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// Delete the shaders as they're linked into our program now and no longer necessery
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);

	m_fShaderCreated = true;
}
