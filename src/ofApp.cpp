#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

#ifdef TARGET_OPENGLES
    shader1.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
        shader1.load("shadersGL3/shader1");
        shader2.load("shadersGL3/shader2");
    }else{
        shader1.load("shadersGL2/shader");
    }
#endif
    int width = ofGetWidth();
    int height = ofGetHeight();
    maskFbo.allocate(width, height);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    maskFbo.begin();
    ofClear(0,0,0,255);
    maskFbo.end();
    
    shader1Fbo.allocate(width, height);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    shader1Fbo.begin();
    ofClear(0,0,0,255);
    shader1Fbo.end();
    
    shader2Fbo.allocate(width, height);
    // Clear the FBO's
    // otherwise it will bring some junk with it from the memory
    shader2Fbo.begin();
    ofClear(0,0,0,255);
    shader2Fbo.end();
    /*
    backgroundImage.load("A.jpg");
    foregroundImage.load("B.jpg");
    */
    brushImage.load("brush.png");
    
    
    bBrushDown = false;
    
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
   
    /*
    shader2.begin();
    shader2.setUniform1f("time", ofGetElapsedTimef());
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    shader2.end();
    
    shader1.begin();
    
    shader1.setUniform1f("time", ofGetElapsedTimef());
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    shader1.end();
     */

    //----------------------------------------------------------
    // this is our alpha mask which we draw into.
    if(bBrushDown) {
        maskFbo.begin();
        
        int brushImageSize = 50;
        int brushImageX = mouseX - brushImageSize * 0.5;
        int brushImageY = mouseY - brushImageSize * 0.5;
        brushImage.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
        
        maskFbo.end();
    }
 
    //----------------------------------------------------------
    // HERE the shader-masking happends
    
    shader1Fbo.begin();
    shader1.begin();
    shader1.setUniform1f("time", ofGetElapsedTimef());
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    shader1.end();
    shader1Fbo.end();
    
    
    shader2Fbo.begin();
    shader2.begin();
    shader2.setUniform1f("time", ofGetElapsedTimef());
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    shader2.end();
    shader2Fbo.end();
    
    
    shader.begin();
    // here is where the fbo is passed to the shader
    //shader.setUniformTexture("background", shader2Fbo.getTexture(), 2 );
    shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );
    shader.setUniformTexture("foregroundTex", shader1Fbo.getTexture(), 2);
    shader2Fbo.draw(0,0);
    
    
    
    
    shader.end();
    
    
    //----------------------------------------------------------
    ofDrawBitmapString("Drag the Mouse to draw", 15,15);
    ofDrawBitmapString("Press spacebar to clear", 15, 30);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') {
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    bBrushDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    bBrushDown = false;
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
