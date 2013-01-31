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
    
    totalPixel = out_put_modes.nXRes * out_put_modes.nYRes;
    
    bBgDepth = false;
    bgCaptureCount = 0;
    freeBgDepth();
    capturePlay = 0;//testAppで初期設定せよ
}

myDepthGenerator::~myDepthGenerator(){

    
}
//----------------------------------------------
bool myDepthGenerator::setup(xn::NodeInfo const& node, int num){
    depthMAX = 8000;
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
    vboMesh.setMode(OF_PRIMITIVE_POINTS);
    vboMesh.disableNormals();
    
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
        depth_generator.GetMetaData(dmd);

        generateTexture();
        generateCurrentDepth();
        generateRealWorld(realWorld);
        if (bCaptureBg[thisNum]) runCapture();
        isNewDataAvailable = true;
    }
    
    if (bSaveBgAsImage[thisNum]) {
        img.allocate(CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_COLOR_ALPHA);
        
        img.setFromPixels(bgDepth, CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_GRAYSCALE);
        img.saveImage("depthCapture_no_" + ofToString(thisNum) + ".png");
        
        bSaveBgAsImage[thisNum] = false;
    }
    
    counter++;
    return isNewDataAvailable;
    
}

//----------------------------------------------
void myDepthGenerator::draw(ofVec3f & pos, float degx, float degy, float degz){
    
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, pos.z);
    ofRotateX(degx);
    ofRotateY(degy);
    ofRotateZ(degz);
    glPointSize(pointSize);
    ofScale(scale[thisNum], scale[thisNum] * 1 / aspect[thisNum], scaleZ[thisNum]);
    vboMesh.draw();
    ofPopMatrix();
    
    img.draw(0,0);

}

//--------------------
void myDepthGenerator::runCapture(){//キャプチャするとき1回だけ呼ぶ
    if (captureBgCount == 0) {
        for (int i = 0; i < TOTAL_PIXEL; i++) {
            bgDepth[i] = 0;
        }
    }
    if (captureCount < 30) {
        planeBgCapthre();
    } else if (captureCount == 30) {
        bCaptureBg[thisNum] = false;
        for (int i = 0; i < TOTAL_PIXEL; i++) {
            if (captureBgCount[i] == 0) bgDepth[i] = 0;
            else bgDepth[i] = (unsigned short) (bgDepth[i] / captureBgCount[i]);
        }
        captureBgCount[thisNum] = 0;
    }
}

void myDepthGenerator::planeBgCapthre(){//Newフレームごとに30回呼ぶ
    const XnDepthPixel * bg = dmd.Data();
    XN_ASSERT(depth);
    if (dmd.FrameID() < 30) return;
    
    for (int i = 0; i < totalPixel; i++) {
        bgDepth[i] += bg[i];
        if (bg[i] != 0) captureBgCount[i]++;
    }
    captureCount++;
}

//----------------------------------------------

//--------------
void myDepthGenerator::freeBgDepth(){
    for (int i = 0; i < totalPixel; i++) {
        bgDepth[i] = depthMAX;
    }
    bgCaptureCount = 0;
}
//----------------------------------------------
unsigned int myDepthGenerator::getCaptureCount() const{
    return bgCaptureCount;
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
            if (bBgDepth && bgDepth[i] - capturePlay <= *depth) {
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
    const XnDepthPixel * depth = dmd.Data();
    XN_ASSERT(depth);
    if (dmd.FrameID() == 0) return;
    XnPoint3D proj[TOTAL_PIXEL] = {0};
    XnPoint3D *p = proj;
    
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
            if (bBgDepth && bgDepth[i] - capturePlay <= *depth) {
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
            
            if (*depth < depthMIN && *depth != 0) depthMIN = *depth;
            if (*depth > depthMAX) depthMAX = *depth;
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

}


