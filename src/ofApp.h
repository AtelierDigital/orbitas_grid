#pragma once

#include "ofMain.h"

#include "Grid.hpp"

#include "ofxMidi.h"

//
//  Circulos girando em torno de si, com renderizacao obedecendo
//grade com perturbacao e restauracao
//  Inspirado em dithering de impressoes antigas
//
//  Para o Atelier Digital, dez/2017. Portado do Processing para o OF em dez/2018
//

#include "ofVec3f.h"

class ofApp : public ofBaseApp, public ofxMidiListener {

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
        void keyReleased(ofKeyEventArgs &e);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void exit();

        void switchMode();
    
        static int last_key;
        static bool applyRestorationForce;
    
        // Angulo de rotacao, em radianos (PI radianos = 180 graus)
        float theta = 0;
    
        float ang_speed_per_frame;
        float invertFactor; //  1.0 or -1.0
    
        int mode = 0;
    
        static float re, rm1, rm2;                   // raios de terra, lua1 e lua2
        static float rOrbit_e, rOrbit_m1, rOrbit_m2; // raios das orbitas de terra, lua1 e lua2
        static ofVec3f pe, pm1, pm2;                 // posicoes de terra, lua1 e lua2
    
        Grid grid;
    
        //
        //  MIDI
    
        void newMidiMessage(ofxMidiMessage& eventArgs);
    
        ofxMidiIn midiIn;
        ofxMidiOut midiOut;
        std::vector<ofxMidiMessage> midiMessages;
        std::size_t maxMessages = 10; //< max number of messages to keep track of
};
