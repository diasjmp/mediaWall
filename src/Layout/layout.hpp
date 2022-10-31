#include "ofMain.h"
#include "ofxGui.h"

#ifndef DATA_CLASSES
#define DATA_CLASSES
#include "DataClasses.cpp"
#endif


class Layout {

public:

	void setup();
	void update();
	void draw(ofImage* image, ofVideoPlayer* video, vector<Thumbnail> newThumbnails);
	void playAndPause();

	int mousePressed(int x, int y);
	Clustering_type getTypecluster();

	ofxPanel gui;

	ofxToggle luminance;
	ofxToggle color;
	ofxToggle keywords;
	ofxToggle num_faces;
	ofxToggle edges;
	ofxToggle texture;
	ofxToggle tags;


private:

	ofImage* centerImage;
	ofVideoPlayer* centerVideo;
	vector<Thumbnail> thumbnails;

	int xButton, yButton;
	int btnWidth, btnHeight;

	float step;

	ofTrueTypeFont font;
	string backButtonText;

	bool insideRect(int x, int y, int rectx, int recty, int width, int height);
	void drawButton();

	void luminanceChecked(bool& b);
	void colorChecked(bool& b);
	void facesChecked(bool& b);
	void keywordsChecked(bool& b);
	void edgesChecked(bool& b);
	void textureChecked(bool& b);

	int findImagePressed(int x, int y);
    bool insideImage(int x, int y, int rectx, int recty, int width, int height);
    
};
