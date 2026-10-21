#ifndef DDS_EFF_CAMERA_PATH_PARAMETERS_H
#define DDS_EFF_CAMERA_PATH_PARAMETERS_H

#include "common.h"

typedef struct EffCameraPathParameters {
    u32 startFrame;
    u8 actorSelection;
    u8 count;
    u8 directionMode;
    u8 pad07;
    u32 defeatSelection;
} EffCameraPathParameters;

typedef char EffCameraPathParameters_size_check[
    (sizeof(EffCameraPathParameters) == 0xC) ? 1 : -1];
typedef char EffCameraPathParameters_actorSelection_offset_check[
    ((u32)&((EffCameraPathParameters *)0)->actorSelection == 0x04) ? 1 : -1];
typedef char EffCameraPathParameters_defeatSelection_offset_check[
    ((u32)&((EffCameraPathParameters *)0)->defeatSelection == 0x08) ? 1 : -1];

#endif
