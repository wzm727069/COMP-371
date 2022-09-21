/*
* The code for the Shader class contained in this file is not our own.
* All credit should be given to learnopengl.com
*
*source: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*
*/
#include "../utils/GL_Error.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>  

std::string Shader::readShaderCode(const char* path) 
{
	std::string code;
	std::ifstream shaderFile;

	// Ensure ifstream objects can throw exceptions:
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try 
	{
		shaderFile.open(path);

		std::stringstream shaderStream;

		shaderStream << shaderFile.rdbuf();

		shaderFile.close();

		code = shaderStream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR: The Shader file was not successfully read." << std::endl;
	}

	return code;


}

unsigned int Shader::compileShader(const char* code, std::string type)
{
	unsigned int shader_id = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(shader_id, 1, &code, NULL));
	GLCall(glCompileShader(shader_id));
	checkCompileErrors(shader_id, type);
	return shader_id;
}

/* Constructor */
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	// Retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::string geometryCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	std::ifstream gShaderFile;

	// Ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// Open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;

		// Read files
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close file handlers
		vShaderFile.close();
		fShaderFile.close();

		// Convert to string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();

		// If geometry path is provided then load the geometry shader
		if (geometryPath != nullptr)
		{
			gShaderFile.open(geometryPath);
			std::stringstream gShaderStream;
			gShaderStream << gShaderFile.rdbuf();
			gShaderFile.close();
			geometryCode = gShaderStream.str();
		}
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << "ERROR: The Shader file was not successfully read." << std::endl;
	}

	// Store shader code in a C string
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;

	// Compile vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	checkCompileErrors(vertex, "VERTEX");

	// Compile fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	checkCompileErrors(fragment, "FRAGMENT");

	// Compile geometry shader (if present)
	unsigned int geometry;
	if (geometryPath != nullptr)
	{
		const char * gShaderCode = geometryCode.c_str();
		geometry = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(geometry, 1, &gShaderCode, NULL);
		glCompileShader(geometry);
		checkCompileErrors(geometry, "GEOMETRY");
	}

	// Create shader program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	if (geometryPath != nullptr)
	{
		glAttachShader(ID, geometry);
	}
	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	// Clean up (shaders are already linked)
	glDeleteShader(vertex);
	glDeleteShader(fragment);
	if (geometryPath != nullptr)
	{
		glDeleteShader(geometry);
	}

	/*
	std::string vCode = readShaderCode(vertexPath);
	std::string fCode = readShaderCode(fragmentPath);

	const char* vertex = vCode.c_str();
	const char* fragment = fCode.c_str();

	unsigned int v_id = compileShader(vertex, "vertex");
	unsigned int f_id =	compileShader(fragment, "fragment");
	unsigned int g_id;

	if (geometryPath != nullptr)
	{
		std::string gCode = readShaderCode(fragmentPath);
		const char* geometry = gCode.c_str();
		g_id = compileShader(geometry, "geometry");
	}

	glLinkProgram(ID);
	checkCompileErrors(ID, "PROGRAM");

	ID = glCreateProgram();
	glAttachShader(ID, v_id);
	glAttachShader(ID, f_id);
	if (geometryPath != nullptr)
	{
		glAttachShader(ID, g_id);
	}

	// Clean up (shaders are already linked)
	glDeleteShader(v_id);
	glDeleteShader(f_id);
	if (geometryPath != nullptr)
	{
		glDeleteShader(g_id);
	}
	*/

	
}


/*
* The following code is not our own.
* All credit should be given to learnopengl.com
*
*source: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*
*/

/* Functions (Public) */
void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const
{
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const
{
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const
{
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}


/*
* The following code is not our own.
* All credit should be given to learnopengl.com
*
*source: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*
*/


void Shader::setVec4(const std::string &name, float x, float y, float z, float w)
{
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string &name, const glm::mat4 &mat) const
{
	GLCall(glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]));
}

/* Utility Functions (Private) */
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
		}
	}
}