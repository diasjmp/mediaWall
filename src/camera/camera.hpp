#include "ofMain.h"

class Camera {
    public:

        void setup(int width, int height);
        void update();
        void draw(int x, int y);
        int getCamWidth();
        int getCamHeight();

    private:

        ofVideoGrabber vidGrabber;
        int camWidth;
        int camHeight;
};
