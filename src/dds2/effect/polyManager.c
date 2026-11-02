#include "sdk_ps2.h"
#include "common.h"
#include "sdf_vu0.h"
#include "par_release_record.h"
#include "par_vertex_colors.h"
#include "eff_random.h"
#include "sdf_asset_state.h"
#include "sdf_asset_create.h"
#include "sdf_chip.h"
#include "sdf_packet_list.h"
#include "sdf_packet_append.h"
#include "bill_object_api.h"
#include "par_cell_api.h"
#include "sdf_heap_api.h"
#include "par_draw.h"
#include "eff_packet_params.h"
#include "par_kind_api.h"
#include "par_kind_resource.h"
#include "par_table.h"
#include "par_draw_block.h"
#include "par_emitter_desc.h"
#include "par_object_client_view.h"
#include "eff.h"
#include "par_dispatch_api.h"
#include "eff_track_poly_list.h"
#include "eff_matrix4.h"
#include "pcp_vu0.h"
#include "ee_mmi.h"
#include "sdf_packet_alloc.h"
#include "sdf_vu_rotation_axes.h"
#include "sdf_trig.h"
#include "sdf_vu_matrix.h"
#include "sdf_vu_matrix_multiply_api.h"
#include "poly_ring_variants.h"
#include "poly_node.h"
#include "bill_work.h"
#include "eff_blur.h"
#include "sdf_packet_header.h"
#include "kwln_draw_buffer.h"
#include "kwln_frame_draw_buffer.h"
#include "sdf_column_packet_api.h"
#include "btl_sound.h"
#include "eff_param.h"
#include "kwln_sprite.h"
#include "eff_dispatch_state.h"
#include "eff_offset_triangle.h"
#include "kwln_texture_reference_flag_api.h"
#include "sdf_debug_api.h"

extern f32 D_003AAEC0[];

extern f32 D_003AAEE0[];

extern f32 D_003AAEF0[];

extern f32 D_003AAF00[];

extern dds3Polygon_t *hDrawListTop;

extern ParSystem *D_00436404;

extern void (*D_003AAF10[])(ParSystem *, u32, u32);

extern void dds3TrackPolySet2(ParSystem *, s32, const u128 *);

extern void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 cellIndex);

struct sdfDrawTag_t;

extern void *memcpy(void *dst, void *src, u32 n);

extern void dds3SpPoly_Ring_Old_BuffInit();

extern EffRandState D_00451F50;

void dds3TrackPolyInit(void);

void dds3PolygonManagerInit(void) {
    hDrawListTop = 0;
    dds3TrackPolyInit();
    sdfRandomizeByTime(&D_00451F50);
}

void dds3PolygonManagerExit(void) {
}

dds3Polygon_t *dds3PolygonCreate(s32 count, s16 flags, s16 options) {
    s32 vertexCount;
    s32 bytes;
    struct sdfMemHandle_t *allocation;
    u8 *buffer;
    dds3Polygon_t *record;
    struct sdfMat_t *asset;

    if (flags & 8) vertexCount = count + 2;
    else vertexCount = count * 3;
    bytes = vertexCount * sizeof(u128);
    if (flags & 1) bytes += vertexCount * sizeof(record->texcoords[0]);
    if (flags & 2) bytes += vertexCount * sizeof(u128);
    if (flags & 4) bytes += vertexCount * sizeof(u32);
    allocation = sdfMemAllocDirect(bytes + sizeof(*record));
    buffer = (u8 *)sdfMemLock(allocation);
    record = (dds3Polygon_t *)(buffer + bytes);
    memset(record, 0, sizeof(*record));
    record->positions = (u128 *)buffer;
    /* Allocation and layout reuse one byte extent; qualified cc1probe:
     * sandbox_ClusterX/ctor_reentry_F4_probe/decisions.log:16561-16660.
     * The saved span ties allocation at 1250, below vertex count at 3500;
     * a separate span instead has priority 3846 and takes the count's home. */
    bytes = vertexCount * sizeof(u128);
    buffer += bytes;
    if (flags & 1) {
        record->texcoords = (f32 (*)[2])buffer;
        buffer += vertexCount * sizeof(record->texcoords[0]);
    }
    if (flags & 2) record->normals = (u128 *)buffer;
    if (flags & 4) record->colors = (u32 *)buffer;
    record->flags = flags;
    record->capacity = vertexCount;
    record->allocation = allocation;
    record->drawCount = 0;
    memset(&record->request, 0, sizeof(record->request));
    record->request.color = 0x80808080;
    if (!(options & 1)) record->request.attr = 0x4000;
    asset = sdfMatNew();
    record->asset = asset;
    if (!(options & 2)) sdfMatSetConstant(asset, 1.0f);
    return record;
}

void dds3PolygonDelete(dds3Polygon_t *record) {
    record->flags = 1;
    sdfMatFreeDelayed(record->asset);
    sdfMemFreeDirect(record->allocation);
}

void dds3PolygonDraw(dds3Polygon_t *node) {
    node->next = hDrawListTop;
    hDrawListTop = node;
}

extern sdfGrOt_t sdfGrOtTable[0x62];

static inline void parAppendDrawSurfacePool(sdfGrOt_t *pool, sdfDrawTag_t *packet) {
    pool->append(pool, packet);
}

extern void *effPkt3DPrimMakePoly(sdf3DPrimPolyParm_t *);

void dds3PolygonLinkOt(void) {
    dds3Polygon_t *record;
    sdfDrawTag_t *packet;
    s32 remaining;
    s16 flags;
    sdfGrOt_t *priorityPool;
    record = hDrawListTop;
    if (record != NULL) {
        priorityPool = &sdfGrOtTable[51];
        do {
            if (record->drawCount != 0 && record->flags != 1) {
                packet = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
                sdfDrawTagInit(packet);
                sdf3DPrimLinkResetTm(packet, 0);
                sdf3DPrimLinkMat(packet, record->asset, 0);
                flags = record->flags;
                remaining = record->drawCount;
                record->request.vertex = record->positions;
                record->request.uv = record->texcoords[0];
                record->request.vertexColor = record->colors;
                record->request.normal = record->normals;
                record->request.faceCount = 16;
                record->request.pointCount = 48;
                while (remaining >= 48) {
                    sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&record->request));
                    record->request.vertex += 48;
                    if (flags & 1) record->request.uv += 96;
                    if (flags & 2) record->request.normal += 48;
                    remaining -= 48;
                    if (flags & 4) record->request.vertexColor += 48;
                }
                if (remaining > 0) {
                    record->request.faceCount = remaining / 3;
                    record->request.pointCount = remaining;
                    sdfDrawTagLink(packet, (u32)effPkt3DPrimMakePoly(&record->request));
                }
                record->drawCount = 0;
                if (flags & 0x10) parAppendDrawSurfacePool(priorityPool, packet);
                else if (flags & 0x20) parAppendDrawSurfacePool(&sdfGrOtTable[55], packet);
                else parAppendDrawSurfacePool(&sdfGrOtTable[39], packet);
            }
            record = record->next;
        } while (record != NULL);
    }
    hDrawListTop = NULL;
}

/* Replace the record asset's primary texture and dirty its draw state. */
void dds3PolygonSetTex(dds3Polygon_t *record, sdfTexHandle_t *texture) {
    sdfSetAssetPrimaryTextureAddress(record->asset, texture);
}

/* Draw parameter block filled per strip by dds3TrackPolyLinkOt. */
extern sdf3DPrimPolyParm_t parDrawControl;

void dds3TrackPolyInit(void) {
    memset(&parDrawControl, 0, 0x2C);
    parDrawControl.attr = 0x4000;
}

ParSystem *dds3TrackPolyCreate(s32 count, s32 perCell, s32 groupDivisor, u32 kind) {
    s32 total;
    struct sdfMemHandle_t *allocation;
    s32 base;
    s32 cellsSize;
    ParSystem *system;
    s32 i;

    if (kind == PAR_CELL_TOPOLOGY_FIVE_VECTOR) {
        perCell = perCell * 5 + 5;
    } else if (kind == PAR_CELL_TOPOLOGY_FOUR_VECTOR) {
        perCell = perCell * 4 + 4;
    } else if (kind == PAR_CELL_TOPOLOGY_SIX_VECTOR) {
        perCell = perCell * 6 + 6;
    } else {
        perCell = perCell * 2;
        if (groupDivisor != 0) {
            if (perCell % groupDivisor != 0) {
                perCell = perCell / groupDivisor + 2;
            } else {
                perCell = perCell / groupDivisor;
            }
        }
        perCell &= ~1;
        perCell += 2;
        if (kind == PAR_CELL_TOPOLOGY_TRIANGLE) {
            perCell += perCell >> 1;
        }
    }
    total = count * perCell;
    cellsSize = (total + count) * 0x14;
    allocation = sdfMemAllocDirect(cellsSize + 0x2C);
    base = sdfMemLock(allocation);
    system = (ParSystem *)(base + cellsSize);
    memset(system, 0, 0x2C);
    system->vertices = (u128 *)base;
    base += total * 0x10;
    system->colors = (u32 *)base;
    base += total * 4;
    system->cells = (ParCell *)base;
    for (i = 0; i < count; i++) {
        ParCell *cell = (ParCell *)(i * sizeof(ParCell) + (s32)system->cells);
        cell->history = (u128 *)((u8 *)system->vertices + i * perCell * 0x10);
        cell->colors = system->colors + i * perCell;
        dds3TrackPolyListInit(system, i);
    }
    system->asset = sdfMatNew();
    sdfMatSetConstant(system->asset, 1.0f);
    system->kind = kind;
    system->cellCount = count;
    system->bucket = 2;
    system->vertexWordCount = perCell;
    system->groupDivisor = groupDivisor;
    system->allocation = allocation;
    system->next = 0;
    system->unk28 = 0;
    return system;
}

void dds3TrackPolyDelete(ParSystem *system) {
    sdfMatFreeDelayed(system->asset);
    sdfMemFreeDirect(system->allocation);
}

void dds3TrackPolyListInit(ParSystem *system, s32 cellIndex) {
    ParCell *cell = (ParCell *)(cellIndex * sizeof(ParCell) + (s32)system->cells);

    cell->color = 0x80808080;
    cell->historyAdvanceCountdown = 0;
    cell->vertexCount = 0;
}

void dds3TrackPolyDraw(ParSystem *node) {
    node->next = D_00436404;
    D_00436404 = node;
}

void dds3TrackPolySet(ParSystem *system, s32 cellIndex, const u128 *vertices) {
    ParCell *cell = &system->cells[cellIndex];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 2;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 2, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 2) {
            cell->vertexCount += 2;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    SDF_VU0_COPYVECTOR(vertex, vertices);
    SDF_VU0_COPYVECTOR(vertex + 1, vertices + 1);
}

void dds3TrackPolyMove(ParSystem *system, s32 cellIndex, const u128 *delta) {
    ParCell *cell = system->cells + cellIndex;
    s32 count = system->vertexWordCount;
    u128 *vertex = cell->history;
    s32 i;
    SDF_VU0_LOADVECTOR_B_BARRIER(delta);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            i--;
            vertex++;
        } while (i != 0);
    }
}

void dds3TrackPolySet2(ParSystem *system, s32 cellIndex, const u128 *vertices) {
    ParCell *cell = &system->cells[cellIndex];
    u128 *vertex;
    s32 shiftCount;
    s32 i;

    if (cell->historyAdvanceCountdown == 0) {
        shiftCount = system->vertexWordCount - 3;
        vertex = cell->history + shiftCount;
        for (i = 0; i < shiftCount; i++) {
            vertex--;
            SDF_VU0_COPYVECTOR(vertex + 3, vertex);
        }
        cell->historyAdvanceCountdown = system->groupDivisor;
        if (cell->vertexCount < shiftCount + 3) {
            cell->vertexCount += 3;
        }
    } else {
        cell->historyAdvanceCountdown--;
        vertex = cell->history;
    }
    SDF_VU0_COPYVECTOR(vertex, vertices);
    SDF_VU0_COPYVECTOR(vertex + 1, vertices + 1);
    SDF_VU0_COPYVECTOR(vertex + 2, vertices + 2);
}

void dds3TrackPolyMove2(ParSystem *system, s32 cellIndex, const u128 *delta) {
    ParCell *cell = system->cells + cellIndex;
    s32 count = cell->vertexCount / 3;
    u128 *vertex = cell->history;
    s32 i;
    SDF_VU0_LOADVECTOR_B(delta);
    if (count > 0) {
        i = count;
        do {
            SDF_VU0_LOADVECTOR(vertex);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex);
            SDF_VU0_LOADVECTOR(vertex + 1);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 1);
            SDF_VU0_LOADVECTOR(vertex + 2);
            SDF_VU0_ADDVECTOR();
            SDF_VU0_STOREVECTOR(vertex + 2);
            i--;
            vertex += 3;
        } while (i != 0);
    }
}

void dds3TrackPolyVCRefresh_ParType1(ParSystem *system, s32 cellIndex) {
    u32 count = system->cells[cellIndex].vertexCount >> 1;
    u32 *vertex = system->cells[cellIndex].colors;
    u32 word = vertex[0];
    u32 i;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    if (count >= 2) {
        alpha[0] = word & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAEC0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_COPYVECTOR_A_TO_C();
        start[0] = word;
        EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
        for (i = 0; i < count; i++) {
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
            out[0] = packed;
            vertex[0] = packed;
            vertex[1] = packed;
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_COPYVECTOR_B_TO_A();
            vertex += 2;
        }
    }
}

extern f32 D_003AAED0[4];

/* Fade one triangle cell's middle and edge colors toward transparent along its history. */
void dds3TrackPolyVCRefresh_ParType2(ParSystem *system, s32 cellIndex) {
    u32 count = system->cells[cellIndex].vertexCount / 3;
    ParTriangleVertexColors *vertex;
    u32 middleWord;
    u32 edgeWord;
    f32 cur0[4];
    f32 cur1[4];
    f32 step0[4];
    f32 step1[4];
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    u32 j;
    u32 packedMiddle;
    u32 packedEdge;

    if (count >= 2) {
        vertex = (ParTriangleVertexColors *)system->cells[cellIndex].colors;
        middleWord = vertex->middle;
        edgeWord = vertex->edge0;
        alpha0[0] = middleWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_STOREVECTOR(step0);
        alpha1[0] = edgeWord & 0xFF000000;
        EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(D_003AAED0);
        VU0_LERP_VF10(1.0f / count);
        SDF_VU0_STOREVECTOR_NOMEM(step1);
        col0[0] = middleWord;
        EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(cur0);
        col1[0] = edgeWord;
        EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
        SDF_VU0_STOREVECTOR_NOMEM(cur1);
        for (j = 0; j < count; j++) {
            SDF_VU0_LOADVECTOR(cur0);
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
            out0[0] = packedMiddle;
            vertex->middle = packedMiddle;
            SDF_VU0_LOADVECTOR_C(step0);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(cur0);
            SDF_VU0_LOADVECTOR(cur1);
            SDF_VU0_COPYVECTOR_A_TO_B();
            EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
            out1[0] = packedEdge;
            vertex->edge0 = packedEdge;
            vertex->edge1 = packedEdge;
            SDF_VU0_LOADVECTOR_C(step1);
            SDF_VU0_SUBVECTOR_B_C();
            SDF_VU0_STOREVECTOR_B(cur1);
            vertex++;
        }
    }
}

void dds3TrackPolyVCSet_Normal(ParSystem *system, u32 firstColor, u32 secondColor) {
    s32 perCell = system->vertexWordCount >> 1;
    s32 cellCount = system->cellCount;
    s32 cellIndex;

    for (cellIndex = 0; cellIndex < cellCount; cellIndex++) {
        u32 *vertices = system->cells[cellIndex].colors;
        s32 i;

        for (i = 0; i < perCell; i++) {
            vertices[0] = firstColor;
            vertices[1] = secondColor;
            vertices += 2;
        }
    }
}

void dds3TrackPolyVCSet_Acute(ParSystem *system, u32 color) {
    u32 perCell = system->vertexWordCount >> 1;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 out[4];
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAEE0);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_COPYVECTOR_A_TO_C();
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = color;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < perCell; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                out[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet_AcuteW(ParSystem *system, u32 color) {
    u32 words = system->vertexWordCount;
    u32 count = system->cellCount;
    u32 quarter = words >> 2;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    s32 alpha[4];
    s32 start[4];
    s32 rise[4];
    s32 fall[4];
    u32 base;
    u32 odd;
    u32 packed;
    alpha[0] = color & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAEF0);
    VU0_LERP_VF10(1.0f / quarter);
    SDF_VU0_COPYVECTOR_A_TO_C();
    i = 0;
    if (count != 0) {
        base = color & 0xFFFFFF;
        odd = (words >> 1) & 1;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            start[0] = base;
            EE_MMI_RGBA_UNPACK(start, 1.0f / 128.0f);
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                rise[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_ADDVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            if (odd) {
                vertex[0] = color;
                vertex[1] = color;
                vertex += 2;
            }
            for (j = 0; j < quarter; j++) {
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
                fall[0] = packed;
                vertex[0] = packed;
                vertex[1] = packed;
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_COPYVECTOR_B_TO_A();
                vertex += 2;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_Normal(ParSystem *system, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 3;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    *(s32 *)(vertex + 4) = middleWord;
                    *(s32 *)(vertex + 8) = edgeWord;
                    *(s32 *)(vertex + 0) = edgeWord;
                    vertex += 0xC;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void dds3TrackPolyVCSet2_Acute(ParSystem *system, u32 middleWord, u32 edgeWord) {
    f32 cur0[4];
    f32 cur1[4];
    f32 step0[4];
    f32 step1[4];
    s32 alpha0[4];
    s32 alpha1[4];
    s32 col0[4];
    s32 col1[4];
    s32 out0[4];
    s32 out1[4];
    s32 groups = 3;
    u32 perCell = system->vertexWordCount / groups;
    u32 count = system->cellCount;
    u32 i;
    u32 j;
    u8 *cell;
    u32 *vertex;
    u32 packedMiddle;
    u32 packedEdge;
    alpha0[0] = middleWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha0, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_STOREVECTOR(step0);
    alpha1[0] = edgeWord & 0xFF000000;
    EE_MMI_RGBA_UNPACK(alpha1, 1.0f / 128.0f);
    SDF_VU0_COPYVECTOR_A_TO_B();
    SDF_VU0_LOADVECTOR(D_003AAF00);
    VU0_LERP_VF10(1.0f / perCell);
    SDF_VU0_STOREVECTOR(step1);
    i = 0;
    if (count != 0) {
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u32 **)cell;
            col0[0] = middleWord;
            EE_MMI_RGBA_UNPACK(col0, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(cur0);
            col1[0] = edgeWord;
            EE_MMI_RGBA_UNPACK(col1, 1.0f / 128.0f);
            SDF_VU0_STOREVECTOR(cur1);
            for (j = 0; j < perCell; j++) {
                SDF_VU0_LOADVECTOR(cur0);
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packedMiddle, 128.0f);
                out0[0] = packedMiddle;
                vertex[1] = packedMiddle;
                SDF_VU0_LOADVECTOR_C(step0);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(cur0);
                SDF_VU0_LOADVECTOR(cur1);
                SDF_VU0_COPYVECTOR_A_TO_B();
                EE_MMI_RGBA_PACK_UNIT(packedEdge, 128.0f);
                out1[0] = packedEdge;
                vertex[0] = packedEdge;
                vertex[2] = packedEdge;
                SDF_VU0_LOADVECTOR_C(step1);
                SDF_VU0_SUBVECTOR_B_C();
                SDF_VU0_STOREVECTOR_B(cur1);
                vertex += 3;
            }
            i++;
            cell += 0x14;
        } while (i < count);
    }
}

void dds3TrackPolyVCSet2_AcuteW(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 halfCount = (words / 3) >> 1;
    u32 middleStep = middleAlpha / halfCount;
    u32 edgeStep = edgeAlpha / halfCount;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParTriangleVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParTriangleVertexColors **)cell;
            middle = 0;
            edge = 0;
            for (j = 0; j < halfCount; j++, vertex++) {
                vertex->middle = middleWord | (middle & 0xFF000000);
                vertex->edge0 = vertex->edge1 = edgeWord | (edge & 0xFF000000);
                middle += middleStep;
                edge += edgeStep;
            }
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < halfCount; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle = middleWord | (middle & 0xFF000000);
                vertex->edge0 = vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void dds3TrackPolyVC3Set_Normal(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 6;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    ((ParStripVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParStripVertexColors *)vertex)->middle0 = middleWord;
                    ((ParStripVertexColors *)vertex)->center0 = centerWord;
                    ((ParStripVertexColors *)vertex)->center1 = centerWord;
                    ((ParStripVertexColors *)vertex)->middle1 = middleWord;
                    ((ParStripVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x18;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Fade the three strip colors toward transparent across each cell. */
void dds3TrackPolyVC3Set_Acute(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 6;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParStripVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParStripVertexColors **)cell;
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Raise then lower the three alphas across the two halves of each strip cell. */
void dds3TrackPolyVC3Set_AcuteW(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 half = (words / 6) >> 1;
    u32 centerStep = centerAlpha / half;
    u32 middleStep = middleAlpha / half;
    u32 edgeStep = edgeAlpha / half;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParStripVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParStripVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < half; j++, vertex++) {
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
            }
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < half; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center0 = centerWord | (center & 0xFF000000);
                vertex->center1 = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void dds3TrackPolyVCSet4_Normal(ParSystem *system, u32 firstColor, u32 secondColor) {
    s32 perCell = system->vertexWordCount >> 2;
    s32 cellCount = system->cellCount;

    s32 cellIndex;

    for (cellIndex = 0; cellIndex < cellCount; cellIndex++) {
        u32 *vertices = system->cells[cellIndex].colors;
        s32 i;

        for (i = 0; i < perCell; i++) {
            vertices[2] = firstColor;
            vertices[1] = firstColor;
            vertices[3] = secondColor;
            vertices[0] = secondColor;
            vertices += 4;
        }
    }
}

/* Keep middle/edge RGB while ramping vertex alpha down to zero in each cell. */
void dds3TrackPolyVCSet4_Acute(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 perCell = system->vertexWordCount >> 2;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParQuadVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParQuadVertexColors **)cell;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Keep middle/edge RGB while ramping alpha up and down around the cell midpoint. */
void dds3TrackPolyVCSet4_AcuteW(ParSystem *system, u32 middleWord, u32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 halfCount = (words >> 2) >> 1;
    u32 middleStep = middleAlpha / halfCount;
    u32 edgeStep = edgeAlpha / halfCount;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParQuadVertexColors *vertex;

    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParQuadVertexColors **)cell;
            middle = 0;
            edge = 0;
            for (j = 0; j < halfCount; j++, vertex++) {
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                middle += middleStep;
                edge += edgeStep;
            }
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < halfCount; j++, vertex++) {
                middle -= middleStep;
                edge -= edgeStep;
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void dds3TrackPolyVC5Set_Normal(ParSystem *system, s32 centerWord, s32 middleWord, s32 edgeWord) {
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    s32 perCell = words / 5;
    s32 i;
    s32 j;
    u8 *cell;
    u8 *vertex;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(u8 **)cell;
            if (perCell > 0) {
                j = perCell;
                do {
                    j--;
                    ((ParSymmetricVertexColors *)vertex)->edge0 = edgeWord;
                    ((ParSymmetricVertexColors *)vertex)->middle0 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->center = centerWord;
                    ((ParSymmetricVertexColors *)vertex)->middle1 = middleWord;
                    ((ParSymmetricVertexColors *)vertex)->edge1 = edgeWord;
                    vertex += 0x14;
                } while (j != 0);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Fade the three symmetric colors toward transparent across each cell. */
void dds3TrackPolyVC5Set_Acute(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 5;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < perCell; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Increase each symmetric color's alpha from zero across the cell. */
void dds3TrackPolyVC5Set_AcuteR(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 perCell = words / 5;
    u32 centerStep = centerAlpha / perCell;
    u32 middleStep = middleAlpha / perCell;
    u32 edgeStep = edgeAlpha / perCell;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < perCell; j++, vertex++) {
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

/* Raise then lower the three alphas across the two halves of each cell. */
void dds3TrackPolyVC5Set_AcuteW(ParSystem *system, u32 centerWord, u32 middleWord, u32 edgeWord)
{
    s32 words = system->vertexWordCount;
    s32 count = system->cellCount;
    u32 centerAlpha = centerWord & 0xFF000000;
    u32 middleAlpha = middleWord & 0xFF000000;
    u32 edgeAlpha = edgeWord & 0xFF000000;
    s32 half = (words / 5) >> 1;
    u32 centerStep = centerAlpha / half;
    u32 middleStep = middleAlpha / half;
    u32 edgeStep = edgeAlpha / half;
    u32 center;
    u32 middle;
    u32 edge;
    s32 i;
    s32 j;
    u8 *cell;
    ParSymmetricVertexColors *vertex;

    centerWord &= 0xFFFFFF;
    middleWord &= 0xFFFFFF;
    edgeWord &= 0xFFFFFF;
    if (count > 0) {
        i = count;
        cell = (u8 *)system->cells + 4;
        do {
            vertex = *(ParSymmetricVertexColors **)cell;
            center = 0;
            middle = 0;
            edge = 0;
            for (j = 0; j < half; j++, vertex++) {
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
                center += centerStep;
                middle += middleStep;
                edge += edgeStep;
            }
            center = centerAlpha;
            middle = middleAlpha;
            edge = edgeAlpha;
            for (j = 0; j < half; j++, vertex++) {
                center -= centerStep;
                middle -= middleStep;
                edge -= edgeStep;
                vertex->edge0 = edgeWord | (edge & 0xFF000000);
                vertex->middle0 = middleWord | (middle & 0xFF000000);
                vertex->center = centerWord | (center & 0xFF000000);
                vertex->middle1 = middleWord | (middle & 0xFF000000);
                vertex->edge1 = edgeWord | (edge & 0xFF000000);
            }
            i--;
            cell += 0x14;
        } while (i != 0);
    }
}

void dds3TrackPolySetMode(ParSystem *system, u16 bucket) {
    system->bucket = bucket;
}

void dds3TrackPolyVCSet(ParSystem *system, s32 sub, u32 colorWordA, u32 colorWordB) {
    u16 id = system->kind;

    D_003AAF10[id * 3 + sub](system, colorWordA, colorWordB);
}

extern const u32 D_003AAC90[];

extern const u32 D_003AAD10[];

extern const u32 D_003AAE50[];

extern const u32 D_003AAD80[];

extern const u32 D_003AAE00[];

extern sdfGrOt_t *D_003AAF50[];

/* Batch pending cell systems by topology, then submit the five draw buckets. */
void dds3TrackPolyLinkOt(void) {
    sdfDrawTag_t *lists[5];
    sdfDrawTag_t **slot;
    sdfDrawTag_t *list;
    sdfDrawTag_t *specialList;
    s32 count;
    s32 i;
    s32 remaining;
    ParSystem *system;
    ParCell *cell;
    u64 *packet;

    if (D_00436404 == NULL) {
        return;
    }
    memset(lists, 0, sizeof(lists));
    for (system = D_00436404; system != NULL; system = system->next) {
        slot = &lists[system->bucket];
        list = *slot;
        if (list == 0) {
            *slot = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
            sdfDrawTagInit(*slot);
            sdf3DPrimLinkResetTm(*slot, NULL);
            list = *slot;
        }
        sdf3DPrimLinkMat(list, (sdfMat_t *)system->asset, NULL);
        count = system->cellCount;
        if (system->kind == PAR_CELL_TOPOLOGY_PAIR) {
            parDrawControl.faceIndex = D_003AAC90;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.faceCount = 16;
                parDrawControl.pointCount = 18;
                remaining = cell->vertexCount;
                parDrawControl.vertex = cell->history;
                parDrawControl.vertexColor = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 18) {
                    remaining -= 16;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.vertex += 16;
                    parDrawControl.vertexColor += 16;
                }
                if (remaining >= 4) {
                    parDrawControl.faceCount = remaining - 2;
                    parDrawControl.pointCount = remaining;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_TRIANGLE) {
            parDrawControl.faceIndex = D_003AAD10;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.faceCount = 16;
                parDrawControl.pointCount = 15;
                remaining = cell->vertexCount;
                parDrawControl.vertex = cell->history;
                parDrawControl.vertexColor = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 15) {
                    remaining -= 12;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.vertex += 12;
                    parDrawControl.vertexColor += 12;
                }
                if (remaining >= 6) {
                    parDrawControl.faceCount = (remaining / 3) * 4 - 4;
                    parDrawControl.pointCount = remaining;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_FIVE_VECTOR) {
            parDrawControl.faceIndex = D_003AAE50;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.faceCount = 8;
                parDrawControl.pointCount = 10;
                remaining = cell->vertexCount;
                parDrawControl.vertex = cell->history;
                parDrawControl.vertexColor = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 10) {
                    remaining -= 5;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.vertex += 5;
                    parDrawControl.vertexColor += 5;
                }
            }
        } else if (system->kind == PAR_CELL_TOPOLOGY_SIX_VECTOR) {
            parDrawControl.faceIndex = D_003AAD80;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.faceCount = 10;
                parDrawControl.pointCount = 12;
                remaining = cell->vertexCount;
                parDrawControl.vertex = cell->history;
                parDrawControl.vertexColor = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 12) {
                    remaining -= 6;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.vertex += 6;
                    parDrawControl.vertexColor += 6;
                }
            }
        } else {
            parDrawControl.faceIndex = D_003AAE00;
            for (i = 0; i < count; i++) {
                cell = &system->cells[i];
                parDrawControl.faceCount = 12;
                parDrawControl.pointCount = 16;
                remaining = cell->vertexCount;
                parDrawControl.vertex = cell->history;
                parDrawControl.vertexColor = cell->colors;
                parDrawControl.color = cell->color;
                while (remaining >= 16) {
                    remaining -= 12;
                    sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&parDrawControl));
                    parDrawControl.vertex += 12;
                    parDrawControl.vertexColor += 12;
                }
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (lists[i] != 0) {
            parAppendDrawSurfacePool(D_003AAF50[i], lists[i]);
        }
    }
    if (lists[4] != 0) {
        specialList = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
        sdfDrawTagInit(specialList);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x8000000026ULL;
        packet[5] = 0x42;
        sdfDrawTagLink(specialList, (u32)packet);
        parAppendDrawSurfacePool(&sdfGrOtTable[56], specialList);
        packet = (u64 *)sdfGrAllocPacketBuff(0x30);
        packet[0] = 2;
        packet[1] = 0x5000000210000000ULL;
        packet[2] = 0x1000000000008001ULL;
        packet[3] = 0xE;
        packet[4] = 0x42;
        packet[5] = 0x42;
        sdfDrawTagLink(lists[4], (u32)packet);
        parAppendDrawSurfacePool(&sdfGrOtTable[56], lists[4]);
    }
    D_00436404 = NULL;
}

ParBlock *dds3TrianglePolyCreate(s32 count) {
    s32 points = count * 3;
    s32 colorBytes = points * 4;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect((colorBytes + points) * 4 + 0x18);
    u8 *base = (u8 *)sdfMemLock(allocation);
    u8 *vertices = base + points * 16;
    ParBlock *block = (ParBlock *)(vertices + colorBytes);
    block->color = 0x80808080;
    block->count = count;
    block->colors = (u32 *)vertices;
    block->allocation = allocation;
    block->positions = (u128 *)base;
    block->asset = sdfMatNew();
    sdfMatSetConstant(block->asset, 1.0f);
    return block;
}

void dds3TrianglePolyDelete(ParBlock *block) {
    sdfMatFreeDelayed(block->asset);
    sdfMemFreeDirect(block->allocation);
}

void dds3TrianglePolyLinkOt(sdfGrOt_t *emitter, ParBlock *cmd) {
    sdfDrawTag_t *list = (sdfDrawTag_t *)sdfGrAllocPacketBuff(0x20);
    sdf3DPrimPolyParm_t state;
    s32 remaining;
    sdfDrawTagInit(list);
    sdf3DPrimLinkResetTm(list, 0);
    remaining = cmd->count * 3;
    memset(&state, 0, sizeof(state));
    state.faceCount = 0x10;
    state.pointCount = 0x30;
    state.attr = 0x4000;
    state.vertex = cmd->positions;
    state.vertexColor = cmd->colors;
    state.color = cmd->color;
    while (remaining >= 0x30) {
        remaining -= 0x30;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&state));
    }
    if (remaining > 0) {
        parDrawControl.faceCount = remaining / 3;
        parDrawControl.pointCount = remaining;
        sdfDrawTagLink(list, (u32)effPkt3DPrimMakePoly(&state));
    }
    parAppendDrawSurfacePool(emitter, list);
}

ParEmitDesc *dds3SpPoly_Ring_Old_Create(ParEmitDesc *src) {
    ParEmitDesc *desc = sdfChipAllocDirect(src->count * 4 + 0xF0);

    memset(desc, 0, 0xF0);
    memcpy(desc, src, src->headerSize);
    memcpy(&desc->cells, (u8 *)src + src->headerSize, sizeof(desc->cells));
    desc->headerSize = 0xC0;
    desc->cells.cellWords = (u32 *)(desc + 1);
    if (desc->cells.verticesPerCell < 3) {
        desc->cells.verticesPerCell = 3;
    }
    desc->cells.cellSystem = dds3TrackPolyCreate(desc->count, desc->cells.verticesPerCell, 1, PAR_CELL_TOPOLOGY_PAIR);
    dds3TrackPolyVCSet(desc->cells.cellSystem, 0,
                   *(u32 *)((u8 *)&desc->cells + 0x14),
                   *(u32 *)((u8 *)&desc->cells + 0x18));
    dds3SpPoly_Ring_Old_BuffInit(desc);
    return desc;
}

/* Keeps its own declaration of the functions below (see sdk_libc.h note). */

#define SDK_SKIP_memcpy

enum {
    POLY_INACTIVE_ENTRY_AGE = -0xFFFFFF,
    POLY_RESET_ENTRY_AGE = 0xFFFFFF0,
    POLY_RESET_DURATION = 0xFFFFFFF
};

void dds3SpPoly_Ring_Old_Calc(PolyNode *node);

void dds3SpPoly_Ring_Old_New(PolyNode *node, s32 cellIndex);

void dds3SpPoly_Ring_Old_Move(PolyNode *node, s32 cellIndex);


void dds3SpPoly_RingAllBuffInit(PolyBand *band);

void dds3SpPoly_SphereRingAllBuffInit(PolyArc *arc);

void dds3SpPoly_RollRingAllBuffInit(PolyRotatingBand *rotatingBand);

/* Release the cell system, then the basic node itself. */
void dds3SpPoly_Ring_Old_Delete(PolyNode *node) {
    dds3TrackPolyDelete(node->strip);
    sdfChipFreeDirect(node);
}

/* The cell initializer writes the same ages that the basic-ring update reads. */
void dds3SpPoly_Ring_Old_BuffInit(PolyNode *node) {
    u16 count;
    s32 age;
    s32 *ages;
    u16 i;
    ParSystem *system;
    ParCell *cells;

    count = node->entryCount;
    age = POLY_INACTIVE_ENTRY_AGE;
    ages = node->ages;
    system = node->strip;
    i = 0;
    if (node->entryCount != 0) {
        cells = system->cells;
        do {
            cells[i].historyAdvanceCountdown = 0;
            cells[i].vertexCount = 0;
            cells[i].color = 0x00808080;
            *ages = age;
            ages++;
            age -= node->spawnDelayStep;
            i++;
        } while (i < count);
    }
}

/* Scale the basic ring radius and its point-pair displacement. */
void dds3SpPoly_Ring_Old_SetScale(f32 scale, PolyNode *node) {
    node->radius = node->radius * scale;
    node->pairDisplacement = node->pairDisplacement * scale;
}

/* Fade active basic-ring cells, update their geometry, and restart or finish ages. */
void dds3SpPoly_Ring_Old_Calc(PolyNode *node) {
    u16 index;
    u16 completed;
    u16 count;
    u8 loop;
    s32 duration;
    f32 alphaStep;
    ParCell *entry;
    s32 *ages;

    if (node->active != POLY_ACTIVITY_INACTIVE) {
        duration = node->duration;
        completed = 0;
        count = node->entryCount;
        entry = node->strip->cells;
        alphaStep = 128.0f / (f32)duration;
        ages = node->ages;
        loop = node->loop;
        for (index = 0; index < count; index++) {
            s32 age = *ages;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_Ring_Old_New(node, index);
                *ages = 0;
            } else if (age >= 0) {
                entry->color = ((u32)(alphaStep * (f32)(duration - age)) << 24) | 0x808080;
                dds3SpPoly_Ring_Old_Move(node, index);
            }
            if (age >= duration) {
                entry->color = 0x808080;
                if (loop != 0) {
                    *ages = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= count) {
                        entry->color = 0;
                        node->active = POLY_ACTIVITY_INACTIVE;
                    }
                }
            } else {
                (*ages)++;
            }
            ages++;
            entry++;
        }
    }
}

/* Run the shared finish step, then enqueue this node's cell system. */
void dds3SpPoly_Ring_Old_Draw(PolyNode *node) {
    dds3SpPoly_Ring_Old_Calc(node);
    dds3TrackPolyDraw(node->strip);
}

void dds3SpPoly_Ring_Old_New(PolyNode *node, s32 cellIndex) {
    ParSystem *strip = node->strip;
    ParCell *cell = &strip->cells[cellIndex];
    f32 radius = node->radius;
    f32 origin[4];
    f32 offset[4];
    f32 angle;
    f32 step;
    f32 cosine;
    f32 sine;
    u128 *p;
    u128 *hist;
    s32 half;
    s32 pairs;
    s32 i;

    step = EFFECT_RING_FULL_TURN / (f32)node->segments;
    cell->vertexCount = strip->vertexWordCount;
    p = cell->history;
    half = strip->vertexWordCount / 2;
    VU0_LOAD_MATRIX(node->matrix);
    angle = 0.0f;
    PCP_COPY_VECTOR_F32(origin, node->origin);
    pairs = half - 1;
    for (i = 0; i < pairs; i++) {
        cosine = sdfCos(angle);
        sine = sdfSin(angle);
        SDF_VU0_LOADVECTOR_B(origin);
        SDF_VU0_STOREVECTOR_B(p);
        offset[0] = cosine * radius;
        offset[1] = 0.0f;
        offset[2] = sine * radius;
        SDF_VU0_LOADVECTOR(offset);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p + 1);
        p += 2;
        angle += step;
    }
    hist = cell->history;
    SDF_VU0_COPYVECTOR(p, hist);
    SDF_VU0_COPYVECTOR(p + 1, hist + 1);
}

/* Apply the same displacement to both points of each pair along their separation direction. */
void dds3SpPoly_Ring_Old_Move(PolyNode *node, s32 cellIndex) {
    ParSystem *strip = node->strip;
    ParCell *entry = &strip->cells[cellIndex];
    f32 scale[4];
    f32 *p;
    s32 pairs;
    s32 i;

    p = (f32 *)entry->history;
    pairs = strip->vertexWordCount / 2;
    scale[0] = scale[1] = scale[2] = node->pairDisplacement;
    SDF_VU0_LOADVECTOR_C(scale);
    for (i = 0; i < pairs; i++) {
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_LOADVECTOR_B(p);
        SDF_VU0_SUBVECTOR();
        SDF_VU0_NORMALIZE();
        SDF_VU0_COPYVECTOR_C_TO_B();
        SDF_VU0_MULVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_B();
        SDF_VU0_LOADVECTOR(p);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p);
        SDF_VU0_LOADVECTOR(p + 4);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(p + 4);
        p += 8;
    }
}

extern f32 polyTimedTintEndColor[4];

extern f32 polyTimedTintStartColor[4];

/* Blend two constant colour vectors by elapsed/duration (1 once elapsed reaches duration) and tint the result with the packed colour. */
u32 dds3SpPoly_Color(u32 elapsed, u32 duration, u32 color) {
    f32 ratio = 1.0f;
    s32 tint[4];
    u8 unused[16]; /* retail frame 0x20: unused local storage */
    u32 packed;

    if (elapsed < duration) {
        ratio = (f32)elapsed / (f32)duration;
    }
    SDF_VU0_LOADVECTOR(polyTimedTintStartColor);
    SDF_VU0_LOADVECTOR_B(polyTimedTintEndColor);
    VU0_SCALE_VF(vf10, 1.0f - ratio);
    VU0_SCALE_VF(vf11, ratio);
    SDF_VU0_ADDVECTOR();
    SDF_VU0_COPYVECTOR_A_TO_B();
    tint[0] = color;
    EE_MMI_RGBA_UNPACK(tint, 1.0f / 128.0f);
    SDF_VU0_MULVECTOR();
    EE_MMI_RGBA_PACK_UNIT(packed, 128.0f);
    return packed;
}

/* Create a band from its template: the template body, then the 0x40-byte parameter tail that follows it, with eight-byte records after the header. */
PolyBand *dds3SpPoly_RingCreate(PolyRingHead *templateHead) {
    u32 size = templateHead->entryCount * sizeof(PolyBandRecord) + sizeof(PolyBand);
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    PolyBand *band = (PolyBand *)sdfMemLock(allocation);

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, 0x40);
    band->head.templateSize = sizeof(PolyRingHead);
    band->records = (PolyBandRecord *)(band + 1);
    band->allocation = allocation;
    if (((PolyRing *)band)->band.segments < 3) {
        ((PolyRing *)band)->band.segments = 3;
    }
    band->strip = dds3TrackPolyCreate(band->head.entryCount, band->segments, 1, 0);
    dds3TrackPolyVCSet(band->strip, 0, band->firstColor, band->secondColor);
    dds3SpPoly_RingAllBuffInit(band);
    VU0_LOAD_MATRIX(band->head.matrix);
    VU0_STORE_MATRIX(band->head.matrixCopy);
    dds3TrackPolySetMode(band->strip, band->head.drawBucket);
    return band;
}

/* Release the band's cell system and backing allocation handle, not the node itself. */
void dds3SpPoly_RingDelete(PolyBand *band) {
    dds3TrackPolyDelete(band->strip);
    sdfMemFreeDirect(band->allocation);
}

/* Seed band records with staggered inactive ages. */
void dds3SpPoly_RingAllBuffInit(PolyBand *band) {
    u32 count = band->head.entryCount;
    PolyBandRecord *record = band->records;
    u32 delayStep = band->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->age = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}

void polyBandLayoutRing(PolyBand *band, s32 index, f32 radius);

extern EffRandState effDefaultRandomState[];

/* Randomize record `index`: pick jittered start and end radii, derive the per-frame radius step, and lay the ring out. */
void dds3SpPoly_RingBuffInit(PolyBand *spawner, s32 index) {
    PolyBandRecord *record = spawner->records;
    f32 spread;
    f32 start;
    f32 end;

    record += index;
    spread = spawner->initialRadiusJitter;
    start = spawner->initialRadius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    end = spawner->targetRadius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    record->age = 0;
    if (spawner->head.duration > 0) {
        record->radiusStep = (end - start) / (f32)spawner->head.duration;
    } else {
        record->radiusStep = end - start;
    }
    polyBandLayoutRing(spawner, index, start);
}

/* Write radii radius and radius + radialWidth, then close the strip with its first pair. */
void polyBandLayoutRing(PolyBand *band, s32 index, f32 radius)
{
    ParSystem *strip = band->strip;
    ParCell *entry = &strip->cells[index];
    f32 radialDirection[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)entry->history;
    pairs = strip->vertexWordCount / 2;
    VEC3_SPLAT(baseRadiusVector, radius);
    VEC3_SPLAT(offsetRadiusVector, radius + band->radialWidth);
    angleStep = 3.14159265f * 2.0f / (f32)band->segments;
    VU0_LOAD_MATRIX(band->head.matrix);
    angle = 0.0f;
    SDF_VU0_LOADVECTOR_C(band->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        radialDirection[0] = sdfCos(angle);
        radialDirection[1] = 0;
        radialDirection[2] = sdfSin(angle);
        SDF_VU0_LOADVECTOR(radialDirection);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(radialDirection);
        SDF_VU0_LOADVECTOR_B(baseRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor + 4);
        SDF_VU0_LOADVECTOR(radialDirection);
        SDF_VU0_LOADVECTOR_B(offsetRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)entry->history;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Add one radius step and local-y lift step to existing point pairs, then close the strip. */
void dds3SpPoly_RingMove(PolyBand *band, s32 index) {
    ParSystem *strip = band->strip;
    PolyBandRecord *rec = &band->records[index];
    ParCell *entry = &strip->cells[index];
    f32 radialDirection[4];
    f32 radiusStepVector[4];
    f32 liftVector[4];
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)entry->history;
    pairs = strip->vertexWordCount >> 1;
    radiusStepVector[2] = radiusStepVector[1] = radiusStepVector[0] = rec->radiusStep;
    liftVector[1] = band->liftStep;
    liftVector[2] = liftVector[0] = 0;
    angleStep = 3.14159265f * 2.0f / (f32)band->segments;
    VU0_LOAD_MATRIX(band->head.matrix);
    angle = 0.0f;
    for (i = 0; i < pairs - 1; i++) {
        radialDirection[0] = sdfCos(angle);
        radialDirection[1] = 0;
        radialDirection[2] = sdfSin(angle);
        SDF_VU0_LOADVECTOR(radialDirection);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_LOADVECTOR_B(radiusStepVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_LOADVECTOR_B(liftVector);
        SDF_VU0_APPLYMATRIX_B();
        SDF_VU0_ADDVECTOR();
        SDF_VU0_COPYVECTOR_A_TO_C();
        SDF_VU0_LOADVECTOR_B(pointCursor + 4);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(pointCursor + 4);
        SDF_VU0_COPYVECTOR_C_TO_A();
        SDF_VU0_LOADVECTOR_B(pointCursor);
        SDF_VU0_ADDVECTOR();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)entry->history;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Scale the band's row width, lift step, and initial/target radii in retail order. */
void dds3SpPoly_RingSetScale(f32 scale, PolyBand *band) {
    band->radialWidth = band->radialWidth * scale;
    band->liftStep = band->liftStep * scale;
    band->initialRadius = band->initialRadius * scale;
    band->targetRadius = band->targetRadius * scale;
}

/* Advance band ages, fades, geometry, and loop completion. */
void dds3SpPoly_RingDraw(PolyBand *band) {
    s32 recordIndex = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyBandRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = band->strip;
    count = band->head.entryCount;
    record = band->records;
    duration = band->head.duration;
    loop = band->head.loop;
    color = band->head.color;
    entry = strip->cells;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_RingBuffInit(band, recordIndex);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < band->head.startColorRampFrames) {
                    entry->color = dds3SpPoly_Color(age, band->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - band->head.endColorRampFrames) {
                    entry->color = dds3SpPoly_Color(duration - age, band->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_RingMove(band, recordIndex);
                    age++;
                } else {
                    entry->vertexCount = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        band->head.active = POLY_ACTIVITY_INACTIVE;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            recordIndex++;
            entry++;
            record++;
        } while (recordIndex < count);
    }
    dds3TrackPolyDraw(band->strip);
}

PolyArc *dds3SpPoly_SphereRingCreate(PolyArc *source) {
    s32 tailBytes = sizeof(PolyArc) - sizeof(PolyRingHead);
    s32 size = source->head.entryCount * sizeof(PolyArcRecord) + sizeof(PolyRingHead) + tailBytes;
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    PolyArc *clone = (PolyArc *)sdfMemLock(allocation);

    memset(clone, 0, size);
    memcpy(clone, source, source->head.templateSize);
    memcpy(&clone->spawnDelayStep, (u8 *)source + source->head.templateSize, tailBytes);
    clone->head.templateSize = 0xC0;
    clone->records = (PolyArcRecord *)(clone + 1);
    clone->allocation = allocation;
    if (((PolyRing *)clone)->arc.segments < 3) {
        ((PolyRing *)clone)->arc.segments = 3;
    }
    clone->strip = dds3TrackPolyCreate(clone->head.entryCount, clone->segments, 1, 0);
    dds3TrackPolyVCSet(clone->strip, 0, clone->firstColor, clone->secondColor);
    dds3SpPoly_SphereRingAllBuffInit(clone);
    VU0_LOAD_MATRIX(clone->head.matrix);
    VU0_STORE_MATRIX(clone->head.matrixCopy);
    dds3TrackPolySetMode(clone->strip, clone->head.drawBucket);
    return clone;
}

/* Release the arc's cell system and backing allocation handle, leaving the node alive. */
void dds3SpPoly_SphereRingDelete(PolyArc *arc) {
    dds3TrackPolyDelete(arc->strip);
    sdfMemFreeDirect(arc->allocation);
}

/* Seed arc records with staggered inactive ages. */
void dds3SpPoly_SphereRingAllBuffInit(PolyArc *arc) {
    u32 count = arc->head.entryCount;
    PolyArcRecord *record = arc->records;
    u32 delayStep = arc->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            index++;
            record->age = age;
            age -= delayStep;
            record++;
        } while (index < count);
    }
}

/* Reset one arc record and choose its jittered radius. */
void dds3SpPoly_SphereRingBuffInit(PolyArc *arc, s32 recordIndex) {
    PolyArcRecord *record = arc->records;
    f32 spread = arc->radiusJitter;

    record += recordIndex;
    record->age = 0;
    record->radius = arc->radius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
}

/* Lay an arc ring's inner and outer point rows at the projected radius and
 * radial offset for this record's age. */
void dds3SpPoly_SphereRingMove(PolyArc *arc, s32 recordIndex) {
    ParSystem *strip = arc->strip;
    PolyArcRecord *record = &arc->records[recordIndex];
    ParCell *stripEntry = &strip->cells[recordIndex];
    f32 innerPointVector[4];
    f32 outerPointVector[4];
    f32 recordRadius;
    f32 projectedInnerRadius;
    f32 verticalOffset;
    f32 radialWidth;
    f32 angleStep;
    f32 angle;
    f32 *pointCursor;
    f32 *firstPointPair;
    s32 pairs;
    s32 i;

    stripEntry->vertexCount = strip->vertexWordCount;
    pointCursor = (f32 *)stripEntry->history;
    pairs = strip->vertexWordCount >> 1;
    recordRadius = record->radius;
    angle = (f32)record->age / (f32)arc->head.duration * 3.14159265f;
    projectedInnerRadius = recordRadius * sdfSin(angle);
    verticalOffset = recordRadius * sdfCos(angle) - recordRadius;
    angleStep = 3.14159265f * 2.0f / (f32)arc->segments;
    VU0_LOAD_MATRIX(arc->head.matrix);
    radialWidth = arc->radialWidth;
    angle = 0.0f;
    SDF_VU0_LOADVECTOR_C(arc->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        innerPointVector[0] = outerPointVector[0] = sdfCos(angle);
        innerPointVector[2] = sdfSin(angle);
        outerPointVector[1] = verticalOffset;
        outerPointVector[2] = innerPointVector[2] * (projectedInnerRadius + radialWidth);
        outerPointVector[0] *= projectedInnerRadius + radialWidth;
        SDF_VU0_LOADVECTOR(outerPointVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(outerPointVector);
        innerPointVector[1] = verticalOffset;
        innerPointVector[0] *= projectedInnerRadius;
        innerPointVector[2] *= projectedInnerRadius;
        SDF_VU0_LOADVECTOR(innerPointVector);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor + 4);
        SDF_VU0_LOADVECTOR(outerPointVector);
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(pointCursor);
        pointCursor += 8;
        angle += angleStep;
    }
    firstPointPair = (f32 *)stripEntry->history;
    SDF_VU0_COPYVECTOR(pointCursor, firstPointPair);
    SDF_VU0_COPYVECTOR(pointCursor + 4, firstPointPair + 4);
}

/* Scale the arc's base radius, leaving its jitter fraction unchanged. */
void dds3SpPoly_SphereRingSetScale(f32 scale, PolyArc *arc) {
    arc->radius = arc->radius * scale;
}

/* Advance arc ages, fades and geometry; no-ramp windows retain the prior tint. */
void dds3SpPoly_SphereRingDraw(PolyArc *arc) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyArcRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = arc->strip;
    count = arc->head.entryCount;
    record = arc->records;
    duration = arc->head.duration;
    loop = arc->head.loop;
    color = arc->head.color;
    entry = strip->cells;

    if (count > 0) {
        do {
            s32 age = record->age;

            if (age == POLY_INACTIVE_ENTRY_AGE) {
                dds3SpPoly_SphereRingBuffInit(arc, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < arc->head.startColorRampFrames) {
                    entry->color = dds3SpPoly_Color(age, arc->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - arc->head.endColorRampFrames) {
                    entry->color = dds3SpPoly_Color(duration - age, arc->head.endColorRampFrames, color);
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_SphereRingMove(arc, index);
                    age++;
                } else {
                    entry->vertexCount = 0;
                    age++;
                }
            }

            if (age >= duration) {
                if (loop != 0) {
                    age = POLY_INACTIVE_ENTRY_AGE;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        arc->head.active = POLY_ACTIVITY_INACTIVE;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    dds3TrackPolyDraw(arc->strip);
}

/* Rotating-band counterpart of dds3SpPoly_RingCreate: twenty-byte records follow the header. */
PolyRotatingBand *dds3SpPoly_RollRingCreate(PolyRingHead *templateHead) {
    u32 size = templateHead->entryCount * sizeof(PolyRotatingBandRecord) + sizeof(PolyRotatingBand);
    struct sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    PolyRotatingBand *band = (PolyRotatingBand *)sdfMemLock(allocation);

    memset(band, 0, size);
    memcpy(band, templateHead, templateHead->templateSize);
    memcpy(&band->spawnDelayStep, (u8 *)templateHead + templateHead->templateSize, 0x40);
    band->head.templateSize = sizeof(PolyRingHead);
    band->records = (PolyRotatingBandRecord *)(band + 1);
    band->allocation = allocation;
    if (((PolyRing *)band)->rotating.segments < 3) {
        ((PolyRing *)band)->rotating.segments = 3;
    }
    band->strip = dds3TrackPolyCreate(band->head.entryCount, band->segments, 1, 0);
    dds3TrackPolyVCSet(band->strip, 0, band->firstColor, band->secondColor);
    dds3SpPoly_RollRingAllBuffInit(band);
    VU0_LOAD_MATRIX(band->head.matrix);
    VU0_STORE_MATRIX(band->head.matrixCopy);
    dds3TrackPolySetMode(band->strip, band->head.drawBucket);
    return band;
}

/* Release the rotating band's cell system and backing allocation handle, not the node. */
void dds3SpPoly_RollRingDelete(PolyRotatingBand *rotatingBand) {
    dds3TrackPolyDelete(rotatingBand->strip);
    sdfMemFreeDirect(rotatingBand->allocation);
}

/* Seed rotating-band records, advancing the age after each delay group. */
void dds3SpPoly_RollRingAllBuffInit(PolyRotatingBand *rotatingBand) {
    u32 count = rotatingBand->head.entryCount;
    PolyRotatingBandRecord *record = rotatingBand->records;
    u32 delayStep = rotatingBand->spawnDelayStep;
    s32 age = POLY_INACTIVE_ENTRY_AGE;
    u32 index;

    index = 0;
    if (count != 0) {
        do {
            record->age = age;
            record++;
            if ((index + 1) % rotatingBand->spawnDelayGroupSize == 0) {
                age -= delayStep;
            }
            index++;
        } while (index < count);
    }
}

/* Randomize record `index`: jittered start and end radii, radius step, start angle (degrees to radians) and the phase within its delay group. */
void dds3SpPoly_RollRingBuffInit(PolyRotatingBand *spawner, u32 index) {
    PolyRotatingBandRecord *record = spawner->records;
    f32 spread;

    record += index;
    spread = spawner->initialRadiusJitter;
    record->age = 0;
    record->radius = spawner->initialRadius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread));
    spread = spawner->targetRadiusJitter;
    record->radiusStep = (spawner->targetRadius * (dds3GetRandFloatA(effDefaultRandomState) * spread + (1.0f - spread)) - record->radius) / (f32)spawner->head.duration;
    record->rotationXRadians = spawner->rotationXDegrees * 0.017453292f;
    record->rotationYRadians = 6.2831852f / (f32)spawner->spawnDelayGroupSize * (f32)(index % spawner->spawnDelayGroupSize);
}

/* Build the paired ring using current radius/rotation, then retain their next-step values. */
void dds3SpPoly_RollRingMove(PolyRotatingBand *rotatingBand, s32 index)
{
    ParSystem *strip = rotatingBand->strip;
    PolyRotatingBandRecord *rec = &rotatingBand->records[index];
    ParCell *entry = &strip->cells[index];
    f32 dir[4];
    f32 baseRadiusVector[4];
    f32 offsetRadiusVector[4];
    f32 step;
    f32 value; /* Radius before emission; angular phase inside the loop. */
    f32 *out;
    f32 *first;
    s32 pairs;
    s32 i;

    entry->vertexCount = strip->vertexWordCount;
    out = (f32 *)entry->history;
    pairs = strip->vertexWordCount >> 1;
    SetRotateMatrixX(rec->rotationXRadians);
    SetRotateMatrixY_B(rec->rotationYRadians);
    MultMatrixRevNoArg();
    rec->rotationYRadians += rotatingBand->rotationStepDegrees * (3.14159265f / 180.0f);
    value = rec->radius;
    rec->radius = value + rec->radiusStep;
    step = 3.14159265f * 2.0f / (f32)rotatingBand->segments;
    SDF_VU0_LOADMATRIX_B(rotatingBand->head.matrix);
    MultMatrixRevNoArg();
    VEC3_SPLAT(baseRadiusVector, value);
    VEC3_SPLAT(offsetRadiusVector, value + rotatingBand->radialWidth);
    value = 0.0f;
    SDF_VU0_LOADVECTOR_C(rotatingBand->head.origin);
    for (i = 0; i < pairs - 1; i++) {
        dir[0] = sdfCos(value);
        dir[1] = 0;
        dir[2] = sdfSin(value);
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_APPLYMATRIX();
        SDF_VU0_STOREVECTOR(dir);
        SDF_VU0_LOADVECTOR_B(baseRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(out + 4);
        SDF_VU0_LOADVECTOR(dir);
        SDF_VU0_LOADVECTOR_B(offsetRadiusVector);
        SDF_VU0_MULVECTOR();
        SDF_VU0_ADDVECTOR_A_C();
        SDF_VU0_STOREVECTOR(out);
        out += 8;
        value += step;
    }
    first = (f32 *)entry->history;
    SDF_VU0_COPYVECTOR(out, first);
    SDF_VU0_COPYVECTOR(out + 4, first + 4);
}

/* Scale the rotating band's initial and target radii. */
void dds3SpPoly_RollRingSetScale(f32 scale, PolyRotatingBand *rotatingBand) {
    rotatingBand->initialRadius = rotatingBand->initialRadius * scale;
    rotatingBand->targetRadius = rotatingBand->targetRadius * scale;
}

/* Advance rotating-band cells; native completion requires a duration of at least two. */
void polyUpdateRotatingBandCells(PolyRotatingBand *rotatingBand) {
    s32 index = 0;
    u32 completed = 0;
    u8 loop;
    ParSystem *strip;
    s32 count;
    PolyRotatingBandRecord *record;
    s32 duration;
    u32 color;
    ParCell *entry;

    strip = rotatingBand->strip;
    count = rotatingBand->head.entryCount;
    record = rotatingBand->records;
    duration = rotatingBand->head.duration;
    loop = rotatingBand->head.loop;
    color = rotatingBand->head.color;
    entry = strip->cells;

    if (count > 0) {
        s32 inactiveAge = POLY_INACTIVE_ENTRY_AGE;
        s32 shortDuration = duration < 2;

        do {
            s32 age = record->age;

            if (age == inactiveAge) {
                dds3SpPoly_RollRingBuffInit(rotatingBand, index);
                age = record->age;
            }
            if (age < 0) {
                age++;
            } else {
                if ((u32)age < rotatingBand->head.startColorRampFrames) {
                    entry->color = dds3SpPoly_Color(age, rotatingBand->head.startColorRampFrames, color);
                } else if (age <= duration &&
                           (u32)age >= duration - rotatingBand->head.endColorRampFrames) {
                    entry->color = dds3SpPoly_Color(duration - age, rotatingBand->head.endColorRampFrames, color);
                } else {
                    entry->color = color;
                }

                if (entry->color & 0xFF000000) {
                    dds3SpPoly_RollRingMove(rotatingBand, index);
                } else {
                    entry->vertexCount = 0;
                }
                age++;
            }

            if (age >= duration && !shortDuration) {
                if (loop != 0) {
                    age = inactiveAge;
                } else {
                    completed++;
                    if (completed >= (u32)count) {
                        rotatingBand->head.active = POLY_ACTIVITY_INACTIVE;
                    }
                }
                entry->vertexCount = 0;
            }

            record->age = age;
            index++;
            entry++;
            record++;
        } while (index < count);
    }
    dds3TrackPolyDraw(rotatingBand->strip);
}

/* Reset active records but leave inactive sentinel entries untouched. */
void dds3SpPoly_RollRingSetEndless(PolyRotatingBand *rotatingBand) {
    u32 count;
    PolyRotatingBandRecord *entry;
    u32 i;

    count = rotatingBand->head.entryCount;
    i = 0;
    rotatingBand->head.duration = POLY_RESET_DURATION;
    rotatingBand->head.endColorRampFrames = 0;
    rotatingBand->head.startColorRampFrames = 0;
    entry = rotatingBand->records;
    if (count != 0) {
        do {
            if (entry->age != POLY_INACTIVE_ENTRY_AGE) {
                entry->age = POLY_RESET_ENTRY_AGE;
            }
            i++;
            entry++;
        } while (i < count);
    }
}

/* Node restarts and scales: retail leaves the dispatch state in a0 (and the
 * factor in f12) for them. */
extern void (*D_003AAFB0[])(EffectDispatchState *);

extern void (*D_003AAFC0[])(f32, EffectDispatchState *);

extern BillDispatch dds3SpPolyTable[];

extern f32 D_003AAEB0[4];

void *dds3SpPolyCreate(s32 index, void *arg) {
    EffectDispatchState *effect = dds3SpPolyTable[index].func(arg);

    effect->handler = index;
    effect->valueB2 = 1;
    return effect;
}

void *dds3SpPolyCopy(EffectDispatchState *effect) {
    EffectDispatchState *result = dds3SpPolyTable[effect->handler].func(effect);

    result->handler = effect->handler;
    result->valueB2 = 1;
    return result;
}

/* Invoke the selected dispatch record's destructor; the handler index is unchecked. */
void dds3SpPolyDelete(EffectDispatchState *effect) {
    dds3SpPolyTable[effect->handler].destroy(effect);
}

/* Invoke the update column of the selected constructor/update/destructor record. */
void dds3SpPolyDraw(EffectDispatchState *effect) {
    dds3SpPolyTable[effect->handler].callback(effect);
}

void dds3SpPolyReset(EffectDispatchState *effect) {
    D_003AAFB0[effect->handler](effect);
    effect->valueB2 = 1;
}

void dds3SpPolySetScale(EffectDispatchState *effect, f32 factor) {
    D_003AAFC0[effect->handler](factor, effect);
    dds3SpPolyReset(effect);
}

u16 dds3SpPolyGetState(EffectDispatchState *effect) {
    return effect->valueB2;
}

void dds3SpPolySetPosition(void *dst, void *src) {
    SDF_VU0_COPYVECTOR(dst, src);
}

/* vu0 routine: effect->composedMatrix = matrix * effect->sourceMatrix via MultMatrixNoArg */
void dds3SpPolySetRotateMatrix(EffectDispatchState *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    SDF_VU0_LOADMATRIX_B(effect->sourceMatrix);
    MultMatrixNoArg();
    VU0_STORE_MATRIX(effect->composedMatrix);
}

/* Set the packed color consumed by the dispatched polygon work. */
void dds3SpPolySetRGBA(EffectDispatchState *effect, u32 value) {
    effect->value60 = value;
}

/* A value staged with no pending work is immediately mirrored to the active slot. */
void dds3SpPolySetLoop(BillWork *work, u8 value) {
    if (work->pendingCount == 0) {
        work->stagedValue = value;
    }
    work->currentValue = value;
}

u8 dds3SpPolyGetLoop(EffectDispatchState *effect) {
    return effect->value64;
}

void dds3PolygonSetColor(EffectDispatchState *effect, u32 value) {
    effect->value1C = value;
}

/* Append position, position+(0,5,0), position+(0,5,5), with optional normals/color. */
void dds3PolygonTriangleSet(EffOffsetTriangleView *work, const f32 *position) {
    s32 count = work->count;
    f32 point[4] __attribute__((aligned(16)));
    f32 (*positions)[4];
    u16 flags;

    SDF_VU0_COPYVECTOR(point, position);
    positions = (f32 (*)[4])(((u32)count << 4) + (u32)work->positions);
    SDF_VU0_COPYVECTOR(positions[0], point);
    point[1] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[1], point);
    point[2] += 5.0f;
    SDF_VU0_COPYVECTOR(positions[2], point);
    flags = work->flags;
    if (flags & 2) {
        f32 (*normals)[4] = (f32 (*)[4])(((u32)count << 4) + (u32)work->normals);
        SDF_VU0_COPYVECTOR(normals[0], D_003AAEB0);
        SDF_VU0_COPYVECTOR(normals[1], D_003AAEB0);
        SDF_VU0_COPYVECTOR(normals[2], D_003AAEB0);
    }
    if (flags & 4) {
        u32 *colors = (u32 *)(((u32)count << 2) + (u32)work->colors);
        colors[0] = 0x80000080;
        colors[1] = 0x80000080;
        colors[2] = 0x80000080;
        count = work->count;
    }
    work->count = count + 3;
}

void dds3TrackPolyColorSet(ParSystem *system, s32 cellIndex, u32 color) {
    system->cells[cellIndex].color = color;
}
INCLUDE_SDATA(const s32, "effect/polyManager", hDrawListTop);

INCLUDE_SDATA(const s32, "effect/polyManager", D_00436404);

