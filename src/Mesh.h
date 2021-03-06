#ifndef MESH_H
#define MESH_H
#include <vector>
#include "Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent; //切线
    glm::vec3 Bitangent; //副切线
};

struct Texture {
    unsigned int id;
    string type;
    string path;  // 我们储存纹理的路径用于与其它纹理进行比较, 避免加载重复的纹理
};

//材质类
class Mesh
{
public:
    /*  网格数据  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    /*  函数  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);
    void DrawInstanced(Shader shader, int amount);

    /*  渲染数据  */
    unsigned int VAO, VBO, EBO;
private:
    /*  函数  */
    void setupMesh();
    void BindTexture(Shader shader);
};

#endif
