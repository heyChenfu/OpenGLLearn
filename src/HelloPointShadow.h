#ifndef HELLO_POINT_SHADOW_H
#define HELLO_POINT_SHADOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Shader;

namespace HELLO_POINT_SHADOW {
	class HelloPointShadow
	{
	public:
		HelloPointShadow();
		void renderScene(const Shader& shader);
		void renderCube();
		void renderQuad();
	private:
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		unsigned int quadVAO = 0;


	};
}


#endif
