//
//  rotationsim.hpp
//  cse169
//
//  Created by Alek Antic on 3/3/20.
//

#ifndef rotationsim_h
#define rotationsim_h

#include <stdio.h>
#include <vector>

#include "box.h"
#include "glm/glm.hpp"

#define NUM_BOXES 5
#define BOX_X 0.25f
#define BOX_Y 0.65f
#define BOX_Z 0.5f

enum Interp {SLERP, CATMULL_ROM};

class RotationSim {
private:
    float time;
    
    int selectedKey;
    std::vector<Box *> keyBoxes;
    Box * box;
    Interp mode;
    
public:
    RotationSim();
    ~RotationSim();
    
    void Update(float delta_t);
    void Draw();
    
    void nextKey() { selectedKey += 1; selectedKey %= NUM_BOXES; }
    void prevKey() {
        selectedKey -= 1;
        selectedKey = (selectedKey < 0) ? NUM_BOXES - 1 : selectedKey;
        
    }
    void rotateBy(float x, float y, float z);
    
    void toggleMode() { mode = (mode == SLERP) ? CATMULL_ROM : SLERP; }
    
    int getSelected() {return selectedKey;}
    Interp getMode() {return mode;}
};

#endif /* rotationsim_hpp */
