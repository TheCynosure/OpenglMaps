//
// Created by jack on 3/22/17.
//

#ifndef OPENGLLEARNING_FREEMOVEMENT_H
#define OPENGLLEARNING_FREEMOVEMENT_H


#include "MovementController.h"

class FreeMovement : public MovementController {
public:
    FreeMovement(const glm::vec3 &startingPosition, const glm::vec3 &startingTarget, const glm::vec3 &startingUp)
            : MovementController(startingPosition, startingTarget, startingUp) {};
    virtual void translate(glm::vec2 units);
    virtual void rotate(float angle, glm::vec3 axis);
private:
    glm::vec3 direction;
};


#endif //OPENGLLEARNING_FREEMOVEMENT_H
