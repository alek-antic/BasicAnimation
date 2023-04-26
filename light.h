//
//  light.hpp
//  cse169
//
//  Created by Alek Antic on 1/28/20.
//

#ifndef light_hpp
#define light_hpp

#include <stdio.h>
#include "vector3.h"
#include <glm/glm.hpp>

class Light {
public:
    Light(glm::vec3 pos, glm::vec3 color);
    
    glm::vec3 getPos();
    glm::vec3 getColor();
private:
    glm::vec3 Position;
    glm::vec3 Color;
};


#endif /* light_hpp */
