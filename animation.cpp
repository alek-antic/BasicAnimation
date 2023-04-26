//
//  animation.cpp
//  cse169
//
//  Created by Alek Antic on 2/3/20.
//

#include "animation.h"
#include "token.h"
#include <iostream>

Animation * Animation::FromFile(const char * filename) {
    Animation * out = new Animation();
    
    Tokenizer tok = Tokenizer();
    tok.Open(filename);
    
    char buffer[128];
    
    tok.GetToken(buffer);
    if (strcmp(buffer, "animation")) {
        std::cerr << "Error in anim file" << std::endl;
        delete out;
        return NULL;
    }
    
    tok.GetToken(buffer);
    if (strcmp(buffer, "{")) {
        std::cerr << "Error in anim file" << std::endl;
        delete out;
        return NULL;
    }
    
    tok.GetToken(buffer);
    if (strcmp(buffer, "range")) {
        std::cerr << "Error in anim file" << std::endl;
        delete out;
        return NULL;
    }
    
    float start = tok.GetFloat();
    float end   = tok.GetFloat();
    out->start = start;
    out->end = end;
    
    tok.GetToken(buffer);
    if (strcmp(buffer, "numchannels")) {
        std::cerr << "Error in anim file" << std::endl;
        delete out;
        return NULL;
    }
    
    int num_channels = tok.GetInt();
    
    for (int i = 0; i < num_channels; i++) {
        tok.GetToken(buffer);
        
        // open channel
        if (strcmp(buffer, "channel")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        tok.GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        tok.GetToken(buffer);
        if (strcmp(buffer, "extrapolate")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        // extrap_in
        Extrap extrap_in = CONSTANT;
        tok.GetToken(buffer);
        if (!strcmp(buffer, "constant")) {
            extrap_in = CONSTANT;
        } else if (!strcmp(buffer, "linear")) {
            extrap_in = LINEAR;
        } else if (!strcmp(buffer, "cycle")) {
            extrap_in = CYCLE;
        } else if (!strcmp(buffer, "cycle_offset")) {
            extrap_in = CYCLE_OFFSET;
        } else if (!strcmp(buffer, "bounce")) {
            extrap_in = BOUNCE;
        } else {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        // extrap_out
        Extrap extrap_out = CONSTANT;
        tok.GetToken(buffer);
        if (!strcmp(buffer, "constant")) {
            extrap_out = CONSTANT;
        } else if (!strcmp(buffer, "linear")) {
            extrap_out = LINEAR;
        } else if (!strcmp(buffer, "cycle")) {
            extrap_out = CYCLE;
        } else if (!strcmp(buffer, "cycle_offset")) {
            extrap_out = CYCLE_OFFSET;
        } else if (!strcmp(buffer, "bounce")) {
            extrap_out = BOUNCE;
        } else {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        Channel * cur = new Channel(extrap_in, extrap_out);
        
        // open key block
        tok.GetToken(buffer);
        if (strcmp(buffer, "keys")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }

        int num_keys = tok.GetInt();
        
        tok.GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        // Create keyframe
        for (int j = 0; j < num_keys; j++) {
            float time = tok.GetFloat();
            float val = tok.GetFloat();
            float fixed_in = 0.0f;
            float fixed_out = 0.0f;
            Tangent rule_in = FLAT;
            Tangent rule_out = FLAT;
            
            tok.GetToken(buffer);
            if (!strcmp(buffer, "flat")) {
                rule_in = FLAT;
            } else if (!strcmp(buffer, "linear")) {
                rule_in = LIN;
            } else if (!strcmp(buffer, "smooth")) {
                rule_in = SMOOTH;
            } else {
                // try to convert token to float
                rule_in = FIXED;
                fixed_in = atof(buffer);
                if (fixed_in == 0.0f) {
                    std::cerr << "Error in anim file" << std::endl;
                    delete out;
                    return NULL;
                }
            }
            
            tok.GetToken(buffer);
            if (!strcmp(buffer, "flat")) {
                rule_out = FLAT;
            } else if (!strcmp(buffer, "linear")) {
                rule_out = LIN;
            } else if (!strcmp(buffer, "smooth")) {
                rule_out = SMOOTH;
            } else {
                // try to convert token to float
                rule_out = FIXED;
                fixed_out = atof(buffer);
                if (fixed_out == 0.0f) {
                    std::cerr << "Error in anim file" << std::endl;
                    delete out;
                    return NULL;
                }
            }
            
            Keyframe * key = new Keyframe(time, val, rule_in, rule_out, fixed_in, fixed_out);
            cur->addFrame(key);
        }
        
        // close of key block
        tok.GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        tok.GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Error in anim file" << std::endl;
            delete out;
            return NULL;
        }
        
        cur->addFrame(NULL);
        cur->computeTangents();
        
        out->channels.push_back(cur);
    }
    
    tok.GetToken(buffer);
    if (strcmp(buffer, "}")) {
        std::cerr << "Error in anim file" << std::endl;
        delete out;
        return NULL;
    }
    
    tok.Close();
    
    return out;
}

Animation::~Animation() {
    for (Channel * c : channels) {
        delete c;
    }
}


std::vector<float> Animation::GetPose(float time) {
    std::vector<float> pose = std::vector<float>();
    
    for (Channel * c : channels) {
        pose.push_back(c->evaluate(time));
    }
    
    return pose;
}
