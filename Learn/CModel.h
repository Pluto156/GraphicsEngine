#pragma once
#include <vector>
#include <string>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Component.h"

// 顶点结构体
struct Vertex {
    float Position[3];
    float Normal[3];
    float TexCoords[2];
};

class Mesh {
public:
    Mesh() = default;  //保证 Mesh 可以默认构造（用于 std::vector、克隆、序列化等情景）
    Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds);
    void Draw(bool useTexture, unsigned int textureID) const;

    const std::vector<Vertex>& GetVertices() const;
    const std::vector<unsigned int>& GetIndices() const;

private:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
};

// 基础几何体类型
enum class PrimitiveType {
    Cube,
    Sphere
};

// MeshFilter：负责持有 Mesh 数据
class MeshFilter : public Component {
    REGISTER_COMPONENT_DERIVED(MeshFilter, ComponentType::MeshFilter, Component)
public:
    std::vector<Mesh> meshes;

    void SetPrimitive(PrimitiveType type, float size = 1.0f, int slices = 16, int stacks = 16);
    void LoadModel(const std::string& path);

private:
    void generateCube(float size);
    void generateSphere(float radius, int slices, int stacks);
    void processMesh(const struct aiMesh* mesh);
};

// MeshRenderer：负责绘制
struct Material {
    CVector3 diffuseColor = { 1.0f, 1.0f, 1.0f }; // 漫反射颜色
    CVector3 specularColor = { 1.0f, 1.0f, 1.0f }; //高光颜色
    float shininess = 32.0f; // 高光强度
};
class MeshRenderer : public Component {
    REGISTER_COMPONENT_DERIVED(MeshRenderer, ComponentType::MeshRenderer, Component)
public:

    MeshRenderer(const std::string& texturePath = "");

    void Draw();
    void SetMaterial(const Material& mat) { material = mat; }
    void SetTexture(const std::string& path);
    const Material& GetMaterial() const { return material; }
    void SetDiffuseColor(CVector3 diffuseColor);

private:
    std::string texturePath;
    unsigned int textureID = 0;
    bool useTexture = false;
    Material material;

    void loadTexture(const std::string& texturePath);
};



