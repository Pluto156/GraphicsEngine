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
    if (useTexture && textureID != 0) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    glVertexPointer(3, GL_FLOAT, sizeof(Vertex), &vertices[0].Position);
    glNormalPointer(GL_FLOAT, sizeof(Vertex), &vertices[0].Normal);
    glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), &vertices[0].TexCoords);

    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());

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
ComponentType MeshFilter::GetType() {
    return ComponentType::MeshFilter;
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


ComponentType MeshRenderer::GetType() {
    return ComponentType::MeshRenderer;
}

MeshRenderer::MeshRenderer(const std::string& texturePath) {
    if (!texturePath.empty()) {
        loadTexture(texturePath);
        useTexture = true;
    }
}

void MeshRenderer::SetTexture(const std::string& path) {
    loadTexture(path);
    useTexture = textureID != 0;
}

void MeshRenderer::Draw() {
    auto* filter = gameObject->GetComponent<MeshFilter>();
    if (!filter || !transform) return;

    glPushMatrix();

    // 应用缩放
    const CVector3& scale = transform->localScale;
    glScalef(scale.x, scale.y, scale.z);



    // 禁用 ColorMaterial（避免颜色被 glColor 覆盖）
    glDisable(GL_COLOR_MATERIAL);

    // 设置材质参数（用于固定管线光照）
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
    GLfloat shininess = material.shininess;

    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

    // 如果使用纹理，启用并设置纹理环境为 MODULATE（乘以材质颜色）
    if (useTexture && textureID) {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    }
    else {
        glDisable(GL_TEXTURE_2D);
    }

    // 绘制所有 mesh
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
