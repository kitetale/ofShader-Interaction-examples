#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

#ifdef TARGET_OPENGLES
    shader1.load("shadersES2/shader");
#else
    if(ofIsGLProgrammableRenderer()){
        shader.load("shadersGL3/shader");
        shader1.load("shadersGL3/shader3");
        shader2.load("shadersGL3/shader2");
    }else{
        shader1.load("shadersGL2/shader");
    }
#endif
    exampleNum = 0;
    
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
    
    brushImage.load("brush.png");
    bBrushDown = false;
    
    movie.load("shadow_videos/1.mov");
    movie.play();
    //scaledMov.allocate(movie.getWidth()*scaler,movie.getHeight()*scaler,GL_RGB);
    
}

//--------------------------------------------------------------
void ofApp::update(){
    /*
    if(exampleNum == 1){
        // ref: https://forum.openframeworks.cc/t/how-to-change-the-video-resolution-in-real-time/2364/2
        int bytes_per_pixel=3;
        ofPixels & pixels = movie.getPixels();
        int w = movie.getWidth();
        int h = movie.getHeight();
           
        unsigned char * nPixels = new unsigned char[w*h*bytes_per_pixel];;
            for (int i = 0; i < w*h; i++) {
                int base = i * bytes_per_pixel;
                int nBase = base*(scaler * scaler);
          
                unsigned char r = pixels[nBase + 0];
                unsigned char g = pixels[nBase + 1];
                unsigned char b = pixels[nBase + 2];
          
                nPixels[base + 0] = r;
                nPixels[base + 1] = g;
                nPixels[base + 2] = b;
              }
        scaledMov.loadData(nPixels,w,h,GL_RGB);
        movie.update();
    }
     */
    movie.update();
    
}

//--------------------------------------------------------------
void ofApp::draw(){

    ofSetColor(255);
    
    //----------------------------------------------------------
    // this is our alpha mask which we draw into.
    if(!exampleNum && bBrushDown) { // example 0
        maskFbo.begin();
        
        int brushImageSize = 50;
        int brushImageX = mouseX - brushImageSize * 0.5;
        int brushImageY = mouseY - brushImageSize * 0.5;
        brushImage.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
        
        maskFbo.end();
    }
    /*
    if(exampleNum == 1){
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
    }
 */
    //----------------------------------------------------------
    // drawing shaders into fbos
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
    
    // output shader
    shader.begin();
    // here is where the fbo is passed to the shader
    //shader.setUniformTexture("background", shader2Fbo.getTexture(), 2 );
    if (!exampleNum){ //example 0
        shader.setUniformTexture("maskTex", maskFbo.getTexture(), 1 );
        shader.setUniformTexture("foregroundTex", shader1Fbo.getTexture(), 2);
        shader.setUniform1f("scaler", 1.);
        shader2Fbo.draw(0,0);
    }
    if (exampleNum == 1){ //example 1
        ofTexture maskTex = movie.getTexture();
        maskTex.setTextureWrap(GL_CLAMP_TO_BORDER,GL_CLAMP_TO_BORDER);
        shader.setUniformTexture("maskTex", maskTex, 1);
        shader.setUniformTexture("foregroundTex", shader1Fbo.getTexture(), 2);
        shader.setUniform1f("scaler", 3.2);
        shader2Fbo.draw(0,0);
    }

    shader.end();
    
    
    //----------------------------------------------------------
    if (!exampleNum){
        ofDrawBitmapString("Drag the Mouse to draw", 15,15);
        ofDrawBitmapString("Press spacebar to clear", 15, 30);
    }

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(!exampleNum && key == ' ') {
        maskFbo.begin();
        ofClear(0,0,0,255);
        maskFbo.end();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == '1') {
        exampleNum = 1;
    }
    if (key == '0') {
        exampleNum = 0;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    if (!exampleNum) bBrushDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    if (!exampleNum) bBrushDown = false;
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
