#ifndef MESH_H
#define MESH_H
#include <vector>
#include "Shader.h"

using namespace std;

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
    glm::vec3 Tangent;
    glm::vec3 Bitangent;
};

struct Texture {
    unsigned int id;
    string type;
    string path;  // ���Ǵ��������·������������������бȽ�, ��������ظ�������
};

//������
class Mesh
{
public:
    /*  ��������  */
    vector<Vertex> vertices;
    vector<unsigned int> indices;
    vector<Texture> textures;
    /*  ����  */
    Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures);
    void Draw(Shader shader);

private:
    /*  ��Ⱦ����  */
    unsigned int VAO, VBO, EBO;
    /*  ����  */
    void setupMesh();
};

#endif
