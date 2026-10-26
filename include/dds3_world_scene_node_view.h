#ifndef DDS3_WORLD_SCENE_NODE_VIEW_H
#define DDS3_WORLD_SCENE_NODE_VIEW_H

#include "dds3obj.h"

/* Typed view over WorldInfo's selected-node words at +0x08 and +0x0C. */
typedef struct {
    u8 precedingWords[0x08];
    dds3Basic_t *activeScene;
    dds3Basic_t *controlScene;
} WorldInfoSceneNodeView;

typedef char WorldInfoSceneNodeView_size_must_be_0x10[
    (sizeof(WorldInfoSceneNodeView) == 0x10) ? 1 : -1];
typedef char WorldInfoSceneNodeView_activeScene_at_0x08[
    ((u32)&((WorldInfoSceneNodeView *)0)->activeScene == 0x08) ? 1 : -1];
typedef char WorldInfoSceneNodeView_controlScene_at_0x0C[
    ((u32)&((WorldInfoSceneNodeView *)0)->controlScene == 0x0C) ? 1 : -1];

#endif
