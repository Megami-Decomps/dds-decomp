#ifndef BTL_CAMERA_MODE_DEFINITION_H
#define BTL_CAMERA_MODE_DEFINITION_H

#include "btl_command.h"

typedef struct BtlCameraModeDefinition {
    void (*initialize)(BtlLinkedCommand *);
    void (*update)(BtlLinkedCommand *);
    u8 allowRepeat;
    u8 pad09[3];
    void *unk0C;
} BtlCameraModeDefinition;

typedef char BtlCameraModeDefinitionSize[(sizeof(BtlCameraModeDefinition) == 16) ? 1 : -1];

#endif
