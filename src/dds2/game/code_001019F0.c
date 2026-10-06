#include "common.h"
#include "dds3Admin.h"
#include "kwln.h"
#include "sdf.h"

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
    func_00100F48(1, object, mask, scope);
}

/* Clear the same scoped object mask bits without changing the selection. */
void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(0, object, mask, scope);
}

typedef struct SdfThreadNode SdfThreadNode;

extern void func_00356F58(void);
extern void sceSifInitRpc(s32);
extern s32 sceSifInitIopHeap(void);
extern void sdfInitDeviceSemaphores(void);
extern s32 sceSifRebootIop(const char *);
extern s32 func_0036DD28(void);
extern void sceSifLoadFileReset(void);
extern void sceFsReset(void);
extern void func_0034CB60(s32);
extern void func_0034D038(u32);
extern s32 sceSifLoadModule(const char *, s32, const char *);
extern void sdfDevStartLoad(s32, s32);
extern void sdfLoadIopModulePair(const char *, s32);
extern void func_00328858(u32, u32, u32);
extern void sdfPadRequestMode(s32, u8);
extern void func_0033EB80(const char *);
extern void func_0033EC28(s8);
extern void sdfEnsureDeviceWorkerThreadStarted(s32);
extern void func_00329C20(const char *, const char *);
extern void sdfStartAndSuspendWorkerThread(void);
extern void sdfStartTrackedThread(SdfThreadNode *, s32, s32, s64, s32, s32);
extern s32 sdfThreadSleepSelf(void);
extern void func_001004B0(void);

extern const char D_004110A8[];
extern const char D_004110C8[];
extern const char D_004110E8[];
extern const char D_00411108[];
extern const char D_00411118[];
extern const char D_00411130[];
extern const char D_00411140[];
extern const char D_00411158[];
extern const char D_00411178[];
extern const s32 D_00435C0C;
extern u32 sdfDiscType;
extern u8 D_00438A8C;
extern u32 D_00438D80;
extern u8 D_00439410[];
s32 func_00101AC0(void) {
    func_00356F58();
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sdfDiscType = D_00435C0C;
    sdfInitDeviceSemaphores();
    while (sceSifRebootIop(D_004110A8) == 0) {
    }
    while (func_0036DD28() == 0) {
    }
    sceSifInitRpc(0);
    sceSifInitIopHeap();
    sceSifLoadFileReset();
    sceFsReset();
    func_0034CB60(0);
    func_0034D038(D_00435C0C);
    while (sceSifLoadModule(D_004110C8, 0, 0) < 0) {
    }
    while (sceSifLoadModule(D_004110E8, 0, 0) < 0) {
    }
    sdfDevStartLoad((s32)D_00411108, (s32)D_00411118);
    sdfLoadIopModulePair(D_00411130, 1);
    func_00328858(0x17C0000, 0x200000, 0x335000);
    D_00438A8C = 1;
    sdfPadRequestMode(0, 3);
    sdfPadRequestMode(1, 3);
    func_0033EB80(D_00411140);
    func_0033EC28(3);
    sdfEnsureDeviceWorkerThreadStarted(3);
    sdfEnsureDeviceWorkerThreadStarted(0);
    func_00329C20(D_00411158, D_00411178);
    sdfStartAndSuspendWorkerThread();
    sdfStartTrackedThread((SdfThreadNode *)((u8 *)&D_00438D80 + 8),
                         (s32)func_001004B0, (s32)D_00439410, 0x4000, 0x60, 0);
    sdfThreadSleepSelf();
    return 0;
}

extern u32 kwlnGetDrawBufferIndex(void);
extern u8 D_003808B0[];
extern SdfPoolNode kwlnDrawSurfaces[];
extern u8 D_00380860[];
extern u8 D_00380870[];
extern void sdfWaitAndSelectBuffer(void);
extern void func_0032D0F0(SdfPoolNode *, s32);
extern void sdfClearPacketListHead(void *);
extern void func_00105070(s32);
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
s32 kwlnPrepareFrameDrawPackets(void) {
    s32 bufferIndex = kwlnGetDrawBufferIndex();
    s32 groupIndex;
    u8 *packetGroups;
    SdfPoolNode *drawSink;

    sdfWaitAndSelectBuffer();
    func_0032D0F0(kwlnDrawSurfaces, KWLN_FRAME_POOL_NODE_COUNT);
    sdfClearPacketListHead(D_00380860);
    func_00105070(bufferIndex);
    packetGroups = D_003808B0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES;
    for (groupIndex = KWLN_FRAME_LAST_GROUP_INDEX; groupIndex >= 0; groupIndex--) {
        sdfInitializeDrawPacketGroups(packetGroups);
        packetGroups += KWLN_FRAME_GROUP_BYTES;
    }
    drawSink = kwlnDrawSurfaces;
    drawSink->append((SdfListHead *)drawSink, (SdfListHead *)(D_00380870 + bufferIndex * KWLN_FRAME_BUFFER_BYTES));
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101D30);

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
extern s32 sdfFlushPoolNodes(SdfPoolNode *);
extern void kwlnDrawBlurErrorCounters(void);
extern void kwlnStepBackgroundFade(void);
extern void func_00106288(void);
extern void kwlnFadeUpdate(void);
extern void func_00105CF8(void);
extern void func_00107108(void);
extern u32 func_001200E0(void);
extern void func_00343468(s32);
extern void sdfQueueFramePackets(SdfListHead *, void *);
extern u8 D_00380788[];
extern u8 kwlnFrameDrawPacketRecords[];
extern SdfPoolNode D_00380708;
extern u8 kwlnDrawOverlayEnabled;
extern s16 kwlnDrawOverlayAlpha;
extern s16 kwlnDrawOverlayScale;
extern s32 D_00435CE0[2];
extern u8 D_00435BC8;
extern SdfListHead *D_00435C14;
extern u32 kwlnDrawControlFlags;
extern s8 D_00438A20;


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
    SdfListHead *poolHead;
    s32 edgeDistances[4];
    s32 i;

    kwlnDrawBlurErrorCounters();
    kwlnStepBackgroundFade();
    func_00106288();
    kwlnFadeUpdate();
    func_00105CF8();
    func_00107108();
    groupHeads = D_00380788;
    packetGroups = D_003808B0 + bufferIndex * KWLN_FRAME_BUFFER_BYTES;
    for (i = KWLN_FRAME_LAST_GROUP_INDEX; i >= 0; i--) {
        sdfSubmitDrawPacketGroups(groupHeads, packetGroups);
        groupHeads += KWLN_FRAME_GROUP_HEAD_BYTES;
        packetGroups += KWLN_FRAME_GROUP_BYTES;
    }
    if (kwlnDrawOverlayEnabled != 0 && func_001200E0() == 0) {
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
        edgeDistances[0] = D_00435CE0[0] + KWLN_FRAME_HALF_WIDTH;
        edgeDistances[1] = D_00435CE0[1] + KWLN_FRAME_HALF_HEIGHT;
        edgeDistances[2] = KWLN_FRAME_HALF_WIDTH - D_00435CE0[0];
        edgeDistances[3] = KWLN_FRAME_HALF_HEIGHT - D_00435CE0[1];
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
        D_00380708.append((SdfListHead *)&D_00380708, (SdfListHead *)packetList);
    }
    poolHead = (SdfListHead *)sdfFlushPoolNodes(kwlnDrawSurfaces);
    D_00435C14 = poolHead;
    if (D_00435BC8 != 0) {
        func_00343468(poolHead->first);
        D_00435BC8 = 0;
    }
    if (!(kwlnDrawControlFlags & KWLN_FRAME_SKIP_POOL_QUEUE_BIT)) {
        sdfQueueFramePackets(poolHead, D_00380860);
    } else {
        kwlnDrawControlFlags &= KWLN_FRAME_CLEAR_SKIP_POOL_QUEUE;
    }
    D_00438A20 = 0;
    return 0;
}

/* Invoke the active world's first callback column; this task step returns zero. */
u32 func_00102740(void) {
    dds3InvokeWorldCallbackFirst(dds3ActiveWorld);
    return 0;
}

/* Invoke the active world's second callback column; this task step returns zero. */
u32 func_00102768(void) {
    dds3InvokeWorldCallbackSecond(dds3ActiveWorld);
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110A8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110C8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_004110E8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411108);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411118);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411130);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411140);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411158);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411178);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C0C);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C14);

