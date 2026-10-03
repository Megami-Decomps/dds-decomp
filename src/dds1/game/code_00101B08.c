#include "common.h"
#include "dds3Admin.h"
#include "kwln.h"

extern AdminWork *dds3GetAdminTaskWork(void);

extern u32 dds3ActiveWorld;

/* Detach a task from its parent's child list. Detached tasks are left untouched;
 * an attached task must already occur in that parent's sibling chain.
 * The hierarchy link is next, separate from scheduler listNext/listPrev. */
void kwlnUnlinkListNode(KwlnTask *task) {
    KwlnTask *parent = task->parent;

    if (parent == 0) {
        return;
    }
    if (parent->childList == task) {
        parent->childList = task->next;
    } else {
        KwlnTask *previousSibling = parent->childList;

        while (previousSibling->next != task) {
            previousSibling = previousSibling->next;
        }
        previousSibling->next = task->next;
    }
    task->parent = 0;
    task->next = 0;
}

/* Set mask bits on the object selection described by object and scope. */
void dds3SetScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00101060(1, object, mask, scope);
}

/* Clear the same scoped object mask bits without changing the selection. */
void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00101060(0, object, mask, scope);
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101BD8);

typedef struct KwlnDrawSink {
    u8 pad00[0x10];
    void (*invoke)(void *, void *); /* 0x10 */
} KwlnDrawSink;
extern s32 kwlnGetDrawBufferIndex(void);
extern u8 D_003258B0[];
extern u8 kwlnDrawSurfaces[];
extern u8 D_00325860[];
extern u8 D_00325870[];
extern void sdfWaitAndSelectBuffer(void);
extern void func_002D4240(void *, s32);
extern void sdfClearPacketListHead(void *);
extern void func_00105150(s32);
extern void sdfInitializeDrawPacketGroups(u8 *);

#define KWLN_FRAME_BUFFER_BYTES 0x1F40
#define KWLN_FRAME_POOL_NODE_COUNT 0x62
#define KWLN_FRAME_LAST_GROUP_INDEX 12
#define KWLN_FRAME_GROUP_BYTES 0x1B0
#define KWLN_FRAME_GROUP_HEAD_BYTES 0x10
#define KWLN_FRAME_PACKET_LIST_BYTES 0x20
#define KWLN_FRAME_GS_PACKET_BYTES 0x40
#define KWLN_FRAME_GIF_AD_REGISTER 0xE
#define KWLN_FRAME_GIF_SINGLE_REG_FIELD 0x10000000
#define KWLN_FRAME_GIF_TWO_WRITES_EOP 0x8002
#define KWLN_FRAME_GS_TEXA 0x3B
#define KWLN_FRAME_GS_TEXFLUSH 0x3F
#define KWLN_FRAME_GS_TEST_PRIMARY 0x47
#define KWLN_FRAME_GS_ALPHA_PRIMARY 0x42
#define KWLN_FRAME_NEUTRAL_COLOR 0x80
#define KWLN_FRAME_HALF_WIDTH 0x100
#define KWLN_FRAME_HALF_HEIGHT 0xE0
#define KWLN_FRAME_SCALE_FRACTION_BITS 12
#define KWLN_FRAME_X_UNITS_PER_PIXEL 16
#define KWLN_FRAME_Y_UNITS_PER_PIXEL 8
#define KWLN_FRAME_LEFT_BASE 0x7000
#define KWLN_FRAME_TOP_BASE 0x7900
#define KWLN_FRAME_RIGHT_BASE 0x9000
#define KWLN_FRAME_BOTTOM_BASE 0x8700
#define KWLN_FRAME_U_EXTENT 0x2000
#define KWLN_FRAME_V_EXTENT 0xE00
#define KWLN_FRAME_SKIP_POOL_QUEUE_BIT 0x2000000
#define KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE 0xFDFFFFFF

/* Prepare the selected buffer's thirteen packet groups and link its draw sink.
 * The buffer index is sampled before waiting/selecting; keep that ordering. */
s32 func_00101D60(void) {
    s32 bufferIndex = kwlnGetDrawBufferIndex();
    s32 groupIndex;
    u8 *packetGroups;
    KwlnDrawSink *drawSink;

    sdfWaitAndSelectBuffer();
    func_002D4240(kwlnDrawSurfaces, KWLN_FRAME_POOL_NODE_COUNT);
    sdfClearPacketListHead(D_00325860);
    func_00105150(bufferIndex);
    packetGroups = D_003258B0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES;
    for (groupIndex = KWLN_FRAME_LAST_GROUP_INDEX; groupIndex >= 0; groupIndex--) {
        sdfInitializeDrawPacketGroups(packetGroups);
        packetGroups += KWLN_FRAME_GROUP_BYTES;
    }
    drawSink = (KwlnDrawSink *)kwlnDrawSurfaces;
    drawSink->invoke(drawSink, D_00325870 + bufferIndex * KWLN_FRAME_BUFFER_BYTES);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00101B08", func_00101E40);

/* Sprite vertex record of a column packet: colour, then two corners of the quad. */
typedef struct KwlnSpriteCorner {
    s32 u, v;
    u8 pad08[8];
    s32 x, y;
    s32 mask;
    s16 flag;
    u8 pad1E[2];
} KwlnSpriteCorner;

typedef struct KwlnSpriteVertex {
    s32 r, g, b, a;
    KwlnSpriteCorner corner[2];
} KwlnSpriteVertex;

extern u64 *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket(void *, void *);
extern void sdfAppendDmaPrimary(void *, u8 *, void *);
extern void sdfSubmitDrawPacketGroups(u8 *, u8 *);
extern s32 *sdfConsAllocateColumnPacket(s32);
extern KwlnSpriteVertex *sdfConsMeasurePacketWithHeader(s32 *);
extern s32 *sdfFlushPoolNodes(void *);
extern void kwlnDrawBlurErrorCounters(void);
extern void kwlnStepBackgroundFade(void);
extern void func_00106368(void);
extern void kwlnFadeUpdate(void);
extern void func_00105DD8(void);
extern void func_001071E8(void);
extern s32 func_0011E278(void);
extern void func_002EA5C0(s32);
extern void func_002D4EE8(s32 *, void *);
extern u8 D_00325788[];
extern u8 kwlnFrameDrawPacketRecords[];
extern u8 D_00325708[];
extern u8 kwlnDrawOverlayEnabled;
extern s16 kwlnDrawOverlayAlpha;
extern s16 kwlnDrawOverlayScale;
extern s32 D_003BA910[2];
extern u8 D_003BA7FC;
extern s32 *D_003BA844;
extern u32 kwlnDrawControlFlags;
extern s8 D_003BD330;


/* Update draw controls, submit thirteen packet groups, optionally draw the
 * vignette, then flush pools. The skip bit suppresses one packet-slot publication. */
s32 kwlnRenderFrame(void) {
    s32 bufferIndex = kwlnGetDrawBufferIndex();
    u8 *groupHeads;
    u8 *packetGroups;
    u64 *packetList;
    u64 *texturePacket;
    u64 *blendPacket;
    KwlnSpriteVertex *vertex;
    s32 *spritePacket;
    s32 *poolHead;
    s32 edgeDistances[4];
    s32 i;

    kwlnDrawBlurErrorCounters();
    kwlnStepBackgroundFade();
    func_00106368();
    kwlnFadeUpdate();
    func_00105DD8();
    func_001071E8();
    groupHeads = D_00325788;
    packetGroups = D_003258B0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES;
    for (i = KWLN_FRAME_LAST_GROUP_INDEX; i >= 0; i--) {
        sdfSubmitDrawPacketGroups(groupHeads, packetGroups);
        groupHeads += KWLN_FRAME_GROUP_HEAD_BYTES;
        packetGroups += KWLN_FRAME_GROUP_BYTES;
    }
    if (kwlnDrawOverlayEnabled != 0 && func_0011E278() == 0) {
        packetList = sdfAllocPacketAligned(KWLN_FRAME_PACKET_LIST_BYTES);
        sdfInitPacketList(packetList);
        sdfAppendDmaPrimary(packetList, kwlnFrameDrawPacketRecords + bufferIndex * KWLN_FRAME_BUFFER_BYTES, sdfAllocPacketAligned(KWLN_FRAME_PACKET_LIST_BYTES));
        texturePacket = sdfAllocPacketAligned(KWLN_FRAME_GS_PACKET_BYTES);
        texturePacket[0] = 3;
        /* VIF FLUSHA, then DIRECT for the three following quadwords. */
        texturePacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        texturePacket[2] = ((u64)KWLN_FRAME_GIF_SINGLE_REG_FIELD << 32) | KWLN_FRAME_GIF_TWO_WRITES_EOP;
        texturePacket[3] = KWLN_FRAME_GIF_AD_REGISTER;
        texturePacket[4] = ((u64)KWLN_FRAME_NEUTRAL_COLOR << 32) | KWLN_FRAME_NEUTRAL_COLOR;
        texturePacket[5] = KWLN_FRAME_GS_TEXA;
        texturePacket[6] = 0;
        texturePacket[7] = KWLN_FRAME_GS_TEXFLUSH;
        sdfAppendPacket(packetList, texturePacket);
        blendPacket = sdfAllocPacketAligned(KWLN_FRAME_GS_PACKET_BYTES);
        blendPacket[0] = 3;
        blendPacket[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        blendPacket[2] = ((u64)KWLN_FRAME_GIF_SINGLE_REG_FIELD << 32) | KWLN_FRAME_GIF_TWO_WRITES_EOP;
        blendPacket[3] = KWLN_FRAME_GIF_AD_REGISTER;
        blendPacket[4] = 0x31001;
        blendPacket[5] = KWLN_FRAME_GS_TEST_PRIMARY;
        blendPacket[6] = 0x44;
        blendPacket[7] = KWLN_FRAME_GS_ALPHA_PRIMARY;
        sdfAppendPacket(packetList, blendPacket);
        edgeDistances[0] = D_003BA910[0] + KWLN_FRAME_HALF_WIDTH;
        edgeDistances[1] = D_003BA910[1] + KWLN_FRAME_HALF_HEIGHT;
        edgeDistances[2] = KWLN_FRAME_HALF_WIDTH - D_003BA910[0];
        edgeDistances[3] = KWLN_FRAME_HALF_HEIGHT - D_003BA910[1];
        /* Signed Q12 scale; X and Y use distinct GS coordinate units below. */
        for (i = 0; i != 4; i++) {
            edgeDistances[i] = edgeDistances[i] * kwlnDrawOverlayScale >> KWLN_FRAME_SCALE_FRACTION_BITS;
        }
        spritePacket = sdfConsAllocateColumnPacket(1);
        vertex = sdfConsMeasurePacketWithHeader(spritePacket);
        vertex->r = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->g = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->b = KWLN_FRAME_NEUTRAL_COLOR;
        vertex->a = kwlnDrawOverlayAlpha;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = KWLN_FRAME_LEFT_BASE - edgeDistances[0] * KWLN_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[0].y = KWLN_FRAME_TOP_BASE - edgeDistances[1] * KWLN_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[0].mask = 0;
        vertex->corner[0].flag = 0;
        vertex->corner[1].u = KWLN_FRAME_U_EXTENT;
        vertex->corner[1].v = KWLN_FRAME_V_EXTENT;
        vertex->corner[1].x = KWLN_FRAME_RIGHT_BASE + edgeDistances[2] * KWLN_FRAME_X_UNITS_PER_PIXEL;
        vertex->corner[1].y = KWLN_FRAME_BOTTOM_BASE + edgeDistances[3] * KWLN_FRAME_Y_UNITS_PER_PIXEL;
        vertex->corner[1].mask = 0;
        vertex->corner[1].flag = 0;
        sdfAppendPacket(packetList, spritePacket);
        ((KwlnDrawSink *)D_00325708)->invoke(D_00325708, packetList);
    }
    poolHead = sdfFlushPoolNodes(kwlnDrawSurfaces);
    D_003BA844 = poolHead;
    if (D_003BA7FC != 0) {
        func_002EA5C0(poolHead[1]);
        D_003BA7FC = 0;
    }
    if (!(kwlnDrawControlFlags & KWLN_FRAME_SKIP_POOL_QUEUE_BIT)) {
        func_002D4EE8(poolHead, D_00325860);
    } else {
        kwlnDrawControlFlags &= KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE;
    }
    D_003BD330 = 0;
    return 0;
}

/* Invoke the active world's first callback column; this task step returns zero. */
u32 func_00102850(void) {
    dds3InvokeWorldCallbackFirst(dds3ActiveWorld);
    return 0;
}

/* Invoke the active world's second callback column; this task step returns zero. */
u32 func_00102878(void) {
    dds3InvokeWorldCallbackSecond(dds3ActiveWorld);
    return 0;
}

extern char dds3AdminTaskName[];
extern void *kwlnTaskGetTaskByName(char *);
extern AdminWork *kwlnTaskGetUserValue(void *);

/* Return the named administration task's user state; the task must exist. */
AdminWork *dds3GetAdminTaskWork(void) {
    return kwlnTaskGetUserValue(kwlnTaskGetTaskByName(dds3AdminTaskName));
}

/* Read the administration state's shared value word, without modifying it. */
u32 dds3GetAdminTaskValue(void) {
    AdminWork *work;

    work = dds3GetAdminTaskWork();
    return work->value;
}

extern void *sdfAllocSizeClassBlock(s32 size);

#define DDS3_ADMIN_REQUEST_PENDING_BIT 1
#define DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT 8
#define DDS3_ADMIN_RESTORE_HISTORY_BIT 0x10000
#define DDS3_ADMIN_MARK_HISTORY_BIT 4
#define DDS3_ADMIN_REQUEST_DATA_MAX_BYTES 0x100
#define DDS3_ADMIN_REQUEST_DELAY 2

/* Request a mode and replace its attached data. NULL data is accepted regardless
 * of dataBytes; only oversized non-NULL data rejects the entire request.
 * Mode and stored byte count retain their byte truncation (256 bytes records zero).
 * Previous data is released before copying; the history flag marks the old slot
 * when the requested mode is activated. */
void dds3AdminSubmitModeRequest(s32 requestedMode, void *requestData, u32 dataBytes, s32 markHistory) {
    AdminWork *work;
    void *previousData;
    u32 flags;

    if (requestData == NULL || dataBytes <= DDS3_ADMIN_REQUEST_DATA_MAX_BYTES) {
        work = dds3GetAdminTaskWork();
        previousData = work->unk1C;
        work->unk09 = requestedMode;
        flags = work->flags;
        flags |= DDS3_ADMIN_REQUEST_PENDING_BIT;
        flags &= ~DDS3_ADMIN_KEEP_HISTORY_SLOT_BIT;
        flags &= ~DDS3_ADMIN_RESTORE_HISTORY_BIT;
        work->flags = flags;
        work->unk21 = DDS3_ADMIN_REQUEST_DELAY;
        if (previousData != NULL) {
            sdfReleaseChipBlock(previousData);
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (requestData != NULL) {
            work->unk1C = sdfAllocSizeClassBlock(dataBytes);
            memcpy(work->unk1C, requestData, dataBytes);
            work->unk20 = dataBytes;
        } else {
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (markHistory != 0) {
            work->flags |= DDS3_ADMIN_MARK_HISTORY_BIT;
        } else {
            work->flags &= ~DDS3_ADMIN_MARK_HISTORY_BIT;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00101B08", D_0039E018);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA83C);

INCLUDE_SDATA(const s32, "game/code_00101B08", D_003BA844);

INCLUDE_SDATA(const s32, "game/code_00101B08", dds3AdminTaskName);

