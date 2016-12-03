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
    
    
    stk::ModalBar instr;
    bool barHitThisTime;
    
    synthSquare (int x, int y, int Sample_Rate) :
    synthShape (x, y, Sample_Rate) {
        instr.clear();
        barHitThisTime = false;
    }
    
    
    void initializeInstrument() override {
        this->frequency = 55.0 + ofRandom(54.0);
        this->originalFrequency = frequency;
        setFrequency(this->frequency);
        instr.setPreset(0); // Modulo 9 - can try different things from 0-8
    }
    
    void setFrequency(float frequency) override {
        this->frequency = frequency;
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
            barHitThisTime = false;
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
            innerRadius = (1 - (timerOffsetCenter / (pulseSpeed / 2))) * (0.5 * radius) + (0.5 * radius);
        }
        else {
            innerRadius = (0.5 * radius);
        }
        
        float innerLength = innerRadius * SQUARE_ROOT_TWO;
        float innerLeft = this->getX() - (innerLength / 2.0);
        float innerTop = this->getY() - (innerLength / 2.0);
        this->color->setHsb(hue, innerSaturation, brightness);
        ofSetColor(*color);
        ofDrawRectangle(innerLeft, innerTop, innerLength, innerLength);
        this->color->setHsb(hue, saturation, brightness);
        
        // Draw the rings, if necessary
        if (initialized && timer >= (pulseSpeed / 2) && timer < (pulseSpeed)) {
            // Strike the bell
            if (!barHitThisTime) {
                instr.strike(ampl);
                barHitThisTime = true;
            }
            float ringRadius = radius + (radius - innerRadius);
            float ringLength = ringRadius * SQUARE_ROOT_TWO;
            float ringLeft = this->getX() - (ringLength / 2.0);
            float ringTop = this->getY() - (ringLength / 2.0);
            
            // Go from 0.0 to 1.0 based on how close we are to the end of the pulse
            float ringState = (timer - (pulseSpeed / 2)) / (pulseSpeed);
            if (ringState > 1.0)
                ringState = 1.0;
            
            float ringSaturation = innerSaturation - (innerSaturation * ringState);
            
            this->color->setHsb(hue, ringSaturation, brightness);
            ofNoFill();
            ofSetColor(*color);
            ofSetLineWidth(1);
            ofDrawRectangle(ringLeft, ringTop, ringLength, ringLength);
            ofFill();
            this->color->setHsb(hue, saturation, brightness);
        }
        
        radius = tempRadius;
    }
    
    stk::StkFloat tick() override {
        
        if (initialized) {
            return instr.tick();
        }
    }
    
};

#endif /* synthSquare_h */
