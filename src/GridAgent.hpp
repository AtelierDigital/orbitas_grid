//
//  GridAgent.hpp
//  orbitas_grid
//
//  Created by Tiago Brizolara da Rosa on 22/12/18.
//

#ifndef GridAgent_hpp
#define GridAgent_hpp

#include <stdio.h>

#include "ofEvents.h"
#include "ofVec2f.h"

class Grid;

class GridAgent
{
    int state;
    
    ofVec2f pos;  //  posicao atual
    ofVec2f anchor_pos;  //  posicao de repouso na grade
    ofVec2f restore_direction;  //  aux
    
    float m;
    ofVec2f a;
    ofVec2f v;
    
    //  Grid 'a qual este agente pertence
    Grid* grid;
    
    int last_key = 0;
    
public:
    
    //  Estados possiveis
    const static int off = 0;
    const static int m1  = 1;
    const static int m2  = 2;
    const static int e   = 3;
    
    GridAgent();
    GridAgent(Grid* g);
    GridAgent(Grid* g, float x, float y, int st);
    GridAgent(Grid* g, ofVec2f xy, int st);
    
    float getAnchor_pos(int xy);
    float getPos(int xy);
    inline int getState() { return state; }
    inline int setState(int s) { state = s; }
    
    void applyForce(ofVec2f f);
    
    void update();
    
    void keyReleased(ofKeyEventArgs &e);
};


#endif /* GridAgent_hpp */


