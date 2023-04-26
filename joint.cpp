//
//  skel.cpp
//  cse169
//
//  Created by Alek Antic on 1/13/20.
//

#include "joint.h"
#include <iostream>

int Joint::jointCount = 0;
std::unordered_map<int, Joint *> Joint::joints = std::unordered_map<int, Joint*>();

Joint::Joint() {
    Reset();
    children = std::vector<Joint *>();
    isSelected = false;
}

Joint::~Joint() {
    for (Joint * j : children) {
        delete j;
    }
}

Joint * Joint::FromFile(const char *file) {
    Tokenizer * token = new Tokenizer();
    token->Open(file);
    
    // parse the outer-most balljoint line
    char buffer[128];
    token->GetToken(buffer);
    if (!strcmp(buffer, "balljoint")) {
        Joint * out = new Joint();
        joints[jointCount] = out;
        out->parent = NULL;
        token->GetToken(out->name); // parse the name
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skel format: no opening { " << out->name << std::endl;
            delete out;
            return NULL;
        }
        
        Joint::FromFileHelper(token, out);
        
        // clamp pose to the rotmin and rotmax
        Joint::Clamp(out);
        Joint::ComputeModelMatrix(out);
        
        return out;
    } else {
        std::cerr << "Invalid .skel format: not a balljoint at top level" << std::endl;
        return NULL;
    }
}

void Joint::FromFileHelper(Tokenizer * token, Joint * joint) {
    char buffer[128];
    token->GetToken(buffer);
    
    while (buffer[0]) {
        if (!strcmp(buffer, "balljoint")) {
            // parse the outer-most balljoint line
            // create new child joint, and add it to this Joint's children
            jointCount++;
            Joint * child = new Joint();
            child->parent = joint;
            joints[jointCount] = child;
            token->GetToken(child->name); // parse the name
            token->GetToken(buffer); // make sure the next token is open parens
            if (strcmp(buffer, "{")) {
                std::cerr << "Invalid .skel format: no open { " << child->name << std::endl;
                delete child;
                return;
            }
            
            // parse everything inside this balljoint
            Joint::FromFileHelper(token, child);
            
            joint->children.push_back(child);
        } else if (!strcmp(buffer, "offset")) {
            float x = token->GetFloat();
            float y = token->GetFloat();
            float z = token->GetFloat();
            joint->Offset.x = x;
            joint->Offset.y = y;
            joint->Offset.z = z;
        } else if (!strcmp(buffer, "boxmin")) {
            float x = token->GetFloat();
            float y = token->GetFloat();
            float z = token->GetFloat();
            joint->BoxMin.x = x;
            joint->BoxMin.y = y;
            joint->BoxMin.z = z;
        } else if (!strcmp(buffer, "boxmax")) {
            float x = token->GetFloat();
            float y = token->GetFloat();
            float z = token->GetFloat();
            joint->BoxMax.x = x;
            joint->BoxMax.y = y;
            joint->BoxMax.z = z;
        } else if (!strcmp(buffer, "rotxlimit")) {
            float min = token->GetFloat();
            float max = token->GetFloat();
            joint->RotMin.x = min;
            joint->RotMax.x = max;
        } else if (!strcmp(buffer, "rotylimit")) {
            float min = token->GetFloat();
            float max = token->GetFloat();
            joint->RotMin.y = min;
            joint->RotMax.y = max;
        } else if (!strcmp(buffer, "rotzlimit")) {
            float min = token->GetFloat();
            float max = token->GetFloat();
            joint->RotMin.z = min;
            joint->RotMax.z = max;
        } else if (!strcmp(buffer, "pose")) {
            float x = token->GetFloat();
            float y = token->GetFloat();
            float z = token->GetFloat();
            joint->Pose.x = x;
            joint->Pose.y = y;
            joint->Pose.z = z;
        } else if (!strcmp(buffer, "}")) {
            // end of ball joint section, break out of loop
            break;
        }
        
        // move to next line
        token->GetToken(buffer);
    }
    return;
}

void Joint::Clamp(Joint * joint) {
    // clamp to maximum
    if (joint->Pose.x > joint->RotMax.x) {
        joint->Pose.x = joint->RotMax.x;
    }
    
    if (joint->Pose.y > joint->RotMax.y) {
        joint->Pose.y = joint->RotMax.y;
    }
    
    if (joint->Pose.z > joint->RotMax.z) {
        joint->Pose.z = joint->RotMax.z;
    }
    
    // clamp to minimum
    if (joint->Pose.x < joint->RotMin.x) {
        joint->Pose.x = joint->RotMin.x;
    }
    
    if (joint->Pose.y < joint->RotMin.y) {
        joint->Pose.y = joint->RotMin.y;
    }
    
    if (joint->Pose.z < joint->RotMin.z) {
        joint->Pose.z = joint->RotMin.z;
    }
    
    // recurse
    for (Joint * j : joint->children) {
        Joint::Clamp(j);
    }
}

void Joint::ComputeModelMatrix(Joint * joint) {
    Matrix34 temp = Matrix34();
    
    // translate
    temp.MakeTranslate(joint->Offset);
    joint->Model.Dot(joint->Model, temp);
    
    // rotate around x, then y, then z
    temp.MakeRotateZ(joint->Pose.z);
    joint->Model.Dot(joint->Model, temp);
    temp.MakeRotateY(joint->Pose.y);
    joint->Model.Dot(joint->Model, temp);
    temp.MakeRotateX(joint->Pose.x);
    joint->Model.Dot(joint->Model, temp);
    
    // recurse
    for (Joint * j : joint->children) {
        Joint::ComputeModelMatrix(j);
    }
}

Matrix34 Joint::GetWorldMatrix() {
    Matrix34 world = Matrix34::IDENTITY;
    
    Joint * cur = this;
    
    while (cur != NULL) {
        world.Dot(cur->Model, world);
        cur = cur->parent;
    }
    
    return world;
}

void Joint::Reset() {
    Model  = Matrix34::IDENTITY;
    Offset = Vector3(0.0, 0.0, 0.0);
    BoxMin = Vector3(-0.1, -0.1, -0.1);
    BoxMax = Vector3(0.1, 0.1, 0.1);
    RotMin = Vector3(-100000, -100000, -100000);
    RotMax = Vector3(100000, 100000, 100000);
    Pose   = Vector3(0.0, 0.0, 0.0);
}

void Joint::Draw() {
    Draw(Matrix34::IDENTITY);
}

void Joint::Draw(Matrix34 & Parent) {
    Matrix34 newModel = Matrix34();
    newModel.Dot(Parent, this->Model);
    glLoadMatrixf(newModel);
    if (isSelected)
        drawWireBox(BoxMin.x, BoxMin.y, BoxMin.z, BoxMax.x, BoxMax.y, BoxMax.z, glm::vec3(1.0f, 1.0f, 0.0f));
   
    drawWireBox(BoxMin.x, BoxMin.y, BoxMin.z, BoxMax.x, BoxMax.y, BoxMax.z);
    
    // recurse
    for (Joint * j : this->children) {
        j->Draw(newModel);
    }
}

void Joint::Update() {
    
}


void Joint::RotateDOF(int DOF, float delta) {
    this->Pose[DOF] += delta;
    this->Recompute();
}


void Joint::setOffset(Vector3 offset) {
    this->Offset = offset;
    this->Recompute();
}

void Joint::setPose(Vector3 pose) {
    this->Pose = pose;
    this->Recompute();
}

void Joint::clamp() {
    // clamp to maximum
    if (this->Pose.x > this->RotMax.x) {
        this->Pose.x = this->RotMax.x;
    }
    
    if (this->Pose.y > this->RotMax.y) {
        this->Pose.y = this->RotMax.y;
    }
    
    if (this->Pose.z > this->RotMax.z) {
        this->Pose.z = this->RotMax.z;
    }
    
    // clamp to minimum
    if (this->Pose.x < this->RotMin.x) {
        this->Pose.x = this->RotMin.x;
    }
    
    if (this->Pose.y < this->RotMin.y) {
        this->Pose.y = this->RotMin.y;
    }
    
    if (this->Pose.z < this->RotMin.z) {
        this->Pose.z = this->RotMin.z;
    }
}


void Joint::Recompute() {
    this->clamp();
    this->Model = Matrix34::IDENTITY;
    
    Matrix34 temp = Matrix34();
    
    // translate
    temp.MakeTranslate(this->Offset);
    this->Model.Dot(this->Model, temp);
    
    // rotate around x, then y, then z
    temp.MakeRotateZ(this->Pose.z);
    this->Model.Dot(this->Model, temp);
    temp.MakeRotateY(this->Pose.y);
    this->Model.Dot(this->Model, temp);
    temp.MakeRotateX(this->Pose.x);
    this->Model.Dot(this->Model, temp);
}
