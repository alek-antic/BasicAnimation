//
//  box.cpp
//  cse169
//
//  Created by Alek Antic on 3/3/20.
//

#include "box.h"
#include <glm/gtc/matrix_transform.hpp>
#include <OpenGL/gl.h>
#include <glm/gtc/type_ptr.hpp>


Box::Box(float x, float y, float z, float xPos) {
    pos = glm::vec3(xPos, 0.0f, 0.0f);
    rot = Quaternion::FromEuler(0.0f, 0.0f, 0.0f);
    rot = Quaternion::normalize(rot);
    size = glm::vec3(x, y, z);
    world = glm::mat4(1.0f);
}


void Box::Update() {
    glm::mat4 rot_mat = rot.toMat();
    glm::mat4 translate = glm::translate(glm::mat4(1.0f), pos);
    
    world = translate * rot_mat;
}


void Box::Draw() {
    float x_w = size.x / 2.0f;
    float y_w = size.y / 2.0f;
    float z_w = size.z / 2.0f;
    
    glLoadMatrixf(glm::value_ptr(world));
    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBegin(GL_TRIANGLES);
    // front (normal towards screen)
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-x_w, y_w, -z_w);  // top left
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x_w, y_w, -z_w);   // top right
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-x_w, -y_w, -z_w); // bottom left
    
    
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x_w, y_w, -z_w);    // top right
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(x_w, -y_w, -z_w);  // bottom right
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(-x_w, -y_w, -z_w); // bottom left
    
    
    // back (normal into screen)
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x_w, y_w, z_w); // top left
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-x_w, y_w, z_w); // top right
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x_w, -y_w, z_w); // bottom left
    
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-x_w, y_w, z_w); // top right
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-x_w, -y_w, z_w); // bottom right
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(x_w, -y_w, z_w); // bottom left
    
    
    // right (normal +x)
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, y_w, -z_w);  // top left
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, y_w, z_w);   // top right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, -y_w, -z_w); // bottom left
    
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, y_w, z_w); // top right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, -y_w, z_w); // bottom right
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(x_w, -y_w, -z_w); // bottom left
    
    
    // left (normal -x)
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, y_w, z_w); // top left
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, y_w, -z_w); // top right
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, -y_w, z_w); // bottom left
    
    
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, y_w, -z_w); // top right
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, -y_w, -z_w); // bottom right
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-x_w, -y_w, z_w); // bottom left
    
    
    // top (normal +y)
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-x_w, y_w, z_w); // top left
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x_w, y_w, z_w); // top right
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-x_w, y_w, -z_w); // bottom left
    
    
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x_w, y_w, z_w); // top right
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(x_w, y_w, -z_w); // bottom right
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-x_w, y_w, -z_w); // bottom left
    
    
    // bottom (normal -y)
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-x_w, -y_w, -z_w); // top left
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x_w, -y_w, -z_w); // top right
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-x_w, -y_w, z_w); // bottom left
    
    
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x_w, -y_w, -z_w); // top right
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(x_w, -y_w, z_w); // bottom right
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-x_w, -y_w, z_w); // bottom left
    
    glEnd();
    glDisable(GL_LIGHTING);
    glDisable(GL_CULL_FACE);
}
