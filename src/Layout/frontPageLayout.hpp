#include "ofMain.h"

class FrontPageLayout {

public:
	void setup();
	void update();
	void draw(vector<ofImage> themes);

	int mousePressed(int x, int y);
	
private:

	int radius;
	int xButton, yButton;
	int btnWidth, btnHeight;

	int xBox, yBox;
	int boxWidth, boxHeight;

	ofTrueTypeFont font;
	string instructionsText;
	string shuffleText;

	vector<ofImage> themes;

	//this function checks if the passed arguments are inside the button.
	bool insideRect(int x, int y, int rectx, int recty, int width, int height);
	void drawButton();
	int insideImage(int x, int y);
	void drawTextBox();

};

