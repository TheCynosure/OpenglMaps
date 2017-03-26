//
// Created by jack on 3/24/17.
//

#include <GLFW/glfw3.h>
#include "InputController.h"


int movementSpeedMax = 6;
int movementSpeedMin = 1;
int movementSpeed = movementSpeedMin;
float mouseSpeed = 0.5;

void InputController::handleMouseInput(GLFWwindow *window) {
    double posx, posy;
    int width, height;
    glfwGetCursorPos(window, &posx, &posy);
    glfwGetWindowSize(window, &width, &height);
    float xDif = (float) (lastX - posx);
    float yDif = (float) (lastY - posy);
    //TODO: Commented out rotation for testing purposes
    camera->rotate(mouseSpeed * xDif, glm::vec3(0,1,0));
    camera->rotate(mouseSpeed * yDif, glm::vec3(1,0,0));
    lastX = posx;
    lastY = posy;
}

void InputController::handleKeyInput(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        //Move the camera forward
        camera->translate(glm::vec2(-movementSpeed, 0));
    }
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        //Move the camera backward
        camera->translate(glm::vec2(movementSpeed, 0));
    }
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        //Move the camera left
        camera->translate(glm::vec2(0, -movementSpeed));
    }
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        //Move the camera right
        camera->translate(glm::vec2(0, movementSpeed));
    }
}
