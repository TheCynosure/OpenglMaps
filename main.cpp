#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include "Rendering/shaders.h"
#include "Rendering/ObjModel.h"
#include "Movement/Camera.h"
#include "Movement/InputController.h"

void error_callback(int error, const char* desc) {
    fprintf(stderr, "Error:  %s\n", desc);
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    if(severity == GL_DEBUG_SEVERITY_MEDIUM || severity == GL_DEBUG_SEVERITY_HIGH) {
        std::cout << "Error: " << message << std::endl;
    }
}

GLuint loadBMP(const char* filePath) {
    //TODO: Write a method that can load a gif and then put it as a texture.

    FILE* file = fopen(filePath, "rb");
    if(!file) {
        std::cout << "File could not be opened: " << filePath << std::endl;
        return 0;
    }
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;
    unsigned char * data;

    if(fread(header, 1, 54, file) != 54 || header[0] != 'B' || header[1] != 'M') {
        std::cout << "Not a correct bmp file: " << filePath << std::endl;
        return 0;
    }

    dataPos = *(int*)&(header[0x0A]);
    imageSize = *(int*)&(header[0x22]);
    width = *(int*)&(header[0x12]);
    height = *(int*)&(header[0x16]);

    if(imageSize == 0)
        imageSize = width * height * 3;
    if(dataPos == 0)
        dataPos = 54;

    data = new unsigned char [imageSize];
    fread(data, 1, imageSize, file);
    fclose(file);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return texID;
}

int main() {

    if(!glfwInit()) {
        std::cerr << "GLFW Failed to initialize" << std::endl;
        return -1;
    }

    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Testing", NULL, NULL);
    if(!window) {
        //Window is NULL
        std::cerr << "Window failed to be created" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        std::cerr << "GLEW Init error" << std::endl;
        return -1;
    } else {
    #ifdef DEBUG
        //Check if we support a debug callback
        if(GL_KHR_debug) {
            //Setup a openGL Error callback
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debugCallback, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
        }
        #endif
    }

    //Draw only the closest triangles.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint programID = LoadShaders("shaders/vertexShader.vs", "shaders/fragShader.fs");
    GLuint debugProgramID = LoadShaders("shaders/simpleVertexShader.vs", "shaders/simplefragShader.fs");

    float debugLineLength = 1000.0f;

    float debugLineVertices[] = {
            0,0,-debugLineLength,
            0, 0, debugLineLength,
            0,-debugLineLength,0,
            0,debugLineLength,0,
            -debugLineLength,0,0,
            debugLineLength,0,0,
    };

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    GLuint matrixID = (GLuint) glGetUniformLocation(programID, "mvp");
    GLuint debugMatrixID = (GLuint) glGetUniformLocation(debugProgramID, "mvp");

    GLuint textureID = loadBMP("textures/UV-Texture.bmp");

    ObjModel model;
    if(!model.loadModel("models/chicken.obj")) {
        std::cerr << "Model failed to load" << std::endl;
        glfwTerminate();
        return 0;
    }
    model.setTexture(textureID);

    GLuint debugLinesVAO;
    glGenVertexArrays(1, &debugLinesVAO);
    glBindVertexArray(debugLinesVAO);

    GLuint debugLinesBuffer = 0;
    glGenBuffers(1, &debugLinesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, debugLinesBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(debugLineVertices), debugLineVertices, GL_STATIC_DRAW);

    Camera camera(glm::vec3(0,0,30), glm::vec3(0,0,0), glm::vec3(0,1,0), width, height);
    InputController inputController(&camera, width, height);

    glfwSetCursorPos(window, width/2, height/2);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    do {
        glClearColor(1.0f,1.0f,1.0f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 modelViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix() * modelMatrix;

        glUseProgram(programID);
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &modelViewMatrix[0][0]);
        model.drawModel(&programID);

        modelViewMatrix = camera.getProjectionMatrix() * camera.getViewMatrix();
        glUseProgram(debugProgramID);
        glUniformMatrix4fv(debugMatrixID, 1, GL_FALSE, &modelViewMatrix[0][0]);
        glBindVertexArray(debugLinesVAO);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, debugLinesBuffer);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void*)0
        );

        glDrawArrays(GL_LINES, 0, sizeof(debugLineVertices) / sizeof(GL_FLOAT) / 3);

        glfwSwapBuffers(window);
        inputController.handleInput(window);
    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    glfwDestroyWindow(window);
    return 0;
}