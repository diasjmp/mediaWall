#include "metadataBuilder.hpp"

void MetadataBuilder::setup(string imagesPath, string videosPath, string filenameMetadata) {
    finder.setup("haarcascade_frontalface_default.xml");
    videoPlaying = false;
    FILENAME_METADATA = filenameMetadata;
    imagesDone = false;
    videosDone = false;
    currVideo = 0;
    newVideo = false;
    dirImages.listDir(imagesPath);
    dirVideos.listDir(videosPath);
    dirImages.allowExt("jpg");
    dirVideos.allowExt("mov");
    dirVideos.sort();
    dirVideos.sort();
    currFile = 0;
}

void MetadataBuilder::update() {
    if (videoPlaying)
        currVideoPlayer.update();
}

int MetadataBuilder::getNumFacesInImage(ofImage image) {
    finder.findHaarObjects(image);
    return finder.blobs.size();
}

void MetadataBuilder::addNumFaces(int nFaces) {
    XML.addValue("NUM_FACES", nFaces);
}

float MetadataBuilder::getMeanFromMat(cv::Mat img) {
    int sum = 0;
    int rows = img.rows;
    int cols = img.cols;
    for (int x = 0; x < rows; ++x) {
        for (int y = 0; y < cols; ++y) {
            if ((int)img.at<uchar>(x, y) > 50) {
                sum++;
            }
        }
    }
    
    return (float)sum / (float)(rows*cols);
}

void MetadataBuilder::addEdgeDistribution(cv::Mat greyImg) {
//    do vertical -> avg and deviation/variance
    cv::Mat f1, f2, f3, f4, f5;
    
    float horizontalEdges[6] = {1, -1, 1, -1};
    cv::Mat horizontalFilter = cv::Mat(2, 2, CV_32F, horizontalEdges);
    
    float verticalEdges[6] = {1, 1, -1, -1};
    cv::Mat verticalFilter = cv::Mat(2, 2, CV_32F, verticalEdges);
    
    float degree45[6] = {(float)sqrt(2), 0, 0, -(float)sqrt(2)};
    cv::Mat degree45Filter = cv::Mat(2, 2, CV_32F, degree45);
    
    float degree135Edges[6] = {0, (float)sqrt(2), -(float)sqrt(2), 0};
    cv::Mat degree135Filter = cv::Mat(2, 2, CV_32F, degree135Edges);
    
    float nonDirectionalEdges[6] = {2, -2, -2, 2};
    cv::Mat nonDirectionalFilter = cv::Mat(2, 2, CV_32F, nonDirectionalEdges);
    
    filter2D(greyImg, f1, greyImg.depth(), horizontalFilter);
    filter2D(greyImg, f2, greyImg.depth(), verticalFilter);
    filter2D(greyImg, f3, greyImg.depth(), degree45Filter);
    filter2D(greyImg, f4, greyImg.depth(), degree135Filter);
    filter2D(greyImg, f5, greyImg.depth(), nonDirectionalFilter);
    
    XML.addTag("EDGES");
    XML.pushTag("EDGES");
    XML.addValue("VERTICAL", getMeanFromMat(f1));
    XML.addValue("HORIZONTAL", getMeanFromMat(f2));
    XML.addValue("DEGREE45", getMeanFromMat(f3));
    XML.addValue("DEGREE135", getMeanFromMat(f4));
    XML.addValue("NONDIRECTIONAL", getMeanFromMat(f5));
    XML.popTag();
}


void MetadataBuilder::addTexture(cv::Mat greyImg) {
    float sumAll = 0.0;
    for (int i=0; i<6; i++) {
        for (int j=0; j<3; j++) {
            cv::Mat newT;
            filter2D(greyImg, newT, greyImg.depth(), getGaborKernel(greyImg.size(), 5+(j*10), i*30, 8, 0.9, 0, CV_32F));
            
            int sum = 0;
            for (int x = 0; x < newT.rows; ++x) {
                for (int y = 0; y < newT.cols; ++y) {
                    if ((int)newT.at<uchar>(x, y) > 50) {
                        sum++;
                    }
                }
            }
            sumAll += (float)sum / (float)(newT.rows * newT.cols);
        }
    }
    float meanTexture = sumAll / (6*3);
    XML.addValue("TEXTURE", meanTexture);
}

void MetadataBuilder::processWithFilters(ofPixels & pixels) {
    cv::Mat img = ofxCv::toCv(pixels);
    cv::Mat greyImg;
    cv::cvtColor(img, greyImg, CV_RGB2GRAY);
    
    addEdgeDistribution(greyImg);
    addTexture(greyImg);
}

void MetadataBuilder::createImageMetadata(string imagePath, string imageName) {
    ofImage image;
    image.load(imagePath);
    
    XML.addTag("FILE");
    XML.pushTag("FILE", currFile++);
    
    XML.addValue("FILENAME", imageName);
    
    addColorImageAndLuminance(image);
    addKeyWords();
    
    addNumFaces(getNumFacesInImage(image));
    
    processWithFilters(image.getPixels());
    //    Texture characteristics (for images and first frame of the video)
    //    Number of times a specific object (input as an image) appears in the video frame
    
    XML.popTag();
}

void MetadataBuilder::buildImagesMetadata() {
    for (int i = 0; i < (int)dirImages.size(); i++) {
        string imagePath, imageName;
        getPathAndName(dirImages, &imagePath, &imageName, i);
        if (!XML.tagExists(imageName)) {
            createImageMetadata(imagePath, imageName);
        }
    }
    imagesDone = true;
}

bool MetadataBuilder::buildMetaData() {
    if ((imagesDone && videosDone) || XML.loadFile(FILENAME_METADATA))
        return true;
    
    if (!imagesDone) {
        buildImagesMetadata();
    }
    
    if (!videosDone)
        processVideos();
    
    if (imagesDone && videosDone)
        XML.saveFile(FILENAME_METADATA);
    
    return false;
}

void MetadataBuilder::getPathAndName(ofDirectory dir, string * path, string * name, int i) {
    *name = dir.getName(i);
    name->erase(remove(name->begin(), name->end(), ' '), name->end());
    *path = dir.getPath(i);
}

void MetadataBuilder::processVideos() {
    static bool processingVideos = false;
    if (!processingVideos) {
        processingVideos = true;
    }
    
    string videoPath, videoName;
    getPathAndName(dirVideos, &videoPath, &videoName, currVideo);
    bool videoCompleted = createVideoMetadata(videoPath, videoName);
    
    if (videoCompleted) {
        currVideo++;
    }
    
    if (currVideo == dirVideos.size()) {
        videosDone = true;
    }
}

void MetadataBuilder::loadAndPlayVideo(string videoPath) {
    currVideoPlayer.load(videoPath);
    currVideoPlayer.setVolume(0);
    currVideoPlayer.setSpeed(2.5);
    currVideoPlayer.setLoopState(OF_LOOP_NONE);
    currVideoPlayer.play();
    videoPlaying = true;
}

bool MetadataBuilder::createVideoMetadata(string videoPath, string videoName) {
    static bool processing = false;
    if (!processing) {
        processing = true;
        loadAndPlayVideo(videoPath);
        XML.addTag("FILE");
        XML.pushTag("FILE", currFile);
        XML.addValue("FILENAME", videoName);
        addKeyWords();
    } else
        XML.pushTag("FILE", currFile);
    
    bool processed = processMeanVideoData(videoName); // -> returns true if video finished processing
    
//    Number of times a specific object (input as an image) appears in the video frame
//    Scene change (only for videos)
    
    if(processed) {
        processing = false;
        XML.popTag();
        videoPlaying = false;
        currVideoPlayer.close();
        currFile++;
    }
    
    return processed;
}

void MetadataBuilder::addKeyWords() {
    XML.addTag("keyWords"); // introduzidas a mao, pode ser diretamente no ficheiro
    XML.pushTag("keyWords");
      XML.addValue("keyWord", "key1: Alterar isto no ficheiro");
      XML.addValue("keyWord", "key2: Alterar isto no ficheiro");
    XML.popTag();
}

void MetadataBuilder::getMeanColorAndLuminance(ofPixels & pixels, float *r, float *g, float *b, float *luminance) {
    size_t nChannels = pixels.getNumChannels();
    size_t nPixels = pixels.size();
    *r = 0.0, *g = 0.0, *b = 0.0;
    for(size_t i = 0; i < nPixels; i+=nChannels){
        //Media: avg += (x - avg) / t; t->1..
        size_t usedPixels = i/nChannels + 1;
        *r += (pixels[i] - *r) / usedPixels;
        *g += (pixels[i+1] - *g) / usedPixels;
        *b += (pixels[i+2] - *b) / usedPixels;
    }
    *luminance = *r *0.2125 + *g *0.7154 + *b *0.0721;
}

void MetadataBuilder::addXMLColorLuminance(float luminance, int r, int g, int b) {
    XML.addValue("luminance", luminance);
    XML.addTag("color");
    XML.pushTag("color");
      XML.addValue("RED", r);
      XML.addValue("GREEN", g);
      XML.addValue("BLUE", b);
    XML.popTag();
}

void MetadataBuilder::addColorImageAndLuminance(ofImage image) {
    ofPixels & pixels = image.getPixels();
    float r, g, b;
    float luminance;
    getMeanColorAndLuminance(pixels, &r, &g, &b, &luminance);
    addXMLColorLuminance(luminance, (int)r, (int)g, (int)b);
}

bool MetadataBuilder::doneProcessingVideo(ofVideoPlayer vidPlayer, int currFrame, int usedFrames) {
    return usedFrames > 0 && !vidPlayer.isPlaying();
}

void MetadataBuilder::processLuminanceAndColor(float*red, float*green, float*blue, float*luminance, int usedFrames) {
    float r,g,b,lum;
    ofPixels & pixels = currVideoPlayer.getPixels();
    getMeanColorAndLuminance(pixels, &r, &g, &b, &lum);
    //Media: avg += (x - avg) / t; t->1..
    *red += (r - *red) / usedFrames;
    *green += (g - *green) / usedFrames;
    *blue += (b - *blue) / usedFrames;
    *luminance += (lum - *luminance) / usedFrames;
}

void MetadataBuilder::processFaces(int *maxNumFaces) {
    ofImage frame;
    frame.allocate(currVideoPlayer.getWidth(), currVideoPlayer.getHeight(), OF_IMAGE_COLOR);
    frame.setFromPixels(currVideoPlayer.getPixels());
    
    int numFaces = getNumFacesInImage(frame);
    if (numFaces > *maxNumFaces)
        *maxNumFaces = numFaces;
}

bool MetadataBuilder::processMeanVideoData(string name) {
    static float red=0.0, green=0.0, blue=0.0;
    static float luminance = 0.0;
    static int usedFrames = 0;
    static int lastFrameProcessed = 0;
    
    static int maxNumFaces = 0;
    static int lastFrameFaces = 0;
    
    static bool processedEdges = false;
    
    int currFrame = currVideoPlayer.getCurrentFrame();
    
    if (doneProcessingVideo(currVideoPlayer, currFrame, usedFrames)) {
        addXMLColorLuminance(luminance, (int)red, (int)green, (int)blue);
        addNumFaces(maxNumFaces);
        
        // reset static variables
        red=0.0, green=0.0, blue=0.0;
        luminance = 0.0;
        usedFrames = 0;
        lastFrameProcessed = 0;
        maxNumFaces = 0;
        lastFrameFaces = 0;
        processedEdges = false;
        return true;
    }
    
    if (currFrame >= 0 && currFrame > lastFrameProcessed + 10) {
        lastFrameProcessed = currFrame;
        usedFrames++;
        processLuminanceAndColor(&red, &green, &blue, &luminance, usedFrames);
    }
    
    if (currFrame >= 0 && currFrame > lastFrameFaces + 60) {
        lastFrameFaces = currFrame;
        processFaces(&maxNumFaces);
    }
    
    if (!processedEdges && currFrame > 5) {
        processedEdges = true;
        processWithFilters(currVideoPlayer.getPixels());
        makeThumbnail(name);
    }
    
    return false;

}

void MetadataBuilder::makeThumbnail(string name) {
    ofImage img;
    img.setFromPixels( currVideoPlayer.getPixels() );
    float thumbWidth = 300;//change this for another thumb size
    img.resize(thumbWidth, thumbWidth * (img.getHeight() / img.getWidth()) );
    img.saveImage("thumbsVideos/"+name+ ".jpg");
}
