/* Refer to the README.md in the example's root folder for more information on usage */

#pragma once

#include "ofMain.h"

#include "imageAndVideo/imageAndVideo.hpp"
#include "camera/camera.hpp"
#include "metadata/metadataBuilder.hpp"

class ofApp : public ofBaseApp {

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ImageAndVideo imageAndVideo;
		Camera camera;
    MetadataBuilder metadataBuilder;

	private:
    
    bool metadataBuilded;
    string filenameMetadata;
};

