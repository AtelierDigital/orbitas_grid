#include "ofApp.h"
#include "GridAgent.hpp"

float ofApp::re;
float ofApp::rm1;
float ofApp::rm2;                   // raios de terra, lua1 e lua2
float ofApp::rOrbit_e;
float ofApp::rOrbit_m1;
float ofApp::rOrbit_m2; // raios das orbitas de terra, lua1 e lua2
ofVec3f ofApp::pe;
ofVec3f ofApp::pm1;
ofVec3f ofApp::pm2;                 // posicoes de terra, lua1 e lua2

bool ofApp::applyRestorationForce;

int ofApp::last_key;

//--------------------------------------------------------------
void ofApp::setup(){
    
    last_key = OF_KEY_PAGE_DOWN;    //  initializing with one that that our program doesn't use
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    ofEnableAntiAliasing();
    
    ofSetBackgroundAuto(false);
    
    ofRectMode(OF_RECTMODE_CENTER);
    
    //rs
    re = 2*45;
    rm1 = rm2 = re/3;
    rOrbit_e = 2*25;
    rOrbit_m1 = 2*50;
    rOrbit_m2 = 1.75*rOrbit_m1;
    
    ang_speed_per_frame = 0.01;
    invertFactor = 1.0;
    
    applyRestorationForce = true;
    
    //ofSetFrameRate(60);
    ofSetVerticalSync(true);
    bool center = true;
    int gridSide = 10;//7
    grid = Grid(gridSide, gridSide, ofGetWidth()/(gridSide), ofGetHeight()/(gridSide), center);

    //
    //  MIDI stuff
    
    // open port by number or name
    midiIn.openPort("Arturia MINILAB");
    //midiIn.openVirtualPort("ofxMidiIn Input"); // open a virtual port
    
    //  We are going to be zeroing some knobs...
    midiOut.openPort(0);
    midiOut.sendControlChange(1, 7, 64);
    midiOut.sendControlChange(1, 74, 64);
    midiOut.sendControlChange(1, 71, 64);
    midiOut.sendControlChange(1, 76, 64);
    midiOut.sendControlChange(1, 77, 64);
    midiOut.sendControlChange(1, 93, 64);
    midiOut.sendControlChange(1, 73, 64);
    midiOut.sendControlChange(1, 75, 64);
    midiOut.sendControlChange(1, 114, 64);
    midiOut.sendControlChange(1, 18, 64);
    midiOut.sendControlChange(1, 19, 64);
    midiOut.sendControlChange(1, 16, 64);
    midiOut.sendControlChange(1, 17, 64);
    midiOut.sendControlChange(1, 91, 64);
    midiOut.sendControlChange(1, 79, 64);
    midiOut.sendControlChange(1, 72, 64);
    
    // don't ignore sysex, timing, & active sense messages,
    // these are ignored by default
    midiIn.ignoreTypes(false, false, false);
    
    // add ofApp as a listener
    midiIn.addListener(this);
    
    // print received messages to the console
    midiIn.setVerbose(true);    //  will   effect if ofSetLogLevel(OF_LOG_VERBOSE);
    
    //
    //  Printing some info
    
    std::cout << "--------------------------------------------------------" << std::endl;
    std::cout << "Window size = " << ofGetWidth() << " x " << ofGetHeight() << std::endl;
    std::cout << "gridSide = " << gridSide << std::endl;
    std::cout << "# of grid elements in horizontal   = " << ofGetWidth()/(gridSide) << std::endl;
    std::cout << "# of grid elements in vertical     = " << ofGetHeight()/(gridSide) << std::endl;
    std::cout << "total grid elements (horiz x vert) = " << ofGetHeight()/(gridSide)*ofGetHeight()/(gridSide) << std::endl;    
    midiIn.listInPorts();
    midiOut.listOutPorts();
    std::cout << "--------------------------------------------------------" << std::endl;
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw()
{
    //logLastMIDImessages();
    
    if(mode == 0) {  //  Desenha com contornos. E apaga o quadro anterior
        //stroke(0);
        ofBackground(255);
    } else {  //  Desenha sem contornos e nao apaga o quadro anterior
        //noStroke();
    }
    
    //  Esquema de pilha de matrizes (OpenGL)
    //  Pense que as operacoes matriciais (escala, translacao, rotacao)
    //sao aplicadas sobre o sistema de coordenadas de forma cumulativa,
    //na ordem que aparecem. Eh importante desenhar essas operacoes
    //ate ganhar intuicao
    ofPushMatrix();
    
    // Translacao ao centro da janela
    ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
    ofSetColor(255,200,50);
    //ellipse(0,0,20,20);
    
    // "A Terra" gira ao redor desse centro
    ofRotateRad(theta);  //  Giramos o sistema de um angulo theta. Para que lado? O lado mais curto que leva
    //do eixo x ao eixo y (conhecido como 'regra da mao direita')
    ofTranslate(rOrbit_e,0);  //  E transladamos 50 em x (no novo sistema)
    ofSetColor(50,200,255, 50);
    //ofDrawCircle(0.0f, 0.0f, re);  //  E desenhamos um circulo 10x10
            //de raio 5) na posicao (0,0). Mas nao esqueca que eh no (0,0) do novo sistema,
            //que sofreu duas translacoes e uma rotacao.
    //  Posicao no sistema sem transformacoes:
    ofMatrix4x4 m;
    m.glTranslate(ofGetWidth()/2,ofGetHeight()/2, 0.0f);
    m.glRotateRad(theta, 0.0f, 0.0f, 1.0f);
    m.glTranslate(rOrbit_e, 0.0f, 0.0f);
    pe *= 0;
    pe = pe * m;
    ofMatrix4x4 m_aux = m;
    //  No Processing era muito mais facil =(
    //pe.x = screenX(0, 0);
    //pe.y = screenY(0, 0);
    
    // Lua #1 roda ao redor da Terra
    // pushMatrix() is called to save the transformation state before drawing moon #1.
    // This way we can pop and return to earth before drawing moon #2.
    // Both moons rotate around the earth (which itself is rotating around the sun).
    ofPushMatrix();
    ofRotateRad(-theta*6);
    ofTranslate(rOrbit_m1,0);
    ofSetColor(50,255,200, 50);
    //ofDrawCircle(0.0f, 0.0f, rm1);
    //  Posicao no sistema sem transformacoes:
    m.glRotateRad(-theta*6, 0.0f, 0.0f, 1.0f);
    m.glTranslate(rOrbit_m1, 0.0f, 0.0f);
    pm1 *= 0.0f;
    pm1 = pm1 * m;
    //pm1.x = screenX(0, 0);
    //pm1.y = screenY(0, 0);
    ofPopMatrix();
    
    
    // Lua #2 tambem roda ao redor da Terra
    ofPushMatrix();
    ofRotateRad(theta*2);
    ofTranslate(rOrbit_m2,0);
    ofSetColor(255,183,39, 50);
    //ofDrawCircle(0.0f, 0.0f, rm2);
    //  Posicao no sistema sem transformacoes:
    m_aux.glRotateRad(theta*2, 0.0f, 0.0f, 1.0f);
    m_aux.glTranslate(rOrbit_m2, 0.0f, 0.0f);
    pm2 *= 0.0f;
    pm2 = pm2 * m_aux;
    //pm2.x = screenX(0, 0);
    //pm2.y = screenY(0, 0);
    ofPopMatrix();
    
    ofPopMatrix();
    
    theta += invertFactor * ang_speed_per_frame;
    
    grid.update();//(pixels);
    grid.show();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(ofKeyEventArgs &e){
    last_key = e.key;
    grid.keyReleased(e);
    //grid.keyUp(); ...
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::switchMode(){
    if(mode == 0) {
        mode = 1;
    } else {
        mode = 0;
    }
    std::cout << "mode = " << mode << std::endl;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    switchMode();
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::newMidiMessage(ofxMidiMessage& msg) {
    
    // draw the last received message contents to the screen,
    // this doesn't print all the data from every status type
    // but you should get the general idea
    stringstream text;
    text << ofxMidiMessage::getStatusString(msg.status);
    while(text.str().length() < 16) { // pad status width
        text << " ";
    }

    if(msg.status < MIDI_SYSEX) {
        text << "chan: " << msg.channel;
        
        //  Arturia MINILab knobs
        //  7   74  71   76   77   93   73   75
        //  114 18  19   16   17   91   79   72
        if(msg.status == MIDI_CONTROL_CHANGE) {
            text << "\tctl: " << msg.control << "val: " << msg.value;
            ofMap(msg.control, 0, 127, 0, ofGetWidth()*0.2);
            switch(msg.control)
            {
                case 7:
                    grid.setXFactor(ofMap(msg.value, 0, 127, 0., 25.));
                    grid.setYFactor(ofMap(msg.value, 0, 127, 0., 25.));
                    break;
                case 114: grid.setXFactor(ofMap(msg.value, 0, 127, 0., 25.)); break;
                case 18: grid.setYFactor(ofMap(msg.value, 0, 127, 0., 25.)); break;
                case 74: ang_speed_per_frame = ofMap(msg.value, 0, 127, -0.1, 0.1); break;
            }
        }
        else if(msg.status == MIDI_PITCH_BEND) {
            text << "\tval: " << msg.value;
            //ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
            //                ofMap(message.value, 0, MIDI_MAX_BEND, 0, ofGetWidth()*0.2), 10);
        }
        //  Arturia MINILab pads use notein/noteoff at channel 10
        //  36  37  38  39   40   41   42   43
        else {
            text << "\tpitch: " << msg.pitch;
            //ofDrawRectangle(x + ofGetWidth()*0.2, y + 12,
            //                ofMap(message.pitch, 0, 127, 0, ofGetWidth()*0.2), 10);
            
            text << "\tvel: " << msg.velocity;
            //ofDrawRectangle(x + (ofGetWidth()*0.2 * 2), y + 12,
            //                ofMap(message.velocity, 0, 127, 0, ofGetWidth()*0.2), 10);
            if(msg.channel == 10 && msg.velocity == 0)
            {
                switch(msg.pitch)
                {
                    case 36: switchMode(); break;
                    case 37: invertFactor *= -1.0; break;
                    case 38: applyRestorationForce = !applyRestorationForce;
                }
            }
        }
        text << " "; // pad for delta print
    }
    std::cout << text.str() << std::endl;
    
    
    // add the latest message to the message queue
    midiMessages.push_back(msg);
    
    // remove any old messages if we have too many
    while(midiMessages.size() > maxMessages) {
        midiMessages.erase(midiMessages.begin());
    }
}

//--------------------------------------------------------------
void ofApp::exit(){
    // clean up MIDI
    midiIn.closePort();
    midiIn.removeListener(this);
    midiOut.closePort();
}


