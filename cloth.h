//
//  cloth.hpp
//  cse169
//
//  Created by Alek Antic on 2/17/20.
//

#ifndef cloth_h
#define cloth_h

#include <stdio.h>
#include <vector>

#include <glm/glm.hpp>


class Particle {
private:
    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 user_force;
    bool fixed;
    float elasticity;
    
public:
    Particle(float mass, glm::vec3 position, bool fixed = false);
    
    void Update(float delta_time);
    void ApplyForce(glm::vec3 &f) {force += f;}
    void ApplyUserForce(glm::vec3 &f) {user_force += f;}
    
    float getMass() {return this->mass;}
    glm::vec3 getPos() {return this->position;}
    glm::vec3 getVel() {return this->velocity;}
};

class ParticleSystem {
private:
    std::vector<Particle *> particles;
    
public:
    void Update(float delta_time);
    void addParticle(Particle * p) {particles.push_back(p);}
};

class SpringDamper {
private:
    float spring_const, damping_factor, rest_length;
    Particle * p1, * p2;
    
public:
    SpringDamper(Particle * p1, Particle * p2, float k_s, float k_d, float l_0);
    void ComputeForce();
};

class Wind {
private:
    glm::vec3 v_air;
    float mag, theta;
    float density;
    
public:
    Wind(float mag = 0.0f, float theta = glm::radians(90.0f), float density = 1.225f);
    glm::vec3 getVel();
    float getDensity() {return density;}
    
    void scaleBy(float delta)  {mag   += delta;}
    void rotateBy(float delta) {theta += delta;}
    
    float getSpeed() {return mag;}
    float getAngle() {return theta;}
};

class Triangle {
private:
    Particle * p1, * p2, * p3;
    Wind * wind;
    float drag;
    
public:
    Triangle(Particle * p1, Particle * p2, Particle * p3, Wind * wind, float drag);
    void ComputeForce();
};

class Cloth {
private:
    // TR-TR-TR-TRIPLE POINTER
    Particle *** particles;
    ParticleSystem * system;
    std::vector<SpringDamper *> dampers;
    std::vector<Triangle *> triangles;
    Wind * wind;
    int size;
    
    void PrivUpdate(float delta_time);
    
public:
    Cloth(int size, float mass, float drag, float spring, float damp, float length);
    ~Cloth();
    
    void Update(float delta_time);
    void Draw();
    
    void ChangeWindSpeed(float delta) {wind->scaleBy(delta);}
    void ChangeWindAngle(float delta) {wind->rotateBy(delta);}
    
    float getWindSpeed() {return wind->getSpeed();}
    float getWindAngle() {return wind->getAngle();}
    
    void MoveCloth(glm::vec3 delta);
};
#endif /* cloth_hpp */
