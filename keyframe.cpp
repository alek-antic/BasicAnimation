//
//  keyframe.cpp
//  cse169
//
//  Created by Alek Antic on 2/4/20.
//

#include <stdio.h>
#include "animation.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

float herm[16] = {
    2, -2,  1,  1,
    -3,  3, -2, -1,
    0,  0,  1,  0,
    1,  0,  0,  0
};
glm::mat4 Keyframe::HERMITE = glm::transpose(glm::make_mat4(herm));

Keyframe::Keyframe(float time, float val, Tangent rule_in, Tangent rule_out, float tan_in, float tan_out) {
    this->time = time;
    this->value = val;
    this->rule_in = rule_in;
    this->rule_out = rule_out;
    this->tan_in = tan_in;
    this->tan_out = tan_out;
}

float Keyframe::evaluate(float time) {
    glm::vec4 t = glm::vec4(time * time * time, time * time, time, 1);
    return glm::dot(t, c);
}

void Keyframe::computeTangents(Keyframe * before, Keyframe * after) {
    if (before == NULL && after == NULL) {
        tan_in = value;
        tan_out = value;
    } else {
        switch (rule_in) {
            case FLAT:
                tan_in = 0;
                break;
            case LIN:
                if (before == NULL) {
                    tan_in = (after->value - this->value) / (after->time - this->time);
                } else {
                    tan_in = (this->value - before->value) / (this->time - before->time);
                }
                break;
            case SMOOTH:
                // use linear rule if this is either first or last
                if (after == NULL) {
                    tan_in = (this->value - before->value) / (this->time - before->time);
                } else if (before == NULL) {
                    tan_in = (after->value - this->value) / (after->time - this->time);
                } else {
                    tan_in = (after->value - before->value) / (after->time - before->time);
                }
                break;
            case FIXED:
                // it should stay the same
                break;
        }
        
        switch (rule_out) {
            case FLAT:
                tan_out = 0;
                break;
            case LIN:
                // copy tan_in
                if (after == NULL) {
                    tan_out = (this->value - before->value) / (this->time - before->time);
                } else {
                    tan_out = (after->value - this->value) / (after->time - this->time);
                }
                break;
            case SMOOTH:
                // use linear rule if this is either first or last
                if (before == NULL) {
                    tan_out = (after->value - this->value) / (after->time - this->time);
                } else if (after == NULL) {
                    tan_out = (this->value - before->value) / (this->time - before->time);
                } else {
                    tan_out = (after->value - before->value) / (after->time - before->time);
                }
                break;
            case FIXED:
                // it should stay the same
                break;
        }
    }
}

void Keyframe::computeCoeff(Keyframe * next) {
    this->c = HERMITE *
                glm::vec4(
                          this->value,
                          next->value,
                          (next->time - this->time) * this->tan_out,
                          (next->time - this->time) *  next->tan_in
                         );
}

float Keyframe::getTime() {
    return time;
}

float Keyframe::getValue() {
    return value;
}

float Keyframe::getTanIn() {
    return tan_in;
}

float Keyframe::getTanOut() {
    return tan_out;
}
