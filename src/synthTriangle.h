//
//  synthTriangle.h
//  cs476a-finalproject
//
//  Created by Hemanth Kini on 11/28/16.
//
//

#include "synthShape.h"
#include "ofxStk.h"

#ifndef synthTriangle_h
#define synthTriangle_h

#define SQUARE_ROOT_TWO 1.4142135623731
#define SQUARE_ROOT_THREE 1.7320508075688

class synthTriangle : public synthShape {
private:
    
public:
    
    
    stk::Mandolin instr;
    bool barHitThisTime;
    
    synthTriangle (int x, int y, int Sample_Rate) :
    synthShape (x, y, Sample_Rate), instr(55.0) {
        instr.clear();
        barHitThisTime = false;
    }
    
    
    void initializeInstrument() override {
        this->frequency = 110.0 + ofRandom(109.0);
        this->originalFrequency = frequency;
        setFrequency(this->frequency);
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
            barHitThisTime = false;
        }
        
        timer = timer + (1.0 / ofGetFrameRate());
        ofSetColor(*color);
        float length = radius;
        float leftX = this->getX() - (length * SQUARE_ROOT_THREE / 2);
        float leftY = this->getY() + (length / 2);
        float rightX = this->getX() + (length * SQUARE_ROOT_THREE / 2);
        float rightY = this->getY() + (length / 2);
        float topX = this->getX();
        float topY = this->getY() - length;
        ofDrawTriangle(leftX, leftY, rightX, rightY, topX, topY);
        
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
        
        length = innerRadius;
        leftX = this->getX() - (length * SQUARE_ROOT_THREE / 2);
        leftY = this->getY() + (length / 2);
        rightX = this->getX() + (length * SQUARE_ROOT_THREE / 2);
        rightY = this->getY() + (length / 2);
        topX = this->getX();
        topY = this->getY() - length;
        this->color->setHsb(hue, innerSaturation, brightness);
        ofSetColor(*color);
        ofDrawTriangle(leftX, leftY, rightX, rightY, topX, topY);
        this->color->setHsb(hue, saturation, brightness);
        
        // Draw the rings, if necessary
        if (initialized && timer >= (pulseSpeed / 2) && timer < (pulseSpeed)) {
            // Strike the bell
            if (!barHitThisTime) {
                instr.pluck(ampl);
                barHitThisTime = true;
            }
            float ringRadius = radius + (radius - innerRadius);
            length = ringRadius;
            leftX = this->getX() - (length * SQUARE_ROOT_THREE / 2);
            leftY = this->getY() + (length / 2);
            rightX = this->getX() + (length * SQUARE_ROOT_THREE / 2);
            rightY = this->getY() + (length / 2);
            topX = this->getX();
            topY = this->getY() - length;
            
            // Go from 0.0 to 1.0 based on how close we are to the end of the pulse
            float ringState = (timer - (pulseSpeed / 2)) / (pulseSpeed);
            if (ringState > 1.0)
                ringState = 1.0;
            
            float ringSaturation = innerSaturation - (innerSaturation * ringState);
            
            this->color->setHsb(hue, ringSaturation, brightness);
            ofNoFill();
            ofSetColor(*color);
            ofSetLineWidth(1);
            ofDrawTriangle(leftX, leftY, rightX, rightY, topX, topY);
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



#endif /* synthTriangle_h */
