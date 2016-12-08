# Shape Sea
Play with shapes and sounds in a sea of muted colors. Inspired by the [Reactable](http://reactable.com) and [Deeply](https://ccrma.stanford.edu/~jfarman/256a/final.html). Final project for CS476a/MUS256a at Stanford’s CCRMA, fall of 2016.

![Screenshot of the application](finalmilestone-screenshot.png)

Contact: hkini [at] stanford [dot] edu

## Installation
Make sure you have OpenFrameworks installed, along with ofxStk and ofxGLSLSimplexNoise.

1. Clone the repository.

2. Copy the rawwaves folder into bin/data of the project (see the ofxStk readme, on GitHub for more details).

3. Use the OpenFrameworks Project Generator to update the project to use ofxStk and ofxGLSLSimplexNoise, and optionally generate a project for your favorite IDE (if you're not using XCode on OSX Sierra.)

4. Compile.

5. Change the resolution and windowed mode in main.cpp, if desired. (It's set to 1280x720 and fullscreen, which is pretty conservative, but if you have a lower resolution, you may want to fix that.)

6. Run the program once - you may not hear any sound. If so, in the setup() method in ofApp.cpp, make sure you’re selecting the right audio output device. (Running the app once will print your output devices to the console.) Note - there is no sound synthesis yet, so you definitely won’t hear anything.

7. Enjoy! 

## Instructions

Clicking on the screen will create a circle, with an accompanying pad noise. Clicking while holding 's' or 't' on the keyboard will create a square or triangle, respectively. Holding 'c' will let you connect two shapes that aren't already connected. Pressing 'd' will delete something - a shape if your mouse is over a shape, a connection if your mouse is over that, or else a random shape. 'esc' will quit the app when you're done. 

Be sure to tweak the resolution and audio output (see the Installation section, above) if you're having trouble getting video or audio output. You can also tweak MAX_SHAPES (defined at the top of ofApp.h) if you'd like to play with more shapes (the current maximum is 15.)

## Acknowledgements

I used and modified the openFrameworks starter code provided by Tim and Romain for drawing and artwork, and used ofxStk for audio synthesis. The background was created with ofxGLSLSimplexNoise. I used additional smoothing code written by Romain. The alpha-motion-blurring technique came from a tutorial online as well. Thanks also to Jennifer Farman and the Reactable team for inspiration.