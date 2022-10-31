#include "ofMain.h"
#include "ofApp.h"

//========================================================================
// Get this project 'gallery_try' to display the images and videos in a kind of grid
int main() {

	//ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(1440, 900, OF_WINDOW);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}