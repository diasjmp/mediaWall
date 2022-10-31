#include "layout.hpp"

void Layout::setup() {
	centerImage = NULL;
	centerVideo = NULL;

	step = 0;

	btnWidth = 200;
	btnHeight = 70;
	xButton = -btnWidth / 2;
	yButton = -btnHeight / 2;

	if (centerVideo != NULL)
		(*centerVideo).play();

	font.load("arial.ttf", 16);
	backButtonText = "Go Back";

	gui.setup("Tags");

	gui.add(luminance.setup("Luminance", false));
	gui.add(color.setup("Color", true));
	gui.add(num_faces.setup("Num_faces", false));
	gui.add(edges.setup("Edges", false));
	gui.add(texture.setup("Texture", false));
	gui.add(keywords.setup("Keywords", false));

	luminance.addListener(this, &Layout::luminanceChecked);
	color.addListener(this, &Layout::colorChecked);
	num_faces.addListener(this, &Layout::facesChecked);
	edges.addListener(this, &Layout::edgesChecked);
	texture.addListener(this, &Layout::textureChecked);
	keywords.addListener(this, &Layout::keywordsChecked);


}

void Layout::update() {
	if (centerVideo != NULL)
		(*centerVideo).update();
}


void Layout::draw(ofImage* image, ofVideoPlayer* video, vector<Thumbnail> newThumbnails) {
	centerImage = image;
	centerVideo = video;
	thumbnails = newThumbnails;
	step = (2 * PI) / (newThumbnails.size());
	int centerX = 0;
	int centerY = 0;

	ofPushMatrix();
		float theta = 0;
		ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);
		for (int i = 0; i < (int)thumbnails.size(); i++) {
			int width = thumbnails[i].img.getWidth();
			int height = thumbnails[i].img.getHeight();
            
            float similarity = newThumbnails[i].similarity;
            
			float factor = 1.0;
			int windowSize = min(ofGetWidth(), ofGetHeight());
			windowSize /= 5;
			if (width > windowSize) {
				factor = windowSize / (float)width;
			}

			width = ((int)(factor * (float)width) * similarity);
			height = ((int)(factor * (float)height) * similarity);

			int radius = 200 + ((1-similarity) * 350);
            if (i % 2 == 0) {
                radius -= 30;
            }
			if (radius > ofGetHeight() / 2) {
				radius = (ofGetHeight() / 2) - (height / 2) - 5;
			}

			thumbnails[i].img.resize(width, height);

			float a = 1.0;
			float b = 1.0;

			float ratio = (float)ofGetWidth() / (float)ofGetHeight();
			//cout << "ratio out of ifs: " << " " << ratio << endl;
	
			if (ratio > 1.0) {
				// achatado / flattened
				a = ratio;
				b = 1.0;
			}
			else {
				a = ratio;
				b = 1.0 / ratio;
			}

			centerX = -width / 2 + a * cos(theta) * radius;
			centerY = -height / 2 + b * sin(theta) * radius;

			thumbnails[i].x = centerX;
			thumbnails[i].y = centerY;
			thumbnails[i].width = width;
			thumbnails[i].height = height;

			thumbnails[i].img.draw(centerX, centerY);

			theta += step;
		}
		// Starting point of an image/video is in the top left hand corner. To center it subtract by getWidth/Height divided by 2
		if (centerImage != NULL) {
			ofImage display; // copy of image
			display.allocate(centerImage->getWidth(), centerImage->getHeight(), OF_IMAGE_COLOR);
			display.setFromPixels(centerImage->getPixels());

			int windowSize = min(ofGetWidth(), ofGetHeight());
			float factor = 800.0 / windowSize;

			int width = display.getWidth();
			int height = display.getHeight();
			
			width = (int)((float)width * (1.0 / factor));
			height = (int)((float)height * (1.0 / factor));
			
			display.resize(width, height);
			display.draw(-width / 2, -height / 2);
		}
		else if (centerVideo != NULL) {
			centerVideo->draw(-centerVideo->getWidth() / 2, -centerVideo->getHeight() / 2);
		}
	ofPopMatrix();

	drawButton();
	gui.draw();
	
}


void Layout::playAndPause() {
	bool setPaused = true;
	if(centerVideo != NULL){ // Added condition to prevent this behaviour with images.
		if ((*centerVideo).isPaused()) {
			setPaused = false;
		}
		(*centerVideo).setPaused(setPaused);
	}
}

void Layout::drawButton() {
	ofRectangle back_button(xButton, yButton, btnWidth, btnHeight);

	ofSetColor(ofColor::darkRed);
	ofDrawRectangle(back_button);

	ofSetColor(ofColor::white);
	font.drawString(backButtonText, (xButton / 2 + btnWidth / 2) + font.stringWidth(backButtonText) / 2 - 85,
		(yButton / 2 + btnHeight / 2) + font.stringHeight(backButtonText) / 2);
}

bool Layout::insideRect(int x, int y, int rectx, int recty, int width, int height) {
	
	int _rectx = btnWidth / 2 + rectx;
	int _recty = btnHeight / 2 + recty;
	bool isInside;

	isInside = (x >= _rectx && x <= _rectx + width && y >= _recty && y <= _recty + height);
	
	return isInside;
}

//this function checks if the passed arguments are inside the button.
bool Layout::insideImage(int x, int y, int rectx, int recty, int width, int height) {
    int _rectx = ofGetWidth() / 2 + rectx;
    int _recty = ofGetHeight() / 2 + recty;
    bool isInside;
    //cout << "rect" << _rectx << " " << _recty << endl;
    
    isInside = (x >= _rectx && x <= _rectx + width && y >= _recty && y <= _recty + height);

    return isInside;
}

int Layout::mousePressed(int x, int y) {
	int result = -2;
	cout << "back_mouse" << x << " " << y << endl;

	if (insideRect(x, y, xButton, yButton, btnWidth, btnHeight)) {
		cout << "clicked" << endl;
		return -1; // change layout
	}

	result = findImagePressed(x, y);
    cout << "Pressed image: " << result << endl;
	return result;
}

int Layout::findImagePressed(int x, int y) {
    for (int i = 0; i < thumbnails.size(); i++) {
        Thumbnail img = thumbnails[i];
        cout << "imgX: " << img.x << " imgY: " << img.y << endl;
        if (insideImage(x, y, img.x, img.y, img.width, img.height)) {
            return i;
        }
    }

	return -2;
}

Clustering_type Layout::getTypecluster() {
	if (luminance) {
		return LUMINANCE;
	}
	else if (color) {
		return COLOR;
	}
	else if (num_faces) {
		return FACES;
	}
	else if (edges) {
		return EDGES;
	}
	else if (texture) {
		return TEXTURE;
	}
	else if (keywords) {
		return TAGS;
	}
	return COLOR;
}

void Layout::luminanceChecked(bool& b) {
	if (b) {
		color = false;
		num_faces = false;
		keywords = false;
		edges = false;
		texture = false;
	}
}
void Layout::colorChecked(bool& b) {
	if (b) {
		luminance = false;
		num_faces = false;
		keywords = false;
		edges = false;
		texture = false;
	}
}
void Layout::facesChecked(bool& b) {
	if (b) {
		luminance = false;
		color = false;
		keywords = false;
		edges = false;
		texture = false;
	}
}
void Layout::keywordsChecked(bool& b) {
	if (b) {
		luminance = false;
		color = false;
		num_faces = false;
		edges = false;
		texture = false;
	}
}
void Layout::edgesChecked(bool& b) {
	if (b) {
		luminance = false;
		color = false;
		num_faces = false;
		keywords = false;
		texture = false;
	}
}
void Layout::textureChecked(bool& b) {
	if (b) {
		luminance = false;
		color = false;
		num_faces = false;
		keywords = false;
		edges = false;
	}
}

