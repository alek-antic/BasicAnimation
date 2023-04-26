//
//  skeleton.cpp
//  cse169
//
//  Created by Alek Antic on 1/23/20.
//

#include "skeleton.h"

Skeleton::Skeleton(const char * skel_file, const char * skin_file, const
                   char * vert_shader, const char * frag_shader) {
//    this->shader = LoadShaders(vert_shader, frag_shader);
    this->root = Joint::FromFile(skel_file);
    this->skin = Skin::FromFile(skin_file, this->shader);
    

    this->selectedJoint = 0;
    this->currentDOF = 0;
    
    this->changeDOF(0);
}

Skeleton::~Skeleton() {
    delete root;
    delete skin;
    glDeleteProgram(shader);
}

void Skeleton::Draw() {
    glUseProgram(shader);
    this->root->Draw();
    this->skin->Draw();
}

void Skeleton::changeDOF(int delta) {
    // deselect current joint
    Joint::joints[selectedJoint]->isSelected = false;
    if (currentDOF + delta >= 3) {
        this->currentDOF = 0;
        this->selectedJoint = (selectedJoint + 1) % Joint::joints.size();
    } else if (currentDOF + delta < 0) {
        this->currentDOF = 2;
        this->selectedJoint = (selectedJoint - 1) % Joint::joints.size();
    } else {
        this->currentDOF = currentDOF + delta;
    }
    
    // select the new joint
    Joint::joints[selectedJoint]->isSelected = true;
    std::cout << "Joint: " << selectedJoint << " DOF: " << currentDOF << std::endl;
}

void Skeleton::rotateJoint(float delta) {
    Joint::joints[this->selectedJoint]->RotateDOF(this->currentDOF, delta);
    this->skin->Dirty();
}


void Skeleton::Update(std::vector<float> pose) {
    if (pose.size() < (Joint::joints.size() + 1) * 3) {
        std::cerr << "Animation does not match skeleton" << std::endl;
    }
    
    // first 3 floats are the offset for the base joint
    Vector3 offset = Vector3(pose[0], pose[1], pose[2]);
    Joint::joints[0]->setOffset(offset);
    
    // rest are rotation DOFs
    for (int i = 0; i < Joint::jointCount; i++) {
        Vector3 p = Vector3(pose[3*i + 3], pose[3*i+4], pose[3*i+5]);
        Joint::joints[i]->setPose(p);
    }
    
    this->skin->Dirty();
}
