//
// Created by jack on 3/19/17.
//

#include <cstdio>
#include <iostream>
#include <cstring>
#include <fstream>
#include <glm/vec2.hpp>
#include "ObjModel.h"

bool ObjModel::loadModel(const char *filePath) {
    //Hold the temporary vertices, uvs, and normalIndices
    std::vector<glm::vec3> temp_verts, temp_normals;
    std::vector<glm::vec2> temp_uvs;
    //Hold the indices from the faces.
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;

    //Open the file for reading
    std::ifstream file(filePath);
    if(!file.is_open()) {
        std::cerr << "Was not able to open file: " << filePath << std::endl;
        return false;
    }

    std::string line;
    while(std::getline(file, line)) {
        //Read the current line and check if we are at the end of the file.
        if(line.compare(0, 2, "v ") == 0) {
            //Create and load a vertice
            glm::vec3 vertex;
            sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_verts.push_back(vertex);
        } else if(line.compare(0, 2, "vt") == 0) {
            //Create and load a uv
            glm::vec2 uv;
            sscanf(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
            temp_uvs.push_back(uv);
        } else if(line.compare(0, 2, "vn") == 0) {
            //Create and load a normal
            glm::vec3 normal;
            sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        } else if(line.compare(0, 2, "f ") == 0) {
            //Create and load a face.
            unsigned int temp_vertex_indices[3]; // The vertex indices
            unsigned int temp_uv_indices[3]; // The UV indices
            unsigned int temp_normal_indices[3]; // The Normal indices
            //Load the indices into their proper arrays
            int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n", &temp_vertex_indices[0], &temp_uv_indices[0], &temp_normal_indices[0], &temp_vertex_indices[1], &temp_uv_indices[1], &temp_normal_indices[1], &temp_vertex_indices[2], &temp_uv_indices[2], &temp_normal_indices[2]);
            //Make sure we loaded all of them
            if(matches < 9) {
                std::cerr << "Cannot read the faces, must be too complex for such a simple reader: " << filePath
                          << std::endl;
                return false;
            }
            //Put them into our complete list of all indexes.
            vertexIndices.push_back(temp_vertex_indices[0]);
            vertexIndices.push_back(temp_vertex_indices[1]);
            vertexIndices.push_back(temp_vertex_indices[2]);
            uvIndices.push_back(temp_uv_indices[0]);
            uvIndices.push_back(temp_uv_indices[1]);
            uvIndices.push_back(temp_uv_indices[2]);
            normalIndices.push_back(temp_normal_indices[0]);
            normalIndices.push_back(temp_normal_indices[1]);
            normalIndices.push_back(temp_normal_indices[2]);
        }
    }

    //Index the vertices, uvs, and normals.
    for(unsigned int i = 0; i < vertexIndices.size(); i++) {
        // Get the current vertex in the order that the faces specified the indexes
        // Make sure to subtract 1 from the vertexIndices as they are not 0 based
        glm::vec3 vertex = temp_verts[vertexIndices[i] - 1];
        vertices.push_back(vertex);
    }

    //Indexing the UVs
    for(unsigned int i = 0; i < uvIndices.size(); i++) {
        glm::vec2 uv = temp_uvs[uvIndices[i] - 1];
        uvs.push_back(uv);
    }

    //Indexing the Normals
    for(unsigned int i = 0; i < normalIndices.size(); i++) {
        glm::vec3 normal = temp_normals[normalIndices[i] - 1];
        normals.push_back(normal);
    }

    genBuffersForModel();
    return true;
}

void ObjModel::genBuffersForModel() {
    glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenBuffers(1, &vertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    glGenBuffers(1, &uvBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void ObjModel::drawModel(GLuint *shaderID) {
//    glUseProgram(*shaderID);
    glBindVertexArray(vaoID);

    if(textureID != 0) {
        glBindTexture(GL_TEXTURE_2D, textureID);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
    glVertexAttribPointer(
            0,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
    glVertexAttribPointer(
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0
    );

    glDrawArrays(GL_TRIANGLES, 0, (GLsizei) vertices.size());
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
}
