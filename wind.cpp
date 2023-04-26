//
//  wind.cpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#include <stdio.h>
#include "cloth.h"

#include <glm/gtx/transform.hpp>

Wind::Wind(float mag, float theta, float density) {
    this->mag = mag;
    this->theta = theta;
    this->density = density;
    
    this->v_air = glm::vec3(1.0f, 0.0f, 0.0f);
}

glm::vec3 Wind::getVel() {
    glm::mat4 model = glm::rotate(this->theta, glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::vec3(this->mag));
    return glm::vec3(model * glm::vec4(this->v_air, 0.0f));
}
