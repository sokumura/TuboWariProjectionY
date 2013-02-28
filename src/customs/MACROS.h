//
//  MACROS.h
//  TuboWariProjection
//
//  Created by Shuya Okumura on 2013/02/12.
//
//

#pragma once

#define SCENE_SIZE      13

//main
#define XTION_NUM       4
#define CAPTURE_WIDTH   XN_QVGA_X_RES
#define CAPTURE_HEIGHT  XN_QVGA_Y_RES
#define TOTAL_PIXEL     CAPTURE_WIDTH * CAPTURE_HEIGHT
#define PROJECTION_SIZE_W   1280
#define PROJECTION_SIZE_H   960
//magic
#define HONDANA_INDEX_NUM 6 //変更するなら、初期化も変更しないと落ちる

//gurotesq
#define GROTESQ_FRAME_NUM 634

#define OFXMSAGUI_DONT_USE_XML
