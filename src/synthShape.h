//
//  synthShape.h
//  cs476a-finalproject
//
//  Created by Hemanth Kini on 11/25/16.
//
//



#include "ofxStk.h"

#ifndef synthShape_h
#define synthShape_h

class synthShape {
public:
    float x;
    float y;
    float radius;
    int index;
    int Enabled;
    
    float hue;
    float saturation;
    float brightness;
    int alpha;
    ofColor* color;
    
    int Window_Width;
    int Window_Height;
    int Sample_Rate;
    
    // Initialization growth things
    bool initialized = false;
    float initializationTimer;
    float initializationSpeed; // How quickly to grow
    float initializationWait; // How long to wait before growing
    
    float pulseSpeed; // Speed of a pulse (divide by two for opening)
    float pulseWait; // wait in between pulses in secs
    
    float timer; // Used to keep pulses on track
    
    int connectionState;
    
    synthShape* connectedParent;
    synthShape* overallConnectedParent;

    // Instrument stuff
    float frequency;
    float originalFrequency;
    
    synthShape (int x, int y, int Sample_Rate) {
        this->x = (float)x;
        this->y = (float)y;
        this->Window_Width = ofGetWindowWidth();
        this->Window_Height = ofGetWindowHeight();
        radius = 20;
        // Limit to non blue colors
        hue = (185.0 + ofRandom(100.0));
        if (hue > 255.0) {
            hue = hue - 255.0;
        }
        brightness = 255.0;
        saturation = ((ofRandom(64.0)) + 65.0);
        this->setRadius();
        setXYandUpdate(x, y);
        alpha = 128;
        color = new ofColor(0, 0, 0);
        color->setHsb(hue, saturation, brightness);
        
        
        
        pulseWait = 0.6 + ofRandom(0.3);
        pulseSpeed = 1.8 + ofRandom(0.6);
        
        initializationWait = 0.1;
        initializationSpeed = 0.2 + ofRandom(0.4);
        
        timer = 0.0;
        initializationTimer = 0.0;
        
        connectionState = 1;
        connectedParent = this;
        overallConnectedParent = this;
    }
    
    synthShape* getConnectedParent() {
        return this->connectedParent;
    }
    
    void setConnectedParent(synthShape* connectedParent) {
        this->connectedParent = connectedParent;
    }
    
    synthShape* getOverallConnectedParent() {
        return this->overallConnectedParent;
    }
    
    void setOverallConnectedParent(synthShape* overallConnectedParent) {
        this->overallConnectedParent = overallConnectedParent;
    }
    
    
    int getBrightness() {
        return this->brightness;
    }
    
    void setRadius() {
        this->radius = (15.0 + 30.0 * (1.0 - ((float)y) / ofGetHeight()));
    }
    
    int getX () {
        return this->x;
    }
    
    int getY () {
        return this->y;
    }
    
    int getRadius() {
        return this->radius;
    }
    
    void setRadius (int radius) {
        this->radius = radius;
    }
    
    int getConnectionState () {
        return this->connectionState;
    }
    
    void setConnectionState(int connectedState) {
        this->connectionState = connectedState;
    }
    
    void setX (int x) {
        this->x = x;
    }
    
    void setY (int y) {
        this->y = y;
    }
    
    int getIndex() {
        return this->index;
    }
    
    float getFrequency() {
        return this->frequency;
    }
    
    float getOriginalFrequency() {
        return this->originalFrequency;
    }
    
    virtual void setFrequency(float frequency) {
        this->frequency = frequency;
    }
    
    // tells us if the provided location is within the shape
    bool within (int x, int y) {
        // TODO make this better
        if (abs(x - this->x) < radius && abs(y - this->y) < radius)
            return true;
        return false;
    }
    
    void setXYandUpdate(int x, int y) {
        this->setX(x);
        this->setY(y);
        
    }
    
    // Each of these functions need to be implemented by the child
    virtual void initializeInstrument() { };
    
    // Draw the circle
    virtual void draw() { };
    
    virtual stk::StkFloat tick() { };
    
};

#endif /* synthShape_h */
