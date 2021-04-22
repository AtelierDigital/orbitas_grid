//
//  Grid.hpp
//  orbitas_grid
//
//  Created by Tiago Brizolara da Rosa on 22/12/18.
//

#ifndef Grid_hpp
#define Grid_hpp

#include <stdio.h>

#include "ofVec3f.h"
#include "ofImage.h"

#include "GridAgent.hpp"

class Grid
{
    
    int dx, dy; // largura e altura de cada celula
    int nx, ny; // qtd de celulas em x e em y
    
    std::vector<GridAgent> gridAgents;
    
    //  Multiplicadores para forca aleatoria
    float xFactor = 0.0f;
    float yFactor = 0.0f;
    
    //  Imagem com 4 pontos de impressao de jornal
    ofImage dots;
    //  Numeros aleatorios multiplos da largura do frame de cada desenho de ponto
    //disposto em dots.png
    int* dots_frames;
    int dot_frameSize;
    
    ofVec3f gridPosition; // aux
    ofVec3f force; // aux
    
public:
    
    Grid();
    Grid(int a_dx, int a_dy, int a_nx, int a_ny, bool center);
    ~Grid();
    void update();
    void show();
    
    void keyReleased(ofKeyEventArgs &e);
    
    inline void setXFactor(float xf) {xFactor = xf;}
    inline void setYFactor(float yf) {yFactor = yf;}
};

#endif /* Grid_hpp */
