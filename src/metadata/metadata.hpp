#include "ofMain.h"
#include "ofxXmlSettings.h"
#ifndef DATA_CLASSES
#define DATA_CLASSES
#include "DataClasses.cpp";
#endif

class Metadata {
    public:
    
    void setup(string filenameMetadata);
    MetadataDTO getMetadataFromFile(string filename);
    map<string,vector<string>> getAllTags();
    map<string, float> getAllLuminance();
    map<string, Color> getAllColor();
    map<string, int> getAllFaces();
    map<string, Edges> getAllEdges();
    map<string, float> getAllTexture();
    vector<MetadataDTO> getAllMetadata();
    
    private:
    
    MetadataDTO extractMetadata();
    float extractLuminance();
    Color extractColor();
    vector<string> extractTags();
    int extractFaces();
    Edges extractEdges();
    float extractTexture();
    string extractFilename();
    
    ofxXmlSettings XML;
};
