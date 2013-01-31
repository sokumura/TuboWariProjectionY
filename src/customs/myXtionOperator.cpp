//
//  myXtionOperator.cpp
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2012/10/22.
//
//

#include "myXtionOperator.h"


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
        depth_GRs[i].draw(axis[i],rotx[i], roty[i], rotz[i]);
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
        /*
        cout << "ProductionTreeの列挙\n" <<
        xnProductionNodeTypeToString((*it).GetDescription().Type ) << ", " <<
        (*it).GetCreationInfo() << ", " <<
        (*it).GetInstanceName() << ", " <<
        (*it).GetDescription().strName << ", " <<
        (*it).GetDescription().strVendor << ", " << endl;
        */
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
