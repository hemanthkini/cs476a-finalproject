#pragma once

#include "ofMain.h"
#include "ofxStk.h"
#include "synthCircle.h"
#include "synthSquare.h"
#include "synthTriangle.h"

#define MAX_SHAPES 15

#define BUFFER_SIZE 512
#define VIS_CIRCLES 15

class ofApp : public ofBaseApp{
    private:
        int num_shapes;
        vector<synthShape *> shapeVector;
        vector<vector<synthShape *> *> connectionVector;
        vector<synthShape *> *currentConnection;
        int rolling_ptr;
    
        float angle;
    
        // When c is pressed
        bool connectionState;
    
        // When s is pressed
        bool createSquare;
    
        // When t is pressed
        bool createTriangle;
    
        bool mouseDown;
        int firstMouseX;
        int firstMouseY;
        int secondMouseX;
        int secondMouseY;
    
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
    
        stk::NRev reverb;
        stk::OnePole highpassLeft;
        stk::OnePole highpassRight;
    
        // Background color
    
        ofColor backgroundColor;
        float backgroundHue;
        float backgroundSaturation;
        float backgroundBrightness;
        
        
        // Vectors for our waveform
        vector<float> audio;
        vector<float> bufferedAudio;
        vector<vector<float>> lol;
    
        // Lock the circleVector between UI & Audio threads
        ofMutex circleMutex;
    
        // Deletion link animation
        bool failedConnectionFlag;
        int failedNodeOneX;
        int failedNodeOneY;
        int failedNodeTwoX;
        int failedNodeTwoY;
    
        // FFT, mutex, and bins to hold stuff
        /*ofxFft* fft;
        
        ofMutex fftMutex;
        vector<float> drawBins, middleBins, audioBins; */
    
    
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
        void deleteConnections(synthShape * cir);
        bool existsConnection(synthShape* one, synthShape* two);
        bool mouseWithinConnection(int x1, int y1, int x2, int y2,
                                   int pointX, int pointY);
};
