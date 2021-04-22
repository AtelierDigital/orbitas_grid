//
//  GridAgent.cpp
//  orbitas_grid
//
//  Created by Tiago Brizolara da Rosa on 22/12/18.
//

#include "GridAgent.hpp"
#include "ofApp.h"

GridAgent::GridAgent() { }

GridAgent::GridAgent(Grid* g)
{
    m = 1;
    grid = g;
    pos = ofVec2f();
    anchor_pos = ofVec2f();
    restore_direction = ofVec2f();
    a = ofVec2f();
    v = ofVec2f();
}
GridAgent::GridAgent(Grid* g, float x, float y, int st)
{
    m = 1;
    grid = g;
    pos = ofVec2f(x, y);
    anchor_pos = ofVec2f(x, y);
    restore_direction = ofVec2f();
    a = ofVec2f();
    v = ofVec2f();
    state = st;
}
GridAgent::GridAgent(Grid* g, ofVec2f xy, int st)
{
    m = 1;
    grid = g;
    pos = ofVec2f(xy.x, xy.y);
    anchor_pos = ofVec2f(xy.x, xy.y);
    restore_direction = ofVec2f();
    a = ofVec2f();
    v = ofVec2f();
    state = st;
}

void GridAgent::applyForce(ofVec2f f)
{
    a += f/m;   //.add(f).div(m);???
}

void GridAgent::update()
{
    //  Comecamos pela forca de restauracao para a posicao de grade
    /*PVector f = new PVector(anchor_pos.x - pos.x, anchor_pos.y - pos.y);
     float dist = anchor_pos.mag();
     f.setMag(0.00001f*dist);
     a.add(f).div(m);*/
    
    v += a;
    //v.limit(0.005f);
    
    pos += ( v * ofGetLastFrameTime() );
    
    if(ofApp::last_key != 'm' && ofApp::applyRestorationForce==true)
    {
        restore_direction.x = anchor_pos.x - pos.x;
        restore_direction.y = anchor_pos.y - pos.y;
        float d = restore_direction.lengthSquared();
        restore_direction.scale(100.0f*d).limit(100.0f*0.25f);//originalmente 0.01
        /*if(anchor_pos.x - pos.x < 0.1f) {
         pos.x = anchor_pos.x;
         v.x = 0.0f;
         } else*/ {
             v.x += restore_direction.x;
             //pos.x += restore_direction.x;
         }
        /*if(anchor_pos.y - pos.y < 0.1f) {
         pos.y = anchor_pos.y;
         v.y = 0.0f;
         } else*/ {
             v.y += restore_direction.y;
             //pos.y += restore_direction.y;
         }
        
        pos += v*ofGetLastFrameTime();
    }
    
    a.set(0.0f, 0.0f);
    v.set(0.0f, 0.0f);
}

float GridAgent::getAnchor_pos(int xy)
{
    return anchor_pos[xy];
}

float GridAgent::getPos(int xy)
{
    return pos[xy];
}

