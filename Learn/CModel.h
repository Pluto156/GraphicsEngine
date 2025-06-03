#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Component.h"

// ����ṹ��
struct Vertex {
    float Position[3];
    float Normal[3];
    float TexCoords[2];
};

// ����������
class Mesh {
public:
    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds, unsigned int texID);
    void Draw();

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int textureID; // OpenGL ���� ID
};

// ģ����
class MeshRenderer : public Component {
public:
    static ComponentType GetType();
public:
    MeshRenderer(const std::string& modelPath, const std::string& texturePath = "");
    void Draw();

private:
    std::vector<Mesh> meshes;
    unsigned int textureID = 0;
    void loadModel(const std::string& path);
    void processMesh(const struct aiMesh* mesh);
    void loadTexture(const std::string& texturePath);
};
