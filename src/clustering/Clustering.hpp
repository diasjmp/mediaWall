#include "ofMain.h"
#include "Metadata.hpp"

class Clustering {
    public:
    
    void setup();
    
    vector<ClusteringResult> getNByTags(int n, string filename);
    vector<ClusteringResult> getNByColor(int n, string filename);
    vector<ClusteringResult> getNByLuminance(int n, string filename);
    vector<ClusteringResult> getNByFaces(int n, string filename);
    vector<ClusteringResult> getNByEdges(int n, string filename);
    vector<ClusteringResult> getNByTexture(int n, string filename);
//    vector<string> getNByTextureVert(int n);
//    vector<string> getNByTextureHoriz(int n);
//    vector<string> getNByTextureDegree45(int n);
//    vector<string> getNByTextureDegree135(int n);
//    vector<string> getNByTextureNonDirectional(int n);
    
    private:
    int differenceTags(vector<string> a, vector<string> b);
    int differenceColors(Color *a, Color *b);
    float differenceEdges(Edges *a, Edges *b);
    
    Metadata metadata;
    
};
