//
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/21.
//
//

#include "myDepthGenerator.h"


//--change to real world
const float FovX = 1.0144686707507438;
const float FovY = 0.78980943449644714;
const float XtoZ = tanf(FovX / 2) * 2;
const float YtoZ = tanf(FovY / 2) * 2;
const unsigned int Xres = 640;
const unsigned int Yres = 480;

ofVec3f ConvertProjectiveToRealWorld(float x, float y, float z) {
	return ofVec3f((x / Xres - .5f) * z * XtoZ,
                   (y / Yres - .5f) * z * YtoZ,
                   z);
}


myDepthGenerator::myDepthGenerator(){
    step = 2;
    pointSize = 0.5f;

    
    
    bUpdateMasks = false;
    // make new map mode -> default to 640 x 480 @ 30fps
    out_put_modes.nXRes = w = XN_VGA_X_RES;
    out_put_modes.nYRes = h = XN_VGA_Y_RES;
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
    depthMAX = zThresholdFar = 10000;
    depthMIN = zThresholdNear = 0;
    
    xtionNum = num;
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
    if (status == XN_STATUS_OK) printf("DepthGenerator %i Generating Start!\n", xtionNum);
    else ofLogError("myDepthGenerator/startGenerating ",xnGetStatusString(status));
}

//----------------------------------------------
bool myDepthGenerator::update(soDepthThresholds thresholds){
    bool isNewDataAvailable = false;
    if (depth_generator.IsNewDataAvailable()) {
        depth_generator.WaitAndUpdateData();
        depth_generator.GetMetaData(dmd);
        
        privThresholds = thresholds;

        generateTexture();
        //generateMonoTexture();
        generateCurrentDepth();
        generateVectors();
        isNewDataAvailable = true;
    }
    return isNewDataAvailable;
    counter++;
}

void myDepthGenerator::generateVectors(){//currentTexture to vector. only without 0.
    vboMesh.clear();
    depthVecs.clear();
    int numberOfVecs = 0;
    int stepHalf = int(step / 2);
    int X = out_put_modes.nXRes;
    int Y = out_put_modes.nYRes;
    for (int y = step; y < Y - 1 - step; y += step) {
        for (int x = stepHalf; x < X - 1 - step - stepHalf; x += step) {
            XnDepthPixel pix = currentDepth[X * y + x];
            if (pix > 0.2f) {
                ofVec3f vec = ConvertProjectiveToRealWorld(x, y,  (-1) * pix + 5000.0f);
//                  ofVec3f vec = ofVec3f(x, y, pix);
//                vec += localAxis;
//                if (degrees.y != 0) {//y軸回転
//                    vec.x = (vec.x - localAxis.x) * cos(degrees.y) - (vec.z - localAxis.z) * sin(degrees.y) + localAxis.x;
//                    vec.z = (vec.x - localAxis.x) * sin(degrees.y) + (vec.z - localAxis.z) * cos(degrees.y) + localAxis.z;
//                }
                depthVecs.push_back(vec);
                numberOfVecs++;
            }
        }
    }
    vboMesh.addVertices(depthVecs);
}
//----------------------------------------------
void myDepthGenerator::draw(ofVec3f & pos, float degx, float degy, float degz){
    ofPushMatrix();
    ofTranslate(pos.x, pos.y, pos.z);
    ofRotateX(degx);
    ofRotateY(degy);
    ofRotateZ(degz);
    ofScale(0.3, 0.3);
    vboMesh.setMode(OF_PRIMITIVE_POINTS);
    vboMesh.draw();
    ofPopMatrix();
//    glTranslatef(0.0f, 0.0f, 0.0f);
//    glTranslatef(0.0f, 0.0f, 0.0f);
}

//----------------------------------------------
XnMapOutputMode& myDepthGenerator::getMapMode() {
    return out_put_modes;
}
//--------------------
void myDepthGenerator::runCapture(){
    //captureBgDepth();
    planeBgCapthre();
    bgCaptureCount++;
}

void myDepthGenerator::planeBgCapthre(){
    const XnDepthPixel * bg = dmd.Data();
    XN_ASSERT(depth);
    
    if (dmd.FrameID() == 0) return;
    for (int i = 0; i < totalPixel; i++) {
        bgDepth[i] = bg[i] - bgCaptureCount;
    }
}

//----------------------------------------------

//--------------
void myDepthGenerator::freeBgDepth(){
    for (int i = 0; i < totalPixel; i++) {
        bgDepth[i] = privThresholds.max;
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
            if (*depth > privThresholds.near && *depth < privThresholds.far){
                currentDepth[i] = *depth;
            } else {
                currentDepth[i] = 0;
            }
        }
    }
}

//-----------------------------
void myDepthGenerator::generateMonoTexture() {
    const XnDepthPixel * depth = dmd.Data();
    XN_ASSERT(depth);
    if (dmd.FrameID() == 0) return;
    
    int i = 0;
    float max = 255;
    for (XnUInt16 y = 0; y < dmd.YRes(); y++) {
        unsigned char * texture = mono_texture + y * dmd.XRes();
		for (XnUInt16 x = 0; x < dmd.XRes(); x++, i++, depth++, texture++) {
            if (bBgDepth && bgDepth[i] - capturePlay <= *depth) {
                *texture = 0;
                continue;
            }
            if (*depth > privThresholds.near && *depth < privThresholds.far)
                *texture = 255;
            else
                *texture = 0;
        }
    }
}


//---------------
void myDepthGenerator::generateTexture() {
    const XnDepthPixel * depth = dmd.Data();
    XN_ASSERT(depth);
    if (dmd.FrameID() == 0) return;
    
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
            XnUInt8 a = (XnUInt8)(((*depth) / (1 - privThresholds.max / max)));
            if (*depth > privThresholds.near && *depth < privThresholds.far) {
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


const unsigned char * myDepthGenerator::getMonoTexture() const{
    const unsigned char * pointer = mono_texture;
    return pointer;
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
    printf("\n--No.%u--privThresholds\nnear:%u\nfar:%u\nmin:%u\nmax:%u\n\n", xtionNum, privThresholds.near, privThresholds.far, privThresholds.min, privThresholds.max);
    
    cout << "\n\n---------------dmd-------------------\n" <<
    "dmd.DataSize() : " << dmd.DataSize() <<
    "\ndmd.FPS() : " << dmd.FPS() <<
    "\ndmd.FrameID() : " << dmd.FrameID() <<
    "\ndmd.FullXRes() : " << dmd.FullXRes() <<
    "\ndmd.FullYRes() : " << dmd.FullYRes() <<
    "\ndmd.XRes() : " << dmd.XRes() <<
    "\ndmd.YRes() : " << dmd.YRes() <<
    "\ndmd.ZRes() : " << dmd.ZRes() <<
    "\ndmd.XOffset() : " << dmd.XOffset() <<
    "\ndmd.YOffset() : " << dmd.YOffset() << endl;
}
















