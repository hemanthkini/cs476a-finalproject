//
//  synthSquare.h
//  cs476a-finalproject
//
//  Created by Hemanth Kini on 11/27/16.
//
//

#include "synthShape.h"
#include "ofxStk.h"

#ifndef synthSquare_h
#define synthSquare_h

#define SQUARE_ROOT_TWO 1.4142135623731

class synthSquare : public synthShape {
private:
    
public:
    
    
    stk::Flute instr;
    
    synthSquare (int x, int y, int Sample_Rate) :
    synthShape (x, y, Sample_Rate), instr(220) {
        instr.clear();
        
        
    }
    
    
    void initializeInstrument() override {
        this->frequency = 220.0 + ofRandom(219.0);
        this->originalFrequency = frequency;
        instr.noteOn(this->frequency, 1.00);
    }
    
    void setFrequency(float frequency) override {
        this->frequency = frequency;
        ofLog(OF_LOG_NOTICE, "new frequency is: %f", frequency);
        instr.setFrequency(frequency);
    }
    
    // Draw the square
    void draw() override {
        // store the actual radius temporarily, in case we replace while initializing
        float tempRadius = radius;
        if (!initialized) {
            initializationTimer = initializationTimer + (1.0 / ofGetFrameRate());
            if (initializationTimer > initializationWait + initializationSpeed) {
                initialized = true;
                initializeInstrument();
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
        float length = radius * SQUARE_ROOT_TWO;
        float left = this->getX() - (length / 2.0);
        float top = this->getY() - (length / 2.0);
        ofDrawRectangle(left, top, length, length);
        
        float innerRadius;
        float innerSaturation = this->saturation / 2.0;
        
        // Have it glow
        if (timer > 0.0 && timer < (pulseSpeed)) {
            float timerOffsetCenter = abs(timer - (pulseSpeed / 2));
            innerRadius = (1 - (timerOffsetCenter / (pulseSpeed / 2))) * (0.5 * radius) + (0.4 * radius);
            
        }
        else {
            innerRadius = (0.4 * radius);
        }
        
        float innerLength = innerRadius * SQUARE_ROOT_TWO;
        float innerLeft = this->getX() - (innerLength / 2.0);
        float innerTop = this->getY() - (innerLength / 2.0);
        this->color->setHsb(hue, innerSaturation, brightness);
        ofSetColor(*color);
        ofDrawRectangle(innerLeft, innerTop, innerLength, innerLength);
        this->color->setHsb(hue, saturation, brightness);
        
        radius = tempRadius;
    }
    
    stk::StkFloat tick() override {
        
        if (initialized) {
            //ofLog(OF_LOG_NOTICE, "%f", ampModifier);
            return instr.tick();
        }
    }
    
};

#endif /* synthSquare_h */
