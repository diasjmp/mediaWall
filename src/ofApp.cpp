#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    filenameMetadata = "metadata.xml";
    metadataBuilded = false;
    metadataBuilder.setup("images/of_logos/", "movies/", filenameMetadata);

    imageAndVideo.setup();

    //camera.setup(160, 120); // problems with this, it gets a bigger picture, needs to extract pictures and draw a smaller subset of it
    camera.setup(320, 240);

    ofBackground(ofColor::cadetBlue);
}

//--------------------------------------------------------------
void ofApp::update() {
    if (!metadataBuilded) {
        metadataBuilder.update();
        return;
    }
    imageAndVideo.update();
    camera.update();
}

//--------------------------------------------------------------
void ofApp::draw() {
    if (!metadataBuilded) {
// TODO: Retirar os toques no ecra por equanto esta a fazer build!!
        ofClear(0);
        ofDrawBitmapString("Building Metadata Files..", 100, 100);
        ofDrawBitmapString("Please do not close this window", 100, 150);
        metadataBuilded = metadataBuilder.buildMetaData();
        return;
    }
    
    imageAndVideo.draw();

    camera.draw(ofGetWidth() * 2 / 3 + 250, ofGetHeight() * 2 / 3 + 50);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'p') {
        imageAndVideo.playAndPause();
    }
    if (key == 'f') {
        ofToggleFullscreen();
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (button == 0) { // 0 is Left mouse button
        imageAndVideo.mousePressed(x, y);
        
    }
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
