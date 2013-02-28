//
//  myXtionOperator.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/22.
//
//

#include "myXtionOperator.h"


extern bool bSetUp;
extern bool bShutDown;

//===========================
//globals
//===========================
//xn::ScriptNode g_scriptNode;///

//===========================
//===========================

myXtionOperator::myXtionOperator(){


}
myXtionOperator::~myXtionOperator(){

}

void myXtionOperator::setUpAgain(){
    bool bCheck = false;
    for (int i = 0; i < XTION_NUM; i++) {
        bCheck = bUpdateXtion;
        bUpdateXtion[i] = false;
    }
    if (bCheck) {
        myShutDownContext();
    }
    this->setup();
}

void myXtionOperator::myShutDownContext(){
    for (int i = 0; i < XTION_NUM; i++) {
        bUpdateXtion[i] = false;
    }
    context.shutdown();
    bShutDown = false;
}

void myXtionOperator::setup(){
    cout << "OpenNI operation was get started" << endl;
    context.setup();
    customEnumerateProductionTrees(context.getXnContext(), XN_NODE_TYPE_DEPTH);
    
    xn::NodeInfoList nodeList;
    XnStatus nRetVal = XN_STATUS_OK;
    //
    nRetVal = context.getXnContext().EnumerateExistingNodes(nodeList);
    if (nRetVal != XN_STATUS_OK) {
        ofLogError("OpenNI error : context.EnumerateExistingNodes();", xnGetStatusString(nRetVal));
        logErrors(errors);
    } else cout << "//////////////nodeExistings Successed!!!!///////////////" << endl;
    int geneNum = 0;
    for (xn::NodeInfoList::Iterator it = nodeList.Begin(); it != nodeList.End(); ++it) {
        //インスタンス名の最後が番号になってるから、
        std::string name = (*it).GetInstanceName();
        cout << "--------------nodeInfomation------------\nname is : " << name << endl;
        int no = *name.rbegin() - '1';
        cout << "no is : " << no << endl;
        
        std::cout <<
        "GetCreationInfo() : " <<
        (*it).GetCreationInfo() << "\n" <<
        "GetInstanceName() : " <<
        (*it).GetInstanceName() << "\n" <<
        "GetDescription().strName : " <<
        (*it).GetDescription().strName << "\n" <<
        "GetDescription().strVendor : " <<
        (*it).GetDescription().strVendor << "\n" <<
        "-------/nodeInfomation-----" << std::endl;
        
        if ((*it).GetDescription().Type == XN_NODE_TYPE_DEPTH) {
            printf("geneNum : %u\n", geneNum);
            xn::NodeInfo info = *it;
            depth_GRs[geneNum].setup(info, geneNum);
            std::cout << "sensor[" << no << "]のdepthGeneratorを作成!!" << std::endl;
            depth_GRs[geneNum].startGenerating();
            geneNum++;
        }
    }
    generatorNum = geneNum;
    printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!\n%i 個のジェネレーターを作りました\n!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", generatorNum);
    
    
    //vboMesh
    
    
}
//--------------------------------------
void myXtionOperator::update(){
    int isNewNum = 0;
    for (int i = 0; i < generatorNum; i++) {//generatorごと
        
        bool isNew = depth_GRs[i].update();
        if (isNew){
            bNewDataXtion[i] = true;
        }
    }
    if (bSetUp) {
        setUpAgain();
        bSetUp = false;
    }
    if (bShutDown){
        myShutDownContext();
    }
}

//--------------------------------------
void myXtionOperator::testDraw(){
    for (int i = 0; i < generatorNum; i++){
        depth_GRs[i].draw();
    }
}

void myXtionOperator::startOniRecording(int n){
    char recordFile[256] = {0};
    std::time_t rawtime;
    struct tm *timeinfo;
    XnUInt32 size;
    time(&rawtime);
    xnOSStrFormat(recordFile, sizeof(recordFile) - 1, &size, "no_%u_%02d_%02d[%02d_%02d_%02d].oni", n, timeinfo->tm_mon + 1, timeinfo->tm_mday, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    if (pRecorder != NULL) {
        stopOniRecording(n);
    }
    
    XnStatus nRetVal = XN_STATUS_OK;
    pRecorder[n] = new xn::Recorder;
    
    nRetVal = context.getXnContext().CreateAnyProductionTree(XN_NODE_TYPE_RECORDER, NULL, *pRecorder[n]);
    CHECK_RC(nRetVal, "Create OniRecorder");
    
    nRetVal = pRecorder[n]->SetDestination(XN_RECORD_MEDIUM_FILE, recordFile);
    CHECK_RC(nRetVal, "set Destination");
    nRetVal = pRecorder[n]->AddNodeToRecording(depth_GRs[n].getXnDepthGenerator(), XN_CODEC_16Z_EMB_TABLES);
    CHECK_RC(nRetVal, "add Node");
    bRecord[n] = true;
}

void myXtionOperator::stopOniRecording(int n){
    bRecord[n] = false;
    if (pRecorder[n] != NULL) {
        pRecorder[n]->RemoveNodeFromRecording(depth_GRs[n].getXnDepthGenerator());
        pRecorder[n]->Release();
    }
    pRecorder[n] = NULL;
}

//--------------------------------------
void myXtionOperator::customEnumerateProductionTrees(xn::Context& con, XnProductionNodeType type){
    xn::NodeInfoList nodes;
    XnStatus rc = XN_STATUS_OK;
    rc = con.EnumerateProductionTrees(type, NULL, nodes);
    if (rc != XN_STATUS_OK) {
        ofLogError("MY TASK NO.1", xnGetStatusString(rc));
    } else if (nodes.Begin() == nodes.End()) {
        ofLogError("MY TASK NO.1", "No device found.");
    }
    
    for (xn::NodeInfoList::Iterator it = nodes.Begin(); it != nodes.End(); ++it) {
        xn::NodeInfo info = *it;
        con.CreateProductionTree(info);
    }
}

//-----------------------------------------
myDepthGenerator& myXtionOperator::getDepthGenerator(int index) {
    return depth_GRs[index];
}
//--------------------------------------
void myXtionOperator::logErrors(xn::EnumerationErrors& rErrors){
    for(xn::EnumerationErrors::Iterator it = rErrors.Begin(); it != rErrors.End(); ++it) {
        XnChar desc[512];
        xnProductionNodeDescriptionToString(&it.Description(), desc,512);
        printf("%s failed: %s\n", desc, xnGetStatusString(it.Error()));
    }
}

/////////////////ofVboMesh////////////////
