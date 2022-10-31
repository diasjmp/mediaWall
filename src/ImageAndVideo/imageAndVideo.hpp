#include "ofMain.h"
#include "layout.hpp"
#include "frontPageLayout.hpp"
#include "Clustering.hpp"

class ImageAndVideo {

    public:

        void setup();
        void update();
        void draw();
        void playAndPause();
        void mousePressed(int x, int y);

    private:

        ofDirectory dir;
        vector<MyImage> images;
        vector<MyImage> frontLayoutThumbnails;
        vector<Thumbnail> thumbnails;

        ofDirectory dirVideos;
        vector<ofVideoPlayer> videoPlayers;

        Layout layout;
        FrontPageLayout frontLayout;

        MyImage *centralImage;
        MyVideo *centralVideo;
    
        Clustering clustering;

        int nThumbnails;

        bool mainLayout;

        Clustering_type currentClusteringType;
        bool clusteringSetUp;
        bool currentIsVideo(int current, int nImages);
        void changeLayout(int action);
        void shuffleFrontLayout();
        ofImage createVideoThumbnail(ofVideoPlayer video);
        void loadThumbnails(string baseFile, Clustering_type clusteringType);
        vector<Thumbnail> loadFromFilenames(vector<ClusteringResult> files);
        vector<ofImage> fromMyImageToOfImage(vector<MyImage> images);
        void loadVideo(string name);

};
