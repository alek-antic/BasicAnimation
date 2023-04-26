//
//  box.hpp
//  cse169
//
//  Created by Alek Antic on 3/3/20.
//

#ifndef box_h
#define box_h

#include <stdio.h>
#include <vector>
#include <glm/glm.hpp>

#include "quaternion.h"

class Box {
private:
    glm::vec3 pos;
    Quaternion rot;
    glm::vec3 size;
    
    glm::mat4 world;
    
public:
    Box(float x, float y, float z, float xPos);
    
    void Update();
    void Draw();
    
    void setPose(Quaternion q) { rot = Quaternion::normalize(q); }
    void moveBy(float delta_x) { pos.x += delta_x; }
    void moveTo(glm::vec3 loc) { pos = loc; }
    
    Quaternion getOrientation() { return rot; }
};

#endif /* box_h */
