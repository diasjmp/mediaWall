#include "camera.hpp"

void Camera::setup(int width, int height) {
    camWidth = width;  // try to grab at this size.
    camHeight = height;

    vector<ofVideoDevice> devices = vidGrabber.listDevices();

    vidGrabber.setDeviceID(0);
    vidGrabber.setDesiredFrameRate(30);
    vidGrabber.initGrabber(camWidth, camHeight);
}

void Camera::update() {
    vidGrabber.update();
}

void Camera::draw(int x, int y) {
    ofSetHexColor(0xffffff);
    vidGrabber.draw(x, y);
}

int Camera::getCamWidth() {
    return camWidth;
}

int Camera::getCamHeight() {
    return camHeight;
}

