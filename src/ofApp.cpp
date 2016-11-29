#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // initialize circle stuff
    num_shapes = 0;
    ofRandom(1337);
    rolling_ptr = 0;
    connectionState = false;
    
    // Initialize per circle stuff
    for (int i = 0; i < MAX_SHAPES; i++) {
        
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
    soundStream.setDeviceID(1);
    
    soundStream.setup(this, 2, 0, sampleRate, bufferSize, 4);
    
    
    
    // on OSX: if you want to use ofSoundPlayer together with ofSoundStream you need to synchronize buffersizes.
    // use ofFmodSetBuffersize(bufferSize) to set the buffersize in fmodx prior to loading a file.
    
    
    ofSetFrameRate(30);
    stk::Stk::setSampleRate((float)sampleRate);
    
    // Initialize empty audio buffer to draw from.
    
    for (int i = 0; i < bufferSize + sampleRate; i++) {
        audio.push_back(0.0);
    }
    
    
    ofSetBackgroundAuto(false);
    
    // Set up background
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    backgroundSaturation = 230.0;
    backgroundBrightness = 100.0;
    backgroundHue = 180.0;
    backgroundColor.setBrightness(backgroundBrightness);
    backgroundColor.setSaturation(backgroundSaturation);
    backgroundColor.setHue(backgroundHue);
    
    ofLog(OF_LOG_NOTICE, "backgroundcolor.getHue(): %f sat: %f bright: %f", backgroundColor.getHue(), backgroundColor.getBrightness(), backgroundColor.getSaturation());
    
    // Alpha blend!
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    
    // Set up reverb
    reverb.clear();
    reverb.setT60(2.7);
    
    // Set up high-pass
    highpassLeft.setPole(-0.7);
    highpassRight.setPole(-0.7);
    
    createSquare = false;
    createTriangle = false;
}

//--------------------------------------------------------------
void ofApp::update(){
    if (abs(backgroundHue - backgroundColor.getHue()) < 1.0) {
        backgroundHue = backgroundColor.getHue() + ofRandom(40.0);
        while (backgroundHue > 200.0) {
            backgroundHue = backgroundHue - 40.0;
        }
    } else {
        // Slowly move background hue towards target background hue
        if (backgroundHue - backgroundColor.getHue() >= 1.0) {
            backgroundColor.setHue(backgroundColor.getHue() + ofRandom(0.5));
        } else {
            backgroundColor.setHue(backgroundColor.getHue() - ofRandom(0.5));
        }
    }
  
}

//--------------------------------------------------------------
void ofApp::draw(){
    // Redraw background for motion blur
    
    int wh = ofGetWindowHeight();
    int ww = ofGetWindowWidth();
    int backgroundRed = backgroundColor.r;
    int backgroundBlue = backgroundColor.b;
    int backgroundGreen = backgroundColor.g;
    
    ofSetColor(backgroundRed, backgroundGreen, backgroundBlue,75);
    ofDrawRectangle(0,0,ww,wh);
    
    ofPushStyle();
    ofPushMatrix();
    
    for (int i = 0; i < num_shapes; i++) {
        shapeVector[i]->draw();
    }
    
   
    ofPopMatrix();
    ofPopStyle();
    
    
  
    // Draw connection in
    if (connectionState) {
        if (mouseDown) {
            ofSetLineWidth(2);
            ofSetColor(255, 255, 255, 200);
            ofPolyline path = ofPolyline();
            path.addVertex((float)firstMouseX, (float)firstMouseY, 0);
            path.addVertex((float)secondMouseX, (float)secondMouseY, 0);
            
            path.draw();
        }
    }
    
    // Draw all connections
    for (int i = 0; i < connectionVector.size(); i++) {
        ofSetLineWidth(3);
        ofSetColor(255, 255, 255, 255);
        ofPolyline path = ofPolyline();
        vector<synthShape *> *currVector = connectionVector[i];
        path.addVertex((*currVector)[0]->getX(), (*currVector)[0]->getY(), 0);
        path.addVertex((*currVector)[1]->getX(), (*currVector)[1]->getY(), 0);
        
        path.draw();
    }
    
    // Draw text
    
    ofSetColor(255, 255, 255, 255);
    ofDrawBitmapString("click to create/drag circles. d deletes a circle", 10, wh - 20);
    ofDrawBitmapString("holding down c and dragging between circles CONNECTS them", 10, wh - 8);
    
    ofDrawBitmapString("hemanth's reactable thingy", ww - 210, wh - 8);
}

// Delete all connections associated with this shape
void ofApp::deleteConnections(synthShape * cir) {
    stack<int> deletionStack;
    
    // Delete the set within the vector
    // Also, fix up the frequency of the connections
    cir->deleteAllConnections();
    
    // Push all the indices of connections in the vector into a stack
    for (int i = 0; i < connectionVector.size(); i++) {
        if (((*connectionVector[i])[0] == cir )||((*connectionVector[i])[1] == cir )) {
            deletionStack.push(i);
        }
    }
    
    // Delete each connection. 
    while (!(deletionStack.empty())) {
        int index = deletionStack.top();
        deletionStack.pop();
        vector<synthShape *> *vecToDelete = connectionVector[index];
        delete vecToDelete;
        connectionVector.erase(connectionVector.begin() + index);
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'd') {
        if (num_shapes == 0) {
            return;
        }
        
        
        // Attempt to delete the circle that our mouse is over
        for (int i = num_shapes - 1; i >= 0; i--) {
            if (shapeVector[i]->within(ofGetMouseX(),ofGetMouseY())) {
                // Lock so we don't delete while audio is occuring
                circleMutex.lock();
                deleteConnections(shapeVector[i]);
                shapeVector.erase(shapeVector.begin() + i);
                num_shapes = num_shapes - 1;
                circleMutex.unlock();
                return;
            }
        }
        
        // Otherwise, delete the least-recent circle we modified
        synthShape * circleToDelete = shapeVector[0];
        circleMutex.lock();
        deleteConnections(circleToDelete);
        delete circleToDelete;
        shapeVector.erase(shapeVector.begin());
        
        num_shapes = num_shapes - 1;
        circleMutex.unlock();
    }
    
    if (key == 'c') {
        connectionState = true;
        
    }
    
    if (key == 's') {
        createSquare = true;
    }
    
    if (key == 't') {
        createTriangle = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if (key == 'c') {
        connectionState = false;
    }
    
    if (key == 's') {
        createSquare = false;
    }
    
    if (key == 't') {
        createTriangle = false;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    if (!connectionState) {
        shapeVector[selected_index]->setXYandUpdate(x + selected_x,y + selected_y);
    } else {
        secondMouseX = x;
        secondMouseY = y;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    mouseDown = true;
    int i = 0;
    if (connectionState) {
        firstMouseX = x;
        firstMouseY = y;
        
        secondMouseX = x;
        secondMouseY = y;
        
        ofLog(OF_LOG_NOTICE, "Connection state and mouse pressed.");
        for (i = num_shapes - 1; i >= 0; i--) {
            if (shapeVector[i]->within(x,y)) {
                // Put in the connection vector
                currentConnection = new vector<synthShape *>();
                currentConnection->push_back(shapeVector[i]);
                return;
            }
        }
    }
    else {
        // Check if we touched a shape that exists, and update it if so
        for (i = num_shapes - 1; i >= 0; i--) {
            if (shapeVector[i]->within(x,y)) {
                // push the selected circle so it's drawn on top
                shapeVector.push_back(shapeVector[i]);
                selected_x = shapeVector[i]->getX() - x;
                selected_y = shapeVector[i]->getY() - y;
                shapeVector.erase(shapeVector.begin() + i);
                selected_index = num_shapes - 1;
                
                return;
            }
        }
        
        // if we've not maxed out the number of shapes, draw one
        if (num_shapes < MAX_SHAPES) {
            ofLog(OF_LOG_NOTICE, "Drawing dis circle.");
            synthShape* newShape;
            if (createSquare) {
                newShape = new synthSquare(x, y, sampleRate);
            } else if (createTriangle) {
                newShape = new synthTriangle(x, y, sampleRate);
            } else {
                newShape = new synthCircle(x, y, sampleRate);
            }
            shapeVector.push_back(newShape);
            selected_index = num_shapes;
            num_shapes = num_shapes+1;
            selected_x = 0;
            selected_y = 0;
        }
        
        // else, pop the head off and update it.
        else
        {
            // TODO copy locking code from adding a circle (above) to here
            shapeVector.erase(shapeVector.begin());
            if (createSquare) {
                shapeVector.push_back(new synthSquare(x,y, sampleRate));
            } else {
                shapeVector.push_back(new synthCircle(x,y, sampleRate));
            }
            selected_index = num_shapes - 1;
            selected_x = 0;
            selected_y = 0;
        }
    }
}

bool ofApp::existsConnection(synthShape* one, synthShape* two) {
    for (int i = 0; i < connectionVector.size(); i++) {
        vector<synthShape *> *currVector = connectionVector[i];
        if ((*currVector)[0] == one && (*currVector)[1] == two)
            return true;
        if ((*currVector)[1] == one && (*currVector)[0] == two)
            return true;
    }
    return false;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    mouseDown = false;
    if (connectionState) {
        // If we've got a live connection that needs a second circle
        if (currentConnection != NULL && currentConnection->size() > 0) {
            for (int i = num_shapes - 1; i >= 0; i--) {
                if (shapeVector[i]->within(x,y) &&
                    (*currentConnection)[0] != shapeVector[i] &&
                    !existsConnection((*currentConnection)[0], shapeVector[i])) {
                    if ((*currentConnection)[0]->getOverallConnectedParent() == shapeVector[i]->getOverallConnectedParent()) {
                        // TODO - animate a red broken line that fades to show no loops
                        
                    } else {
                        // Put in the connection vector, and empty the
                        // in progress connection
                        ofLog(OF_LOG_NOTICE, "CONNECTING TWO CIRCLES");
                        
                        // Set shape connection stuff
                        synthShape* childShape = (*currentConnection)[0];
                        synthShape* parentShape = shapeVector[i];
                        
                        childShape->connectAndParent(parentShape);
                        parentShape->connect(childShape);
                        
                        
                        currentConnection->push_back(shapeVector[i]);
                        connectionVector.push_back(currentConnection);
                        currentConnection = NULL;
                        return;
                    }
                }
            }
            
        }
        delete currentConnection;
        currentConnection = NULL;
    } else {
        shapeVector.push_back(shapeVector[selected_index]);
        shapeVector.erase(shapeVector.begin() + selected_index);
    }
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
    circleMutex.lock();
    for(int i = 0; i < bufferSize * nChannels; i += nChannels) {
        // Compute each synth
        for (int j = 0; j < num_shapes; j++) {
            output[i] = output[i] + (shapeVector[j]->tick() / (float)num_shapes);
        }
        
        // Compute reverb
        output[i] = reverb.tick(output[i], 0);
        
        // Compute high-pass
        output[i] = highpassLeft.tick(output[i]);
        
        // Compute stereo if stereo output is available
        if (nChannels > 1) {
            output[i+1] = reverb.lastOut(1);
            output[i+1] = highpassRight.tick(output[i+1]);
        }
        
        // Copy over for multichannel - more than two, just copy the excess
        for (int j = 2; j < nChannels; j++) {
            output[i+j] = output[i];
        }
        
        // Store audio history
        audio.push_back(output[i]);
        audio.erase(audio.begin());
        
    }
    circleMutex.unlock();
}
