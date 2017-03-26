//
// Created by jack on 3/22/17.
//

#ifndef OPENGLLEARNING_CAMERA_H
#define OPENGLLEARNING_CAMERA_H


#include <glm/vec3.hpp>
#include <glm/detail/type_mat.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MovementController.h"
#include "FreeMovement.h"

class Camera {
public:
    Camera(const glm::vec3 &startingPosition, const glm::vec3 &startingTarget, const glm::vec3 &startingUp, int screenWidth, int screenHeight) {
        FreeMovement *freeMovement = new FreeMovement(startingPosition, startingTarget, startingUp);
        movementController = freeMovement;
        projectionMatrix = glm::perspective(FOV, (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
    }

    ~Camera() {
        free(movementController);
    }

    glm::mat4 getViewMatrix() {
        return glm::lookAt(movementController->position, movementController->target, movementController->up);
    }

    glm::mat4 getProjectionMatrix() {
        return projectionMatrix;
    }

    MovementController* getMovementController() {
        return movementController;
    }

    void setMovementController(MovementController &movementController) {
        //Assign a pointer of movementController to our movement controller
        //This is because abstract classes can only be referenced through pointers
        //As they do not have fixed size in memory! (Makes sense)
        this->movementController = &movementController;
    }

    //Wrapper for the movementController methods
    void rotate(double angle, glm::vec3 axis) {
        movementController->rotate(angle, axis);
    }

    void translate(glm::vec2 units) {
        movementController->translate(units);
    }
private:
    const float FOV = 45.0f;
    MovementController* movementController;
    glm::mat4 projectionMatrix;
};


#endif //OPENGLLEARNING_CAMERA_H
