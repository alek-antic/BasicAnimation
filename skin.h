//
//  skin.hpp
//  cse169
//
//  Created by Alek Antic on 1/21/20.
//

#ifndef skin_hpp
#define skin_hpp


#include "vector3.h"
#include "matrix34.h"
#include "token.h"
#include "joint.h"
#include <vector>
#include <unordered_map>

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Skin {
public:
    Skin(GLuint shader);
    ~Skin();
    static Skin * FromFile(const char * file, GLuint shader);
    
    void Update();
    void Draw();
    void Reset();
    
    void Dirty();
    
private:
    std::vector<glm::vec3> Positions;
    std::vector<glm::vec3> Normals;
    std::vector<glm::vec3> WorldPos;
    std::vector<glm::vec3> WorldNorm;
    std::vector<std::unordered_map<int, float> > Weights;
    std::vector<glm::ivec3> Triangles;
    std::unordered_map<int, Matrix34> Bindings;
    
    bool needUpdate;
    
    GLuint shader;
    
    GLuint vao;
    GLuint vbos[3];
};

#endif /* skin_hpp */
