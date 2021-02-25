#include "HelloOpenGL.h"
#include "HelloLight.h"
#include "HelloModel.h"
#include "HelloCubemap.h"
#include "HelloGeometryShader.h"
#include "HelloInstancing.h"
#include "HelloShadowMapping.h"
#include "HelloPointShadow.h"
#include "HelloNormalMapping.h"

int main() {
	//HelloOpenGL * hello = new HelloOpenGL();
	//HelloLight* hello = new HelloLight();
	//HelloModel* hello = new HelloModel();
	//HELLOCUBEMAP::HelloCubemap* hello = new HELLOCUBEMAP::HelloCubemap();
	//HELLO_GEOMETRY_SHADER::HelloGeometryShader * hello = new HELLO_GEOMETRY_SHADER::HelloGeometryShader();
	//HELLO_POINT_SHADOW::HelloPointShadow* hello = new HELLO_POINT_SHADOW::HelloPointShadow();
	HELLO_NORMAL_MAPPING::HelloNormalMapping* hello = new HELLO_NORMAL_MAPPING::HelloNormalMapping();

}