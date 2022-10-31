#include "imageAndVideo.hpp"

void ImageAndVideo::setup() {
    clusteringSetUp = false;
    dir.listDir("images/of_logos/");
    dir.allowExt("jpg");
    dir.sort(); // in linux the file system doesn't return file lists ordered in alphabetical order

    //allocate the vector to have as many ofImages as files
    // you can now iterate through the files and load them into the ofImage vector
    for (int i = 0; i < (int)dir.size(); i++) {
        ofImage img;
        img.load(dir.getPath(i));
        MyImage myImg;
        myImg.name = dir.getName(i);
        myImg.img = img;
        images.push_back(myImg);
    }


    /*-----------------Video -> depois passar para uma class----------------*/

    dirVideos.listDir("movies/");
    dirVideos.allowExt("mov");
    dirVideos.sort();

    if (dirVideos.size()) {
        videoPlayers.assign(dirVideos.size(), ofVideoPlayer());
    }

    for (int i = 0; i < (int)dirVideos.size(); i++) {
        videoPlayers[i].load(dirVideos.getPath(i));
    }

    /*----------------------------------------------------------------------*/

    currentClusteringType = COLOR;
    mainLayout = false;
    nThumbnails = 4;
    shuffleFrontLayout();
    centralImage = NULL;
    centralVideo = NULL;

    layout.setup();
    frontLayout.setup();
    
}

void ImageAndVideo::update() {
    if(mainLayout)
        layout.update();
    else
        frontLayout.update();
}

vector<ofImage> ImageAndVideo::fromMyImageToOfImage(vector<MyImage> images) {
    vector<ofImage> result;
    for (MyImage img : images) {
        result.push_back(img.img);
    }
    return result;
}

void ImageAndVideo::draw() {
    if (!clusteringSetUp){
        clusteringSetUp = true;
        clustering.setup();
    }

    if (mainLayout) {
        Clustering_type type = layout.getTypecluster();

        if(currentClusteringType != type) {
            cout << currentClusteringType << " cType" << endl;
            currentClusteringType = type;
            if (centralImage != NULL) {
                loadThumbnails(centralImage->name, type);
            } else if (centralVideo != NULL){
                loadThumbnails(centralVideo->name, type);
            }
        }

        static bool isPlaying = false;
        if (!isPlaying) {
            isPlaying = true;
            videoPlayers[0].play();
        }
        
        if (centralImage != NULL) {
            layout.draw(&centralImage->img, NULL, thumbnails);
        }
        else if(centralVideo != NULL) {
            layout.draw(NULL, &centralVideo->vid, thumbnails);
        }
    }
    else {
        vector<ofImage> frontThumbnails = fromMyImageToOfImage(frontLayoutThumbnails);
        frontLayout.draw(frontThumbnails);
    }

}

ofImage ImageAndVideo::createVideoThumbnail(ofVideoPlayer video) {
    return ofImage();
}


void ImageAndVideo::playAndPause() {
    layout.playAndPause();
}

bool ImageAndVideo::currentIsVideo(int current, int nImages) {
    return current >= nImages;
}

void ImageAndVideo::mousePressed(int x, int y) {
    if (mainLayout) {
        int action = layout.mousePressed(x, y);

        if (action == -1) {
            mainLayout = !mainLayout;
        }
        else if (action >= 0) {
            // Update central image
            if (thumbnails[action].name.find(".jpg")) {
                centralVideo = NULL;
                ofImage img;
                img.allocate(thumbnails[action].img.getWidth(), thumbnails[action].img.getHeight(), OF_IMAGE_COLOR);
                img.setFromPixels(thumbnails[action].img.getPixels());
                
                centralImage->img = img;
                centralImage->name = thumbnails[action].name;
                
                Clustering_type type = layout.getTypecluster();
                loadThumbnails(centralImage->name, type);
            } else {
                centralImage = NULL;
                loadVideo(thumbnails[action].name);
            }
        }
    }
    else {
        int action = frontLayout.mousePressed(x, y);
        
        if (action == -1){
            shuffleFrontLayout();
        }
        else if (action >= 0){
            changeLayout(action);

        }
    }
}

void ImageAndVideo::loadVideo(string name) {
    ofVideoPlayer vidPlayer;
    vidPlayer.load("movies/" + name);
    MyVideo myVideo;
    myVideo.name = name;
    myVideo.vid = vidPlayer;
    centralVideo = &myVideo;
}

void ImageAndVideo::changeLayout(int action) {
    cout << "action num: " << action << endl;
    Clustering_type type = layout.getTypecluster();
    // Update central image
    if (frontLayoutThumbnails[action].name.find(".jpg")) {
        centralImage = &frontLayoutThumbnails[action];
        centralVideo = NULL;
        loadThumbnails(centralImage->name, type);
    } else {
        loadVideo(frontLayoutThumbnails[action].name);
        centralImage = NULL;
        loadThumbnails(centralVideo->name, type);
    }
    
    //TODO: PASSAR AQUI PARAMETROS DIFERENTES COM O CHECKBOX
    
    
    // Change to main layout
    mainLayout = !mainLayout;
}

vector<Thumbnail> ImageAndVideo::loadFromFilenames(vector<ClusteringResult> files) {
//    TODO: FALTA POR ISTO A FUNCIONAR PARA OS VIDEOS
    vector<Thumbnail> thumbnails;
    for (ClusteringResult file : files) {
        string filename = file.name;
        ofImage img;
        if (filename.find("jpg")) {
            img.load("images/of_logos/" + filename);
        } else {
            img.load("thumbsVideos/" + filename + ".jpg");
        }
        Thumbnail thumbnail;
        thumbnail.name = filename;
        thumbnail.img = img;
        thumbnail.similarity = (float)((((double)rand()) / ((double)RAND_MAX) * 0.8) + 0.2);
        thumbnails.push_back(thumbnail);
    }
    
    return thumbnails;
}

void ImageAndVideo::loadThumbnails(string baseFile, Clustering_type searchType) {
    cout << "searchType: " << " " << searchType << endl;
    vector<ClusteringResult> files;
    int nImages = 20;
    switch(searchType) {
        case LUMINANCE:
            files = clustering.getNByLuminance(nImages, baseFile);
            break;
        case COLOR:
            files = clustering.getNByColor(nImages, baseFile);
            break;
        case TAGS:
            files = clustering.getNByTags(nImages, baseFile);
            break;
        case FACES:
            files = clustering.getNByFaces(nImages, baseFile);
            break;
        case EDGES:
            files = clustering.getNByEdges(nImages, baseFile);
            break;
        case TEXTURE:
            files = clustering.getNByTexture(nImages, baseFile);
            break;
    }
    
    thumbnails = loadFromFilenames(files);
}

void ImageAndVideo::shuffleFrontLayout() {
    // TODO: Add videos
    vector<int> indexes;
    frontLayoutThumbnails.clear();

    for (int i = 0; i < images.size(); i++) {
        indexes.push_back(i); // image indexes
    }

    std::random_shuffle(indexes.begin(), indexes.end());

    for (int j = 0; j < nThumbnails; j++) {
        frontLayoutThumbnails.push_back(images[indexes[j]]);
        cout << indexes[j] << endl;
    }

}
