#include "Clustering.hpp"

void Clustering::setup() {
    metadata.setup("metadata.xml");
}

int Clustering::differenceTags(vector<string> a, vector<string> b) {
    int sum = 0;
    for (string s1 : a) {
        for (string s2: b) {
            if (s1 == s2) {
                sum++;
            }
        }
    }
    return sum;
}

int Clustering::differenceColors(Color * a, Color * b) {
    return abs(a->red - b->red) +
        abs(a->green - b->green) +
        abs(a->blue - b->blue);
}

float Clustering::differenceEdges(Edges *a, Edges *b) {
    return abs(a->vertical - b->vertical) +
        abs(a->horizontal - b->horizontal) +
        abs(a->degree45 - b->degree45) +
        abs(a->degree135 - b->degree135) +
        abs(a->nonDirectional - b->nonDirectional);
}

template <typename T>
bool cmp(const pair<ClusteringResult, T>& a, const pair<ClusteringResult, T>& b) {
    return a.second < b.second;
}

vector<ClusteringResult> Clustering::getNByTags(int n, string filename) {
    map<string, vector<string>> tags = metadata.getAllTags();
    vector<pair<ClusteringResult, int>> sortedFiles;
    
    vector<string> tagsBase = tags.at(filename);
    tags.erase(filename);
    
    int max = 0;
    
    for (pair<string, vector<string>> pair : tags) {
        int diff = differenceTags(pair.second, tagsBase);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, int> pair : sortedFiles) {
        pair.first.similarity = 1.0 - ((float)pair.second / (float)max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<int>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=n-1; i>=0; i--) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
}

vector<ClusteringResult> Clustering::getNByColor(int n, string filename) {
    map<string, Color> colors = metadata.getAllColor();
    vector<pair<ClusteringResult, int>> sortedFiles;
    
    Color baseColor = colors.at(filename);
    colors.erase(filename);
    
    int max = 0;
    
    for (pair<string, Color> pair : colors) {
        int diff = differenceColors(&baseColor, &pair.second);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, int> pair : sortedFiles) {
        pair.first.similarity = 1.0 - ((float)pair.second / (float)max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<int>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=0; i<n; i++) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
}

vector<ClusteringResult> Clustering::getNByLuminance(int n, string filename) {
    map<string, float> luminances = metadata.getAllLuminance();
    vector<pair<ClusteringResult, float>> sortedFiles;
    
    float luminanceBase = luminances.at(filename);
    luminances.erase(filename);
    
    float max = 0.0;
    
    for (pair<string, float> pair : luminances) {
        float diff = abs(luminanceBase - pair.second);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, float> pair : sortedFiles) {
        pair.first.similarity = 1.0 - (pair.second / max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<float>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=0; i<n; i++) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
}

vector<ClusteringResult> Clustering::getNByFaces(int n, string filename) {
    map<string, int> faces = metadata.getAllFaces();
    vector<pair<ClusteringResult, int>> sortedFiles;
    
    int nFacesBase = faces.at(filename);
    faces.erase(filename);
    
    int max = 0;
    
    for (pair<string, int> pair : faces) {
        int diff = abs(nFacesBase - pair.second);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, int> pair : sortedFiles) {
        pair.first.similarity = 1.0 - ((float)pair.second / (float)max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<int>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=0; i<n; i++) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
}

vector<ClusteringResult> Clustering::getNByEdges(int n, string filename) {
    map<string, Edges> edges = metadata.getAllEdges();
    vector<pair<ClusteringResult, float>> sortedFiles;
    
    Edges baseEdges = edges.at(filename);
    edges.erase(filename);
    
    float max = 0.0;
    
    for (pair<string, Edges> pair : edges) {
        float diff = differenceEdges(&baseEdges, &pair.second);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, float> pair : sortedFiles) {
        pair.first.similarity = 1.0 - (pair.second / max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<float>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=0; i<n; i++) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
    
}

vector<ClusteringResult> Clustering::getNByTexture(int n, string filename) {
    map<string, float> textures = metadata.getAllTexture();
    vector<pair<ClusteringResult, float>> sortedFiles;
    
    float texturesBase = textures.at(filename);
    textures.erase(filename);
    
    float max = 0.0;
    
    for (pair<string, float> pair : textures) {
        float diff = abs(texturesBase - pair.second);
        ClusteringResult clust;
        clust.name = pair.first;
        sortedFiles.push_back({clust, diff});
        if (diff > max)
            max = diff;
    }
    
    for (pair<ClusteringResult, float> pair : sortedFiles) {
        pair.first.similarity = 1.0 - (pair.second / max);
    }
    
    sort(sortedFiles.begin(), sortedFiles.end(), cmp<float>);
    
    if (sortedFiles.size() < n) {
        n = sortedFiles.size();
    }
    
    vector<ClusteringResult> result;
    for (int i=0; i<n; i++) {
        result.push_back(sortedFiles[i].first);
    }
    
    return result;
}



//vector<string> Clustering::getNByTextureVert(int n) {
//
//}
//
//vector<string> Clustering::getNByTextureHoriz(int n) {
//
//}
//
//vector<string> Clustering::getNByTextureDegree45(int n) {
//
//}
//
//vector<string> Clustering::getNByTextureDegree135(int n) {
//
//}
//
//vector<string> Clustering::getNByTextureNonDirectional(int n) {
//
//}
