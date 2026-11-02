#include "common.h"
#include "eeregs.h"
#include "libdma.h"
#include "sdfGsMemIO.h"
#include "sdf_types.h"
#include "sdf_resource.h"

/* Register selectors carried by GIF A+D writes in the two GS contexts. */
enum {
    SDF_GS_PRIM = 0x00,
    SDF_GS_RGBAQ = 0x01,
    SDF_GS_XYZ2 = 0x05,
    SDF_GS_TEX0_1 = 0x06,
    SDF_GS_TEX0_2 = 0x07,
    SDF_GS_CLAMP_1 = 0x08,
    SDF_GS_CLAMP_2 = 0x09,
    SDF_GIF_REGISTER_AD = 0x0E,
    SDF_GS_TEX1_1 = 0x14,
    SDF_GS_TEX1_2 = 0x15,
    SDF_GS_XYOFFSET_1 = 0x18,
    SDF_GS_XYOFFSET_2 = 0x19,
    SDF_GS_PRMODECONT = 0x1A,
    SDF_GS_TEXA = 0x3B,
    SDF_GS_TEXFLUSH = 0x3F,
    SDF_GS_SCISSOR_1 = 0x40,
    SDF_GS_SCISSOR_2 = 0x41,
    SDF_GS_ALPHA_1 = 0x42,
    SDF_GS_ALPHA_2 = 0x43,
    SDF_GS_DTHE = 0x45,
    SDF_GS_COLCLAMP = 0x46,
    SDF_GS_TEST_1 = 0x47,
    SDF_GS_TEST_2 = 0x48,
    SDF_GS_FRAME_1 = 0x4C,
    SDF_GS_FRAME_2 = 0x4D,
    SDF_GS_ZBUF_1 = 0x4E,
    SDF_GS_ZBUF_2 = 0x4F,
    SDF_GS_BITBLTBUF = 0x50,
    SDF_GS_TRXPOS = 0x51,
    SDF_GS_TRXREG = 0x52,
    SDF_GS_TRXDIR = 0x53
};
/* Values written to the PRIM and TEST registers by the centered-bounds packet. */
enum {
    SDF_GS_PRIMITIVE_SPRITE = 0x06,
    SDF_GS_CENTERED_VIEW_BOUNDS_TEST = 0x30003
};

extern u8 sdfFrameId;

#define SDF_DMA_TAG_REF_BYTE 0x30

#define SDF_DMA_CNT_ONE_WORD 0x10000001

#define SDF_VIF_DIRECT_ONE_FLUSHE 0x5000000110000000ULL

#define SDF_GIF_PACKED_AD_BITS 0x1000000000008000ULL

#define SDF_VIF_DIRECT_ONE_NOP 0x5000000100000000ULL

#define SDF_GS_DEFAULT_TEXA 0x4000000080ULL

#define SDF_GS_SCENE_TEST 0x517FB

#define SDF_TEXTURE_SCENE_PAYLOAD_QWORDS 0x15

#define SDF_GRAPH_DEPTH_BUFFER_INDEX 2

#define D_0055188A sdfGrBufferSlotIndex

extern u8 D_0055188A;

extern u16 sdfReqDrawDelaySleepCount;

extern u32 sdfDrawFrameCount;

extern u8 sdfDrawImageTrans;

extern volatile u8 sdfDrawSyncFlag;

extern u32 sdfGetRunTimeCountBase(void);

extern u32 sdfGetRunTimeCount(u32 aBaseTime);

extern void sdfWaitVblank(s32 count);

struct sdfPrimTransElem_t;

struct sdfGrRefPacketHeader_t;

struct sdfPreDrawHook_t;

struct sdfGrDrawFramePacket_t;

struct sdfGrClearPacket_t;

struct sdfGrRefGifPacketHeader_t;

struct sdfGrDrawFrame_t;


struct sdfGrDrawEnv_t;

struct sdfGsMemSlice_t;

struct sdfGrFrameSet_t;

struct sdfDrawTag_t;

struct sdfGrOt_t;

struct sdfGrFramePacket_t;

struct SdfDmaTag;

struct sdfDrawReqRec_t;

struct sdfPreDrawTag_t;

struct sdfPreDrawChain_t;

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)();

#define SDF_DMA_TAG_NEXT_BYTE 0x20

#define SDF_DMA_TAG_CALL_BYTE 0x50

#define SDF_DMA_TAG_END_BYTE 0x70

#define SDF_DMA_QWC_MASK 0xFFFF

#define SDF_DMA_ADDRESS_MASK 0x0FFFFFFF

#define SDF_QWORD_BYTES 0x10

#define SDF_DMA_EXTENDED_TAIL_OFFSET 0x30

#define SDF_DMA_REFERENCE_NODE_BYTES 0x20

#define SDF_DMA_TAG_REF_WORD 0x30000000

#define SDF_DMA_TAG_NEXT_WORD 0x20000000

#define SDF_VIF_DIRECT_WORD 0x50000000

#define SDF_VIF_FLUSHE_WORD 0x10000000

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GIF_EOP_BIT 0x8000

#define SDF_GS_CENTER_BIAS 0x1000

#define SDF_GS_FIELD_OFFSET_STEP 8

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_SCENE_DRAW_PAYLOAD_QWORDS 5

extern volatile s8 sdfReqDrawDelayCount;

/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct sdfPrimTransElem_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;

/* REF tag and VIF commands followed by the trailing chain tag (0x20).
 * The complete trailing quadword is cleared before its NEXT header is patched. */
typedef struct sdfGrRefPacketHeader_t {
    u64 dmaTag;
    u64 vifCommands;
    s128 nextTag;
} sdfGrRefPacketHeader_t;

/* Pre-draw hook: the engine's record continues with two option words, which
 * each owner reads under its own name. */
typedef struct sdfPreDrawHook_t {
    struct sdfPreDrawHook_t *next;
    SdfPacketPatchCallback predrawFunc;
} sdfPreDrawHook_t;

/* Four A+D writes define one GS drawing context, without a DMA/GIF header. */
typedef struct sdfGrDrawFramePacket_t {
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuf;
    sdfGrTagStdReg_t xyoffset;
    sdfGrTagStdReg_t scissor;
} sdfGrDrawFramePacket_t;

/* A+D writes that draw the centered viewport bounds as a sprite. */
typedef struct sdfGrClearPacket_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t primitive;
    sdfGrTagStdReg_t rgbaq;
    sdfGrTagStdReg_t xyz2[2];
} sdfGrClearPacket_t;

/* Shared A+D state for GS primitive modes, color clamping, dithering and TEXA. */
typedef struct sdfGrRefGifPacketHeader_t {
    sdfGrTagStdReg_t dmatag;
    sdfGrTagStdReg_t giftag;
    sdfGrTagStdReg_t reftag;
    sdfGrTagStdReg_t linktag;
} sdfGrRefGifPacketHeader_t;

/* Accessed output span of the common scene-context builder. */
typedef struct sdfGrDrawFrame_t {
    sdfGrPacketHeader_t header;
    sdfGrDrawFramePacket_t packet;
} sdfGrDrawFrame_t;

/* Complete DMA/GIF owner used by scene builders and held-texture draw banks. */
typedef struct sdfGrDrawEnv_t {
    sdfGrPacketHeader_t header;
    sdfGrRefGifPacketHeader_t defParm;
    sdfGrDrawFramePacket_t frame1;
    sdfGrDrawFramePacket_t frame2;
    sdfGrClearPacket_t clear;
    sdfGrTagStdReg_t test1;
    sdfGrTagStdReg_t alpha1;
    sdfGrTagStdReg_t test2;
    sdfGrTagStdReg_t alpha2;
} sdfGrDrawEnv_t;


/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 h;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;


/* Draw-surface pool entry (0x20); the SDK initializes and flushes this owner. */
typedef struct sdfGrOt_t {
    struct sdfGrOt_t *next; /* 0x00 */
    sdfDrawTag_t *tagHead;     /* 0x04 */
    sdfDrawTag_t *tagTail;     /* 0x08 */
    u32 noSortTag;
    void (*regFunc)(struct sdfGrOt_t *, sdfDrawTag_t *);          /* 0x10 */
    s32 (*callbackFunc)(struct sdfGrOt_t *, s32, sdfDrawTag_t *); /* 0x14 */
    u32 data;
    u32 pad;
} sdfGrOt_t;

/* Frame/depth builder state: one DMA/GIF header and two A+D register writes. */
typedef struct sdfGrFramePacket_t {
    u8 pad00[4];
    void (*prepare)(struct sdfGrFramePacket_t *, s32 bufferIndex);
    u8 pad08[8];
    sdfGrPacketHeader_t packetHeader;
    sdfGrTagStdReg_t frame;
    sdfGrTagStdReg_t zbuff;
    sdfGrFrameSet_t *frameSet;
    u32 frameMask;
    s32 zbuffMask;
    s32 context;
} sdfGrFramePacket_t;

/* Packet-list code views the same 16-byte row as QWC/ID/ADDR/VIF commands. */
typedef struct SdfDmaTag {
    u16 quadwordCount;
    u8 pad02;
    u8 kind;
    u32 address;
    u64 vifCommands;
} QWORD_ALIGNED SdfDmaTag;

void sdfDrawTagLink2(struct sdfDrawTag_t *aTag, u32 aHeadPacket,
                          u32 aTailPacket);

void sdfDrawTagLinkRef(struct sdfDrawTag_t *aTag, u32 aPacket);

/* One 0x10-byte entry in the three-slot frame packet submission ring. */
typedef struct sdfDrawReqRec_t {
    struct sdfDrawTag_t *drawTag;
    sdfPreDrawHook_t *preDraw;
    u8 drawCount;
    u8 queuedBufferIndex;
    u8 bufferIndex;
    u8 bufferSlotIndex;
    u8 pad0C[4];
} sdfDrawReqRec_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 next;
    sdfPreDrawHook_t *head;
    sdfPreDrawHook_t *tail;
    u32 pad;
} sdfPreDrawTag_t;

/* Frame submission joins these 0x10-byte linked owners through head/tail. */
typedef struct sdfPreDrawChain_t {
    sdfPreDrawTag_t *tagHead;
    sdfPreDrawTag_t *tail;
} sdfPreDrawChain_t;

typedef struct sdfTexPacket_t {
    sdfGifTag_t giftag;
    sdfGrTagStdReg_t tex1;
    sdfGrTagStdReg_t tex0;
    sdfGrTagStdReg_t clamp;
} sdfTexPacket_t;

/* Linked draw environment, including its two frame-mask writes and textures. */
typedef struct sdfGrStdDrawEnv_t {
    sdfPreDrawHook_t link;
    sdfGrFrameSet_t *view;
    u8 unk0C[4];
    sdfGrPacketHeader_t header;
    sdfGrRefGifPacketHeader_t defParm;
    sdfGrDrawFramePacket_t frame1;
    sdfGrDrawFramePacket_t frame2;
    sdfGrClearPacket_t clear;
    sdfGrTagStdReg_t test1;
    sdfGrTagStdReg_t alpha1;
    sdfGrTagStdReg_t test2;
    sdfGrTagStdReg_t alpha2;
    sdfGrTagStdReg_t frameContextOne;
    sdfGrTagStdReg_t frameContextTwo;
    sdfTexPacket_t texturePackets[2];
} sdfGrStdDrawEnv_t;

void sdfGrStdDrawEnvPredraw(sdfGrStdDrawEnv_t *aPre, s32 aEvenOdd);

void sdfGrSetRefGifPacket(sdfGrRefGifPacketHeader_t *aPacket, u32 aRegs, s32 aNumRegs);

void sdfGrSetDrawDefParmPacket(sdfGrRefGifPacketHeader_t *aDef);

extern s32 EIntr(void);

extern s32 DIntr(void);

extern s32 sdfDrawReqQueueIndex;

extern sdfDrawReqRec_t sdfGrDrawState[];

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfDrawTagInit(sdfDrawTag_t *a);

void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket);

void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, u32 aPacket);

void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, u32 aPacket);

void sdfGrOtFuncLinkToHead(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

void sdfGrOtNormalReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag);

s32 sdfGrOtNormalCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData);

void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt);

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *ex);

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *p, SdfDmaTag *ex);

void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack);

void sdfGrInitExPacket(sdfDrawTag_t *p);

/* DDS keeps its own name: the shorter one shifts DDS1 code_001DEFE0. */
#define sdfGrOtClose sdfFlushPoolNodes
sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag);

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords);

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                  s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat,
                  s32 aHalfOffset, s32 aContext);

void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat);

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                           s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat, s32 aContext);

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv);

void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt);

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *aPre, s32 aEvenOdd);

void sdfGrSetFramePacket(sdfGrFramePacket_t *aFrame, sdfGrFrameSet_t *aFrameSet, u32 aFrameMask, s32 aZBuffMask, s32 aContext);

void sdfGrReqDrawDelay(s32 count);

void sdfGrInitDraw(void);

void sdfDrawTagInit(sdfDrawTag_t *a) {
    a->z = 0xFFFF;
    a->next = NULL;
    a->head = 0;
    a->tail = 0;
    a->lightPacket = 0;
    a->fogPacket = 0;
    a->texPacket[0] = 0;
    a->texPacket[1] = 0;
}

void sdfDrawTagLink(sdfDrawTag_t *aTag, u32 aPacket) {
    s32 last;

    last = aTag->tail;
    if (last == 0) {
        aTag->head = aPacket;
    }
    else {
        ((SdfDmaTag *)last)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)last)->address = aPacket & SDF_DMA_ADDRESS_MASK;
    }
    aTag->tail = aPacket;
}

void sdfDrawTagLink2(sdfDrawTag_t *aTag, u32 aHeadPacket, u32 aTailPacket) {
    s32 previousTailAddress;

    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aHeadPacket;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = aHeadPacket & SDF_DMA_ADDRESS_MASK;
    }
    aTag->tail = aTailPacket;
}

void sdfDrawTagLinkRef(sdfDrawTag_t *aTag, u32 aPacket) {
    s32 previousTailAddress;

    ((SdfDmaTag *)aPacket)->kind = SDF_DMA_TAG_REF_BYTE;
    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aPacket;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = aPacket & SDF_DMA_ADDRESS_MASK;
    }
    aTag->tail = aPacket + SDF_QWORD_BYTES;
}

void sdfDrawTagLinkRefGif(sdfDrawTag_t *aTag, u32 aPacket) {
    s32 previousTailAddress;

    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aPacket;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = aPacket & SDF_DMA_ADDRESS_MASK;
    }
    aTag->tail = aPacket + SDF_DMA_EXTENDED_TAIL_OFFSET;
}

void sdfDrawTagLinkCall(sdfDrawTag_t *aTag, u32 aPacket) {
    s32 previousTailAddress;

    ((SdfDmaTag *)aPacket)->kind = SDF_DMA_TAG_CALL_BYTE;
    previousTailAddress = aTag->tail;
    if (previousTailAddress == 0) {
        aTag->head = aPacket;
    }
    else {
        ((SdfDmaTag *)previousTailAddress)->kind = SDF_DMA_TAG_NEXT_BYTE;
        ((SdfDmaTag *)previousTailAddress)->address = aPacket & SDF_DMA_ADDRESS_MASK;
    }
    aTag->tail = aPacket + SDF_QWORD_BYTES;
}

void sdfGrOtFuncLinkToHead(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    sdfDrawTag_t *firstList;

    if (aTag->tail == 0) {
        return;
    }
    firstList = aOt->tagHead;
    if (firstList == NULL) {
        aOt->tagTail = aTag;
    } else {
        sdfGrJoinDrawTag(aTag, firstList);
    }
    aTag->next = firstList;
    aOt->tagHead = aTag;
}

void sdfGrOtNormalReg(sdfGrOt_t *aOt, sdfDrawTag_t *aTag) {
    sdfDrawTag_t *last;

    if (aTag->head != 0) {
        last = aOt->tagTail;
        if (last == NULL) {
            aOt->tagHead = aTag;
        }
        else {
            last->next = aTag;
            sdfGrJoinDrawTag(last, aTag);
        }
        aOt->tagTail = aTag;
    }
}

s32 sdfGrOtNormalCallback(sdfGrOt_t *aOt, s32 aFunc, sdfDrawTag_t *aData) {
    if (aFunc == 1) {
        sdfGrOtFuncLinkToHead(aOt, aData);
    }
}

void sdfGrOtInit(sdfGrOt_t *aOt, s32 aNumOt) {
    sdfGrOt_t *node = aOt;
    sdfGrOt_t *next;
    s32 remaining = aNumOt - 1;

    node->regFunc = sdfGrOtNormalReg;
    node->callbackFunc = sdfGrOtNormalCallback;
    node->tagHead = 0;
    node->tagTail = 0;
    node->noSortTag = 0;
    node->data = 0;
    if (remaining == 0) {
        node->next = NULL;
        return;
    }
link:
    next = node + 1;
    remaining--;
    node->next = next;
    node = next;
    next->regFunc = sdfGrOtNormalReg;
    next->callbackFunc = sdfGrOtNormalCallback;
    next->tagHead = 0;
    next->tagTail = 0;
    next->noSortTag = 0;
    next->data = 0;
    if (remaining != 0) {
        goto link;
    }
    node->next = NULL;
}

sdfGrRefPacketHeader_t *sdfGrNewExPacket(SdfDmaTag *ex) {
    sdfGrRefPacketHeader_t *referenceNode = (sdfGrRefPacketHeader_t *)sdfGrAllocPacketBuff(SDF_DMA_REFERENCE_NODE_BYTES);
    u64 dmaHeader = ex->quadwordCount;
    u32 payloadAddress = ((u32)ex + SDF_QWORD_BYTES) & SDF_DMA_ADDRESS_MASK;
    s64 packedAddress = (s64)payloadAddress << 32;

    dmaHeader |= SDF_DMA_TAG_REF_WORD;
    dmaHeader |= packedAddress;
    referenceNode->dmaTag = dmaHeader;
    QUAD_ZERO(referenceNode->nextTag);
    referenceNode->vifCommands = ex->vifCommands;
    return referenceNode;
}

SdfDmaTag *sdfGrAddExPacket(SdfDmaTag *p, SdfDmaTag *ex) {
    sdfGrRefPacketHeader_t *referenceNode;

    referenceNode = sdfGrNewExPacket(ex);
    p->kind = SDF_DMA_TAG_NEXT_BYTE;
    p->address = (u32)referenceNode & SDF_DMA_ADDRESS_MASK;
    return (SdfDmaTag *)((u32)referenceNode + SDF_QWORD_BYTES);
}

void sdfGrJoinDrawTag(sdfDrawTag_t *aFore, sdfDrawTag_t *aBack) {
    SdfDmaTag *tailTag = (SdfDmaTag *)aFore->tail;
    u32 previousSource;
    u32 incomingSource;

    previousSource = aFore->lightPacket;
    incomingSource = aBack->lightPacket;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            aBack->lightPacket = previousSource;
        }
    }
    previousSource = aFore->fogPacket;
    incomingSource = aBack->fogPacket;
    if (previousSource != incomingSource) {
        if (incomingSource != 0) {
            tailTag = sdfGrAddExPacket(tailTag, (SdfDmaTag *)incomingSource);
        } else {
            aBack->fogPacket = previousSource;
        }
    }
    tailTag->kind = SDF_DMA_TAG_NEXT_BYTE;
    tailTag->address = aBack->head & SDF_DMA_ADDRESS_MASK;
}

void sdfGrInitExPacket(sdfDrawTag_t *p) {
    sdfGrRefPacketHeader_t *referenceNode;
    u32 priorHeadAddress;
    u32 sourceTagAddress;

    sourceTagAddress = p->fogPacket;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = p->head & SDF_DMA_ADDRESS_MASK;
        p->head = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
    sourceTagAddress = p->lightPacket;
    if (sourceTagAddress != 0) {
        referenceNode = sdfGrNewExPacket((SdfDmaTag *)sourceTagAddress);
        priorHeadAddress = p->head & SDF_DMA_ADDRESS_MASK;
        p->head = (u32)referenceNode;
        *(u64 *)&referenceNode->nextTag = ((s64)priorHeadAddress << 32) | SDF_DMA_TAG_NEXT_WORD;
    }
}

sdfDrawTag_t *sdfGrOtClose(sdfGrOt_t *aOt) {
    sdfDrawTag_t *tail = NULL;
    sdfDrawTag_t *head = NULL;

    for (; aOt != NULL; aOt = aOt->next) {
        aOt->callbackFunc(aOt, 0, NULL);
        if (aOt->tagHead != 0) {
            if (head != 0) {
                sdfGrJoinDrawTag(tail, aOt->tagHead);
            } else {
                head = aOt->tagHead;
                sdfGrInitExPacket(head);
            }
            tail = aOt->tagTail;
        }
    }
    if (tail != NULL) {
        ((SdfDmaTag *)tail->tail)->kind = SDF_DMA_TAG_END_BYTE;
        ((SdfDmaTag *)tail->tail)->address = 0;
    }
    return head;
}

void sdfPreDrawTagInit(sdfPreDrawTag_t *aTag) {
    aTag->next = 0;
    aTag->head = 0;
    aTag->tail = 0;
    aTag->pad = 0;
}

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook) {
    if (aTag->tail == 0) {
        aTag->head = aHook;
    }
    else {
        aTag->tail->next = aHook;
    }
    aTag->tail = aHook;
    aHook->next = NULL;
}

void sdfPreDrawChainInit(sdfPreDrawChain_t *aChain) {
    aChain->tagHead = NULL;
    aChain->tail = NULL;
}

void sdfPreDrawChainLink(sdfPreDrawChain_t *aChain, sdfPreDrawTag_t *aTag) {
    if (aChain->tail == NULL) {
        aChain->tagHead = aTag;
    }
    else {
        aChain->tail->tail->next = aTag->tail;
    }
    aChain->tail = aTag;
}

void sdfGrSetRegsPacketHeader(sdfGrPacketHeader_t *header, s32 payloadQwords) {
    s64 dmaQwords;

    dmaQwords = payloadQwords + 1;
    header->dmaWords.vif = (((dmaQwords | SDF_VIF_DIRECT_WORD) << 32) | SDF_VIF_FLUSHE_WORD);
    header->giftag.parm = (payloadQwords | (((s64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_EOP_BIT));
    header->dmaWords.tag = dmaQwords;
    header->giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfGrSetRefGifPacket(sdfGrRefGifPacketHeader_t *aPacket, u32 aRegs, s32 aNumRegs) {
    aPacket->dmatag.data = SDF_DMA_CNT_ONE_WORD;
    aPacket->dmatag.reg = SDF_VIF_DIRECT_ONE_FLUSHE;
    aPacket->giftag.data = (u64)aNumRegs | SDF_GIF_PACKED_AD_BITS;
    aPacket->giftag.reg = SDF_GIF_REGISTER_AD;
    aPacket->reftag.data = (u32)((aNumRegs & SDF_DMA_QWC_MASK) | SDF_DMA_TAG_REF_WORD) | ((u64)(aRegs & SDF_DMA_ADDRESS_MASK) << 32);
    aPacket->reftag.reg = SDF_VIF_DIRECT_ONE_NOP;
    aPacket->linktag.data = SDF_DMA_TAG_NEXT_WORD;
    aPacket->linktag.reg = 0;
}

void sdfGrSetDrawFramePacket(sdfGrDrawFramePacket_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                  s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat,
                  s32 aHalfOffset, s32 aContext) {
    s64 frameRegisterId;
    s64 depthRegisterId;
    s64 offsetRegisterId;
    s64 scissorRegisterId;
    s32 xOffset;
    s32 yOffset;

    if (aContext == 0) {
        frameRegisterId = SDF_GS_FRAME_1;
        depthRegisterId = SDF_GS_ZBUF_1;
        offsetRegisterId = SDF_GS_XYOFFSET_1;
        scissorRegisterId = SDF_GS_SCISSOR_1;
    } else {
        frameRegisterId = SDF_GS_FRAME_2;
        depthRegisterId = SDF_GS_ZBUF_2;
        offsetRegisterId = SDF_GS_XYOFFSET_2;
        scissorRegisterId = SDF_GS_SCISSOR_2;
    }
    aFrame->scissor.reg = scissorRegisterId;
    yOffset = (SDF_GS_CENTER_BIAS - aH) << 3;
    xOffset = (SDF_GS_CENTER_BIAS - aW) << 3;
    aFrame->frame.data = (s64)(aPixelFormat << 24) | (s64)(((aW + 63) >> 6) << 16) | (s64)(aFrameAddr >> 11);
    if (aHalfOffset != 0) {
        yOffset += SDF_GS_FIELD_OFFSET_STEP;
    }
    aFrame->frame.reg = frameRegisterId;
    aFrame->zbuf.data = (s64)(aZBuffFormat << 24) | (s64)(aZBuffAddr >> 11);
    aFrame->zbuf.reg = depthRegisterId;
    aFrame->xyoffset.reg = offsetRegisterId;
    aFrame->scissor.data = ((s64)(aH - 1) << 48) | ((s64)(aW - 1) << 16);
    aFrame->xyoffset.data = xOffset | ((s64)yOffset << 32);
}

void sdfGrSetClearPacket(sdfGrClearPacket_t *aClear, s32 aW, s32 aH, s32 aPixelFormat, s32 aZBuffFormat) {
    u32 lowerBounds = ((SDF_GS_CENTER_BIAS - aH) << 19) | ((SDF_GS_CENTER_BIAS - aW) << 3);
    u32 upperBounds = ((aH + SDF_GS_CENTER_BIAS) << 19) | ((aW + SDF_GS_CENTER_BIAS) << 3);

    aClear->primitive.reg = SDF_GS_PRIM;
    aClear->xyz2[0].reg = SDF_GS_XYZ2;
    aClear->test.data = SDF_GS_CENTERED_VIEW_BOUNDS_TEST;
    aClear->test.reg = SDF_GS_TEST_1;
    aClear->primitive.data = SDF_GS_PRIMITIVE_SPRITE;
    aClear->rgbaq.data = (u64)0xFE00 << 46;
    aClear->rgbaq.reg = SDF_GS_RGBAQ;
    aClear->xyz2[0].data = lowerBounds;
    aClear->xyz2[1].data = upperBounds;
    aClear->xyz2[1].reg = SDF_GS_XYZ2;
}

void sdfGrSetDrawDefParmPacket(sdfGrRefGifPacketHeader_t *aDef) {
    aDef->dmatag.data = 1;
    aDef->dmatag.reg = SDF_GS_PRMODECONT;
    aDef->giftag.data = 1;
    aDef->giftag.reg = SDF_GS_COLCLAMP;
    aDef->reftag.data = 0;
    aDef->reftag.reg = SDF_GS_DTHE;
    aDef->linktag.data = SDF_GS_DEFAULT_TEXA;
    aDef->linktag.reg = SDF_GS_TEXA;
}

void sdfGrSetDrawFrame(sdfGrDrawFrame_t *aFrame, s32 aFrameAddr, s32 aW, s32 aH,
                           s32 aPixelFormat, s32 aZBuffAddr, s32 aZBuffFormat, s32 aContext) {
    sdfGrSetRegsPacketHeader(&aFrame->header, SDF_SCENE_DRAW_PAYLOAD_QWORDS);
    sdfGrSetDrawFramePacket(&aFrame->packet, aFrameAddr, aW, aH, aPixelFormat, aZBuffAddr, aZBuffFormat, 0, aContext);
}

void sdfGrSetDrawEnv(sdfGrDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet, s32 aEvenOdd) {
    s32 frameAddress;
    s32 depthAddress;
    s32 width;
    s32 height;
    s32 frameFormat;
    s32 depthFormat;

    sdfGrSetRegsPacketHeader(&aEnv->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    frameAddress = aFrameSet->buffers[aEvenOdd]->addr;
    depthAddress = aFrameSet->buffers[SDF_GRAPH_DEPTH_BUFFER_INDEX]->addr;
    width = aFrameSet->width;
    frameFormat = aFrameSet->bufferFormat;
    height = aFrameSet->height;
    depthFormat = aFrameSet->auxiliaryFormat;
    sdfGrSetDrawFramePacket(&aEnv->frame1, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_0055188A, 0);
    sdfGrSetDrawFramePacket(&aEnv->frame2, frameAddress, width, height, frameFormat, depthAddress, depthFormat, D_0055188A, 1);
    sdfGrSetClearPacket(&aEnv->clear, aFrameSet->width, aFrameSet->height, aFrameSet->bufferFormat, aFrameSet->auxiliaryFormat);
    aEnv->test1.data = SDF_GS_SCENE_TEST;
    aEnv->test1.reg = SDF_GS_TEST_1;
    aEnv->alpha1.data = SDF_GS_DEFAULT_ALPHA;
    aEnv->alpha1.reg = SDF_GS_ALPHA_1;
    aEnv->test2.data = SDF_GS_SCENE_TEST;
    aEnv->test2.reg = SDF_GS_TEST_2;
    aEnv->alpha2.data = SDF_GS_DEFAULT_ALPHA;
    aEnv->alpha2.reg = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&aEnv->defParm);
}

extern void sdfTexSetPacket(sdfTexPacket_t *packet, s32 width, s32 height, s32 gsAddr,
                            s32 pixelFormat, s32 clutAddr, s32 clutFormat, s32 colorComp,
                            s32 mipmapCount, s32 mipmapParm, s32 wrapMode, s32 context);

#ifdef NON_MATCHING
/* 86 of 110 words; first differs at +0x88, where the scheduler issues the
 * height argument copy before the pixel-format one. The whole body is one
 * basic block, so the remaining 24 words are that one tie cascading. */
void sdfGrStdDrawEnvPredraw(sdfGrStdDrawEnv_t *env, s32 evenOdd)
{
    sdfGrFrameSet_t *view = env->view;
    s32 width = view->width;
    s32 height = view->height;
    s32 frameAddress = view->buffers[evenOdd]->addr;
    s32 depthAddress = view->buffers[2]->addr;
    s32 frameFormat = view->bufferFormat;
    s32 depthFormat = view->auxiliaryFormat;
    s64 frame;

    sdfGrSetDrawFramePacket(&env->frame1, frameAddress, width, height, frameFormat,
                            depthAddress, depthFormat, D_0055188A, 0);
    sdfGrSetDrawFramePacket(&env->frame2, frameAddress, width, height, frameFormat,
                            depthAddress, depthFormat, D_0055188A, 1);
    frame = (s64)(s32)((u32)(frameAddress >> 11) | ((u32)frameFormat << 24) |
                       ((u32)(width >> 6) << 16)) | 0x00FFFFFF00000000ULL;
    env->frameContextOne.data = frame;
    env->frameContextOne.reg = SDF_GS_FRAME_1;
    env->frameContextTwo.data = frame;
    env->frameContextTwo.reg = SDF_GS_FRAME_2;
    if (frameFormat == 0) frameFormat = 1;
    sdfTexSetPacket(&env->texturePackets[1], width, height, frameAddress, frameFormat,
                    0, 0, 1, 0, 0, 5, 0);
    sdfTexSetPacket(&env->texturePackets[0], width, height, view->buffers[evenOdd ^ 1]->addr,
                    frameFormat, 0, 0, 1, 0, 0, 5, 0);
}
#else
INCLUDE_ASM(const s32, "sdf/sdfDraw", sdfGrStdDrawEnvPredraw);
#endif

void sdfGrSetStdDrawEnv(sdfGrStdDrawEnv_t *aEnv, sdfGrFrameSet_t *aFrameSet) {
    sdfGrSetRegsPacketHeader(&aEnv->header, SDF_TEXTURE_SCENE_PAYLOAD_QWORDS);
    aEnv->view = aFrameSet;
    aEnv->link.predrawFunc = sdfGrStdDrawEnvPredraw;
    sdfGrSetClearPacket(&aEnv->clear, aFrameSet->width, aFrameSet->height, aFrameSet->bufferFormat, aFrameSet->auxiliaryFormat);
    aEnv->test1.data = SDF_GS_SCENE_TEST;
    aEnv->test1.reg = SDF_GS_TEST_1;
    aEnv->alpha1.data = SDF_GS_DEFAULT_ALPHA;
    aEnv->alpha1.reg = SDF_GS_ALPHA_1;
    aEnv->test2.data = SDF_GS_SCENE_TEST;
    aEnv->test2.reg = SDF_GS_TEST_2;
    aEnv->alpha2.data = SDF_GS_DEFAULT_ALPHA;
    aEnv->alpha2.reg = SDF_GS_ALPHA_2;
    sdfGrSetDrawDefParmPacket(&aEnv->defParm);
}

void sdfGrLinkStdDrawEnv(sdfDrawTag_t *aTag, sdfPreDrawTag_t *aPreDrawTag, sdfPreDrawHook_t *aEnv) {
    sdfPreDrawTagLink(aPreDrawTag, aEnv);
    sdfDrawTagLink(aTag, (s32)aEnv + SDF_QWORD_BYTES);
}

void sdfGrSetFrameMaskRGB(const u8 *aEnv, sdfGrRefGifPacketHeader_t *aHdr, s32 aContext) {
    if (aContext == 0) {
        sdfGrSetRefGifPacket(aHdr, (u32)(aEnv + 0x180), 1);
        return;
    }
    sdfGrSetRefGifPacket(aHdr, (u32)(aEnv + 400), 1);
}

void sdfGrSetFrameMaskReset(const u8 *aEnv, sdfGrRefGifPacketHeader_t *aHdr, s32 aContext) {
    if (aContext == 0) {
        sdfGrSetRefGifPacket(aHdr, (u32)(aEnv + 0x70), 1);
        return;
    }
    sdfGrSetRefGifPacket(aHdr, (u32)(aEnv + 0xb0), 1);
}

void sdfGrLinkDispFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt) {
    aPkt->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    aPkt->dmaTag = ((u64)((aEnv + 0x1a0) & 0xfffffff) << 32) | 0x30000004;
    QUAD_ZERO(aPkt->nextTag);
    sdfDrawTagLinkRef(aTag, (u32)aPkt);
}

void sdfGrLinkDrawFrameTex(sdfDrawTag_t *aTag, u32 aEnv, sdfGrRefPacketHeader_t *aPkt) {
    aPkt->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    aPkt->dmaTag = ((u64)((aEnv + 0x1e0) & 0xfffffff) << 32) | 0x30000004;
    QUAD_ZERO(aPkt->nextTag);
    sdfDrawTagLinkRef(aTag, (u32)aPkt);
}

void sdfGrFramePacketPredraw(sdfGrFramePacket_t *aPre, s32 aEvenOdd) {
    sdfGrFrameSet_t *view = aPre->frameSet;
    u64 frameRegister;
    u64 depthRegister;
    s16 widthPages;

    if (aPre->context == 0) {
        frameRegister = 0x4C;
        depthRegister = 0x4E;
    } else {
        frameRegister = 0x4D;
        depthRegister = 0x4F;
    }
    widthPages = (view->width + 63) >> 6;
    aPre->frame.data = ((u64)aPre->frameMask << 32) |
        (u64)(view->buffers[aEvenOdd]->addr >> 11) |
        ((s64)(view->bufferFormat << 24) | ((s64)widthPages << 16));
    aPre->frame.reg = frameRegister;
    aPre->zbuff.data = (s64)(view->auxiliaryFormat << 24) |
        (u64)(view->buffers[2]->addr >> 11);
    aPre->zbuff.reg = depthRegister;
}

void sdfGrSetFramePacket(sdfGrFramePacket_t *aFrame, sdfGrFrameSet_t *aFrameSet, u32 aFrameMask, s32 aZBuffMask, s32 aContext) {
    sdfGrSetRegsPacketHeader(&aFrame->packetHeader, 2);
    aFrame->context = aContext;
    aFrame->frameSet = aFrameSet;
    aFrame->frameMask = aFrameMask;
    aFrame->zbuffMask = aZBuffMask;
    aFrame->prepare = sdfGrFramePacketPredraw;
}

void sdfGrReqDraw(sdfDrawTag_t *aTag, sdfPreDrawChain_t *aPreDraw) {
    u32 start = sdfGetRunTimeCountBase();
    s32 interrupts;
    s32 index;
    sdfDrawReqRec_t *slot;

    while (sdfReqDrawDelayCount != 0) {
    }
    sdfReqDrawDelaySleepCount = sdfGetRunTimeCount(start);
    interrupts = DIntr();
    index = sdfDrawReqQueueIndex + 1;
    if (index == 3) {
        index = 0;
    }
    slot = &sdfGrDrawState[index];
    slot->drawTag = NULL;
    slot->preDraw = NULL;
    slot->drawCount = 0;
    if (aTag != NULL && aTag->tail != 0) {
        slot->drawTag = aTag;
        if (aPreDraw != NULL) {
            slot->preDraw = aPreDraw->tagHead->head;
            if (slot->preDraw != NULL) {
                aPreDraw->tail->tail->next = NULL;
            }
        }
        slot->queuedBufferIndex = sdfFrameId;
    }
    sdfDrawReqQueueIndex = index;
    sdfDrawImageTrans = 1;
    sdfDrawFrameCount++;
    if (interrupts != 0) {
        EIntr();
    }
}

void sdfGrReqDrawDelay(s32 count) {
    sdfReqDrawDelayCount = (count < 0) ? 0 : count;
}

void sdfGrInitDraw(void) {
    sdfReqDrawDelayCount = 0;
    sdfGrDrawState[0].drawTag = NULL;
    sdfDrawReqQueueIndex = 0;
}

/* The draw worker publishes this to the kernel process, which clears it. */
#define D_00551888 sdfGrFrameComplete

extern volatile u8 D_00551888;

extern volatile u8 sdfDrawFrameFlag;
extern volatile u8 sdfDispThreadFlag;
extern u8 sdfFrameEvenOdd;
extern u8 sdfGrDrawFrameId;
extern u8 sdfGrDrawFirstFlag;
extern u32 sdfGrDrawFrameTagHead;
extern s32 dds3GsThreadId;
extern s32 SignalSema(s32 sema);
extern s32 SleepThread(void);
extern void FlushCache(s32 mode);
extern sceDmaChan *sceDmaGetChan(u32 channel);
extern void sceDmaSend(sceDmaChan *channel, void *tag);
extern void sceDmaRecvN(sceDmaChan *channel, void *data, u32 qwc);
extern u64 sceGsGetIMR(void);
extern u64 sceGsPutIMR(u64 mask);
extern void sdfDeliverDrawStart(s32 frameId, s32 bufferId);
extern void sdfDeliverDrawEnd(s32 mode, s32 frameId, s32 halfOffset);
extern void sdfImageSetTrans(void);
extern void sdfDevConsDrawAll(void);
extern void sdfDelayedResrcProc(void);

void sdfGrDrawFrame(void)
{
    sdfDrawReqRec_t *request;
    sdfDrawTag_t *drawTag;
    sdfPreDrawHook_t *hook;
    sceDmaChan *channel;
    sdfStoreTransfer_t *transfer;
    u32 mask, resumeAddress, address;
    u8 bufferSlot, frameId, firstFlag, drawCount;
    s32 remaining, count;

    for (;;) {
        request = &sdfGrDrawState[sdfDrawReqQueueIndex];
        drawTag = request->drawTag;
        if (drawTag == NULL) {
            SignalSema(dds3GsThreadId);
            break;
        }
        if (request->drawCount == 2) {
            SignalSema(dds3GsThreadId);
            break;
        }
        if (request->drawCount == 0) {
            bufferSlot = sdfFrameEvenOdd ^ 1;
            request->bufferSlotIndex = bufferSlot;
            D_0055188A = bufferSlot;
        } else {
            bufferSlot = request->bufferSlotIndex ^ 1;
            request->bufferSlotIndex = bufferSlot;
            D_0055188A = bufferSlot;
        }
        request->bufferIndex = sdfGrDrawFrameId;
        sdfDeliverDrawStart(request->bufferIndex, request->queuedBufferIndex);
        SignalSema(dds3GsThreadId);
        if (sdfDrawImageTrans != 0) {
            sdfDrawImageTrans = 0;
            sdfImageSetTrans();
        }
        for (hook = request->preDraw; hook != NULL; hook = hook->next) {
            hook->predrawFunc(hook, request->bufferIndex);
        }
        FlushCache(0);
        channel = sceDmaGetChan(1);
        HW_FIELD_OR(channel->chcr, 0xC0);
        sdfDispThreadFlag = 3;
        sceDmaSend(channel, (void *)(drawTag->head & SDF_DMA_ADDRESS_MASK));
        sdfGrDrawFrameTagHead = drawTag->head;
        for (;;) {
            SleepThread();
            if (sdfDrawFrameFlag != 0) return;
            if (((HW_FIELD_READ(channel->chcr) >> 16) & 0x7000) == 0x7000) {
                break;
            }
            mask = (u32)sceGsGetIMR();
            sceGsPutIMR(mask | 0x200);
            HW_WRITE(GS_CSR, 2);
            HW_FIELD_OR(channel->chcr, 0x100);
            SleepThread();
            if ((HW_READ(GS_CSR) & 2) == 0) {
                do {
                    if (sdfDrawFrameFlag != 0) {
                        sceGsPutIMR(mask);
                        return;
                    }
                } while ((HW_READ(GS_CSR) & 2) == 0);
            }
            HW_WRITE(GS_CSR, 0);
            sceGsPutIMR(mask);
            HW_FIELD_OR(channel->chcr, 0x100);
            SleepThread();
            resumeAddress = HW_FIELD_READ(channel->tadr);
            transfer = (sdfStoreTransfer_t *)HW_FIELD_READ(channel->madr);
            while ((HW_READ(VIF1_STAT) & 3) != 0) {
                if (sdfDrawFrameFlag != 0) return;
            }
            while ((HW_READ(GIF_STAT) & 0x80) != 0) {
                if (sdfDrawFrameFlag != 0) return;
            }
            HW_WRITE(GIF_CTRL, 8);
            HW_WRITE(VIF1_STAT, 0x800000);
            HW_WRITE(GS_BUSDIR, 1);
            HW_WRITE(GIF_CTRL, 0);
            remaining = transfer->length;
            address = transfer->address;
            while (remaining > 0) {
                count = remaining;
                if (count > 0xF000) {
                    count = 0xF000;
                }
                remaining -= count;
                sceDmaRecvN(channel, (void *)address, count);
                SleepThread();
                if (sdfDrawFrameFlag != 0) break;
                address += count * 16;
            }
            HW_WRITE(VIF1_STAT, 0);
            HW_WRITE(GS_BUSDIR, 0);
            HW_FIELD_AND(channel->chcr, ~1);
            if (sdfDrawFrameFlag != 0) return;
            if (transfer->callback != NULL) transfer->callback(transfer->callbackParameter);
            sceDmaSend(channel, (void *)resumeAddress);
        }
        while ((HW_READ(VIF1_STAT) & 3) != 0) {
        }
        while ((HW_READ(GIF_STAT) & 0x180) != 0) {
        }
        sdfDeliverDrawEnd(request->drawCount, request->bufferIndex, request->bufferSlotIndex);
        drawCount = request->drawCount;
        frameId = sdfGrDrawFrameId;
        sdfGrDrawFirstFlag = 1;
        request->drawCount = drawCount + 1;
        sdfGrDrawFrameId = frameId ^ 1;
        break;
    }
    firstFlag = sdfGrDrawFirstFlag;
    D_00551888 = 1;
    if (firstFlag != 0) sdfDevConsDrawAll();
    sdfDelayedResrcProc();
}

void sdfDrawSync(void) {
    sdfDrawReqRec_t *table = sdfGrDrawState;

    for (;;) {
        while (sdfDrawSyncFlag != 0) {
        }
        if (table[sdfDrawReqQueueIndex].drawTag == NULL) {
            break;
        }
        if (table[sdfDrawReqQueueIndex].drawCount >= 2) {
            break;
        }
        sdfWaitVblank(0);
    }
}

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawFrameFlag);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawSyncFlag);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", D_00438A1E);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrFrameComplete);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrDrawFrameId);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrBufferSlotIndex);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfReqDrawDelayCount);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfReqDrawDelaySleepCount);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawReqQueueIndex);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfGrDrawFrameTagHead);

INCLUDE_SDATA(const s32, "sdf/sdfDraw", sdfDrawFrameCount);
