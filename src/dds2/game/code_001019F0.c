#include "common.h"
#include "dds3Admin.h"

extern u32 dds3ActiveWorld;

extern AdminWork *dds3GetAdminTaskWork(void);
extern char D_00435C18[];
extern void *func_00101740(char *);
extern u32 kwlnTaskGetUserValue(void *);

typedef struct KwlnLinkNode {
    u8 unk00[0x44];              /* 0x0 */
    struct KwlnLinkNode *next;   /* 0x44: chain head */
    struct KwlnLinkNode *first;  /* 0x48 */
    struct KwlnLinkNode *link;   /* 0x4C: intrusive link */
} KwlnLinkNode;

void func_001019F0(KwlnLinkNode *node) {
    KwlnLinkNode *head = node->next;

    if (head == 0) {
        return;
    }
    if (head->first == node) {
        head->first = node->link;
    } else {
        KwlnLinkNode *prev = head->first;

        while (prev->link != node) {
            prev = prev->link;
        }
        prev->link = node->link;
    }
    node->next = 0;
    node->link = 0;
}

void dds3SetScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(1, object, mask, scope);
}

void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(0, object, mask, scope);
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101AC0);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101C50);

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
extern s32 *sdfFlushPoolNodes(void *);
extern s32 func_00100400(void);
extern void kwlnDrawBlurErrorCounters(void);
extern void func_00106188(void);
extern void func_00106288(void);
extern void kwlnFadeUpdate(void);
extern void func_00105CF8(void);
extern void func_00107108(void);
extern s32 func_001200E0(void);
extern void func_00343468(s32);
extern void func_0032DD98(s32 *, void *);
extern u8 D_00380788[];
extern u8 D_003808B0[];
extern u8 kwlnFrameDrawPacketRecords[];
extern u8 kwlnDrawSurfaces[];
extern u8 D_00380860[];
extern u8 D_00380708[];
extern u8 kwlnDrawOverlayEnabled;
extern s16 kwlnDrawOverlayAlpha;
extern s16 kwlnDrawOverlayScale;
extern s32 D_00435CE0[2];
extern u8 D_00435BC8;
extern s32 *D_00435C14;
extern u32 kwlnDrawControlFlags;
extern s8 D_00438A20;

typedef struct KwlnDrawSink {
    u8 pad00[0x10];
    void (*invoke)(void *, void *); /* 0x10 */
} KwlnDrawSink;

/* Per-frame render task: update the HUD pieces, submit the thirteen draw-packet groups of the current buffer, optionally draw the
 * screen-edge vignette, then flush the packet pools. */
s32 kwlnRenderFrame(void) {
    s32 buf = func_00100400();
    u8 *target;
    u8 *block;
    u64 *list;
    u64 *packet;
    u64 *packet2;
    KwlnSpriteVertex *vtx;
    s32 *column;
    s32 *pool;
    s32 rect[4];
    s32 i;

    kwlnDrawBlurErrorCounters();
    func_00106188();
    func_00106288();
    kwlnFadeUpdate();
    func_00105CF8();
    func_00107108();
    target = D_00380788;
    block = D_003808B0 + buf * 0x1F40;
    for (i = 12; i >= 0; i--) {
        sdfSubmitDrawPacketGroups(target, block);
        target += 0x10;
        block += 0x1B0;
    }
    if (kwlnDrawOverlayEnabled != 0 && func_001200E0() == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        sdfAppendDmaPrimary(list, kwlnFrameDrawPacketRecords + buf * 0x1F40, sdfAllocPacketAligned(0x20));
        packet = sdfAllocPacketAligned(0x40);
        packet[0] = 3;
        packet[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        packet[2] = ((u64)0x10000000 << 32) | 0x8002;
        packet[3] = 0xE;
        packet[4] = ((u64)0x80 << 32) | 0x80;
        packet[5] = 0x3B;
        packet[6] = 0;
        packet[7] = 0x3F;
        sdfAppendPacket(list, packet);
        packet2 = sdfAllocPacketAligned(0x40);
        packet2[0] = 3;
        packet2[1] = ((u64)0x50000003 << 16 | 0x1000) << 16;
        packet2[2] = ((u64)0x10000000 << 32) | 0x8002;
        packet2[3] = 0xE;
        packet2[4] = 0x31001;
        packet2[5] = 0x47;
        packet2[6] = 0x44;
        packet2[7] = 0x42;
        sdfAppendPacket(list, packet2);
        rect[0] = D_00435CE0[0] + 0x100;
        rect[1] = D_00435CE0[1] + 0xE0;
        rect[2] = 0x100 - D_00435CE0[0];
        rect[3] = 0xE0 - D_00435CE0[1];
        for (i = 0; i != 4; i++) {
            rect[i] = rect[i] * kwlnDrawOverlayScale >> 12;
        }
        column = sdfConsAllocateColumnPacket(1);
        vtx = sdfConsMeasurePacketWithHeader(column);
        vtx->r = 0x80;
        vtx->g = 0x80;
        vtx->b = 0x80;
        vtx->a = kwlnDrawOverlayAlpha;
        vtx->corner[0].u = 0;
        vtx->corner[0].v = 0;
        vtx->corner[0].x = 0x7000 - rect[0] * 16;
        vtx->corner[0].y = 0x7900 - rect[1] * 8;
        vtx->corner[0].mask = 0;
        vtx->corner[0].flag = 0;
        vtx->corner[1].u = 0x2000;
        vtx->corner[1].v = 0xE00;
        vtx->corner[1].x = 0x9000 + rect[2] * 16;
        vtx->corner[1].y = 0x8700 + rect[3] * 8;
        vtx->corner[1].mask = 0;
        vtx->corner[1].flag = 0;
        sdfAppendPacket(list, column);
        ((KwlnDrawSink *)D_00380708)->invoke(D_00380708, list);
    }
    pool = sdfFlushPoolNodes(kwlnDrawSurfaces);
    D_00435C14 = pool;
    if (D_00435BC8 != 0) {
        func_00343468(pool[1]);
        D_00435BC8 = 0;
    }
    if (!(kwlnDrawControlFlags & 0x2000000)) {
        func_0032DD98(pool, D_00380860);
    } else {
        kwlnDrawControlFlags &= 0xFDFFFFFF;
    }
    D_00438A20 = 0;
    return 0;
}

u32 func_00102740(void) {
    func_0010FC28(dds3ActiveWorld);
    return 0;
}

u32 func_00102768(void) {
    func_0010FC68(dds3ActiveWorld);
    return 0;
}

AdminWork *dds3GetAdminTaskWork(void) {
    return (AdminWork *)kwlnTaskGetUserValue(func_00101740(D_00435C18));
}

u32 dds3GetAdminTaskValue(void) {
    AdminWork *work;

    work = (AdminWork *)dds3GetAdminTaskWork();
    return work->value;
}

extern void *func_00328D68(s32 size);

/* Replace the admin task's attached data block (copied, max 0x100 bytes) and set its mode byte and flags. */
void dds3AdminSubmitModeRequest(s32 value, void *data, u32 size, s32 flag) {
    AdminWork *work;
    void *old;
    u32 flags;

    if (data == NULL || size <= 0x100) {
        work = dds3GetAdminTaskWork();
        old = work->unk1C;
        work->unk09 = value;
        flags = work->flags;
        flags |= 1;
        flags &= ~8;
        flags &= ~0x10000;
        work->flags = flags;
        work->unk21 = 2;
        if (old != NULL) {
            sdfReleaseChipBlock(old);
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (data != NULL) {
            work->unk1C = func_00328D68(size);
            memcpy(work->unk1C, data, size);
            work->unk20 = size;
        } else {
            work->unk1C = NULL;
            work->unk20 = 0;
        }
        if (flag != 0) {
            work->flags |= 4;
        } else {
            work->flags &= ~4;
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C0C);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C14);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C18);

