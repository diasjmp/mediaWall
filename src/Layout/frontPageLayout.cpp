#include "frontPageLayout.hpp"

void FrontPageLayout::setup() {
	radius = 400;
	
	instructionsText = "Instructions:\n\nTo get the most out of the\nexperience please enter\nfullscreen mode.\n\nClick on a picture to start\nnavigating the gallery or\nthe shuffle button to\nload different themes\n\nTo filter images by tags\nuse the tags checkbox,\nselecting only one\nat a time.\n\n\You may come back to\nthis screen at any time\nby pressing the Go-Back\nbutton on the top\nlefthand corner\nof the navigation screen";

}

void FrontPageLayout::update() {
	
}

void FrontPageLayout::draw(vector<ofImage> newThemes) {
	themes = newThemes;
	ofPushMatrix();
		float step = (PI / 2);
		float angle = (PI / 4);
		int centerX = 0;
		int centerY = 0;

		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		for (int i = 0; i < 4; i++) {
			int width = themes[i].getWidth();
			int height = themes[i].getHeight();

			float factor = 1.0;
			int windowSize = min(ofGetWidth(), ofGetHeight());

			int _windowSize = windowSize;

			windowSize /= 2;
			windowSize -= radius / 2;

			if (width > windowSize) {
				factor = windowSize / (float)width;
			}

			width = (int)(factor * (float)width);
			height = (int)(factor * (float)height);

			radius = ((_windowSize / 2) - (height / 3));

			themes[i].resize(width, height);

			centerX = -width / 2 + cos(angle) * radius;
			centerY = -height / 2 + sin(angle) * radius;
			
			themes[i].draw(centerX, centerY);

			angle += step;
		}
		
		drawButton();
	ofPopMatrix();

	drawTextBox();

}

void FrontPageLayout::drawTextBox() {
	boxWidth = 182;
	boxHeight = 500;

	int windowSize = min(ofGetWidth(), ofGetHeight());
	float factor = 600.0 / windowSize;

	boxWidth = (int)((float)boxWidth * (1.0 / factor));
	boxHeight = (int)((float)boxHeight * (1.0 / factor));

	xBox = 30;
	yBox = (ofGetHeight() / 2) - boxHeight / 2;

	ofRectangle textBox(xBox, yBox, boxWidth, boxHeight);

	ofSetColor(ofColor::black, 90);
	ofDrawRectRounded(textBox, 75.0);

	font.load("arial.ttf", 16);
	ofSetColor(ofColor::white);
	font.drawString(instructionsText, boxWidth / 4.5 - xBox / 4.5, boxHeight / 4.5 - yBox / 4.5);
}

void FrontPageLayout::drawButton() {
	btnWidth = 150;
	btnHeight = 50;

	int windowSize = min(ofGetWidth(), ofGetHeight());
	float factor = 600.0 / windowSize;

	btnWidth = (int)((float)btnWidth * (1.0 / factor));
	btnHeight = (int)((float)btnHeight * (1.0 / factor));
	
	xButton = -btnWidth / 2;
	yButton = -btnHeight / 2;

	ofRectangle button(xButton, yButton, btnWidth, btnHeight);
	
	ofSetColor(ofColor::lightGoldenRodYellow);
	ofDrawRectangle(button);

	font.load("arial.ttf", 20);

	shuffleText = "Shuffle";
	ofSetColor(ofColor::black);
	font.drawString(shuffleText, xButton / 4 - font.stringWidth(shuffleText) / 4 + 10, 
		yButton / 4 - font.stringHeight(shuffleText) / 4 + 20);

}


//this function checks if the passed arguments are inside the button.
bool FrontPageLayout::insideRect(int x, int y, int rectx, int recty, int width, int height) {
	int _rectx = ofGetWidth() / 2 + rectx;
	int _recty = ofGetHeight() / 2 + recty;
	bool isInside;
	//cout << "rect" << _rectx << " " << _recty << endl;
	
	isInside = (x >= _rectx && x <= _rectx + width && y >= _recty && y <= _recty + height);

	return isInside;
}

int FrontPageLayout::mousePressed(int x, int y) {
	//cout << "mouse" << x << " " << y << endl;
	
	if (insideRect(x, y, xButton, yButton, btnWidth, btnHeight)) {
		cout << "clicked" << endl;
		return -1; // shuffle
	}
	return insideImage(x, y);
}

int FrontPageLayout::insideImage(int x, int y) {
	
	
	float step = (PI / 2);
	float angle = (PI / 4);
	int centerX = 0;
	int centerY = 0;

	for (int i = 0; i < 4; i++) {
		int width = themes[i].getWidth();
		int height = themes[i].getHeight();
		centerX = -width / 2 + cos(angle) * radius;
		centerY = -height / 2 + sin(angle) * radius;

		if (insideRect(x, y, centerX, centerY, width, height)) {
			return i;
		}
		angle += step;
		//cout << "for centerX: " << centerX << " " << "for centerY: " << centerY << endl;
	}
	return -2;

}
