//
//  rotationsim.cpp
//  cse169
//
//  Created by Alek Antic on 3/3/20.
//

#include "rotationsim.h"

RotationSim::RotationSim() {
    selectedKey = 0;
    time = 0.0f;
    mode = SLERP;
    keyBoxes = std::vector<Box *>();
    
    float start = -NUM_BOXES / 2.0f;
    for (int i = 0; i < NUM_BOXES; i++) {
        keyBoxes.push_back(new Box(BOX_X, BOX_Y, BOX_Z, start + i));
    }
    
    box = new Box(BOX_X, BOX_Y, BOX_Z, start);
}


RotationSim::~RotationSim() {
    for (Box * b : keyBoxes)
        delete b;
    
    delete box;
}

void RotationSim::Update(float delta_t) {
    time += delta_t;
    if (this->time >= NUM_BOXES - 1.0f) {
        this->time = 0;
        box->moveTo(glm::vec3(-NUM_BOXES / 2.0f, 0.0f, 0.0f));
    }
    
    for(Box * b : keyBoxes) {
        b->Update();
    }
    
    // Ensure shortest rotation path
    for (int i = 1; i < NUM_BOXES; i++) {
        Quaternion q0 = keyBoxes[i-1]->getOrientation();
        Quaternion q1 = keyBoxes[i]->getOrientation();
        if (Quaternion::dot(q0, q1) < 0) {
            keyBoxes[i]->setPose(-q1);
        }
    }
    
    // get pose for moving box
    Quaternion pose;
    if (mode == SLERP) {
        if (time < 1.0f) {
            Quaternion q0 = keyBoxes[0]->getOrientation();
            Quaternion q1 = keyBoxes[1]->getOrientation();
            pose = Quaternion::slerp(q0, q1, time);
        } else if (time < 2.0f) {
            Quaternion q0 = keyBoxes[1]->getOrientation();
            Quaternion q1 = keyBoxes[2]->getOrientation();
            pose = Quaternion::slerp(q0, q1, time - 1.0f);
        } else if (time < 3.0f) {
            Quaternion q0 = keyBoxes[2]->getOrientation();
            Quaternion q1 = keyBoxes[3]->getOrientation();
            pose = Quaternion::slerp(q0, q1, time - 2.0f);
        } else if (time < 4.0f) {
            Quaternion q0 = keyBoxes[3]->getOrientation();
            Quaternion q1 = keyBoxes[4]->getOrientation();
            pose = Quaternion::slerp(q0, q1, time - 3.0f);
        } else {
            pose = keyBoxes[4]->getOrientation();
        }
    } else {
        if (time < 1.0f) {
            Quaternion q0 = keyBoxes[0]->getOrientation();
            Quaternion q1 = keyBoxes[1]->getOrientation();
            Quaternion q2 = keyBoxes[2]->getOrientation();
            pose = Quaternion::CatmullRom(q0, q0, q1, q2, time);
        } else if (time < 2.0f) {
            Quaternion q0 = keyBoxes[0]->getOrientation();
            Quaternion q1 = keyBoxes[1]->getOrientation();
            Quaternion q2 = keyBoxes[2]->getOrientation();
            Quaternion q3 = keyBoxes[3]->getOrientation();
            pose = Quaternion::CatmullRom(q0, q1, q2, q3, time - 1.0f);
        } else if (time < 3.0f) {
            Quaternion q1 = keyBoxes[1]->getOrientation();
            Quaternion q2 = keyBoxes[2]->getOrientation();
            Quaternion q3 = keyBoxes[3]->getOrientation();
            Quaternion q4 = keyBoxes[4]->getOrientation();
            pose = Quaternion::CatmullRom(q1, q2, q3, q4, time - 2.0f);
        } else if (time < 4.0f) {
            Quaternion q2 = keyBoxes[2]->getOrientation();
            Quaternion q3 = keyBoxes[3]->getOrientation();
            Quaternion q4 = keyBoxes[4]->getOrientation();
            pose = Quaternion::CatmullRom(q2, q3, q4, q4, time - 3.0f);
        } else {
            pose = keyBoxes[4]->getOrientation();
        }
    }
    
    box->setPose(pose);
    box->moveBy(delta_t);
    box->Update();
}

void RotationSim::Draw() {
    for (Box * b : keyBoxes) {
        b->Draw();
    }
    
    box->Draw();
}

void RotationSim::rotateBy(float x, float y, float z) {
    Quaternion rot = Quaternion::FromEuler(y, x, z);
    rot = Quaternion::normalize(rot);
    Quaternion currPose = keyBoxes[selectedKey]->getOrientation();
    
    keyBoxes[selectedKey]->setPose(currPose * rot);
}
