#ifndef MDL_LIGHT_EDITOR_STATE_H
#define MDL_LIGHT_EDITOR_STATE_H

#include "common.h"

struct MdlPadState;
struct SdfListHead;

typedef struct MdlLightEditorState {
    f32 (**lights)[4];
    f32 *ambient;
    struct MdlPadState *pad;
    u8 editing;
    u8 blinkTick;
    s16 selection;
    struct SdfListHead *packetList;
} MdlLightEditorState;

typedef char MdlLightEditorState_size[(sizeof(MdlLightEditorState) == 0x14) ? 1 : -1];
typedef char MdlLightEditorState_offsets[
    (((u32)&((MdlLightEditorState *)0)->lights == 0x00) &&
     ((u32)&((MdlLightEditorState *)0)->ambient == 0x04) &&
     ((u32)&((MdlLightEditorState *)0)->pad == 0x08) &&
     ((u32)&((MdlLightEditorState *)0)->editing == 0x0C) &&
     ((u32)&((MdlLightEditorState *)0)->blinkTick == 0x0D) &&
     ((u32)&((MdlLightEditorState *)0)->selection == 0x0E) &&
     ((u32)&((MdlLightEditorState *)0)->packetList == 0x10)) ? 1 : -1];

#endif
