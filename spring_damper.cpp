//
//  spring_damper.cpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#include "cloth.h"
#include <iostream>

// k_s in kg/s^2 (N/m)
// k_d in kg/s   (N*s/m)
// l_0 in meters
SpringDamper::SpringDamper(Particle * p1, Particle * p2, float k_s, float k_d, float l_0) {
    this->p1 = p1;
    this->p2 = p2;
    this->spring_const = k_s;
    this->damping_factor = k_d;
    this->rest_length = l_0;
}

void SpringDamper::ComputeForce() {
    glm::vec3 e = this->p2->getPos() - this->p1->getPos();
    float l = glm::length(e);
    e = glm::normalize(e);
    
    float v1 = glm::dot(e, this->p1->getVel());
    float v2 = glm::dot(e, this->p2->getVel());
    
    float d_l = this->rest_length - l;
    float d_v = v1-v2;
    
    float f_sd = -this->spring_const * d_l - this->damping_factor * d_v;
    
//    if (d_v == 0.0f) {
//           std::cout << "No velocity to dampen -- v1: " << v1 << " v2: " << v2<< " f_sd: " << damping_factor * d_v << std::endl;
//       }
    
    glm::vec3 f_app = f_sd * e;
    glm::vec3 f_opp = -f_sd * e;
    this->p1->ApplyForce(f_app);
    this->p2->ApplyForce(f_opp);
}
