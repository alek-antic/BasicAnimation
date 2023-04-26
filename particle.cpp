//
//  particle.cpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#include <stdio.h>
#include "cloth.h"
#include <iostream>
#include <glm/gtx/string_cast.hpp>

#define GROUND -1.5

float friction = 0.2f;

// mass in kg
Particle::Particle(float mass, glm::vec3 position, bool fixed) {
    this->mass = mass;
    this->position = position;
    this->fixed = fixed;
    
    this->velocity = glm::vec3(0.0f);
    this->force = glm::vec3(0.0f);
    this->user_force = glm::vec3(0.0f);
    this->elasticity = 0.0f;
}

void Particle::Update(float delta_time) {
    glm::vec3 to_use_force;
    if (!this->fixed) {
        to_use_force = this->force;
    } else {
        to_use_force = this->user_force;
    }
    
    glm::vec3 accel = to_use_force * (1.0f/this->mass);
    
    this->velocity += accel * delta_time;
    
    if (this->position.y < GROUND) {
        this->position.y =2 * GROUND -this->position.y;

        this->velocity.y = - this->elasticity * this->velocity.y;
        this->velocity.x = (1 - friction) * this->velocity.x;
        this->velocity.z = (1 - friction) * this->velocity.z;
    }

    
    this->position += this->velocity * delta_time;
    this->force = glm::vec3(0.0f);
    
    if (this->fixed) {
        this->user_force = glm::vec3(0.0f);
        this->velocity = glm::vec3(0.0f);
    }
}

void ParticleSystem::Update(float delta_time) {
    glm::vec3 grav = glm::vec3(0.0f, -9.8f, 0.0f);
    for (Particle * p : this->particles) {
        glm::vec3 f = grav * p->getMass();
        p->ApplyForce(f);
    }
    
    for (Particle * p : this->particles) {
        p->Update(delta_time);
    }
}
