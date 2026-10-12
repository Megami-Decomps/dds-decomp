#include "eff.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_shooting.h"

void itfDrawModelInstanceImage(ModelInstance *item);

extern void itfDrawUniformlyScaledIndexedImage(s32, s32, s32, s32, s32, s32, s32, f32);

void itfDrawModelInstanceImage(ModelInstance *item) {
    f32 alpha = 1.0f;
    s32 kind;

    if (item->initialLifetime != 0) {
        alpha = (f32)item->remainingLifetime / (f32)item->initialLifetime;
    }
    kind = (item->flags >> 3) & 0xFF;
    switch (kind) {
    case 17:
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, 0, 0x53, item->scale);
        return;
    case 18:
        if (item->animationLength <= item->animationFrame) {
            item->animationFrame = 0;
        }
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, item->animationFrame + 31, 0x53, item->scale);
        return;
    case 19:
        item->animationFrame = 0;
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, 29, 0x53, item->scale);
        break;
    case 20:
        if (item->animationLength <= item->animationFrame) {
            item->animationFrame = 0;
        }
        itfDrawUniformlyScaledIndexedImage((s32)item->position[0], (s32)item->position[1],
            0, (s32)(alpha * 128.0f), 0, item->animationFrame + 9, 0x53, item->scale);
        break;
    }
}
