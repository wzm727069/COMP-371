#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <glm/glm.hpp>

/*
*The code for the following class is not our own.
* All credit should be given to learnopengl.com
*
*source: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
*
*/
class Shader
{
public:
	/* Attributes */
	unsigned int ID;	// This ID corresponds to the ID of the shader program

	/* Constructor */
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

	std::string readShaderCode(const char* path);
	unsigned int compileShader(const char* code, std::string type);


	
	/* Functions */
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, const glm::vec2 &value) const;
	void setVec2(const std::string &name, float x, float y) const;
	void setVec3(const std::string &name, const glm::vec3 &value) const;
	void setVec3(const std::string &name, float x, float y, float z) const;
	void setVec4(const std::string &name, const glm::vec4 &value) const;
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat) const;
	void setMat3(const std::string &name, const glm::mat3 &mat) const;
	void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
	/* Utility Functions */
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif