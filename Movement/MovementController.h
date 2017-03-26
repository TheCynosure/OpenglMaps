//
// Created by jack on 3/22/17.
//

#ifndef OPENGLLEARNING_MOVEMENTCONTROLLER_H
#define OPENGLLEARNING_MOVEMENTCONTROLLER_H

#include <glm/vec3.hpp>
#include <iostream>

//Handles the movement of a Camera object through a series of methods
class MovementController {
public:
    MovementController(const glm::vec3 &startingPosition, const glm::vec3 &startingTarget, const glm::vec3 &startingUp) {
        position = startingPosition;
        target = startingTarget;
        up = startingUp;
    };
    virtual void translate(glm::vec2 units) = 0;
    virtual void rotate(float angle, glm::vec3 axis) = 0;
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
};


#endif //OPENGLLEARNING_MOVEMENTCONTROLLER_H
