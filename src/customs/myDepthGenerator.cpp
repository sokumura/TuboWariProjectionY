//
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/21.
//
//

#include "myDepthGenerator.h"


myDepthGenerator::myDepthGenerator(){

    out_put_modes.nXRes = CAPTURE_WIDTH;
    out_put_modes.nYRes = CAPTURE_HEIGHT;
    out_put_modes.nFPS  = 30;
    
    gettingPointDepth = false;
}

myDepthGenerator::~myDepthGenerator(){

}
//----------------------------------------------
bool myDepthGenerator::setup(xn::NodeInfo const& node, int num){
    if ( thisNum == 0) bDraw[thisNum] = true;
    
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
    
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = depthMAX;
        captureBgCountByPix[i] = 0;
    }
    bgImg.allocate(CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_GRAYSCALE);
    loadBgImage();
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
        generateCurrentDepth();
        generateTexture();
        generateRealWorld(realWorld);
        isNewDataAvailable = true;
    }
    
    checkSwitchMethods();
    counter++;
    return isNewDataAvailable;
    
}
//-------------------------------------------------
void myDepthGenerator::checkSwitchMethods(){
    if (bCaptureBg[thisNum]) {
        planeBgCapthre();
        bCaptureBg[thisNum] = false;
    }
    if (trLoadBg[thisNum]) {
        loadBgImage();
        trLoadBg[thisNum];
    }
    if (bSaveBgAsImage[thisNum]) {
        saveBgImage();
        bSaveBgAsImage[thisNum];
    }
}
//----------------------------------------------
void myDepthGenerator::draw(){
    if (bDraw[thisNum]) {
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
}
//----------------------------------------------
//----------------------------------------------
void  myDepthGenerator::generateCurrentDepth(){//不要なものを省いたdepthData
    const XnDepthPixel * depth = dmd.Data();
    if (dmd.FrameID() == 0) return;
    int i = 0;
    for (XnUInt16 y = 0; y < dmd.YRes(); y++) {
        XnDepthPixel * texture = currentDepth + y * dmd.XRes();
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, i++, depth++) {
            if (bUseBgDepth[thisNum] && bgDepth[i] - bgCapturePlay[thisNum] <= *depth) {//bgDepth以上の数値の時
                currentDepth[i] = 0;
                continue;
            }
            if (*depth > thresholdNear[thisNum] && *depth < thresholdFar[thisNum]){//thresholdに捕まらなかったら
                currentDepth[i] = *depth;
            } else {
                currentDepth[i] = 0;
            }
            if (realDepthMax[thisNum] < *depth && *depth < depthMAX ) {
                realDepthMax[thisNum] = *depth;
            }
            if (counter %100 == 1) {
                realDepthMax[thisNum] = 0;
            }
        }
    }
}

unsigned short myDepthGenerator::getPointDepth(int x, int y){
    const XnDepthPixel * depth = dmd.Data();
    if (dmd.FrameID() == 0) return;
    x = MAX(0, x);
    y = MIN(CAPTURE_HEIGHT, y);
    
    unsigned short s = depth[y * CAPTURE_WIDTH + x];
    
    return s;
}

void myDepthGenerator::generateRealWorld( XnPoint3D * p3d){//3dにする
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
            p->Z = *depth;
        }
    }
    depth_generator.ConvertProjectiveToRealWorld(TOTAL_PIXEL, proj, p3d);
    
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        if (p3d[i].Z != 0) {
            ofVec3f v = ofVec3f( p3d[i].X, p3d[i].Y * (-1) , p3d[i].Z * (-1) + 5000.0f);
            depthVecs.push_back(v);
        }
    }

    if (depthVecs.size() > 0) vboMesh.addVertices(depthVecs);
}

//---------------
void myDepthGenerator::generateTexture() {//モニター用
    const XnDepthPixel * depth = dmd.Data();
    float max = 255;
    int i = 0;
    if (trGetPointDepth[thisNum]) {
        depthPointValue[thisNum] = (int)getPointDepth((int)depthCheckPoint[thisNum].x, (int)depthCheckPoint[thisNum].y);
    }
    for (XnUInt16 y = 0; y < dmd.YRes(); y++) {
        unsigned char * texture = monitor_texture + y * dmd.XRes() * 4;
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, i++, depth++, texture += 4) {
            XnUInt8 a = (XnUInt8)(((*depth) / (1 - depthMAX / max)));
            if (bUseBgDepth[thisNum] && bgDepth[i] - bgCapturePlay[thisNum] <= *depth) {
                
                texture[0] = a;
                texture[1] = a;
                texture[2] = a;
                texture[3] = 10;
                continue;
            }
            
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
//----------------------------------------------
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
//--------------------------------------------------
void myDepthGenerator::saveBgImage(){
    bgImg.setFromPixels((unsigned short *)bgDepth, CAPTURE_WIDTH, CAPTURE_HEIGHT, OF_IMAGE_GRAYSCALE);
    bgImg.saveImage("depthCapture_no_" + ofToString(thisNum) + ".tiff");
}
//-------------------------------------------------
void myDepthGenerator::planeBgCapthre(){//1回のデータを蓄積する
    unsigned int unCount = 0;
    const XnDepthPixel * bg = dmd.Data();
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = bg[i];
        if (bg[i] == 0 ) {
            bgDepth[i] = depthMAX;
        }
    }
}
//-------------------------------------------------
void myDepthGenerator::freeBgDepth(){
    for (int i = 0; i < TOTAL_PIXEL; i++) {
        bgDepth[i] = depthMAX;
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


