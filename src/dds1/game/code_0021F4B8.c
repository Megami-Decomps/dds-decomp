#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "dds3_path.h"
#include "dds3_admin_control_api.h"
#include "sdf_request.h"
#include "kwln.h"
#include "kwln_task_create.h"
#include "sdf_packet_list.h"
#include "sdf_packet_builders.h"
#include "sdf_bordered_rectangle_packet.h"
#include "common.h"
#include "sdf_file_existence_api.h"
#include "dds3_object_flags.h"
#include "sdf_vu0.h"
#include "mdl_option_label.h"
#include "eff_inner_float_api.h"
#include "mdl_light_editor_state.h"
#include "kwln_task_add_child.h"
#include "mdl_motion_api.h"
#include "sdf_motion_loop.h"
#include "sdf_texture_draw_packet.h"
#include "file_request_api.h"
#include "bill_object_api.h"
#include "sdf_chip.h"
#include "sdf_heap_api.h"
#include "sdf.h"
#include "sdf_texture_registry.h"
#include "sdf_projection.h"
#include "mdl.h"
#include "mdl_blur_api.h"
#include "mdl_part_info_api.h"
#include "mdl_p2a_resource_api.h"
#include "mdl_part_entry.h"
#include "mdl_resource_entries.h"
#include "mdl_track_poly_record.h"
#include "mdl_mark_params.h"
#include "fld_integer_editor_api.h"
#include "mdl_resource_table.h"
#include "mdl_grouped_parts_api.h"
#include "mdl_flag_api.h"
#include "mdl_world_object_source_vector_api.h"
#include "dds3_world_counter.h"
#include "evt_unit_object_key.h"
#include "sdf_vu_matrix.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "dat_state.h"
#include "sdf_draw.h"
#include "sdf_chunk.h"
#include "sdf_pac_state.h"
#include "eff.h"
#include "eff_node_descriptor.h"
#include "eff_node.h"
#include "sdf_sif_command.h"
#include "eff_transform.h"
#include "evt_world_source_transform.h"
#include "evt_script_event_id.h"
#include "file.h"
#include "kwln_task_lifecycle.h"
#include "mdl_object_stream.h"
#include "dds3obj.h"
#include "sdf_gs_header.h"
#include "mdl_option_state.h"
#include "dds3_world_secondary.h"

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

extern char D_003BBC80[]; /* "%5.2f" */

extern char D_003BBC28[]; /* "" */

extern struct sdfMemHandle_t *mdlHddFileIndexHandle;

extern struct sdfMemHandle_t *mdlHddFileListHandle;

/* Five-character device prefix copied in front of each work-file path. */
typedef struct {
    char text[5];
} DebugWorkPrefix;

extern DebugWorkPrefix mdlWorkPathPrefix[];

extern char D_003BBB80[], D_003BBB88[];

extern char D_003BBBB0[];

extern char D_003BBBB8[];

extern s8 dds3PadBitTable[];

extern char D_003BBC20[];

extern char D_003BBC30[];

extern char D_003BBC38[];

extern char D_003BBC40[];

extern char D_003BBC48[];

extern char D_003BBC50[];

extern char D_003BBC58[];

extern char D_003BBC60[];

extern char D_003BBC68[];

extern char D_003BBC88[];

extern char D_003BBC90[];

extern char D_003BBC98[];

extern char D_003BBCA0[];

extern char D_003BBCA8[];

extern const char D_003BBCB0[];

extern s32 D_003BBCB8;

extern s32 sdfGrAllocPacketBuff(s32 size);

extern char D_003BBCC0[]; /* "%x" */

extern char D_003BBCC8[]; /* "fovy=" */

extern char D_003BBCD0[]; /* "%f" */

extern char D_003BBCD8[]; /* "fog=" */

extern char D_003BBCF8[]; /* ">" in .sdata */

extern char D_003BBD00[]; /* "%6.2f" in .sdata */

extern sdfGrOt_t D_00325708;

extern void FromRotation(f32 angle);

extern char D_003BBD38[];

extern const char D_003BBD58[];

extern const char D_003BBD60[];

extern const char D_003BBD68[];

extern const char D_003BBD70[];

extern const char D_003BBD78[];

extern void evtDisableMoonCalc(void);

extern void evtDisableMoonDraw(void);

extern void evtSetAgeOfMoon(s32 phase);

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}

void evtBitAllOff(void) {
    s32 i = 0x7f;
    u32 *word = DDS3_GBWK->EventBit.words;

    do {
        i -= 1;
        *word = 0;
        word += 1;
    } while (i >= 0);
}

void evtBitAllOff_FieldDebug(void) {
    s32 i = 0;
    do {
        evtBitOff(i++);
    } while (i < 0xC00);
    i = 0xD00;
    do {
        evtBitOff(i++);
    } while (i < 0x1000);
}

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void evtBitOn(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->EventBit.words[adjustedFlag >> 5] |= 1 << flag;
}

void evtBitOff(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->EventBit.words[adjustedFlag >> 5] &= ~(1 << flag);
}

EvtBitTestResult evtBitCheck(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)DDS3_GBWK->EventBit.words[adjustedFlag >> 5] >> flag) & 1);
}

extern s32 D_003BBD80, D_003BBD84, D_003BBD88;

extern char D_003BBD90[], D_003BBD98[], D_003BBDA0[];

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32,
    s32 *, s32 *, s32 *, s32 *);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* Native 0021F664..74 copies this space/NUL template as one block;
     * see docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_003BBD90, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_003BBD80 + D_003BBD88) * 16 + D_003BBD84));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_003BBD80 + D_003BBD88) * 16 + D_003BBD84));
    }
    for (column = 0; column < 16; column++) {
        style = D_003BBD84 == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_003BBD98, column));
    }
    for (row = 0; row < 16; row++) {
        style = D_003BBD88 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_003BBDA0, (D_003BBD80 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_003BBD80 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = D_003BBD84 == column || D_003BBD88 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = D_003BBD84 == column || D_003BBD88 == row ? 14 : 8;
            }
            if (D_003BBD84 == column && D_003BBD88 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    D_00325708.append(&D_00325708, packets);
    if ((dds3PadBitTable[24] & 2) && D_003BBD80 > 0) {
        D_003BBD80 -= 16;
        if (D_003BBD80 < 0) D_003BBD80 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_003BBD80 + 16 < 256) {
            if (D_003BBD80 + 32 > 256) D_003BBD80 = 240;
            else D_003BBD80 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_003BBD80, &D_003BBD84, &D_003BBD88);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84)) {
            evtBitOff((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84);
        } else {
            evtBitOn((D_003BBD80 + D_003BBD88) * 16 + D_003BBD84);
        }
    }
    return 0;
}

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 counter, u128 *position, u128 *rotation);

extern void dds3ObjectBaseCreateMover(dds3Basic_t *object);

void evtSetTestCamera(void) {
    f32 position[4] = { 0.0f, -100.0f, -600.0f, 0.0f };
    f32 rotation[4];
    dds3Basic_t *object;

    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    object = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)position, (u128 *)rotation);
    dds3GetRadiusPoseSet((dds3Basic_t *)object, 10.0f);
    dds3ObjectBaseCreateMover(object);
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), (dds3Basic_t *)object);
    dds3ObjectBaseSetPadControl(object, 0);
}

extern void dds3RebuildObjectModelContext(dds3Basic_t *object, s32, s32);

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 counter, u128 *position, u128 *rotation);

s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource) {
    f32 position[4];
    f32 rotation[4];
    s32 counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, (u128 *)position, (u128 *)rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3RebuildObjectModelContext(object, slotKind, resource);
    if (((s32 *)dds3ObjectBaseGetModelHandle(object))[7] != 0) {
        mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    }
    dds3ObjectBaseSetPadControl(object, 0);
    return counter;
}

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *object);

EvtUnitObjectKey evtLoadUnit(s32 slotKind, s32 resource) {
    f32 position[4];
    f32 rotation[4];
    EvtUnitObjectKey counter;
    dds3Basic_t *object;

    memset(position, 0, 0x10);
    position[3] = 1.0f;
    memset(rotation, 0, 0x10);
    rotation[3] = 1.0f;
    counter = dds3AllocObjectId();
    object = dds3AddUnitObjectBasic(counter, (u128 *)position, (u128 *)rotation);
    dds3ObjectBaseOnFlag(object, 0x20);
    dds3GetRadiusPoseSet(object, 10.0f);
    dds3RebuildObjectModelContext(object, slotKind, resource);
    mdlLoopMotion((void *)dds3ObjectBaseGetModelHandle(object), 0, 0);
    dds3ObjectBaseSetControlBasic(object, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(object);
    dds3ObjectBaseSetPadControl(object, 0);
    dds3ObjectBaseOnFlag(object, 0x400);
    return counter;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 quaternion[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
    dds3Basic_t *target;
    dds3Basic_t *source;
    EvtWorldSourceTransformPrefix *sourceTransform;

    target = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (target != NULL) {
        source = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
        if (source != NULL) {
            sourceTransform = source->workdata;
            dds3SetPosPoseSet(target, (u128 *)&sourceTransform->position);
                        SDF_VU0_LOADVECTOR(quaternion);
            FromRotation(3.14159265f);
                        SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
                        SDF_VU0_STOREVECTOR(quaternion);
            dds3SetRotPoseSet(target, (u128 *)quaternion);
            dds3GetPosPoseSet(target);
            SDF_VU0_STOREVECTOR(target->poseset->smoothedPosition);
        }
    }
}

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBD80);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBD84);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBD88);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBD90);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBD98);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", D_003BBDA0);

INCLUDE_SDATA(const s32, "game/code_0021F4B8", evtPendingEventSelection);

