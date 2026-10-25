#ifndef DDS3_SCENE_MEMBERSHIP_H
#define DDS3_SCENE_MEMBERSHIP_H

#include "common.h"

struct dds3Basic_t;

typedef s32 Dds3SceneMembershipResult;

#define DDS3_SCENE_MEMBERSHIP_ABSENT 0
#define DDS3_SCENE_MEMBERSHIP_PRESENT 1

Dds3SceneMembershipResult dds3SceneBasicExistBasic(
    struct dds3Basic_t *object, struct dds3Basic_t *targetNode);
Dds3SceneMembershipResult dds3SceneBasicExistBasicFromType(
    struct dds3Basic_t *object, s32 slotIndex, const struct dds3Basic_t *targetNode);

#endif /* DDS3_SCENE_MEMBERSHIP_H */
