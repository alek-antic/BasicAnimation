//
//  quaternion.cpp
//  cse169
//
//  Created by Alek Antic on 3/1/20.
//

#include "quaternion.h"
#include <iostream>

Quaternion::Quaternion(float a, float b, float c, float d) {
    q = glm::vec4(a, b, c, d);
}

Quaternion Quaternion::FromEuler(float yaw, float pitch, float roll) {
    float cy = glm::cos(yaw * 0.5);
    float sy = glm::sin(yaw * 0.5);
    float cp = glm::cos(pitch * 0.5);
    float sp = glm::sin(pitch * 0.5);
    float cr = glm::cos(roll * 0.5);
    float sr = glm::sin(roll * 0.5);

    float a = cy * cp * cr + sy * sp * sr;
    float b = cy * cp * sr - sy * sp * cr;
    float c = sy * cp * sr + cy * sp * cr;
    float d = sy * cp * cr - cy * sp * sr;
    Quaternion q = Quaternion(a, b, c, d);

    return q;
}


float Quaternion::dot(Quaternion &q0, Quaternion &q1) {
    return glm::dot(q0.q, q1.q);
}

Quaternion Quaternion::multiply(Quaternion &q0, Quaternion &q1) {
    float s0 = q0.q.x;
    float s1 = q1.q.x;
    
    glm::vec3 v0 = glm::vec3(q0.q.y, q0.q.z, q0.q.w);
    glm::vec3 v1 = glm::vec3(q1.q.y, q1.q.z, q1.q.w);
    
    float s_new = s0*s1 - glm::dot(v0, v1);
    glm::vec3 v_new = s0*v1 + s1*v0 + glm::cross(v0, v1);
    
    return Quaternion(glm::vec4(s_new, v_new));
}

Quaternion Quaternion::normalize(Quaternion &q) {
    return Quaternion(glm::normalize(q.q));
}

glm::mat4 Quaternion::toMat() {
    glm::mat4 out = glm::mat4(1.0f);
    
    out[0][0] = 1 - 2 * q.z * q.z - 2 * q.w * q.w;
    out[1][0] = 2 * q.y * q.z - 2 * q.x * q.w;
    out[2][0] = 2 * q.y * q.w + 2 * q.x * q.z;
    out[3][0] = 0.0f;
    
    out[0][1] = 2 * q.y * q.z + 2 * q.x * q.w;
    out[1][1] = 1 - 2 * q.y * q.y - 2 * q.w * q.w;
    out[2][1] = 2 * q.z * q.w - 2 * q.x * q.y;
    out[3][1] = 0.0f;
    
    out[0][2] = 2 * q.y * q.w - 2 * q.x * q.z;
    out[1][2] = 2 * q.z * q.w + 2 * q.x * q.y;
    out[2][2] = 1 - 2 * q.y * q.y - 2 * q.z * q.z;
    out[3][2] = 0.0f;
    
    out[0][3] = 0.0f;
    out[1][3] = 0.0f;
    out[2][3] = 0.0f;
    out[3][3] = 1.0f;
    
    return out;
}

Quaternion Quaternion::slerp(Quaternion &q0, Quaternion &q1, float t) {
    float dot = glm::clamp(Quaternion::dot(q0, q1), -1.0f, 1.0f);
    
    float theta = glm::acos(dot);
    
    if (glm::isnan(theta)) {
        std::cerr << "theta nan" << std::endl;
    }
    
    if (glm::abs(glm::sin(theta)) < 0.00001f) {
        return q0;
    }
    
    return glm::sin((1-t) * theta) / glm::sin(theta) * q0 + glm::sin(t * theta) / glm::sin(theta) * q1;
}

Quaternion Quaternion::CatmullRom(Quaternion &q0, Quaternion &q1, Quaternion &q2, Quaternion &q3, float t) {
    Quaternion q10 = Quaternion::slerp(q0, q1, t + 1.0f);
    Quaternion q11 = Quaternion::slerp(q1, q2, t);
    Quaternion q12 = Quaternion::slerp(q2, q3, t - 1.0f);
    
    Quaternion q20 = Quaternion::slerp(q10, q11, (t + 1.0f) / 2.0f);
    Quaternion q21 = Quaternion::slerp(q11, q12, t / 2.0f);
    
    return Quaternion::slerp(q20, q21, t);
}
