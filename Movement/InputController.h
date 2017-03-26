//
// Created by jack on 3/24/17.
//

#ifndef OPENGLLEARNING_INPUTCONTROLLER_H
#define OPENGLLEARNING_INPUTCONTROLLER_H


#include "Camera.h"

class InputController {
public:
    InputController(Camera *camera, int width, int height) {
        this->camera = camera;
        lastX = width / 2;
        lastY = height / 2;
    }
    void handleInput(GLFWwindow *window) {
        glfwPollEvents();
        handleMouseInput(window);
        handleKeyInput(window);
    }
    void handleMouseInput(GLFWwindow *window);
    void handleKeyInput(GLFWwindow *window);
private:
    Camera *camera;
    double lastX, lastY;
};


#endif //OPENGLLEARNING_INPUTCONTROLLER_H
