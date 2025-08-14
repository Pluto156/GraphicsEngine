#include "stdafx.h"
#include "CModel.h"
#include <iostream>
#include <SOIL.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <cmath>

// ----------- Mesh --------------
Mesh::Mesh(std::vector<Vertex> verts, std::vector<unsigned int> inds)
    : vertices(std::move(verts)), indices(std::move(inds)) {
}

void Mesh::Draw(bool useTexture, unsigned int textureID) const {
    // === 启用纹理 ===
    if (useTexture && textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    // === 启用客户端状态 ===
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].Position);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), &vertices[0].Normal);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].TexCoords);

    // === 第一次绘制：填充面 ===
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

    // === 如果启用描边 ===
    if (DebugManager::Instance().EnableGizmos) {
        glDisable(GL_TEXTURE_2D); // 轮廓线不使用纹理
        glColor3f(0.0f, 0.0f, 0.0f); // 黑色线条
        glLineWidth(1.5f);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // 改为线框绘制

        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // 恢复为填充模式
    }

    // === 关闭客户端状态 ===
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    if (useTexture && textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }
}


const std::vector<Vertex>& Mesh::GetVertices() const { return vertices; }
const std::vector<unsigned int>& Mesh::GetIndices() const { return indices; }

// ----------- MeshFilter --------------
void MeshFilter::RegisterFields(TypeInfo& info) {
    REGISTER_FIELD_CUSTOM(MeshFilter, meshes,
        [](const std::vector<Mesh>& oldMeshes, CloneContext&) {
            // 调用 vector 的复制构造
            return std::vector<Mesh>(oldMeshes);
        });

}

void MeshFilter::PostClone(CloneContext& ctx) {
}

void MeshFilter::SetPrimitive(PrimitiveType type, float size, int slices, int stacks) {
    meshes.clear();
    if (type == PrimitiveType::Cube)
        generateCube(size);
    else if (type == PrimitiveType::Sphere)
        generateSphere(size, slices, stacks);
}

void MeshFilter::LoadModel(const std::string& path) {
    meshes.clear();
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate |
        aiProcess_GenNormals |         // 自动生成法线
        aiProcess_FlipUVs |
        aiProcess_JoinIdenticalVertices |
        aiProcess_ImproveCacheLocality |
        aiProcess_SortByPType);
    if (!scene || !scene->HasMeshes()) {
        std::cerr << "Model load error: " << importer.GetErrorString() << std::endl;
        return;
    }
    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
        processMesh(scene->mMeshes[i]);
    }
}

void MeshFilter::processMesh(const aiMesh* mesh) {
std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex vertex;

        vertex.Position[0] = mesh->mVertices[i].x;
        vertex.Position[1] = mesh->mVertices[i].y;
        vertex.Position[2] = mesh->mVertices[i].z;

        if (mesh->HasNormals()) {
            vertex.Normal[0] = mesh->mNormals[i].x;
            vertex.Normal[1] = mesh->mNormals[i].y;
            vertex.Normal[2] = mesh->mNormals[i].z;
        }

        if (mesh->HasTextureCoords(0)) {
            vertex.TexCoords[0] = mesh->mTextureCoords[0][i].x;
            vertex.TexCoords[1] = mesh->mTextureCoords[0][i].y;
        }
        else {
            vertex.TexCoords[0] = 0.0f;
            vertex.TexCoords[1] = 0.0f;
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
        const aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    meshes.emplace_back(vertices, indices);
}

void MeshFilter::generateCube(float s) {
    s *= 0.5f;
    std::vector<Vertex> v = {
        // 前
        {{-s,-s,s},{0,0,1},{0,0}},{{s,-s,s},{0,0,1},{1,0}},{{s,s,s},{0,0,1},{1,1}},{{-s,s,s},{0,0,1},{0,1}},
        // 后
        {{-s,-s,-s},{0,0,-1},{1,0}},{{-s,s,-s},{0,0,-1},{1,1}},{{s,s,-s},{0,0,-1},{0,1}},{{s,-s,-s},{0,0,-1},{0,0}},
        // 左
        {{-s,-s,-s},{-1,0,0},{0,0}},{{-s,-s,s},{-1,0,0},{1,0}},{{-s,s,s},{-1,0,0},{1,1}},{{-s,s,-s},{-1,0,0},{0,1}},
        // 右
        {{s,-s,-s},{1,0,0},{0,0}},{{s,s,-s},{1,0,0},{0,1}},{{s,s,s},{1,0,0},{1,1}},{{s,-s,s},{1,0,0},{1,0}},
        // 上
        {{-s,s,-s},{0,1,0},{0,1}},{{-s,s,s},{0,1,0},{0,0}},{{s,s,s},{0,1,0},{1,0}},{{s,s,-s},{0,1,0},{1,1}},
        // 下
        {{-s,-s,-s},{0,-1,0},{0,1}},{{s,-s,-s},{0,-1,0},{1,1}},{{s,-s,s},{0,-1,0},{1,0}},{{-s,-s,s},{0,-1,0},{0,0}},
    };
    std::vector<unsigned int> i;
    for (int f = 0; f < 6; ++f) {
        int base = f * 4;
        i.push_back(base); i.push_back(base + 1); i.push_back(base + 2);
        i.push_back(base); i.push_back(base + 2); i.push_back(base + 3);
    }
    meshes.emplace_back(v, i);
}

void MeshFilter::generateSphere(float r, int slices, int stacks) {
    std::vector<Vertex> verts;
    std::vector<unsigned int> inds;
    for (int i = 0; i <= stacks; ++i) {
        float v = i / (float)stacks;
        float phi = M_PI * v;
        for (int j = 0; j <= slices; ++j) {
            float u = j / (float)slices;
            float theta = u * 2.0f * M_PI;
            float x = sin(phi) * cos(theta);
            float y = cos(phi);
            float z = sin(phi) * sin(theta);
            verts.push_back({ {r * x, r * y, r * z}, {x, y, z}, {u, v} });
        }
    }
    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            int first = i * (slices + 1) + j;
            int second = first + slices + 1;
            inds.push_back(first); inds.push_back(second); inds.push_back(first + 1);
            inds.push_back(second); inds.push_back(second + 1); inds.push_back(first + 1);
        }
    }
    meshes.emplace_back(verts, inds);
}


// 注册字段
void MeshRenderer::RegisterFields(TypeInfo& info) {
    // textureID 是 GPU 句柄，不直接 memcpy，clone 时需要重新加载纹理或标记重新加载
    REGISTER_FIELD_CUSTOM(MeshRenderer, textureID,
        [](unsigned int oldID, CloneContext&) {
            // 直接返回0，clone后需要重新load纹理
            return 0u;
        });
    REGISTER_FIELD_CUSTOM(MeshRenderer, texturePath,
        [](const std::string& oldStr, CloneContext&) {
            return std::string(oldStr);
        });

    REGISTER_FIELD(MeshRenderer, useTexture);
    REGISTER_FIELD(MeshRenderer, material);
}

// 克隆后调用，重新加载纹理
void MeshRenderer::PostClone(CloneContext& ctx) {
    if (useTexture && textureID == 0) {
        // 重新加载纹理逻辑（假设你有成员变量存路径，或者其他机制）
        // 这里示例调用 SetTexture，路径应存储或传递
        SetTexture(texturePath);
    }
}

MeshRenderer::MeshRenderer(const std::string& texturePath) {
    if (!texturePath.empty()) {
        this->texturePath = texturePath;
        loadTexture(texturePath);
        useTexture = true;
    }
}

void MeshRenderer::SetTexture(const std::string& path) {
    loadTexture(path);
    useTexture = textureID != 0;
}

void MeshRenderer::SetDiffuseColor(CVector3 diffuseColor)
{
    material.diffuseColor = diffuseColor;
}


void MeshRenderer::Draw() {
    auto* filter = gameObject->GetComponent<MeshFilter>();
    if (!filter || !transform) return;

    glPushMatrix();
    const CVector3& scale = transform->localScale;
    glScalef(scale.x, scale.y, scale.z);

    // 设置材质属性（用材质控制，不使用 glColor4f）
    GLfloat diffuse[] = {
        material.diffuseColor.x,
        material.diffuseColor.y,
        material.diffuseColor.z,
        1.0f
    };
    GLfloat specular[] = {
        material.specularColor.x,
        material.specularColor.y,
        material.specularColor.z,
        1.0f
    };
    GLfloat ambientMat[] = {
        material.diffuseColor.x * 0.2f,
        material.diffuseColor.y * 0.2f,
        material.diffuseColor.z * 0.2f,
        1.0f
    };
    GLfloat shininess = material.shininess;

    // 设置材质影响光照
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientMat);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    if (LightManager::Instance().EnableLighting)
    {
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);  // 启用颜色材质跟踪
        glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);  // 使用 glColor 设置环境+漫反射

        std::vector<PointLightData> lights;
        LightManager::Instance().GatherLightUniforms(lights);

        for (int i = 0; i < std::min(8, (int)lights.size()); ++i) {
            GLenum lightId = GL_LIGHT0 + i;
            glEnable(lightId);

            const auto& light = lights[i];
            GLfloat ambient[] = {
                light.color.x * light.intensity * 0.5f,
                light.color.y * light.intensity * 0.5f,
                light.color.z * light.intensity * 0.5f,
                1.0f
            };
            GLfloat col[] = {
                light.color.x * light.intensity,
                light.color.y * light.intensity,
                light.color.z * light.intensity,
                1.0f
            };
            GLfloat pos[] = {
                light.position.x,
                light.position.y,
                light.position.z,
                1.0f
            };

            glLightfv(lightId, GL_AMBIENT, ambient);
            glLightfv(lightId, GL_DIFFUSE, col);
            glLightfv(lightId, GL_SPECULAR, col);
            glLightfv(lightId, GL_POSITION, pos);
            glLightf(lightId, GL_CONSTANT_ATTENUATION, 1.0f);
            glLightf(lightId, GL_LINEAR_ATTENUATION, 0.0f);
            glLightf(lightId, GL_QUADRATIC_ATTENUATION, 0.0f);
        }

        // 清除未使用光源
        for (int i = lights.size(); i < 8; ++i) {
            glDisable(GL_LIGHT0 + i);
        }

        // 设置绘制颜色（将影响 GL_COLOR_MATERIAL 追踪）
        glColor4f(
            material.diffuseColor.x,
            material.diffuseColor.y,
            material.diffuseColor.z,
            1.0f
        );
    }
    else {
        glDisable(GL_LIGHTING);
        glDisable(GL_COLOR_MATERIAL);
        glColor4f(
            material.diffuseColor.x,
            material.diffuseColor.y,
            material.diffuseColor.z,
            1.0f
        );
    }

    // 处理纹理
    if (useTexture && textureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        if (LightManager::Instance().EnableLighting)
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);  // 光照混合纹理
        else
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);   // 无光照直接用纹理色
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    // 绘制网格
    for (const auto& mesh : filter->meshes) {
        mesh.Draw(useTexture, textureID);
    }

    glPopMatrix();
}



void MeshRenderer::loadTexture(const std::string& path) {
    textureID = SOIL_load_OGL_texture(
        path.c_str(),
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y
    );

    if (!textureID) {
        std::cerr << "Failed to load texture: " << path << std::endl;
        useTexture = false;
        return;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}
