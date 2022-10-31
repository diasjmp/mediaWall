#include "Metadata.hpp"

void Metadata::setup(string filenameMetadata) {
    if (!XML.loadFile(filenameMetadata)) {
        throw std::invalid_argument("Did not find metadata file");
    }
}

Color Metadata::extractColor() {
    XML.pushTag("color");
    Color color = Color();
    color.red = XML.getValue("RED", -1);
    color.green = XML.getValue("GREEN", -1);
    color.blue = XML.getValue("BLUE", -1);
    XML.popTag();
    
    return color;
}

float Metadata::extractLuminance() {
    return XML.getValue("luminance", -1.0);
}

vector<string> Metadata::extractTags() {
    vector<string> tags;
    XML.pushTag("keyWords");
    for (int i=0; i < XML.getNumTags("keyWord"); i++) {
        tags.push_back(XML.getValue("keyWord", "", i));
    }
    XML.popTag();
    
    return tags;
    
}

MetadataDTO Metadata::extractMetadata() {
//    TODO: Falta acabar este!
    MetadataDTO metadata = MetadataDTO();
    metadata.luminance = extractLuminance();
    metadata.color = extractColor();
    metadata.tags = extractTags();
    
    return metadata;
}

int Metadata::extractFaces() {
    return XML.getValue("NUM_FACES", -1);
}

Edges Metadata::extractEdges() {
    Edges edges;
    XML.pushTag("EDGES");
    edges.vertical = XML.getValue("VERTICAL", -1.0);
    edges.horizontal = XML.getValue("HORIZONTAL", -1.0);
    edges.degree45 = XML.getValue("DEGREE45", -1.0);
    edges.degree135 = XML.getValue("DEGREE135", -1.0);
    edges.nonDirectional = XML.getValue("NONDIRECTIONAL", -1.0);
    XML.popTag();
    
    return edges;
}

float Metadata::extractTexture() {
    return XML.getValue("TEXTURE", -1.0);
}

string Metadata::extractFilename() {
    return XML.getValue("FILENAME", "");
}

MetadataDTO Metadata::getMetadataFromFile(string filename) {
    MetadataDTO metadata;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        string _filename = extractFilename();
        if (_filename == filename ) {
            metadata = extractMetadata();
        }
        XML.popTag();
    }
    
    return metadata;
}

map<string,vector<string>> Metadata::getAllTags() {
    map<string, vector<string>> tags;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        tags.insert({extractFilename(), extractTags()});
        XML.popTag();
    }
    
    return tags;
}

map<string, float> Metadata::getAllLuminance(){
    map<string, float> luminances;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        luminances.insert({extractFilename(), extractLuminance()});
        XML.popTag();
    }
    
    return luminances;
}

map<string, Color> Metadata::getAllColor(){
    map<string, Color> colors;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        colors.insert({extractFilename(), extractColor()});
        XML.popTag();
    }
    
    return colors;
}

map<string, int> Metadata::getAllFaces() {
    map<string, int> faces;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        faces.insert({extractFilename(), extractFaces()});
        XML.popTag();
    }
    
    return faces;
}
map<string, Edges> Metadata::getAllEdges() {
    map<string, Edges> edges;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        edges.insert({extractFilename(), extractEdges()});
        XML.popTag();
    }
    
    return edges;
}
map<string, float> Metadata::getAllTexture() {
    map<string, float> textures;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        textures.insert({extractFilename(), extractTexture()});
        XML.popTag();
    }
    
    return textures;
}

vector<MetadataDTO> Metadata::getAllMetadata(){
    vector<MetadataDTO> metadataFiles;
    for (int i=0; i < XML.getNumTags("FILE"); i++) {
        XML.pushTag("FILE", i);
        metadataFiles.push_back(extractMetadata());
        XML.popTag();
    }
    
    return metadataFiles;
}
