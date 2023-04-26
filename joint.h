//
//  skel.hpp
//  cse169
//
//  Created by Alek Antic on 1/13/20.
//

#ifndef skel_hpp
#define skel_hpp

#include "core.h"
#include "vector3.h"
#include "matrix34.h"
#include "token.h"
#include <vector>
#include <unordered_map>

class Joint {
public:
    Joint();
    ~Joint();
    static Joint * FromFile(const char * file);
    
    static std::unordered_map<int, Joint *> joints;
    static int jointCount;
    
    void Update();
    void Reset();
    void Draw();
    
    Matrix34 GetWorldMatrix();
    
    void RotateDOF(int DOF, float delta);
    
    bool isSelected;
    
    void setOffset(Vector3 offset);
    void setPose(Vector3 pose);
    
private:
    char name[128];
    Matrix34 Model;
    Vector3 Offset;
    Vector3 BoxMin;
    Vector3 BoxMax;
    Vector3 RotMin;
    Vector3 RotMax;
    Vector3 Pose;
    std::vector<Joint *> children;
    Joint * parent;
    
    static void FromFileHelper(Tokenizer * token, Joint * joint);
    static void Clamp(Joint * joint);
    static void ComputeModelMatrix(Joint * joint);
    
    void Draw(Matrix34 & parent);
    
    void clamp();
    void Recompute();
};

#endif /* skel_hpp */
