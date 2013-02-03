//
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/21.
//
//

#include "myDepthGenerator.h"


myDepthGenerator::myDepthGenerator(){

    // make new map mode -> default to 640 x 480 @ 30fps
    out_put_modes.nXRes = CAPTURE_WIDTH;
    out_put_modes.nYRes = CAPTURE_HEIGHT;
    out_put_modes.nFPS  = 30;
}

myDepthGenerator::~myDepthGenerator(){

    
}
//----------------------------------------------
bool myDepthGenerator::setup(xn::NodeInfo const& node, int num){
    depthMAX = 9000;
    depthMIN = 0;
    
    thisNum = num;
    XnStatus result = XN_STATUS_OK;
    result = node.GetInstance(depth_generator);
    if (result != XN_STATUS_OK) {
        ofLogError("myDepthGenerator.setup()", xnGetStatusString(result));
        return false;
    } else {
        depth_generator.SetMapOutputMode(out_put_modes);
        return true;
    }
    
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = depthMAX;
        captureBgCount[i] = 0;
    }
    bgImg.allocate(CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_GRAYSCALE);
    //loadBgImage();
    
    vboMesh.setMode(OF_PRIMITIVE_POINTS);
    vboMesh.disableNormals();
    
    contourFinderSetup();
    
}



//----------------------------------------------
void myDepthGenerator::startGenerating(){
    XnStatus status = depth_generator.StartGenerating();
    if (status == XN_STATUS_OK) printf("DepthGenerator %i Generating Start!\n", thisNum);
    else ofLogError("myDepthGenerator/startGenerating ",xnGetStatusString(status));
}

//----------------------------------------------
bool myDepthGenerator::update(){
    bool isNewDataAvailable = false;
    if (depth_generator.IsNewDataAvailable()) {
        depth_generator.WaitAndUpdateData();
//        if( DO_RECORED  && !USE_RECORED_DATA ){
//            XnStatus nRetVal = recorder[thisNum].Record();
//            CHECK_RC(nRetVal, "Record");
//        }
        depth_generator.GetMetaData(dmd);

        generateTexture();
        generateCurrentDepth();
        generateRealWorld(realWorld);
        if (bCaptureBg[thisNum]) runCapture();
        isNewDataAvailable = true;
    }
    
    if (bSaveBgAsImage[thisNum]) {
        saveBgImage();
    }
    
    counter++;
    return isNewDataAvailable;
}

//----------------------------------------------
void myDepthGenerator::draw(){
    ofPushMatrix();
    ofTranslate(axis[thisNum].x, axis[thisNum].y, axis[thisNum].z);
    ofRotateX(rotx[thisNum]);
    ofRotateY(roty[thisNum]);
    ofRotateZ(rotz[thisNum]);
    glPointSize(pointSize);
    ofScale(scale[thisNum], scale[thisNum] * 1 / aspect[thisNum], scaleZ[thisNum]);
    vboMesh.draw();
    ofPopMatrix();

}

bool myDepthGenerator::loadBgImage(){
    bool loadCheck = bgImg.loadImage("depthCapture_no_" + ofToString(thisNum) + ".tiff");
    if (!loadCheck) {
        printf("bgImg load error!!\n");
        return false;
    }
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = bgImg.getPixels()[i];
    }
    return true;
}

void myDepthGenerator::saveBgImage(){
    bgImg.setFromPixels((unsigned short *)bgDepth, CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_GRAYSCALE);
    bgImg.saveImage("depthCapture_no_" + ofToString(thisNum) + ".tiff");
    bSaveBgAsImage[thisNum] = false;
}

//--------------------
void myDepthGenerator::runCapture(){//キャプチャするとき1回だけ呼ぶ
    if (captureCount == 0) {
        for (int i = 0; i < TOTAL_PIXEL; i += 3) {
            bgDepth[i] = 0;
        }
    }
    if (captureCount < 30) {
        planeBgCapthre();
    } else if (captureCount == 30) {
        for (int i = 0; i < TOTAL_PIXEL; i++) {
            if (captureBgCount[i] == 0) bgDepth[i] = 0;
            else bgDepth[i] = (unsigned short) (bgDepth[i] / captureBgCount[i]);
        }
        bCaptureBg[thisNum] = false;
        captureBgCount[thisNum] = 0;
    } else if (captureCount > 30) return;
}

void myDepthGenerator::planeBgCapthre(){//Newフレームごとに30回呼ぶ
    const XnDepthPixel * bg = dmd.Data();
    XN_ASSERT(depth);
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] += bg[i];
        if (bg[i] != 0) captureBgCount[i]++;
    }
    captureCount++;
}

//----------------------------------------------

//--------------
void myDepthGenerator::freeBgDepth(){
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = depthMAX;
    }
    captureCount = 0;
}
//----------------------------------------------
unsigned int myDepthGenerator::getCaptureCount() const{
    return captureCount;
}
//----------------------------------------------
void  myDepthGenerator::generateCurrentDepth(){
    const XnDepthPixel * depth = dmd.Data();
    XN_ASSERT(depth);
    if (dmd.FrameID() == 0) return;
    
    int i = 0;
    float max = 255;
    for (XnUInt16 y = 0; y < dmd.YRes(); y++) {
        XnDepthPixel * texture = currentDepth + y * dmd.XRes();
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, i++, depth++) {
            if (bUseBgDepth[thisNum] && bgDepth[i] - bgCapturePlay[thisNum] <= *depth) {
                currentDepth[i] = 0;
                continue;
            }
            if (*depth > thresholdNear[thisNum] && *depth < thresholdFar[thisNum]){
                currentDepth[i] = *depth;
            } else {
                currentDepth[i] = 0;
            }
        }
    }
}

void myDepthGenerator::generateRealWorld( XnPoint3D * p3d){
    vboMesh.clear();
    depthVecs.clear();
    vboMesh.setMode(OF_PRIMITIVE_POINTS);
    if (dmd.FrameID() == 0) return;
    XnPoint3D proj[TOTAL_PIXEL] = {0};
    XnPoint3D *p = proj;
    const XnDepthPixel * depth = currentDepth;
    for (int y = 0; y < CAPTURE_HEIGHT; y++) {
        for (int x = 0; x < CAPTURE_WIDTH; x++, p++, depth++) {
            p->X = x;
            p->Y = y;
            p->Z = *depth;//from mm to meters
        }
    }
    depth_generator.ConvertProjectiveToRealWorld(TOTAL_PIXEL, proj, p3d);
    
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        if (p3d[i].Z < thresholdFar[thisNum]
            && p3d[i].Z > thresholdNear[thisNum]) {
            if (bUseBgDepth[thisNum]) {
                if (bgDepth[i] <= p3d[i].Z + bgCapturePlay[thisNum]) {
                    continue;
                }
            }
        
            ofVec3f v = ofVec3f( p3d[i].X, p3d[i].Y * (-1) , p3d[i].Z * (-1) + 5000.0f);
            depthVecs.push_back(v);
        }
    }

    if (depthVecs.size() > 0) vboMesh.addVertices(depthVecs);
    
}

//---------------
void myDepthGenerator::generateTexture() {
    const XnDepthPixel * depth = currentDepth;
    float max = 255;
    int i = 0;
    for (XnUInt16 y = 0; y < dmd.YRes(); y++) {
        unsigned char * texture = monitor_texture + y * dmd.XRes() * 4;
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, i++, depth++, texture += 4) {
            if (bUseBgDepth[thisNum] && bgDepth[i] - bgCapturePlay[thisNum] <= *depth) {
                texture[0] = 0;
                texture[1] = 0;
                texture[2] = 0;
                texture[3] = 0;
                continue;
            }
            XnUInt8 a = (XnUInt8)(((*depth) / (1 - depthMAX / max)));
            if (*depth > thresholdNear[thisNum] && *depth < thresholdFar[thisNum]) {
                texture[0] = 255;
            } else texture[0] = a;
            
            texture[1] = a;
            texture[2] = a;
            
            if (*depth == 0)
                texture[3] = 0;
            else
                texture[3] = 255;
        }
    }
}

const unsigned char * myDepthGenerator::getMonitorTexture() const{
    const unsigned char * pointer = monitor_texture;
    return pointer;
}

const XnDepthPixel * myDepthGenerator::getDistanceTexture() const{
    const XnDepthPixel * pointer = currentDepth;
    return pointer;
}

void myDepthGenerator::console(bool bOut){
    for (int i = 0; i < TOTAL_PIXEL - 100; i++) {
        printf( "bgDepth[%u] : %u\n",i, bgDepth[i]);
    }
}


