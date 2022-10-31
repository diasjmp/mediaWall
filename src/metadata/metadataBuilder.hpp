

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxCvHaarFinder.h"
#include "ofxCv.h"
#include <cmath>

// classe responsavel por gerar metadados e processamento de metadados
class MetadataBuilder {
    public:
    
    bool buildMetaData();
    void update();
    void setup(string imagesPath, string videosPath, string filenameMetadata);
    private:
    
    void createImageMetadata(string imagePath, string imageName);
    bool createVideoMetadata(string videoPath, string videoName);
    void addColorImageAndLuminance(ofImage image);
    bool processMeanVideoData(string name);
    void addKeyWords();
    void addNumFaces(int nFaces);
    int getNumFacesInImage(ofImage image);
    void getMeanColorAndLuminance(ofPixels & pixels, float *r, float *g, float *b, float *luminance);
    void addXMLColorLuminance(float luminance, int r, int g, int b);
    void buildImagesMetadata();
    void processVideos();
    void getPathAndName(ofDirectory dir, string *path, string *name, int i);
    void loadAndPlayVideo(string videoPath);
    bool doneProcessingVideo(ofVideoPlayer vidPlayer, int currFrame, int usedFrames);
    void processLuminanceAndColor(float*red, float*green, float*blue, float*luminance, int usedFrames);
    void processFaces(int *maxNumFaces);
    void addEdgeDistribution(cv::Mat greyImg);
    float getMeanFromMat(cv::Mat img);
    void addTexture(cv::Mat greyImg);
    void processWithFilters(ofPixels & pixels);
    void makeThumbnail(string name);
    
    ofxXmlSettings XML;
    
    ofDirectory dirImages;
    ofDirectory dirVideos;
    
    ofVideoPlayer currVideoPlayer;
    bool videoPlaying;
    
    bool imagesDone;
    bool videosDone;
    int currVideo;
    bool newVideo;
    
    int currFile;
    
    ofxCvHaarFinder finder;
    
    
    string FILENAME_METADATA;
};
