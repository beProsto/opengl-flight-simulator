#include "OWL/Utility/Vec3.hpp"
#include <OWL/Main.hpp>
#include <OWL/OWL.hpp>
#include <OWL/Time.hpp>
#include <util/pch.hpp>

#include <glad/glad.h>

#include <OWL/OpenGL.hpp>

#include <assimp/Importer.hpp>
#include <assimp/cimport.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <vector>

struct Vertex {
  OWL::Vec3f Position;
  OWL::Vec3f Normal;
  OWL::Vec2f TexCoords;
};

class Mesh {
public:
  Mesh(const std::vector<Vertex> &vertices,
       const std::vector<uint32_t> &indices) {
    setupMesh(vertices, indices);
  }

  void draw() const {
    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
  }

  static std::vector<Mesh> loadFromOBJ(const std::string &path) {
    std::vector<Mesh> meshes;
    Assimp::Importer importer;
    const aiScene *scene =
        importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs |
                                    aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
        !scene->mRootNode) {
      std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
      return meshes;
    }

    for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
      aiMesh *mesh = scene->mMeshes[i];
      std::vector<Vertex> vertices;
      std::vector<uint32_t> indices;

      for (unsigned int j = 0; j < mesh->mNumVertices; ++j) {
        Vertex vertex;
        vertex.Position = OWL::Vec3f(mesh->mVertices[j].x, mesh->mVertices[j].y,
                                     mesh->mVertices[j].z);
        if (mesh->HasNormals()) {
          vertex.Normal = OWL::Vec3f(mesh->mNormals[j].x, mesh->mNormals[j].y,
                                     mesh->mNormals[j].z);
        }
        if (mesh->mTextureCoords[0]) {
          vertex.TexCoords = OWL::Vec2f(mesh->mTextureCoords[0][j].x,
                                        mesh->mTextureCoords[0][j].y);
        } else {
          vertex.TexCoords = OWL::Vec2f(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
      }

      for (unsigned int j = 0; j < mesh->mNumFaces; ++j) {
        aiFace face = mesh->mFaces[j];
        for (unsigned int k = 0; k < face.mNumIndices; ++k) {
          indices.push_back(face.mIndices[k]);
        }
      }

      meshes.emplace_back(vertices, indices);
    }

    return meshes;
  }

private:
  GLuint m_VAO, m_VBO, m_EBO;
  GLsizei m_numIndices;

  void setupMesh(const std::vector<Vertex> &vertices,
                 const std::vector<uint32_t> &indices) {
    m_numIndices = indices.size();

    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
                 &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, TexCoords));

    glBindVertexArray(0);
  }
};