#include "ofMain.h"

class Color {
public:
    int red, green, blue;
};

class Edges {
public:
    float vertical;
    float horizontal;
    float degree45;
    float degree135;
    float nonDirectional;
};

class MetadataDTO {
public:
    vector<string> tags;
    float luminance;
    Color color;
    int nFaces;
    Edges edges;
    float texture;
};

enum Clustering_type{
    LUMINANCE,
    COLOR,
    TAGS,
    FACES,
    EDGES,
    TEXTURE
};

class MyImage {
public:
    string name;
    ofImage img;
    int x;
    int y;
    int width;
    int height;
};

class MyVideo {
public:
    string name;
    ofVideoPlayer vid;
    int x;
    int y;
    int width;
    int height;
};

class MyVideoPlayer {
public:
    string name;
    ofVideoPlayer videoPlayer;
};

class ClusteringResult {
public:
    string name;
    float similarity;
};

class Thumbnail : public ClusteringResult {
public:
    ofImage img;
    int x;
    int y;
    int width;
    int height;
};

