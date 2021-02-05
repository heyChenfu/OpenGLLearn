#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = "");
	~Shader();
	//����
	void use();
	// uniform���ߺ���
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
	void setVec2(const std::string& name, float value1, float value2) const;
	void setVec2(const std::string& name, glm::vec2) const;
	void setVec3(const std::string& name, float value1, float value2, float value3) const;
	void setVec3(const std::string& name, glm::vec3) const;
	void setMat4(const std::string& name, glm::mat4) const;

	unsigned int ID;

private:

};

#endif