//
//  triangle.cpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#include <stdio.h>
#include "cloth.h"

// drag is unitless
Triangle::Triangle(Particle * p1, Particle * p2, Particle * p3, Wind * wind, float drag) {
    this->p1 = p1;
    this->p2 = p2;
    this->p3 = p3;
    this->wind = wind;
    this->drag = drag;
}

void Triangle::ComputeForce() {
    glm::vec3 n = glm::cross(this->p2->getPos() - this->p1->getPos(), this->p3->getPos() - this->p1->getPos());
    float area = glm::length(n) * 0.5f;
    n = glm::normalize(n);
    glm::vec3 v_surface = this->p1->getVel() + this->p2->getVel() + this->p3->getVel();
    v_surface /= 3.0f;
    
    glm::vec3 v = v_surface - this->wind->getVel();
    
    area = area * glm::dot(v, n) / glm::length(v);
    
    glm::vec3 force = - 1.0f / 2.0f * this->wind->getDensity() * glm::dot(v, v) * drag * area * n;
    
    if (!glm::isnan(force.x) && !glm::isnan(force.y) && !glm::isnan(force.z)) {
        
        force = force / 3.0f;
        
        this->p1->ApplyForce(force);
        this->p2->ApplyForce(force);
        this->p3->ApplyForce(force);
    }
}
