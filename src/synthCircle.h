//
//  synthCircle.h
//  cs476a-finalproject
//
//  Created by Hemanth Kini on 11/15/16.
//
//

#ifndef synthCircle_h
#define synthCircle_h

class synthCircle : public ofBaseApp{
private:
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
    bool initialized;
    float initializationTimer;
    float initializationSpeed; // How quickly to grow
    float initializationWait; // How long to wait before growing
    
    float pulseSpeed; // Speed of a pulse (divide by two for opening)
    float pulseWait; // wait in between pulses in secs
    
    float timer; // Used to keep pulses on track
    
public:
    synthCircle (int x, int y, int Sample_Rate) {
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
        
        initialized = false;
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
    
    
    void setX (int x) {
        this->x = x;
    }
    
    void setY (int y) {
        this->y = y;
    }
    
    int getIndex() {
        return this->index;
    }
    
    
    // tells us if the provided location is within the circle
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
    
    // Draw the circle
    void draw() {
        // store the actual radius temporarily, in case we replace while initializing
        float tempRadius = radius;
        if (!initialized) {
            initializationTimer = initializationTimer + (1.0 / ofGetFrameRate());
            if (initializationTimer > initializationWait + initializationSpeed) {
                initialized = true;
            }
            else if (initializationTimer > initializationWait) {
                radius = ((initializationTimer - initializationWait) / initializationSpeed) * tempRadius;
            } else {
                radius = 0.0;
            }
        }
        
        while (timer > (pulseWait + pulseSpeed)) {
            timer = timer - (pulseWait + pulseSpeed);
        }
        timer = timer + (1.0 / ofGetFrameRate());
        ofSetColor(*color);
        ofDrawCircle((float)this->getX(), (float)this->getY(), (float)this->getRadius());
        
        float innerRadius;
        float innerSaturation = this->saturation / 2.0;
        if (timer > 0.0 && timer < (pulseSpeed)) {
            float timerOffsetCenter = abs(timer - (pulseSpeed / 2));
            innerRadius = (1 - (timerOffsetCenter / (pulseSpeed / 2))) * (0.5 * radius) + (0.4 * radius);
            
        }
        else {
            innerRadius = (0.4 * radius);
        }
        
        this->color->setHsb(hue, innerSaturation, brightness);
        ofSetColor(*color);
        ofDrawCircle((float)this->getX(), (float)this->getY(), innerRadius);
        this->color->setHsb(hue, saturation, brightness);
        
        radius = tempRadius;
    }
    
};
#endif /* synthCircle_h */
