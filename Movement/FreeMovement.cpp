//
// Created by jack on 3/22/17.
//

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "FreeMovement.h"

//Small Method to return a negative if less than zero
//And a positive if more than zero
float getSign(float testValue) {
    if(testValue < 0) {
        return -1.0f;
    }
    return 1.0f;
}

void FreeMovement::translate(glm::vec2 units) {
    //Best explanation I could find for this calculation: http://math.stackexchange.com/questions/333350/moving-point-along-the-vector
    //Basically we are going to be multiply the vector difference between the camera
    //position and the target position by the fraction of the units.x / the total length between the two.

    //To do this we need the total length between the two. The length in the three directions
    glm::vec3 difference = position - target;
    //The distance is calculated with the simple formula of a^2 = x^2 + y^2 + z^2
    float distanceBetweenPT = (float) sqrt(pow(difference.x, 2) + pow(difference.y, 2) + pow(difference.z, 2));
    //If you read the stackOverflow post then distanceBetweenPT is like the BP vector.
    //Then we need to move the camera by adding a fraction of the distanceBetweenPT vector
    glm::vec3 forwardBackwardChange = ((units.x / distanceBetweenPT) * difference);
    //Now we need to do the same thing but for the left and right movement, which is
    //Just in a different direction. We are going to do the same calculation but then rotate it.
    //Recalculate the difference as it is rotated 90 degrees.
    difference = glm::vec3(glm::rotate(glm::mat4(1.0f), glm::radians(getSign(units.y) * 90.0f), glm::vec3(0,1,0)) * glm::vec4(difference,0));
    //Calculate the units needed to move in that direction.
    glm::vec3 leftRightTranslation = ((units.y / distanceBetweenPT) * difference);
    //Cancel out the y movement as this is only left and right strafing. Also vary the direction based on the negativity of the movement units.
    leftRightTranslation = glm::vec3(leftRightTranslation.x * getSign(units.y), 0, leftRightTranslation.z * getSign(units.y));
    //Translate the position of the camera
    position += leftRightTranslation + forwardBackwardChange;
    //Translate the position of the target as well.
    target += leftRightTranslation + forwardBackwardChange;
}

void FreeMovement::rotate(float angle, glm::vec3 axis) {
    //The distance between the camera and the target.
    float distanceX = position.x - target.x;
    float distanceY = position.y - target.y;
    float distanceZ = position.z - target.z;

    //Translation matrix to move the point to the camera.
    glm::mat4 translateToCamera = glm::translate(glm::mat4(1.0f), glm::vec3(distanceX, distanceY, distanceZ));

    //Rotation matrix used to rotate the point
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);

    //Moving the target to the camera. X Y Z W <- W must be 1 for translations to work.
    glm::vec4 targetMovedToCamera = translateToCamera * glm::vec4(target, 1);

    //Moving the target by the translation Back vector
    //TODO: X Rotation becomes inverted at negative Z's because negating the Z only works when positive Z is there.
    glm::vec3 translateBackVector = glm::vec3(rotationMatrix * glm::vec4(-distanceX, -distanceY, -distanceZ, 0));

    //Translation matrix based on our translate vector
    glm::mat4 translateBackMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translateBackVector.x, translateBackVector.y, translateBackVector.z));

    //Translate the target by our translation back matrix
    target = glm::vec3(translateBackMatrix * targetMovedToCamera);
    glm::vec3 right = glm::vec3(glm::rotate(glm::mat4(1.0f), (float) (M_PI / 2), glm::vec3(0,1,0)) * glm::vec4(translateBackVector,0));
    right = glm::vec3(right.x, position.y, right.z);
    std::cout << position.x << ":" << -target.x << " " << position.y << " " << position.z << ":" << -target.z << std::endl;
    up = glm::cross(right, target);
}
