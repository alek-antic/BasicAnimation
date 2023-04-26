//
//  cloth.cpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#include "cloth.h"
#include <OpenGL/gl.h>
#include "matrix34.h"

#include <iostream>

// mass in kg
// drag is unitless
// spring in N/m
// damp in kg/s
// length in m
Cloth::Cloth(int size, float mass, float drag, float spring, float damp, float length)  {
    this->size = size;
    float delta = length / (float) size; // how far over each particle should be
    float l_0 = delta;
    // create array to store all particle pointers
    float p_mass = mass / (float)size / (float)size; // mass of each particle: mass of whole cloth divided by number of particles
    
    // create particle array
    particles = new Particle**[size];
    for (int i = 0; i < size; i++) {
        particles[i] = new Particle*[size];
    }
    
    
    // initialize other components
    system    = new ParticleSystem();
    wind      = new Wind();
    dampers   = std::vector<SpringDamper *>();
    triangles = std::vector<Triangle *>();
    
    
    // initialize fixed row of particles
    for (int j = 0; j < size; j++) {
        particles[0][j] = new Particle(p_mass, glm::vec3(-length / 2 + delta * j , length / 2, 0.0f), true);
        system->addParticle(particles[0][j]);
    }
    
    delta = delta - 0.00001;
    
    // initialize all other particles
    for (int i = 1; i < size; i++) {
        for (int j = 0; j < size; j++) {
            particles[i][j] = new Particle(p_mass, glm::vec3(-length / 2 + delta * j , length / 2 - delta * i, 0.0f), false);
            system->addParticle(particles[i][j]);
        }
    }
    
    // initialize dampers
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (i != 0 && j != size - 1) {
                // do / (up-right)
                SpringDamper * d = new SpringDamper(particles[i][j], particles[i-1][j+1], spring, damp, l_0 * glm::sqrt(2.0f));
                dampers.push_back(d);
            }
            
            if (i != size - 1 && j != size - 1) {
                // do \ (down-right)
                SpringDamper * d = new SpringDamper(particles[i][j], particles[i+1][j+1], spring, damp, l_0 * glm::sqrt(2.0f));
                dampers.push_back(d);
            }
            
            if (i != size - 1) {
                // do | (down)
                SpringDamper * d = new SpringDamper(particles[i][j], particles[i+1][j], spring, damp, l_0);
                dampers.push_back(d);
            }
            
            if (j != size - 1) {
                // do - (right)
                SpringDamper * d = new SpringDamper(particles[i][j], particles[i][j+1], spring, damp, l_0);
                dampers.push_back(d);
            }
        }
    }
    
    // initialize triangles
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - 1; j++) {
            Triangle * t = new Triangle(   particles[i][j]
                                        , particles[i][j+1]
                                        , particles[i+1][j]
                                        , wind
                                        , drag);
            triangles.push_back(t);
            
            t = new Triangle(   particles[i][j]
                             , particles[i][j+1]
                             , particles[i+1][j+1]
                             , wind
                             , drag);
            triangles.push_back(t);
            
            t = new Triangle(   particles[i][j]
                             , particles[i+1][j+1]
                             , particles[i+1][j]
                             , wind
                             , drag);
            triangles.push_back(t);
            
            t = new Triangle(   particles[i][j+1]
                             , particles[i+1][j+1]
                             , particles[i+1][j]
                             , wind
                             , drag);
            triangles.push_back(t);
        }
    }
}


void Cloth::Update(float delta_time) { // delta_time in seconds
//    for (int i = 0; i < 5; i++) {
//        this->PrivUpdate(delta_time / 5.0f);
//    }
    
    for (SpringDamper * sd : dampers) {
        sd->ComputeForce();
    }

    for (Triangle * t : triangles) {
        t->ComputeForce();
    }

    system->Update(delta_time);
}

void Cloth::PrivUpdate(float delta_time) {

}

void Cloth::Draw() {
    glEnable(GL_LIGHTING);
    glLoadMatrixf(Matrix34::IDENTITY);
    glBegin(GL_TRIANGLES);
        glColor4f(1.0f,1.0f,1.0f,1.0f);
        for (int i = 0; i < size - 1; i++) {
            for (int j = 0; j < size - 1; j++) {
                glm::vec3 v1 = particles[i][j]->getPos();
                glm::vec3 v2 = particles[i+1][j]->getPos();
                glm::vec3 v3 = particles[i][j+1]->getPos();

                glm::vec3 n1 = glm::normalize(glm::cross(v2-v1, v3-v1));
                glm::vec3 n2 = glm::normalize(glm::cross(v3-v2, v1-v2));
                glm::vec3 n3 = glm::normalize(glm::cross(v1-v3, v2-v3));

                // top left
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n1.x, n1.y, n1.z);
                glVertex3f(v1.x, v1.y, v1.z);

                // bottom left
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n2.x, n2.y, n2.z);
                glVertex3f(v2.x, v2.y, v2.z);

                // top right
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n3.x, n3.y, n3.z);
                glVertex3f(v3.x, v3.y, v3.z);

                v1 = particles[i+1][j+1]->getPos();

                n1 = glm::normalize(glm::cross(v3-v1, v2-v1));

                // bottom right
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n1.x, n1.y, n1.z);
                glVertex3f(v1.x, v1.y, v1.z);

                // bottom left
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n2.x, n2.y, n2.z);
                glVertex3f(v2.x, v2.y, v2.z);
                
                // top right
                glColor4f(1.0f,1.0f,1.0f,1.0f);
                glNormal3f(n3.x, n3.y, n3.z);
                glVertex3f(v3.x, v3.y, v3.z);
            }
        }
    
    // Lines between points
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < size; j++) {
//            glm::vec3 v1 = particles[i][j]->getPos();
//            if (j < size - 1) {
//                glm::vec3 v2 = particles[i][j+1]->getPos();
//
//                glColor3f(255.0f, 255.0f, 255.0f);
//                glVertex3f(v1.x, v1.y, v1.z);
//
//                glColor3f(255.0f, 255.0f, 255.0f);
//                glVertex3f(v2.x, v2.y, v2.z);
//            }
//
//            if (i < size - 1) {
//                glm::vec3 v3 = particles[i+1][j]->getPos();
//                glColor3f(255.0f, 255.0f, 255.0f);
//                glVertex3f(v1.x, v1.y, v1.z);
//
//                glColor3f(255.0f, 255.0f, 255.0f);
//                glVertex3f(v3.x, v3.y, v3.z);
//            }
//        }
//    }
    
    // visualize spring dampers
//    for (int i = 0; i < size; i++) {
//        for (int j = 0; j < size; j++) {
//            glm::vec3 v1 = particles[i][j]->getPos();
//            if (i != 0 && j != size - 1) {
//                // do / (up-right)
//                glm::vec3 v2 = particles[i-1][j+1]->getPos();
//                glVertex3f(v1.x, v1.y, v1.z);
//                glVertex3f(v2.x, v2.y, v2.z);
//            }
//
//            if (i != size - 1 && j != size - 1) {
//                // do \ (down-right)
//                glm::vec3 v2 = particles[i+1][j+1]->getPos();
//                glVertex3f(v1.x, v1.y, v1.z);
//                glVertex3f(v2.x, v2.y, v2.z);
//            }
//
//            if (i != size - 1) {
//                // do | (down)
//                glm::vec3 v2 = particles[i+1][j]->getPos();
//                glVertex3f(v1.x, v1.y, v1.z);
//                glVertex3f(v2.x, v2.y, v2.z);
//            }
//
//            if (j != size - 1) {
//                // do - (right)
//                glm::vec3 v2 = particles[i][j+1]->getPos();
//                glVertex3f(v1.x, v1.y, v1.z);
//                glVertex3f(v2.x, v2.y, v2.z);
//            }
//        }
//    }
    glEnd();
    glDisable(GL_LIGHTING);
}

Cloth::~Cloth() {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            delete particles[i][j];
        }
    }
    
    for (int i = 0; i < size; i++) {
        delete particles[i];
    }
    
    delete particles;
    
    delete system;
    for (SpringDamper * sd : dampers) {
        delete sd;
    }
    
    for (Triangle * t : triangles) {
        delete t;
    }
    
    delete wind;
}


void Cloth::MoveCloth(glm::vec3 delta) {
    for (int j = 0; j < size; j++) {
        particles[0][j]->ApplyUserForce(delta);
    }
}
