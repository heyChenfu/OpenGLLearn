#ifndef HELLO_SHADOW_MAPPING_H
#define HELLO_SHADOW_MAPPING_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader;

namespace HELLO_SHADOW_MAPPING {
	class HelloShadowMapping
	{
	public:
		HelloShadowMapping();
		void renderScene(unsigned int VAO, const Shader& shader);
		void renderCube();
		void renderQuad();
	private:
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		unsigned int quadVAO = 0;

	};
}


#endif
