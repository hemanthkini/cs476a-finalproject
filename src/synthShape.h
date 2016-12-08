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
    float originalRadius;
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
    float originalPulseSpeed;
    float originalPulseWait;
    
    float timer; // Used to keep pulses on track
    
    int connectionState;
    
    synthShape* connectedParent;
    synthShape* overallConnectedParent;

    set<synthShape *> connectionSet;
    
    // Instrument stuff
    float frequency;
    float originalFrequency;
    
    float ampl;
    
    // Set up overall shape design.
    synthShape (int x, int y, int Sample_Rate) {
        this->x = (float)x;
        this->y = (float)y;
        this->Window_Width = ofGetWindowWidth();
        this->Window_Height = ofGetWindowHeight();
        radius = 20;
        originalRadius = 20;
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
        
        // Set up pulsing.
        pulseWait = 0.6 + ofRandom(0.3);
        pulseSpeed = 1.8 + ofRandom(0.6);
        originalPulseSpeed = pulseSpeed;
        originalPulseWait = pulseWait;
        
        // Set up initial initialization growth.
        initializationWait = 0.1;
        initializationSpeed = 0.2 + ofRandom(0.4);
        
        timer = 0.0;
        initializationTimer = 0.0;
        
        connectionState = 1;
        connectedParent = this;
        overallConnectedParent = this;
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
    
    
    int getConnectionState () {
        return this->connectionState;
    }
    
    void setConnectionState(int connectedState) {
        this->connectionState = connectedState;
    }
    
    // Connect a shape to this shape, and set that one as this one's parent.
    void connectAndParent(synthShape* parent) {
        // Set parent and overall parent pointers, and insert into set
        this->connectionSet.insert(parent);
        this->setConnectedParent(parent);
        this->setOverallConnectedParent(parent->getOverallConnectedParent());
        
        // Set the new frequency, pulsespeed/wait and state as necessary.
        int newConnectionState = parent->getConnectionState() + 1;
        this->setConnectionState(newConnectionState);
        float newFrequency = getOverallConnectedParent()->getFrequency() * (float)newConnectionState;
        this->setFrequency(newFrequency);
        
        this->setPulseSpeed(getOverallConnectedParent()->getPulseSpeed());
        this->setPulseWait(getOverallConnectedParent()->getPulseWait());
        
        
        // Update all children to treat me as the parent
        std::set<synthShape *>::iterator it;
        for (it = this->connectionSet.begin(); it != this->connectionSet.end(); ++it) {
            if (*it != parent) {
                (*it)->connectAndParent(this);
            }
        }
    }
    
    // Connect a shape to this shape.
    void connect(synthShape* child) {
        this->connectionSet.insert(child);
    }
    
    // Delete my connection to this child
    void deleteConnection(synthShape* child) {
        this->connectionSet.erase(child);
        
        if (child == this->getConnectedParent()) {
            // If the thing we're deleting is our parent, we set ourselves as the parent
            this->setConnectedParent(this);
            this->setOverallConnectedParent(this);
            this->setConnectionState(1);
            this->setFrequency(this->originalFrequency);
            this->setPulseSpeed(this->originalPulseSpeed);
            this->setPulseWait(this->originalPulseWait);
            
            // Notify all children that we are the parent.
            std::set<synthShape *>::iterator it;
            for (it = this->connectionSet.begin(); it != this->connectionSet.end(); ++it) {
                (*it)->connectAndParent(this);
            }
        }
    }
    
    void deleteAllConnections() {
        
        // Delete our connection from all of our neighbors
        std::set<synthShape *>::iterator it;
        for (it = this->connectionSet.begin(); it != this->connectionSet.end(); ++it) {
            (*it)->deleteConnection(this);
        }
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
    
    float getPulseSpeed() {
        return this->pulseSpeed;
    }
    
    void setPulseSpeed(float pulseSpeed) {
        this->pulseSpeed = pulseSpeed;
    }
    
    float getPulseWait() {
        return this->pulseWait;
    }
    
    void setPulseWait(float pulseWait) {
        this->pulseWait = pulseWait;
    }
    
    int getRadius() {
        return this->radius;
    }
    
    void setRadius (int radius) {
        this->radius = radius;
    }

    void setX (int x) {
        this->x = x;
    }
    
    void setY (int y) {
        this->y = y;
        updateAmpl();
        this->setRadius();
    }
    
    int getIndex() {
        return this->index;
    }

    
    // tells us if the provided location is within the shape
    bool within (int x, int y) {
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
    
    // Draw the shape
    virtual void draw() { };
    
    // Compute one sample of audio
    virtual stk::StkFloat tick() { };
    
    // Update the amplitude, based on the distance from the top of the page
    virtual void updateAmpl() {
        float decimal_downward = ((float)y / (float)Window_Height);
        decimal_downward = (decimal_downward < 0.1 ? 0 : decimal_downward);
        
        ampl = 1.0 - (0.6 * decimal_downward);
    };
    
};

#endif /* synthShape_h */
