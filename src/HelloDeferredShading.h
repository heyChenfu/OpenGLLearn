#ifndef HELLO_DEFERRED_SHADING_H
#define HELLO_DEFERRED_SHADING_H

namespace HELLO_DEFERRED_SHADING {
	//—”≥Ÿ‰÷»æ
	class HelloDeferredShading
	{
	public:
		HelloDeferredShading();
		void renderCube();
		void renderQuad();

	private:
		unsigned int cubeVAO = 0;
		unsigned int cubeVBO = 0;
		unsigned int quadVAO = 0;
		unsigned int quadVBO = 0;

	};

}


#endif

