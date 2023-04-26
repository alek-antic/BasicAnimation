//
//  skin.cpp
//  cse169
//
//  Created by Alek Antic on 1/21/20.
//

#include "skin.h"
#include <iostream>

Skin::Skin(GLuint shader) {
    this->needUpdate = true;
    this->shader = shader;
    this->Positions = std::vector<glm::vec3>();
    this->Normals = std::vector<glm::vec3>();
    this->Triangles = std::vector<glm::ivec3>();
    this->WorldPos = std::vector<glm::vec3>();
    this->WorldNorm = std::vector<glm::vec3>();
}

Skin::~Skin() {
    
}

Skin * Skin::FromFile(const char * file, GLuint shader) {
    Skin * out = new Skin(shader);
    
    Tokenizer * token = new Tokenizer();
    token->Open(file);
    
    char buffer[128];
    token->GetToken(buffer);
    if (!strcmp(buffer, "positions")) {
        int numverts = token->GetInt();
        
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skin format: no opening { in positions" << std::endl;
            delete out;
            return NULL;
        }
        
        for (int i = 0; i < numverts; i++) {
            float x, y, z;
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            
            out->Positions.push_back(glm::vec3(x,y,z));
            out->WorldPos.push_back(glm::vec3(x,y,z));
        }
        
        token->GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Invalid .skin format: no closing } in positions" << std::endl;
            delete out;
            return NULL;
        }
    }
    
    token->GetToken(buffer);
    if (!strcmp(buffer, "normals")) {
        int numnorms = token->GetInt();
        
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skin format: no opening { in normals" << std::endl;
            delete out;
            return NULL;
        }
        
        for (int i = 0; i < numnorms; i++) {
            float x, y, z;
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            
            out->Normals.push_back(glm::vec3(x,y,z));
            out->WorldNorm.push_back(glm::vec3(x,y,z));
        }
        
        token->GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Invalid .skin format: no closing } in normals" << std::endl;
            delete out;
            return NULL;
        }
    }
    
    token->GetToken(buffer);
    if (!strcmp(buffer, "skinweights")) {
        int numweights = token->GetInt();
        
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skin format: no opening { in skinweights" << std::endl;
            delete out;
            return NULL;
        }
        
        for (int i = 0; i < numweights; i++) {
            int numbindings = token->GetInt();
            std::unordered_map<int, float> weightmap;
            for (int j = 0; j < numbindings; j++) {
                int joint = token->GetInt();
                float weight = token->GetFloat();
                
                weightmap[joint] = weight;
            }
            out->Weights.push_back(weightmap);
        }
        
        token->GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Invalid .skin format: no closing } in skinweights" << std::endl;
            delete out;
            return NULL;
        }
    }
    
    token->GetToken(buffer);
    if (!strcmp(buffer, "triangles")) {
        int numfaces = token->GetInt();
        
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skin format: no opening { in triangles" << std::endl;
            delete out;
            return NULL;
        }
        
        for (int i = 0; i < numfaces; i++) {
            int v1, v2, v3;
            v1 = token->GetInt();
            v2 = token->GetInt();
            v3 = token->GetInt();
            
            out->Triangles.push_back(glm::ivec3(v1,v2,v3));
        }
        
        token->GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Invalid .skin format: no closing } in triangles" << std::endl;
            delete out;
            return NULL;
        }
    }
    
    token->GetToken(buffer);
    if (!strcmp(buffer, "bindings")) {
        int numjoints = token->GetInt();
        
        token->GetToken(buffer);
        if (strcmp(buffer, "{")) {
            std::cerr << "Invalid .skin format: no opening { in bindings" << std::endl;
            delete out;
            return NULL;
        }
        
        for (int i = 0; i < numjoints; i++) {
            token->GetToken(buffer);
            if (strcmp(buffer, "matrix")) {
                std::cerr << "Invalid .skin format: no matrix tag in bindings" << std::endl;
                delete out;
                return NULL;
            }
            
            token->GetToken(buffer);
            if (strcmp(buffer, "{")) {
                std::cerr << "Invalid .skin format: no opening { in matrix in bindings" << std::endl;
                delete out;
                return NULL;
            }
            
            Matrix34 binding = Matrix34();
            
            // read each row
            float x, y, z;
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            binding.a = Vector3(x,y,z);
            
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            binding.b = Vector3(x,y,z);
            
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            binding.c = Vector3(x,y,z);
            
            x = token->GetFloat();
            y = token->GetFloat();
            z = token->GetFloat();
            binding.d = Vector3(x,y,z);
            
            binding.Inverse();
            
            out->Bindings[i] = binding;
            
            token->GetToken(buffer);
            if (strcmp(buffer, "}")) {
                std::cerr << "Invalid .skin format: no closing } in matrix in bindings" << std::endl;
                delete out;
                return NULL;
            }
        }
        
        token->GetToken(buffer);
        if (strcmp(buffer, "}")) {
            std::cerr << "Invalid .skin format: no closing } in bindings" << std::endl;
            delete out;
            return NULL;
        }
    }
    
    
    return out;
}

void Skin::Update() {
    if (this->needUpdate) {
        // compute new points & normals
        for (int i = 0; i < Positions.size(); i++) {
            std::unordered_map<int, float> weights = Weights[i];
            glm::vec3 pos = Positions[i];
            glm::vec3 norm = Normals[i];
            glm::vec3 finalPoint = glm::vec3(0.0f);
            glm::vec3 finalNorm = glm::vec3(0.0f);
            // need to loop over every bound joint's weight
            // (and world matrix and binding matrix)
            for (std::pair<int, float> pair : weights) {
                int jointInd = pair.first;
                float jointWeight = pair.second;
                Matrix34 B_i = Bindings[jointInd];
                Matrix34 W_i = Joint::joints[jointInd]->GetWorldMatrix();
                
                Matrix34 M_i = Matrix34();
                M_i.Dot(W_i, B_i);
                glm::vec3 v_i = glm::vec3(
                                M_i.a.x * pos.x + M_i.b.x * pos.y + M_i.c.x * pos.z + M_i.d.x,
                                M_i.a.y * pos.x + M_i.b.y * pos.y + M_i.c.y * pos.z + M_i.d.y,
                                M_i.a.z * pos.x + M_i.b.z * pos.y + M_i.c.z * pos.z + M_i.d.z
                                      );
                
                glm::vec3 n_i = glm::vec3(
                M_i.a.x * norm.x + M_i.b.x * norm.y + M_i.c.x * norm.z,
                M_i.a.y * norm.x + M_i.b.y * norm.y + M_i.c.y * norm.z,
                M_i.a.z * norm.x + M_i.b.z * norm.y + M_i.c.z * norm.z
                      );
                
                finalPoint += v_i * jointWeight;
                finalNorm += n_i * jointWeight;
            }
            
            this->WorldPos[i] = finalPoint;
            this->WorldNorm[i] = glm::normalize(finalNorm);
        }
        
        this->needUpdate = false;
    }
}

void Skin::Draw() {
    if (this->needUpdate)
        this->Update();
    // send material uniforms to shader
    glUniform3fv(glGetUniformLocation(this->shader, "matAmbient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
    glUniform3fv(glGetUniformLocation(this->shader, "matDiffuse"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
    
    // Lighting
    glEnable(GL_LIGHTING);
    // draw triangles
    glLoadMatrixf(Matrix34::IDENTITY);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLES);
        for (glm::ivec3 triangle : Triangles) {
            glm::vec3 v1 = WorldPos[triangle[0]];
            glm::vec3 n1 = WorldNorm[triangle[0]];
            
            glm::vec3 v2 = WorldPos[triangle[1]];
            glm::vec3 n2 = WorldNorm[triangle[1]];
            
            glm::vec3 v3 = WorldPos[triangle[2]];
            glm::vec3 n3 = WorldNorm[triangle[2]];

            // first
            glNormal3f(n1.x, n1.y, n1.z);
            glVertex3f(v1.x, v1.y, v1.z);
            // second
            glNormal3f(n2.x, n2.y, n2.z);
            glVertex3f(v2.x, v2.y, v2.z);
            // third
            glNormal3f(n3.x, n3.y, n3.z);
            glVertex3f(v3.x, v3.y, v3.z);
        }
    glEnd();
    glDisable(GL_CULL_FACE);
    glDisable(GL_LIGHTING);
    
}

void Skin::Reset() {
    
}

void Skin::Dirty() {
    this->needUpdate = true;
}
