#include "HelloOpenGL.h"
#include "HelloLight.h"
#include "HelloModel.h"
#include "HelloCubemap.h"
#include "HelloGeometryShader.h"
#include "HelloInstancing.h"
#include "HelloShadowMapping.h"
#include "HelloPointShadow.h"
#include "HelloNormalMapping.h"
#include "HelloParallaxMapping.h"
#include "HelloBloom.h"
#include "HelloDeferredShading.h"

int main() {
	//HelloOpenGL * hello = new HelloOpenGL();
	//HelloLight* hello = new HelloLight();
	//HelloModel* hello = new HelloModel();
	//HELLOCUBEMAP::HelloCubemap* hello = new HELLOCUBEMAP::HelloCubemap();
	//HELLO_GEOMETRY_SHADER::HelloGeometryShader * hello = new HELLO_GEOMETRY_SHADER::HelloGeometryShader();
	//HELLO_POINT_SHADOW::HelloPointShadow* hello = new HELLO_POINT_SHADOW::HelloPointShadow();
	//HELLO_NORMAL_MAPPING::HelloNormalMapping* hello = new HELLO_NORMAL_MAPPING::HelloNormalMapping();
	//HELLO_PARALLAX_MAPPING::HelloParallaxMapping* hello = new HELLO_PARALLAX_MAPPING::HelloParallaxMapping();
	//HELLO_BLOOM::HelloBloom* hello = new HELLO_BLOOM::HelloBloom();
	HELLO_DEFERRED_SHADING::HelloDeferredShading* hello = new HELLO_DEFERRED_SHADING::HelloDeferredShading();

}