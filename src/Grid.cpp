//
//  Grid.cpp
//  orbitas_grid
//

#include "Grid.hpp"
#include "ofApp.h"
#include "ofVec3f.h"

float rFactor;

Grid::Grid() { }

Grid::Grid(int a_dx, int a_dy, int a_nx, int a_ny, bool center)
{
    //ofAddListener(ofEvents().keyReleased, this, &Grid::keyReleased);  BUGGY THIS WAY =(
    
    dx = a_dx;
    dy = a_dy;
    nx = a_nx;
    ny = a_ny;
    
    for(int i=0; i<nx; i++) {
        for(int j=0; j<ny; j++) {
            gridAgents.push_back(GridAgent(this, dx/2.0f + i*dx, dy/2.0f + j*dy, 0));
        }
    }
    
    rFactor = 1;
    
    /*dots = loadImage("dots.png");
    dot_frameSize = 6;
    int new_size[2] = {dot_frameSize*4, dot_frameSize};
    dots.resize(new_size[0], new_size[1]);
    
    //  Convertendo valores de cinza para alpha
    dots.loadPixels();
    color clr;
    for(int i=0; i<new_size[0]*new_size[1]; i++)
    {
        clr = color(0, 0, 0, 256-(dots.pixels[i] >> 16) & 0xFF);
        dots.pixels[i] = clr;
    }
    dots.updatePixels();
    
    //  Sorteando o frame para cada posicao na grade
    dots_frames = new int[nx*ny];
    for(int i=0; i<gridAgents.size(); i++) {
        dots_frames[i] = dot_frameSize*(int)ofRandom(4.0);
    }*/
}

Grid::~Grid()
{
    //delete [] dots_frames;
}

void Grid::update()
{
    //  Determinando estado (terra, moon1, moon2, off)
    ofVec3f agentXY;
    int agent_index = 0;
    bool found = false;
    for(int i=0; i<ny; i++) {
        for(int j=0; j<nx; j++) {
            found = false;
            agent_index = j + nx*i;
            //  check earth
            gridPosition.x = i*dx;
            gridPosition.y = j*dy;
            agentXY.x = gridAgents.at(agent_index).getAnchor_pos(0);
            agentXY.y = gridAgents.at(agent_index).getAnchor_pos(1);
            if( (agentXY - ofApp::pe).lengthSquared() < ofApp::re*ofApp::re)
            {
                gridAgents.at(agent_index).setState(GridAgent::e);
                found = true;
                //continue;
            }
            //else {
            // check moon 1
            agentXY.x = gridAgents.at(agent_index).getAnchor_pos(0);
            agentXY.y = gridAgents.at(agent_index).getAnchor_pos(1);
            if( (agentXY - ofApp::pm1).lengthSquared() < ofApp::rm1*ofApp::rm1)
            {
                if((gridAgents.at(agent_index).getState() == GridAgent::e) && (ofApp::pm1.x < ofApp::pe.x + 0.8*ofApp::re)) {
                    gridAgents.at(agent_index).setState(GridAgent::e);
                } else {
                    gridAgents.at(agent_index).setState(GridAgent::m1);
                }
                found = true;
                //continue;
            }
            //  else { //  check moon 2
            agentXY.x = gridAgents.at(agent_index).getAnchor_pos(0);
            agentXY.y = gridAgents.at(agent_index).getAnchor_pos(1);
            if((agentXY - ofApp::pm2).lengthSquared() < ofApp::rm2*ofApp::rm2)
            {
                gridAgents.at(agent_index).setState(GridAgent::m2);
                found = true;
            }
            if(!found) {
                gridAgents.at(agent_index).setState(GridAgent::off);
            }
            //}
            //}
        }
    }
    
    //  Aplicando forca de perturbacao
    for(int i=0; i<gridAgents.size(); i++)
    {
        force.x = 50.0f*xFactor*ofRandom(-1.0f,1.0f);
        force.y = 50.0f*yFactor*ofRandom(-1.0f,1.0f);
        gridAgents.at(i).applyForce( force );
        gridAgents.at(i).update();
    }
}

//  Ao inves de dizer respeito a agentes, pensa apenas nos pixels do canvas.
//Eh um pos-processamento.
//  Instead of relating to agents, thinks just of the canvas pixels.
//Its a post-processing
/*void update(color[] all_pixels)
 {
 pushStyle();
 noStroke();
 for(int i=0; i<width; i+=dx) {
 for(int j=0; j<height; j+=dy) {
 if( ((all_pixels[j*width+i] >> 16) & 0xFF) == 255 ) {
 fill(0, 255);
 }
 else {
 fill(0, 0);
 }
 ellipse(i, j, 4, 4);
 }
 }
 popStyle();
 }*/

void Grid::show()
{
    for(int i=0; i<gridAgents.size(); i++)
    {
        if(gridAgents.at(i).getState() == GridAgent::m1 || gridAgents.at(i).getState() == GridAgent::m2)
        {
            ;
        }
        else if(gridAgents.at(i).getState() == GridAgent::e)
        {
            /*
             float r = map( gridAgents.get(i).pos.x - pe.x, -re, re, dx+6, 4);
             image(
             dots.get(dots_frames[i], 0, dot_frameSize, dot_frameSize),
             gridAgents.get(i).pos.x,
             gridAgents.get(i).pos.y,
             r,
             r);
            /**/
            ofPushStyle();
            //noStroke(); no OF contorno e preenchimento sao desenhados um de cada vez
            ofSetColor(20);
            float r = rFactor * ofMap( gridAgents.at(i).getPos(0) - ofApp::pe.x, -ofApp::re, ofApp::re, dx+2, 2);
            ofDrawEllipse(gridAgents.at(i).getPos(0), gridAgents.at(i).getPos(1), r, r);
            ofPopStyle();/**/
        }
        else {
            ofPushStyle();
            //noStroke();
            ofFill();
            ofSetColor(50*ofRandom(0.75,1.25));
            ofDrawEllipse(gridAgents.at(i).getPos(0), gridAgents.at(i).getPos(1), 2.0*ofRandom(0.65,1.4), 2.0*ofRandom(0.65,1.4));
            ofPopStyle();
        }
    }
}

void Grid::keyReleased(ofKeyEventArgs &e)
{
    if(e.key == ',') {
        xFactor -= 1.0f;
    } else if(e.key == '.') {
        xFactor += 1.0f;
    } else if(e.key == ';') {
        yFactor -= 1.0f;
    } else if(e.key == '/') {
        yFactor += 1.0f;
    }
    else if(e.key == 'a') {
        xFactor = 0.5f;
    } else if(e.key == 's') {
        xFactor = 1.0f;
    } else if(e.key == 'd') {
        xFactor = 2.0f;
    } else if(e.key == 'f') {
        xFactor = 3.0f;
    } else if(e.key == 'g') {
        xFactor = 4.0f;
    }
    else if(e.key == 'y') {
        yFactor = 0.5f;
    } else if(e.key == 'u') {
        yFactor = 1.0f;
    } else if(e.key == 'i') {
        yFactor = 2.0f;
    } else if(e.key == 'o') {
        yFactor = 3.0f;
    } else if(e.key == 'p') {
        yFactor = 4.0f;
    }
    else if(e.key == 'l') {
        xFactor = 0.0f;
    }
    else if(e.key == 'q') {
        yFactor = 0.0f;
    }
    else if(e.key == 'z') {
        xFactor = yFactor = 0.0f;
    }
    
    else if(e.key == OF_KEY_LEFT) {
        rFactor -= 0.2;
    }
    else if(e.key == OF_KEY_RIGHT) {
        std::cout << "......." << std::endl;
        rFactor += 0.2;
    }
    std::cout << e.keycode << std::endl;
    std::cout << "xFactor = " << xFactor << " | yFactor = " << yFactor << std::endl;
    std::cout << "rFactor = " << rFactor << ". OF_KEY_RIGHT = " << OF_KEY_RIGHT << std::endl;
}
