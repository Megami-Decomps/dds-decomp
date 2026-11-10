#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "dds3_path.h"
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
#include "dds3_admin_control_api.h"
#include "kwln_task_add_child.h"
#include "mdl_motion_api.h"
#include "sdf_motion_loop.h"
#include "sdf_texture_draw_packet.h"
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
#include "file_request_api.h"
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
#include "dds3_world_secondary.h"
#include "sdf_gs_header.h"
#include "mdl_option_state.h"

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

extern char D_00437068[]; /* "" */

extern dds3Basic_t *dds3AddUnitObjectBasic(s32 world, u128 *pos, u128 *rot);

extern void dds3RebuildObjectModelContext(dds3Basic_t *obj, s32 a, s32 b);

extern char D_00436FF0[];

extern char D_00436FF8[];

extern char D_004370C0[]; /* "%5.2f" */

extern struct sdfMemHandle_t *mdlHddFileIndexHandle;

extern struct sdfMemHandle_t *mdlHddFileListHandle;

/* Five-character device prefix copied in front of each work-file path. */
typedef struct {
    char text[5];
} DebugWorkPrefix;

extern DebugWorkPrefix mdlWorkPathPrefix[];

typedef struct MdlItemCfg {
    u8 pad0;
    u8 enabled; /* 0x01 */
    u8 unk2;
    u8 unk3;
} MdlItemCfg;

extern char D_00436FC0[], D_00436FC8[];

extern s8 dds3PadBitTable[];

extern char D_00437060[];

extern char D_00437070[];

extern char D_00437078[];

extern char D_00437080[];

extern char D_00437088[];

extern char D_00437090[];

extern char D_00437098[];

extern char D_004370A0[];

extern char D_004370A8[];

extern char D_004370C8[];

extern char D_004370D0[];

extern char D_004370D8[];

extern char D_004370E0[];

extern char D_004370E8[];

extern const char D_004370F0[];

extern s32 D_004370F8;

extern s32 sdfGrAllocPacketBuff(s32 size);

extern char D_00437100[]; /* "%x" */

extern char D_00437108[]; /* "fovy=" */

extern char D_00437110[]; /* "%f" */

extern char D_00437118[]; /* "fog=" */

extern char D_00437138[]; /* ">" in .sdata */

extern char D_00437140[]; /* "%6.2f" in .sdata */

extern sdfGrOt_t D_00380708;

extern void FromRotation(f32 angle);

extern char D_00437178[];

extern const char D_00437198[];

extern const char D_004371A0[];

extern const char D_004371A8[];

extern const char D_004371B0[];

extern const char D_004371B8[];

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
    u32 *word;
    s32 remaining;

    remaining = 0x7f;
    word = DDS3_GBWK->EventBit.words;
    do {
        remaining = remaining - 1;
        *word = 0;
        word = word + 1;
    } while (-1 < remaining);
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

extern s32 D_004371C0, D_004371C4, D_004371C8;

extern char D_004371D0[], D_004371D8[], D_004371E0[];

extern s32 dds3SelectMenu(s32, s32, s32, s32, s32,
    s32 *, s32 *, s32 *, s32 *);

s32 dds3BitEditorProcess(void) {
    char glyph[2];
    sdfDrawTag_t *packets;
    s32 column, row, style, result;
    s32 cellWidth = 0xC0, cellHeight = 0x60;

    /* Native 0023A1D4..E4 copies this space/NUL template as one block;
     * see docs/idioms.md, "Bit-editor glyph template block copy". */
    memcpy(glyph, D_004371D0, sizeof(glyph));
    packets = (sdfDrawTag_t *)sdfGrAllocPacketBuff(sizeof(*packets));
    sdfDrawTagInit(packets);
    dds3ConsPanelTagLink(packets, 50, 50, 23, 20);
    sdfDrawTagLink(packets, (u32)sdfDevPrintf(
        0x7320, 0x7A90, 0xFF0000, 0, "BitEditor"));
    if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = ON",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    } else {
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7AA0, 0x7A90, 0xFF0000, 7, "%04X = OFF",
            (D_004371C0 + D_004371C8) * 16 + D_004371C4));
    }
    for (column = 0; column < 16; column++) {
        style = D_004371C4 == column ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x7320 + (column + 6) * cellWidth, 0x7A90 + cellHeight * 2,
            0xFF0000, style, D_004371D8, column));
    }
    for (row = 0; row < 16; row++) {
        style = D_004371C8 == row ? 14 : 7;
        sdfDrawTagLink(packets, (u32)sdfDevPrintf(
            0x73E0, 0x7A90 + (row + 3) * cellHeight, 0xFF0000,
            style, D_004371E0, (D_004371C0 + row) * 16));
        for (column = 0; column < 16; column++) {
            if (evtBitCheck((D_004371C0 + row) * 16 + column)) {
                glyph[0] = 'O';
                style = D_004371C4 == column || D_004371C8 == row ? 6 : 0;
            } else {
                glyph[0] = '+';
                style = D_004371C4 == column || D_004371C8 == row ? 14 : 8;
            }
            if (D_004371C4 == column && D_004371C8 == row) {
                style = 4;
            }
            sdfDrawTagLink(packets, (u32)sdfDevPrintf(
                0x7320 + (column + 6) * cellWidth,
                0x7A90 + (row + 3) * cellHeight, 0xFF0000, style, glyph));
        }
    }
    D_00380708.append(&D_00380708, packets);
    if ((dds3PadBitTable[24] & 2) && D_004371C0 > 0) {
        D_004371C0 -= 16;
        if (D_004371C0 < 0) D_004371C0 = 0;
    }
    if (dds3PadBitTable[26] & 2) {
        if (D_004371C0 + 16 < 256) {
            if (D_004371C0 + 32 > 256) D_004371C0 = 240;
            else D_004371C0 += 16;
        }
    }
    result = dds3SelectMenu(1, 16, 256, 16, 16, NULL,
        &D_004371C0, &D_004371C4, &D_004371C8);
    if (result == -1) return -1;
    if (result == 1) {
        if (evtBitCheck((D_004371C0 + D_004371C8) * 16 + D_004371C4)) {
            evtBitOff((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        } else {
            evtBitOn((D_004371C0 + D_004371C8) * 16 + D_004371C4);
        }
    }
    return 0;
}

extern dds3Basic_t *dds3AddCameraObjectBasicTypeFree(s32 world, u128 *pos, u128 *rot);

void evtSetTestCamera(void) {
    f32 pos[4] = {0.0f, -100.0f, -600.0f, 0.0f};
    f32 rot[4];
    dds3Basic_t *obj;

    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    obj = dds3AddCameraObjectBasicTypeFree(dds3AllocObjectId(), (u128 *)pos, (u128 *)rot);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3ObjectBaseCreateMover(obj);
    dds3ReplaceActiveCameraSceneBasic(dds3GetControlScene(), obj);
    dds3ObjectBaseSetPadControl(obj, 0);
}

s32 mdlSpawnCameraSlotViewerObject(s32 slotKind, s32 resource) {
    f32 pos[4];
    f32 rot[4];
    s32 world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, slotKind, resource);
    if (((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj))->first != NULL) {
        mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    }
    dds3ObjectBaseSetPadControl(obj, 0);
    return world;
}

extern s32 dds3ObjectBaseRegistArea(dds3Basic_t *obj);

EvtUnitObjectKey evtLoadUnit(s32 slotKind, s32 resource) {
    f32 pos[4];
    f32 rot[4];
    EvtUnitObjectKey world;
    dds3Basic_t *obj;

    memset(pos, 0, 0x10);
    pos[3] = 1.0f;
    memset(rot, 0, 0x10);
    rot[3] = 1.0f;
    world = dds3AllocObjectId();
    obj = dds3AddUnitObjectBasic(world, (u128 *)pos, (u128 *)rot);
    dds3ObjectBaseOnFlag(obj, 0x20);
    dds3GetRadiusPoseSet(obj, 10.0f);
    dds3RebuildObjectModelContext(obj, slotKind, resource);
    mdlLoopMotion((dds3ModelHandle_t *)dds3ObjectBaseGetModelHandle(obj), 0, 0);
    dds3ObjectBaseSetControlBasic(obj, dds3SearchSceneBasic(dds3GetControlScene(), 0x10000, 2));
    dds3ObjectBaseRegistArea(obj);
    dds3ObjectBaseSetPadControl(obj, 0);
    dds3ObjectBaseOnFlag(obj, 0x400);
    return world;
}

void evtSetUnitPos(u32 targetKey, u32 sourceKey) {
    f32 axis[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    dds3Basic_t *obj;
    dds3Basic_t *src;
    EvtWorldSourceTransformPrefix *sourceTransform;

    obj = dds3SearchSceneBasic(dds3GetControlScene(), targetKey, 5);
    if (obj != NULL) {
        src = dds3SearchSceneBasic(dds3GetControlScene(), sourceKey, 0x11);
        if (src != NULL) {
            sourceTransform = src->workdata;
            dds3SetPosPoseSet(obj, (u128 *)&sourceTransform->position);
            SDF_VU0_LOADVECTOR(axis);
            FromRotation(3.14159265f);
            SDF_VU0_LOADVECTOR_B(sourceTransform->rotation);
            Mult();
            SDF_VU0_STOREVECTOR(axis);
            dds3SetRotPoseSet(obj, (u128 *)axis);
            dds3GetPosPoseSet(obj);
            SDF_VU0_STOREVECTOR(obj->poseset->smoothedPosition);
        }
    }
}

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371C0);

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371C4);

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371C8);

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371D0);

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371D8);

INCLUDE_SDATA(const s32, "game/code_0023A028", D_004371E0);

INCLUDE_SDATA(const s32, "game/code_0023A028", evtPendingEventSelection);

