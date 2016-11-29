//
//  synthCircle.h
//  cs476a-finalproject
//
//  Created by Hemanth Kini on 11/15/16.
//
//

#include "synthShape.h"

#ifndef synthCircle_h
#define synthCircle_h

class synthCircle : public synthShape {
private:
    
public:
    
    stk::BlowBotl instr;
    
    
    synthCircle (int x, int y, int Sample_Rate) :
    synthShape (x, y, Sample_Rate) {        
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
    
    // Draw the circle
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
        ofDrawCircle((float)this->getX(), (float)this->getY(), (float)this->getRadius());
        
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
        
        this->color->setHsb(hue, innerSaturation, brightness);
        ofSetColor(*color);
        ofDrawCircle((float)this->getX(), (float)this->getY(), innerRadius);
        this->color->setHsb(hue, saturation, brightness);
        
        radius = tempRadius;
    }
    
    stk::StkFloat tick() override {
        
        if (initialized) {
            return instr.tick() * ampl;
        }
    }
    
};
#endif /* synthCircle_h */
