#ifndef MODEL_H
#define MODEL_H
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"

class Model
{
public:
    /*  ����   */
    Model(string path);
    void Draw(Shader shader);
    void DrawInstanced(Shader shader, int amount);
    int GetMeshesCount();
    unsigned int GetVAO(int meshIdx);
private:
    /*  ģ������  */
    vector<Mesh> meshes;
    string directory;
    vector<Texture> textures_loaded;
    /*  ����   */
    void loadModel(string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
        string typeName);
};

#endif