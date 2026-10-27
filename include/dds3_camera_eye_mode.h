#ifndef DDS3_CAMERA_EYE_MODE_H
#define DDS3_CAMERA_EYE_MODE_H

#include "common.h"

/* Camera eye-position selectors are stored and passed as signed words. */
typedef s32 CameraEyePositionMode;

/* The known modes select an explicit world-space eye or a node-relative eye. */
#define CAMERA_EYE_POSITION_WORLD_SPACE 0
#define CAMERA_EYE_POSITION_NODE_RELATIVE 1

/* Camera providers preserve selector values; only node-relative mode transforms
 * the local eye offset during basis rebuild. */

#endif /* DDS3_CAMERA_EYE_MODE_H */
