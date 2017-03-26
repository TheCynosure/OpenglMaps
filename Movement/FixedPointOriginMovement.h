//
// Created by jack on 3/22/17.
//

#ifndef OPENGLLEARNING_CENTEREDMOVEMENT_H
#define OPENGLLEARNING_CENTEREDMOVEMENT_H

#include <glm/vec2.hpp>
#include "MovementController.h"

//A type of movement for moving around a fixed point easily
class FixedPointOriginMovement : public MovementController {
public:
    FixedPointOriginMovement(glm::vec3 &startingPosition, glm::vec3 &startingTarget, glm::vec3 &startingUp)
            : MovementController(startingPosition, startingTarget, startingUp) {}
    virtual void translate(glm::vec2 units);
    virtual void rotate(float angle, glm::vec3 axis);
};


#endif //OPENGLLEARNING_CENTEREDMOVEMENT_H
