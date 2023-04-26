//
//  skeleton.hpp
//  cse169
//
//  Created by Alek Antic on 1/23/20.
//

#ifndef skeleton_hpp
#define skeleton_hpp

#include "joint.h"
#include "skin.h"
#include "shader.h"
#include <string>

class Skeleton {
public:
    Skeleton(const char * skel_file, const char * skin_file, const char * vert_shader, const char * frag_shader);
    ~Skeleton();
    
    void Update(std::vector<float> pose);
    void Draw();
    
    void changeDOF(int delta);
    void rotateJoint(float delta);
    
    int selectedJoint;
    int currentDOF;
    
    GLuint shader;
    
private:
    Joint * root;
    Skin * skin;    
};



#endif /* skeleton_hpp */
