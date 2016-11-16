#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // initialize circle stuff
    num_circles = 0;
    ofRandom(1337);
    rolling_ptr = 0;
    
    // Initialize per circle stuff
    for (int i = 0; i < MAX_CIRCLES; i++) {
        
    }

    ofLog(OF_LOG_NOTICE, "INITIALIZING...");
    
    // initialize audio
    
    // 2 output channels,
    // 0 input channels
    // 44100 samples per second
    // 256 samples per buffer
    // 4 num buffers (latency)
    
    bufferSize		= 256;
    sampleRate 			= 44100;
    nInputChans         = 2;
    volume				= 0.5f;
    
    
    soundStream.printDeviceList();
    
    // INSERT THE DEVICE YOU WANT TO PLAY ON HERE!
    soundStream.setDeviceID(4);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
    
    
    ofSetFrameRate(60);
    stk::Stk::setSampleRate((float)sampleRate);
    
    // Initialize empty audio buffer to draw from.
    
    for (int i = 0; i < bufferSize + sampleRate; i++) {
        audio.push_back(0.0);
    }
    
    // Alpha blend!
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetBackgroundAuto(false);
    
    
    ofBackground(0);  // Clear the screen with a black color
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){
    // Redraw background for motion blur
    
    int wh = ofGetWindowHeight();
    int ww = ofGetWindowWidth();
    ofSetColor(0,0,0,75);
    ofDrawRectangle(0,0,ww,wh);
    
    ofPushStyle();
    ofPushMatrix();
    
    for (int i = 0; i < num_circles; i++) {
        circleVector[i]->draw();
    }
    
    
    ofPopMatrix();
    ofPopStyle();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        if (num_circles == 0) {
            return;
        }
        
        // Attempt to delete the circle that our mouse is over
        for (int i = num_circles - 1; i >= 0; i--) {
            if (circleVector[i]->within(ofGetMouseX(),ofGetMouseY())) {
                int delayIndex = circleVector[i]->getIndex();
                delay_id_queue.push(delayIndex);
                circleVector.erase(circleVector.begin() + i);
                num_circles = num_circles - 1;
                return;
            }
        }
        
        // Otherwise, delete the least-recent circle we modified
        int delayIndex = circleVector[0]->getIndex();
        delay_id_queue.push(circleVector[0]->getIndex());
        circleVector.erase(circleVector.begin());
        num_circles = num_circles - 1;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    circleVector[selected_index]->setXYandUpdate(x + selected_x,y + selected_y);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    int i = 0;
    
    // Check if we touched a circle that exists, and update it if so
    for (i = num_circles - 1; i >= 0; i--) {
        if (circleVector[i]->within(x,y)) {
            // push the selected circle so it's drawn on top
            circleVector.push_back(circleVector[i]);
            selected_x = circleVector[i]->getX() - x;
            selected_y = circleVector[i]->getY() - y;
            circleVector.erase(circleVector.begin() + i);
            selected_index = num_circles - 1;
            
            return;
        }
    }
    
    // if we've not maxed out the number of delayCircles, draw one
    if (num_circles < MAX_CIRCLES) {
        circleVector.push_back(new synthCircle(x, y, sampleRate));
        selected_index = num_circles;
        num_circles = num_circles+1;
        selected_x = 0;
        selected_y = 0;
    }
    
    // else, pop the head off and update it.
    else
    {
        circleVector.erase(circleVector.begin());
        circleVector.push_back(new synthCircle(x,y, sampleRate));
        selected_index = num_circles - 1;
        selected_x = 0;
        selected_y = 0;
    }
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    circleVector.push_back(circleVector[selected_index]);
    circleVector.erase(circleVector.begin() + selected_index);
    
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

void ofApp::audioOut( float * output, int bufferSize, int nChannels ) {
    // TODO get audio playing
    for(int i = 0; i < bufferSize * nChannels; i += 2) {
        //output[i] = fileInput.tick(0) * volume; // writing to the left channel
        //output[i+1] = fileInput.lastOut(1) * volume; // writing to the right channel
        
        
        
        // Compute each delay
        for (int j = 0; j < num_circles; j++) {
            
        }
        audio.push_back(output[i]);
        audio.erase(audio.begin());
        
    }
}
