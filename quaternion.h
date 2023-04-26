//
//  quaternion.hpp
//  cse169
//
//  Created by Alek Antic on 3/1/20.
//

#ifndef quaternion_h
#define quaternion_h

#include <stdio.h>
#include <glm/glm.hpp>

class Quaternion {
private:
    glm::vec4 q;
public:
    Quaternion() { Quaternion(glm::vec4(0.0f)); }
    Quaternion(float a, float b, float c, float d);
    Quaternion(glm::vec4 q) {this->q = q;}
    static Quaternion FromEuler(float yaw, float pitch, float roll);
    
    
    static float dot(Quaternion &q0, Quaternion &q1);
    static Quaternion multiply(Quaternion &q0, Quaternion &q1);
    static Quaternion normalize(Quaternion &q);
    
    glm::mat4 toMat();
    static Quaternion slerp(Quaternion &q0, Quaternion &q1, float t);
    static Quaternion CatmullRom(Quaternion &q0, Quaternion &q1, Quaternion &q2, Quaternion &q3, float t);
    
    
    Quaternion operator+ (const Quaternion &other) const
    {
        return Quaternion(q + other.q);
    }
    
    const Quaternion & operator+= (const Quaternion &other)
    {
        q = q + other.q;
        return *this;
    }
    
    Quaternion operator- (const Quaternion &other) const
    {
        return Quaternion(q - other.q);
    }
    
    const Quaternion & operator-= (const Quaternion &other)
    {
        q = q - other.q;
        return *this;
    }
    
    Quaternion operator* (float s) const
    {
        return Quaternion(s * q);
    }
    
    friend Quaternion operator * (float s, const Quaternion &rhs) {
        return rhs * s;
    }
    
    const Quaternion & operator*= (float s)
    {
        q = s*q;
        return *this;
    }
    
    Quaternion operator * (Quaternion &other)
    {
        return Quaternion::multiply(*this, other);
    }
    
    const Quaternion & operator *= (Quaternion &other)
    {
        q = Quaternion::multiply(*this, other).q;
        return *this;
    }
    
    Quaternion operator / (float s) const
    {
        return Quaternion(q / s);
    }
    
    const Quaternion & operator /= (float s)
    {
        q = q / s;
        return *this;
    }
    
    Quaternion operator - ()
    {
        return Quaternion(-q);
    }
};

#endif /* quaternion_hpp */
