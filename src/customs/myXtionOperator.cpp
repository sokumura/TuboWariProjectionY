//
//  myXtionOperator.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/22.
//
//

#include "myXtionOperator.h"

//===========================
//globals
//===========================
xn::ScriptNode g_scriptNode;///
//===========================
//===========================

XnBool fileExists(const char *fn){
	XnBool exists;
	xnOSDoesFileExist(fn, &exists);
	return exists;
}

myXtionOperator::myXtionOperator(){


}
myXtionOperator::~myXtionOperator(){

}

void myXtionOperator::setup(){
    cout << "OpenNI operation was get started" << endl;
    context.setup();
    customEnumerateProductionTrees(context.getXnContext(), XN_NODE_TYPE_DEPTH);
    
    xn::NodeInfoList nodeList;
    XnStatus nRetVal = XN_STATUS_OK;

//    if( USE_RECORED_DATA ){
//        context.getXnContext().OpenFileRecording(RECORD_FILE_PATH);
//        xn::Player player;
//        
//        nRetVal = context.getXnContext().FindExistingNode(XN_NODE_TYPE_PLAYER, player);
//        CHECK_RC(nRetVal, "Find player");
//        
//        xn::NodeInfoList nodeList;
//        player.EnumerateNodes(nodeList);
//        for( xn::NodeInfoList::Iterator it = nodeList.Begin();
//            it != nodeList.End(); ++it){
//            
//            if( (*it).GetDescription().Type == XN_NODE_TYPE_DEPTH ){
//                nRetVal = context.getXnContext().FindExistingNode(XN_NODE_TYPE_DEPTH, depth_GRs[it]);
//                CHECK_RC(nRetVal, "Find depth node");
//            }
//        }
//    } else {
//        LOG_I("Reading config from: '%s'", CONFIG_XML_PATH);
//        
//        nRetVal = g_Context.FindExistingNode(XN_NODE_TYPE_DEPTH, g_DepthGenerator);
//        
//    }
//    //
//    xn::Recorder recorder;
//    if( DO_RECORED && !USE_RECORED_DATA ){
//        nRetVal = recorder.Create(g_Context);
//    
//        nRetVal = recorder.SetDestination(XN_RECORD_MEDIUM_FILE, RECORD_FILE_PATH);
//        CHECK_RC(nRetVal, "Set recorder destination file");
//        
//        nRetVal = recorder.AddNodeToRecording(g_DepthGenerator, XN_CODEC_NULL);
//        CHECK_RC(nRetVal, "Add depth node to recording");
//        nRetVal = recorder.AddNodeToRecording(g_ImageGenerator, XN_CODEC_NULL);
//        CHECK_RC(nRetVal, "Add image node to recording");
//    }
    
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
    for (int i = 0; i < generatorNum; i++) {
        bNewDataXtion[i] = false;
    }
    
    
}
//--------------------------------------
void myXtionOperator::update(){
    int isNewNum = 0;
    for (int i = 0; i < generatorNum; i++) {//generatorごと
        bool isNew = depth_GRs[i].update();
        if (isNew) bNewDataXtion[i] = true;
    }
}

//--------------------------------------
void myXtionOperator::testDraw(){
    for (int i = 0; i < generatorNum; i++){
        depth_GRs[i].draw();
    }
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
