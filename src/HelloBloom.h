#ifndef HELLO_BLOOM_H
#define HELLO_BLOOM_H

namespace HELLO_BLOOM {
	//·º¹âÐ§¹û
	class HelloBloom
	{
	public:
		HelloBloom();
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

