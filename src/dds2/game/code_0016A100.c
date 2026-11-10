#include "common.h"
#include "ps2asm.h"
#include "sdf_chip.h"
#include "eff_param.h"
#include "eff_param_operations.h"
#include "eff_thunder_vector.h"
#include "par_cell_api.h"
#include "sdf_heap_api.h"
#include "btl_state.h"
#include "eff.h"
#include "eff_battle_color_keyframe.h"
#include "eff_battle_transition_duration.h"
#include "evt_unit.h"
#include "evt_unit_color_transition.h"
#include "eff_matrix4.h"
#include "mdl.h"
#include "btl_grouped_entity_api.h"
#include "bill_object_api.h"

extern u32 effBTLSkillGetInvokeFormation(void);

extern u32 effBTLFieldColorGetVariantSelector(void);

extern void effBattleMiscDirectionTo(BtlUnit *, EffectVectorRequest *, f32 *);

extern u32 btlCameraVectorHasNaN(void);

extern u32 btlBlendColorVec(f32 *, f32 *, f32);

/* Apply and restore a selected group's two-color and direction keyframe. */
void effBattleApplyUnitColorDirection(BtlUnit *unit, EffBattleUnitColorCommand *command, u32 frame) {
    BtlUnit *selected[16];
    f32 direction[4];
    u32 packedStart[4];
    u32 packedEnd[4];
    u32 startFrame = command->startFrame;
    u32 endFrame = command->endFrame;
    EffBattleTransitionDuration startDuration = effBattleMiscGetTableEntry(command->startDurationIndex);
    EffBattleTransitionDuration endDuration = effBattleMiscGetTableEntry(command->endDurationIndex);
    u32 restoreFrame;
    u32 mask = 0;
    u32 count;
    u32 index;
    BtlState *battle;
    BtlUnit *current;
    evtUnitHandle_t *eventUnit;

    if (startFrame >= endFrame) {
        return;
    }
    if (endFrame < endDuration) {
        return;
    }
    restoreFrame = endFrame - endDuration;
    if (startFrame >= restoreFrame) {
        return;
    }
    if (frame != startFrame && frame != restoreFrame) {
        return;
    }
    battle = (BtlState *)btlGetRuntime();
    if (battle->battleFlags & 0x40000000) {
        return;
    }
    count = 0;
    switch (command->request.kind) {
    case 0:
        selected[0] = unit;
        count = 1;
        break;
    case 1:
        mask = ((BtlUnit *)effBTLSkillGetInvokeFormation())->status.flags & 0xE00;
        break;
    case 2:
        mask = ((BtlUnit *)effBTLFieldColorGetVariantSelector())->status.flags & 0xE00;
        break;
    case 3:
        mask = 0xE00;
        break;
    }
    if (count == 0) {
        for (current = battle->units; current != NULL; current = current->next) {
            u32 flags = current->status.flags;
            if ((flags & 2) && (current->status.stateFlags & 0x10) &&
                current->ext != NULL && (flags & mask)) {
                selected[count++] = current;
            }
        }
    }
    if (frame == startFrame) {
        for (index = 0; index < count; index++) {
            eventUnit = selected[index]->ext;
            evtUnitLightOn(eventUnit);
            evtSetUnitLightAndAmbient(eventUnit, startDuration, command->firstColor, command->secondColor);
            effBattleMiscDirectionTo(selected[index], &command->request, direction);
            SDF_VU0_LOADVECTOR(direction);
            evtSetUnitLightDir(eventUnit, startDuration);
        }
    }
    if (frame == restoreFrame) {
        for (index = 0; index < count; index++) {
            u32 firstColor;
            u32 secondColor;

            eventUnit = selected[index]->ext;

            evtUnitLightOn(eventUnit);
            if (btlCameraVectorHasNaN()) {
                firstColor = btlBlendColorVec(battle->lightColor, selected[index]->colorStart, 0.3f);
                secondColor = btlBlendColorVec(battle->ambientColor, selected[index]->colorEnd, 0.3f);
            } else {
                SDF_VU0_LOADVECTOR(selected[index]->colorStart);
                MMI_RGBA_PACK_UNIT(packedStart[0], 128.0f);
                firstColor = packedStart[0];
                SDF_VU0_LOADVECTOR(selected[index]->colorEnd);
                MMI_RGBA_PACK_UNIT(packedEnd[0], 128.0f);
                secondColor = packedEnd[0];
            }
            evtSetUnitLightAndAmbient(eventUnit, endDuration, firstColor, secondColor);
            SDF_VU0_LOADVECTOR(selected[index]->lightDirection);
            evtSetUnitLightDir(eventUnit, endDuration);
        }
    }
}

void effBattleApplyUnitRgbKeyframe(BtlUnit *unit, EffBattleUnitRgbCommand *command, s32 frame) {
    u32 startFrame = command->startFrame;
    u32 endFrame = command->endFrame;
    EffBattleTransitionDuration startDuration = effBattleMiscGetTableEntry(command->startDurationIndex);
    EffBattleTransitionDuration endDuration = effBattleMiscGetTableEntry(command->endDurationIndex);
    u32 restoreFrame;

    if ((unit->status.flags & 0xE0) != 0) {
        return;
    }
    if ((unit->status.flags & 2) == 0) {
        return;
    }
    if (startFrame >= endFrame) {
        return;
    }
    if (endFrame < endDuration) {
        return;
    }
    restoreFrame = endFrame - endDuration;
    if (startFrame >= restoreFrame) {
        return;
    }
    if (frame != startFrame && frame != restoreFrame) {
        return;
    }
    /* Preserve the runtime touch before the event-unit color is changed. */
    btlGetRuntime();
    if (frame == startFrame) {
        evtUnitHandle_t *eventUnit = (evtUnitHandle_t *)unit->ext;
        u32 baseColor = unit->baseColor;
        u32 blendedColor;

        if ((baseColor & 0xFFFFFF) != 0x808080) {
            u32 color = command->color;
            u32 differentBits = color ^ baseColor;
            u32 sharedBits = color & baseColor;
            blendedColor = sharedBits + ((differentBits & 0xFEFEFEFE) >> 1);
        } else {
            blendedColor = command->color;
        }
        evtSetUnitColorRGB(eventUnit, startDuration, blendedColor);
    }
    if (frame == restoreFrame) {
        evtSetUnitColorRGB((evtUnitHandle_t *)unit->ext, endDuration, unit->baseColor);
    }
}

extern void dds3AdminiCallSequence(s32 requestedMode, const void *requestData, u32 dataBytes, s32 markHistory);

void dds3AdminSubmitEmptyModeRequest(void) {
    dds3AdminiCallSequence(0, 0, 0, 0);
}
