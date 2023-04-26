//
//  animation.hpp
//  cse169
//
//  Created by Alek Antic on 2/3/20.
//

#ifndef animation_hpp
#define animation_hpp

#include <stdio.h>
#include <vector>

#include <glm/glm.hpp>

enum Extrap { CONSTANT, LINEAR, CYCLE, CYCLE_OFFSET, BOUNCE };
enum Tangent { FLAT, LIN, SMOOTH, FIXED };

class Keyframe {
private:
    float time;
    float value;
    float tan_in, tan_out;
    Tangent rule_in, rule_out;
    glm::vec4 c;

public:
    static glm::mat4 HERMITE;
    
    Keyframe(float time, float val, Tangent rule_in, Tangent rule_out, float tan_in = 0.0f, float tan_out = 0.0f);
    
    float evaluate(float time);
    
    void computeTangents(Keyframe * before, Keyframe * after);
    void computeCoeff(Keyframe * next);
    
    float getTime();
    float getValue();
    float getTanIn();
    float getTanOut();
};

class Channel {
private:
    Extrap extrap_in, extrap_out;
    std::vector<Keyframe *> frames;
    
public:
    Channel(Extrap e_in, Extrap e_out);
    ~Channel();
    
    void addFrame(Keyframe * key);
    float evaluate(float time);
    void computeTangents();
};


class Animation {
private:
    std::vector<Channel *> channels;
    float start, end;
    
public:
    static Animation * FromFile(const char * filename);
    
    std::vector<float> GetPose(float time);
    
    ~Animation();
};


#endif /* animation_hpp */
