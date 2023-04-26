//
//  light.cpp
//  cse169
//
//  Created by Alek Antic on 1/28/20.
//

#include "light.h"


Light::Light(glm::vec3 pos, glm::vec3 color) {
    this->Position = pos;
    this->Color = color;
}

glm::vec3 Light::getPos() {
    return this->Position;
}

glm::vec3 Light::getColor() {
    return this->Color;
}
