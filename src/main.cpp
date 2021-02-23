#include "HelloOpenGL.h"
#include "HelloLight.h"
#include "HelloModel.h"
#include "HelloCubemap.h"
#include "HelloGeometryShader.h"
#include "HelloInstancing.h"
#include "HelloShadowMapping.h"

int main() {
	//HelloOpenGL * hello = new HelloOpenGL();
	//HelloLight* hello = new HelloLight();
	//HelloModel* hello = new HelloModel();
	//HELLOCUBEMAP::HelloCubemap* hello = new HELLOCUBEMAP::HelloCubemap();
	//HELLO_GEOMETRY_SHADER::HelloGeometryShader * hello = new HELLO_GEOMETRY_SHADER::HelloGeometryShader();
	HELLO_SHADOW_MAPPING::HelloShadowMapping* hello = new HELLO_SHADOW_MAPPING::HelloShadowMapping();

}