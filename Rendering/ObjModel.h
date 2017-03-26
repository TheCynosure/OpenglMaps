//
// Created by jack on 3/19/17.
//

#ifndef OPENGLLEARNING_LOADOBJMODEL_H
#define OPENGLLEARNING_LOADOBJMODEL_H

#include <GL/glew.h>
#include <vector>
#include <glm/vec3.hpp>

class ObjModel {
public:
    bool loadModel(const char* filePath);
    void setTexture(GLuint textureID) {
        this->textureID = textureID;
    }
    void drawModel(GLuint *shaderID);
private:
    GLuint vaoID;
    GLuint vertexBufferID;
    GLuint uvBufferID;
    GLuint textureID = 0;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    void genBuffersForModel();
};


#endif //OPENGLLEARNING_LOADOBJMODEL_H
