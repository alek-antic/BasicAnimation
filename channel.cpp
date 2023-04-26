//
//  channel.cpp
//  cse169
//
//  Created by Alek Antic on 2/4/20.
//

#include <stdio.h>
#include "animation.h"
#include <iostream>


Channel::Channel(Extrap e_in, Extrap e_out) {
    this->extrap_in  = e_in;
    this->extrap_out = e_out;
}

Channel::~Channel() {
    for (Keyframe * f : frames) {
        delete f;
    }
}

void Channel::addFrame(Keyframe * key) {
    this->frames.push_back(key);
}

float Channel::evaluate(float time) {
    float min_time = frames[0]->getTime();
    float max_time = frames[frames.size()-1]->getTime();
    float total_time = max_time - min_time;
    
    if (time < min_time) {
        Keyframe * first = frames[0];
        float new_time = max_time - min_time + time - int((min_time - time) / total_time) * total_time;
        // case before start of keyframes (extrap_in)
        switch (this->extrap_in) {
            case CONSTANT: {
                return first->getValue();
            }
            case LINEAR: {
                // use first tan_in
                // point-slope form of a line
                return first->getTanIn() * (time - first->getTime()) + first->getValue();
            }
            case CYCLE: {
                return this->evaluate(new_time);
            }
            case CYCLE_OFFSET: {
                float offset = this->evaluate(new_time);
                int cycle = int((time - min_time) / total_time);
                return cycle * first->getValue() * 2 + offset;
            }
            case BOUNCE: {
                // reverse the animation
                int cycle = int((time - min_time) / total_time);
                int forward = cycle % 2;
                if (forward == 0) {
                    return this->evaluate(new_time);
                } else {
                    return this->evaluate(max_time - new_time);
                }
            }
        }
    } else if (time > max_time) {
        float new_time = time - int((time - min_time) / total_time) * total_time;
        // case after end of keyframes (extrap_out)
        Keyframe * last = frames[frames.size() - 1];
        switch (this->extrap_out) {
            case CONSTANT: {
                return last->getValue();
            }
            case LINEAR: {
                // use last tan_out
                // point-slope form of a line
                return last->getTanOut() * (time - last->getTime()) + last->getValue();
            }
            case CYCLE: {
                // start over
                // compute the time as if it is within min_time & max_time
                return this->evaluate(new_time);
            }
            case CYCLE_OFFSET: {
                // start over, but from current position
                float offset = this->evaluate(new_time);
                int cycle = int((time - min_time) / total_time);
                return cycle * last->getValue() * 2 + offset;
            }
            case BOUNCE: {
                // reverse the animation
                int cycle = int((time - min_time) / total_time);
                int forward = cycle % 2;
                if (forward == 0) {
                    return this->evaluate(new_time);
                } else {
                    return this->evaluate(max_time - new_time);
                }
            }
        }
    } else {
        // case in the keyframes
        // find correct keyframe
        int frameInd = 0;
        for (int i = 0; i < frames.size() - 1; i++) {
            // on a keyframe exactly
            if (time == frames[i]->getTime()) {
                return frames[i]->getValue();
            }

            // between two keyframes
            if (time < frames[i+1]->getTime() && time > frames[i]->getTime()) {
                frameInd = i;
                break;
            }
        }
        
        // normalize time
        float u = (time - frames[frameInd]->getTime()) / (frames[frameInd + 1]->getTime() - frames[frameInd]->getTime());
        
        // evaluate on the keyframe with normalized time
        return frames[frameInd]->evaluate(u);
    }
    return 0.0f;
}

void Channel::computeTangents() {
    frames[0]->computeTangents(NULL, frames[1]);
    
    for (int i = 1; i < frames.size() - 1; i++) {
        frames[i]->computeTangents(frames[i-1], frames[i+1]);
    }
    
    for (int i = 0; i < frames.size() - 2; i++) {
        frames[i]->computeCoeff(frames[i+1]);
    }
    
    frames.pop_back();
}


