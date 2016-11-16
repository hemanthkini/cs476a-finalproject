#pragma once

#include "ofMain.h"
#include "ofxStk.h"
#include "synthCircle.h"

#define MAX_CIRCLES 15

class ofApp : public ofBaseApp{
    private:
        int num_circles;
        vector<synthCircle *> circleVector;
        int rolling_ptr;
    
        // When c is pressed
        bool connectionState;
    
        // For selected circle
        int selected_index;
        int selected_x;
        int selected_y;
        
        // audio stuff
        int	sampleRate, nInputChans;
        int bufferSize;
        float volume;
        int fileNumChannels;
        bool fileLoaded;
        ofSoundStream soundStream;
    
        // Background color
    
        ofColor backgroundColor;
        float backgroundHue;
        float backgroundSaturation;
        float backgroundBrightness;
        
        
        // Vectors for our waveform
        vector<float> audio;
        vector<float> bufferedAudio;
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        void audioOut(float * input, int bufferSize, int nChannels);
};
