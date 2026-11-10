#include "common.h"
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

extern u8 sdfGrBufferSlot;

extern u16 sdfReqDrawDelaySleepCount;

extern u32 sdfDrawFrameCount;

extern volatile u8 sdfDrawSyncFlag;


struct sdfPrimRectElem_t;



struct sdfPrimF3Elem_t;

struct sdfPrimF4Elem_t;

struct sdfPrimBoxLineElem_t;

struct sdfPrimTransElem_t;

struct sdfPrimTexflushPacket_t;

struct sdfPreDrawHook_t;

struct sdfPrimGt3Elem_t;

struct sdfPrimGt4Elem_t;

struct sdfPrimRectPacket_t;

struct sdfPrimSpriteElem_t;

struct sdfPrimFt3Elem_t;

struct sdfPrimFt4Elem_t;

struct sdfPrimG3Elem_t;

struct sdfPrimG4Elem_t;

struct sdfPrimBitBltElem_t;

struct sdfPrimF3Packet_t;

struct sdfPrimF4Packet_t;

struct sdfPrimBoxLinePacket_t;

struct sdfPrimTransPacket_t;

struct sdfPrimGt3Packet_t;

struct sdfPrimGt4Packet_t;

struct sdfPrimSpritePacket_t;

struct sdfPrimFt3Packet_t;

struct sdfPrimFt4Packet_t;

struct sdfPrimG3Packet_t;

struct sdfPrimG4Packet_t;

struct sdfPrimBitBltPacket_t;

struct sdfGsMemSlice_t;

struct sdfGrFrameSet_t;

struct sdfDrawTag_t;

struct sdfMemHandle_t;

struct sdfPrimFrameMovePacket_t;

struct sdfPreDrawTag_t;

/* The frame worker passes the node and queued buffer index. The registered
 * handlers project the node into their distinct complete packet owners. */
typedef void (*SdfPacketPatchCallback)(void *, s32);


/* GIF REGLIST stream: PRIM, RGBAQ and two packed XYZ2 vertices. */
typedef struct sdfPrimRectElem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[2];
} sdfPrimRectElem_t;

/* GIF REGLIST stream: PRIM, RGBAQ, then UV/XYZ2 for each vertex. */

/* GIF stream for one flat-color triangle: tag/register list, PRIM, RGBAQ,
 * and the three XYZ2 values. */
typedef struct sdfPrimF3Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[3];
} sdfPrimF3Elem_t;

/* GIF stream for four flat-color vertices: PRIM, RGBAQ and four XYZ2 values. */
typedef struct sdfPrimF4Elem_t {
    u64 gifTag;
    u64 gifRegisterList;
    u64 primitive;
    u64 rgbaq;
    u64 xyz2[4];
} sdfPrimF4Elem_t;

/* GIF stream for a closed rectangle: its first XYZ2 value is repeated last. */
typedef struct sdfPrimBoxLineElem_t {
    u64 giftag;
    u64 gifRegisterList;
    u64 prim;
    u64 rgbaq;
    u64 xyz2[5];
} sdfPrimBoxLineElem_t;

#define SDF_PACKET_LIST_BYTES 0x20

#define SDF_GIF_ONE_REGISTER_WORD 0x10000000

#define SDF_GS_DEFAULT_ALPHA 0x44

#define SDF_GS_ALPHA_TEST 0x717FB

#define SDF_ALPHA_DMA_QWORDS 3

#define SDF_VIF_DIRECT_THREE_WORD 0x50000003

#define SDF_VIF_FLUSHE_HALFWORD 0x1000

#define SDF_GIF_TWO_AD_LOOPS_EOP 0x8002

extern volatile s8 sdfReqDrawDelayCount;

/* TEST and ALPHA form the two A+D writes shared by the blend builders. */
typedef struct sdfPrimTransElem_t {
    sdfGrTagStdReg_t test;
    sdfGrTagStdReg_t alpha;
} sdfPrimTransElem_t;

/* A single A+D write follows the texture-flush DMA/VIF/GIF header. */
typedef struct sdfPrimTexflushPacket_t {
    sdfGrPacketHeader_t header;
    sdfGrTagStdReg_t texflush;
} sdfPrimTexflushPacket_t;

/* Pre-draw hook: the engine's record continues with two option words, which
 * each owner reads under its own name. */
typedef struct sdfPreDrawHook_t {
    struct sdfPreDrawHook_t *next;
    SdfPacketPatchCallback predrawFunc;
    u32 option0;
    u32 option1;
} sdfPreDrawHook_t;

/* PRIM followed by 3 individually textured and colored vertices. */
typedef struct sdfPrimGt3Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 uv0;
    u64 rgbaq0;
    u64 xy0;
    u64 uv1;
    u64 rgbaq1;
    u64 xy1;
    u64 uv2;
    u64 rgbaq2;
    u64 xy2;
} sdfPrimGt3Elem_t;

/* PRIM followed by 4 individually textured and colored vertices. */
typedef struct sdfPrimGt4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 uv0;
    u64 rgbaq0;
    u64 xy0;
    u64 uv1;
    u64 rgbaq1;
    u64 xy1;
    u64 uv2;
    u64 rgbaq2;
    u64 xy2;
    u64 uv3;
    u64 rgbaq3;
    u64 xy3;
} sdfPrimGt4Elem_t;

/* DMA/VIF header followed by the complete two-vertex GIF payload. */
typedef struct sdfPrimRectPacket_t {
    /* The chain tag's two data words carry the VIF codes that upload the elem. */
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimRectElem_t elem;
} sdfPrimRectPacket_t;

typedef struct sdfPrimSpriteElem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
} sdfPrimSpriteElem_t;

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt3Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
    u64 uv2;
    u64 xy2;
} sdfPrimFt3Elem_t;

/* GIF REGLIST stream: PRIM, one RGBAQ, then UV/XYZ2 for each vertex. */
typedef struct sdfPrimFt4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq;
    u64 uv0;
    u64 xy0;
    u64 uv1;
    u64 xy1;
    u64 uv2;
    u64 xy2;
    u64 uv3;
    u64 xy3;
} sdfPrimFt4Elem_t;

/* PRIM followed by three independently colored vertices. */
typedef struct sdfPrimG3Elem_t {
    u64 gifTag;
    u64 gifRegisters;
    u64 primitive;
    u64 rgbaq0;
    u64 xy0;
    u64 rgbaq1;
    u64 xy1;
    u64 rgbaq2;
    u64 xy2;
} sdfPrimG3Elem_t;

/* PRIM followed by four independently colored vertices. */
typedef struct sdfPrimG4Elem_t {
    u64 giftag;
    u64 gifRegisters;
    u64 prim;
    u64 rgbaq0;
    u64 xy0;
    u64 rgbaq1;
    u64 xy1;
    u64 rgbaq2;
    u64 xy2;
    u64 rgbaq3;
    u64 xy3;
} sdfPrimG4Elem_t;

/* The four A+D writes controlling a GS image transfer. */
typedef struct sdfPrimBitBltElem_t {
    sdfGrTagStdReg_t bitbltbuf;
    sdfGrTagStdReg_t trxpos;
    sdfGrTagStdReg_t trxreg;
    sdfGrTagStdReg_t trxdir;
} sdfPrimBitBltElem_t;

/* The caller prepends DMA/VIF words to the triangle GIF stream. Allocation
 * reserves 0x50 bytes; the final qword is not written by these helpers. */
typedef struct sdfPrimF3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF3Elem_t elem;
    u64 unwrittenAllocationWord;
} sdfPrimF3Packet_t;

/* The allocation contains the DMA/VIF prefix and the entire quad GIF stream. */
typedef struct sdfPrimF4Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimF4Elem_t elem;
} sdfPrimF4Packet_t;

/* The caller prepends DMA/VIF words. The final allocated qword is not written. */
typedef struct sdfPrimBoxLinePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimBoxLineElem_t line;
    u64 unwrittenAllocationWord;
} sdfPrimBoxLinePacket_t;

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;

/* Fixed packets allocated by the paired blend and composite-quad renderers. */
typedef struct sdfPrimTransPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimTransElem_t elem;
} sdfPrimTransPacket_t;

typedef struct sdfPrimGt3Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimGt3Elem_t elem;
} sdfPrimGt3Packet_t;

typedef struct sdfPrimGt4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimGt4Elem_t elem;
    u64 unwrittenTail; /* Final allocated word is not written by the builder. */
} sdfPrimGt4Packet_t;

typedef struct sdfPrimSpritePacket_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimSpriteElem_t spr;
} sdfPrimSpritePacket_t;

typedef struct sdfPrimFt3Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimFt3Elem_t elem;
} sdfPrimFt3Packet_t;

typedef struct sdfPrimFt4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimFt4Elem_t elem;
} sdfPrimFt4Packet_t;

typedef struct sdfPrimG3Packet_t {
    u64 dmaTag;
    u64 vifCommands;
    sdfPrimG3Elem_t elem;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG3Packet_t;

typedef struct sdfPrimG4Packet_t {
    u64 dmatag;
    u64 vifCommands;
    sdfPrimG4Elem_t elem;
    u64 unwrittenTail; /* Allocation rounds the REGLIST payload up to a qword. */
} sdfPrimG4Packet_t;

/* DMA/VIF and GIF header followed by the complete image-transfer register block. */
typedef struct sdfPrimBitBltPacket_t {
    sdfGrPacketHeader_t header;
    sdfPrimBitBltElem_t bitblt;
} sdfPrimBitBltPacket_t;


/* Graph target: two color buffers followed by the auxiliary/depth buffer (0x14). */
typedef struct sdfGrFrameSet_t {
    s16 width;
    s16 h;
    s16 height;
    u8 bufferFormat;
    u8 auxiliaryFormat;
    sdfGsMemSlice_t *buffers[3];
} sdfGrFrameSet_t;



/* Metadata node plus the extended local-to-local image-copy payload (0x70). */
typedef struct sdfPrimFrameMovePacket_t {
    sdfPreDrawHook_t worker; /* option0 selects the source resource for the buffer */
    sdfPrimBitBltPacket_t packet;
} sdfPrimFrameMovePacket_t;

/* Complete linked-packet list, distinct from the 0x20-byte DMA list head. */
typedef struct sdfPreDrawTag_t {
    u32 next;
    sdfPreDrawHook_t *head;
    sdfPreDrawHook_t *tail;
    u32 pad;
} sdfPreDrawTag_t;

extern s32 sdfGrPacketBuffSize;

extern s32 sdfGrPacketBuffBase[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

extern s32 sdfDelayedResrcSemaId;

extern s32 sdfDrawReqQueueIndex;

extern sdfGrFrameSet_t sdfGrFrameSet;

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

void sdfDrawTagLink(sdfDrawTag_t *aTag, addr_t aPacket);

void sdfPreDrawTagLink(sdfPreDrawTag_t *aTag, sdfPreDrawHook_t *aHook);

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32));

void sdfPrimLinkSimplePacket(s32 aTag, void (*aFunc)(s32), s32 aPacketSize, s32 (*aAllocator)(s32));

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *aElem);

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *aPkt);

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *aElem);

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *aPkt);

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *aPacket, u32 aDstGsAddr, s32 aDstBuffWidth,
                                    s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                                    u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                                    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode);

void sdfPrimLinkBitBltPacket(s32 aTag, u32 aDstGsAddr, s32 aDstBuffWidth,
                             s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                             u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                             s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                             s32 aDirMode, s32 (*aAllocator)(s32));

void sdfPrimFrameMovePredraw(sdfPrimFrameMovePacket_t *aPre, s32 aEvenOdd);

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag,
                   sdfGsMemSlice_t *aBuffHandle, s32 aDstX, s32 aDstY,
                   s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                   s32 aFrameSw, s32 (*aAllocator)(s32));

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ);

void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ);

void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                   s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ);

void sdfPrimLinkF4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ);

void sdfPrimLinkG3(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetG4Elem(sdfPrimG4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3, s32 aY3, s32 aColor3, s32 aZ);

void sdfPrimLinkG4(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1,
                   s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3,
                   s32 aY3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aZ);

void sdfPrimLinkFt3(s32 aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2,
                   s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ);

void sdfPrimLinkFt4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aColor2, s32 aZ);

void sdfPrimLinkGt3(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aZ,
                   s32 (*aAllocator)(s32));

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2,
                   s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ);

void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3,
                   s32 aU3, s32 aV3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetRectElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetLineElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ);

void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32));

sdfDrawTag_t *sdfPrimAllocDrawTag(s32 (*aAllocator)(s32)) {
    sdfDrawTag_t *list;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    list = (sdfDrawTag_t *)EE_ADDR_PTR(aAllocator(SDF_PACKET_LIST_BYTES));
    sdfDrawTagInit(list);
    return list;
}

void sdfPrimLinkSimplePacket(s32 aTag, void (*aFunc)(s32), s32 aPacketSize, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(aPacketSize);
    aFunc(packetAddress);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), packetAddress);
}

void sdfPrimSetOpaqueElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = SDF_GS_ALPHA_TEST;
    aElem->test.reg = SDF_GS_TEST_1;
    aElem->alpha.data = SDF_GS_DEFAULT_ALPHA;
    aElem->alpha.reg = SDF_GS_ALPHA_1;
}

void sdfPrimSetOpaque2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = SDF_GS_ALPHA_TEST;
    aElem->test.reg = SDF_GS_TEST_2;
    aElem->alpha.data = SDF_GS_DEFAULT_ALPHA;
    aElem->alpha.reg = SDF_GS_ALPHA_2;
}

void sdfPrimSetOpaquePacket(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetOpaqueElem(&aPkt->elem);
    aPkt->header.dmaWords.tag = SDF_ALPHA_DMA_QWORDS;
    aPkt->header.dmaWords.vif = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    aPkt->header.giftag.parm = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    aPkt->header.giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetOpaque2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetOpaque2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = SDF_ALPHA_DMA_QWORDS;
    aPkt->header.dmaWords.vif = (((u64)SDF_VIF_DIRECT_THREE_WORD << 16 | SDF_VIF_FLUSHE_HALFWORD) << 16);
    aPkt->header.giftag.parm = (((u64)SDF_GIF_ONE_REGISTER_WORD << 32) | SDF_GIF_TWO_AD_LOOPS_EOP);
    aPkt->header.giftag.regs = SDF_GIF_REGISTER_AD;
}

void sdfPrimSetSemiTransElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x717FB;
    aElem->test.reg = 0x47;
    aElem->alpha.data = 0x44;
    aElem->alpha.reg = 0x42;
}

void sdfPrimSetSemiTrans2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x717FB;
    aElem->test.reg = 0x48;
    aElem->alpha.data = 0x44;
    aElem->alpha.reg = 0x43;
}

void sdfPrimSetSemiTransPacket(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetSemiTransElem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetSemiTrans2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetSemiTrans2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetAddTransElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x47;
    aElem->alpha.data = 0x48;
    aElem->alpha.reg = 0x42;
}

void sdfPrimSetAddTrans2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x48;
    aElem->alpha.data = 0x48;
    aElem->alpha.reg = 0x43;
}

void sdfPrimSetAddTransPacket(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetAddTransElem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetAddTrans2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetAddTrans2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetSubTransElem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x47;
    aElem->alpha.data = 0x42;
    aElem->alpha.reg = 0x42;
}

void sdfPrimSetSubTrans2Elem(sdfPrimTransElem_t *aElem) {
    aElem->test.data = 0x71801;
    aElem->test.reg = 0x48;
    aElem->alpha.data = 0x42;
    aElem->alpha.reg = 0x43;
}

void sdfPrimSetSubTransPacket(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetSubTransElem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetSubTrans2Packet(sdfPrimTransPacket_t *aPkt) {
    sdfPrimSetSubTrans2Elem(&aPkt->elem);
    aPkt->header.dmaWords.tag = 3;
    aPkt->header.dmaWords.vif = (((u64)0x50000003 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8002);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetTexflushElem(sdfGrTagStdReg_t *aElem) {
    aElem->data = 0;
    aElem->reg = SDF_GS_TEXFLUSH;
}

void sdfPrimSetTexflushPacket(sdfPrimTexflushPacket_t *aPkt) {
    sdfPrimSetTexflushElem(&aPkt->texflush);
    aPkt->header.dmaWords.tag = 2;
    aPkt->header.dmaWords.vif = (((u64)0x50000002 << 16 | 0x1000) << 16);
    aPkt->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8001);
    aPkt->header.giftag.regs = 0xE;
}

void sdfPrimSetBitBltElem(sdfPrimBitBltElem_t *aElem, u32 aDstGsAddr, s32 aDstBuffWidth,
                  s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                  u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                  s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode) {
    aElem->bitbltbuf.data = (aDstPixelFormat << 56) |
        ((u64)(aDstBuffWidth >> 6) << 48) |
        ((u64)(aDstGsAddr >> 6) << 32) |
        (aSrcPixelFormat << 24) | ((aSrcBuffWidth >> 6) << 16) |
        (aSrcGsAddr >> 6);
    aElem->bitbltbuf.reg = SDF_GS_BITBLTBUF;
    aElem->trxpos.data = (aDstY << 48) | (aDstX << 32) |
        (aSrcY << 16) | aSrcX;
    aElem->trxpos.reg = SDF_GS_TRXPOS;
    aElem->trxreg.data = aWidth | ((u64)aHeight << 32);
    aElem->trxreg.reg = SDF_GS_TRXREG;
    aElem->trxdir.data = aDirMode;
    aElem->trxdir.reg = SDF_GS_TRXDIR;
}

void sdfPrimSetBitBltPacket(sdfPrimBitBltPacket_t *aPacket, u32 aDstGsAddr, s32 aDstBuffWidth,
                                    s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                                    u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                                    s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight, s32 aDirMode) {
    aPacket->header.dmaWords.tag = 5;
    aPacket->header.dmaWords.vif = (((u64)0x50000005 << 16 | 0x1000) << 16);
    aPacket->header.giftag.parm = (((u64)0x10000000 << 32) | 0x8004);
    aPacket->header.giftag.regs = 0xE;
    sdfPrimSetBitBltElem(&aPacket->bitblt, aDstGsAddr, aDstBuffWidth,
                  aDstPixelFormat, aDstX, aDstY, aSrcGsAddr, aSrcBuffWidth,
                  aSrcPixelFormat, aSrcX, aSrcY, aWidth, aHeight, aDirMode);
}

void sdfPrimLinkBitBltPacket(s32 aTag, u32 aDstGsAddr, s32 aDstBuffWidth,
                             s64 aDstPixelFormat, s64 aDstX, s64 aDstY,
                             u32 aSrcGsAddr, s32 aSrcBuffWidth, s32 aSrcPixelFormat,
                             s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                             s32 aDirMode, s32 (*aAllocator)(s32)) {
    s32 packetAddress;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packetAddress = aAllocator(0x60);
    sdfPrimSetBitBltPacket(EE_ADDR_AS(sdfPrimBitBltPacket_t *, packetAddress), aDstGsAddr, aDstBuffWidth,
                                    aDstPixelFormat, aDstX, aDstY, aSrcGsAddr, aSrcBuffWidth,
                                    aSrcPixelFormat, aSrcX, aSrcY, aWidth, aHeight, aDirMode);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), packetAddress);
}

void sdfPrimFrameMovePredraw(sdfPrimFrameMovePacket_t *aPre, s32 aEvenOdd) {
    aPre->packet.bitblt.bitbltbuf.data =
        (aPre->packet.bitblt.bitbltbuf.data & ~0x3FFF) |
        (u64)(u32)(sdfGrFrameSet.buffers[aEvenOdd ^ aPre->worker.option0]->addr >> 6);
}

void sdfPrimLinkFrameMovePacket(sdfDrawTag_t *aDrawTag, sdfPreDrawTag_t *aPreDrawTag,
                   sdfGsMemSlice_t *aBuffHandle, s32 aDstX, s32 aDstY,
                   s32 aSrcX, s32 aSrcY, s32 aWidth, s32 aHeight,
                   s32 aFrameSw, s32 (*aAllocator)(s32)) {
    sdfPrimFrameMovePacket_t *packet;
    sdfPrimBitBltPacket_t *drawPacket;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimFrameMovePacket_t *, aAllocator(0x70));
    packet->worker.option0 = aFrameSw;
    packet->worker.predrawFunc = sdfPrimFrameMovePredraw;
    drawPacket = &packet->packet;

    sdfPrimSetBitBltPacket(
        drawPacket, aBuffHandle->addr, aBuffHandle->w,
        aBuffHandle->pixelFormat, aDstX, aDstY, sdfGrFrameSet.buffers[0]->addr,
        sdfGrFrameSet.width, sdfGrFrameSet.bufferFormat, aSrcX, aSrcY, aWidth,
        aHeight, 2);
    sdfPreDrawTagLink(aPreDrawTag, &packet->worker);
    sdfDrawTagLink(aDrawTag, EE_ADDR_S(drawPacket));
}

void sdfPrimSetSpriteElem(sdfPrimSpriteElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisters = 0x535310;
    aElem->primitive = (u32)(aPrim | 0x116);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
}

void sdfPrimLinkSprite(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimSpritePacket_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimSpritePacket_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetSpriteElem(&packet->spr, aColor, aPrim, aX0, aY0, aU0, aV0, aX1, aY1, aU1, aV1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetF3Elem(sdfPrimF3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;
    u32 first = ((u32)aX0 & 0xFFFF) | ((u32)aY0 << 16);
    u32 second = ((u32)aX1 & 0xFFFF) | ((u32)aY1 << 16);
    u32 third = ((u32)aX2 & 0xFFFF) | ((u32)aY2 << 16);

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisterList = 0xF55510;
    aElem->primitive = (u32)aPrim | 0x104;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = first | depthHigh;
    aElem->xyz2[1] = second | depthHigh;
    aElem->xyz2[2] = third | depthHigh;
}

void sdfPrimLinkF3(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1,
                   s32 aY1, s32 aX2, s32 aY2, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimF3Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimF3Packet_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004;
    packet->vifCommands = (((u64)0x50000004 << 16) | 0x1000) << 16;
    sdfPrimSetF3Elem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aX2, aY2, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetF4Elem(sdfPrimF4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x6400000000008001ULL;
    aElem->gifRegisterList = 0x555510;
    aElem->primitive = (u32)(aPrim | 0x104);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->xyz2[0] = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->xyz2[1] = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->xyz2[2] = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->xyz2[3] = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}

void sdfPrimLinkF4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aX2, s32 aY2, s32 aX3, s32 aY3, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimF4Packet_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimF4Packet_t *, aAllocator(0x50));
    packet->dmaTag = 0x20000004ULL;
    packet->vifCommands = 0x5000000410000000ULL;
    sdfPrimSetF4Elem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aX2, aY2, aX3, aY3, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetG3Elem(sdfPrimG3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->gifTag = 0x8400000000008001ULL;
    aElem->gifRegisters = 0xFFFFFFFFF5151510ULL;
    aElem->primitive = (u32)(aPrim | 0x10C);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
}

void sdfPrimLinkG3(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimG3Packet_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimG3Packet_t *, aAllocator(0x60));
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetG3Elem(&packet->elem, aPrim, aX0, aY0, aColor0, aX1, aY1, aColor1, aX2, aY2, aColor2, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetG4Elem(sdfPrimG4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1, s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3, s32 aY3, s32 aColor3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0xA400000000008001ULL;
    aElem->gifRegisters = 0xF515151510ULL;
    aElem->prim = (u32)(aPrim | 0x10C);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->rgbaq3 = (u32)aColor3 | ((u64)0xFE00 << 46);
    aElem->xy3 = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}

void sdfPrimLinkG4(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aColor0, s32 aX1,
                   s32 aY1, s32 aColor1, s32 aX2, s32 aY2, s32 aColor2, s32 aX3,
                   s32 aY3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimG4Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimG4Packet_t *, aAllocator(0x70));
    packet->dmatag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetG4Elem(&packet->elem, aPrim, aX0, aY0, aColor0, aX1, aY1, aColor1, aX2, aY2, aColor2, aX3, aY3, aColor3, aZ);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), EE_ADDR_S(packet));
}

void sdfPrimSetFt3Elem(sdfPrimFt3Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0x8400000000008001ULL;
    aElem->gifRegisters = 0x53535310;
    aElem->prim = (u32)(aPrim | 0x114);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->uv2 = (aU2 & 0xFFFF) | (aV2 << 16);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
}

void sdfPrimLinkFt3(s32 aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimFt3Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimFt3Packet_t *, aAllocator(0x60));
    packet->dmatag = 0x20000005;
    packet->vifCommands = (((u64)0x50000005 << 16) | 0x1000) << 16;
    sdfPrimSetFt3Elem(&packet->elem, aColor, aPrim, aX0, aY0, aU0, aV0, aX1, aY1, aU1, aV1, aX2, aY2, aU2, aV2, aZ);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), EE_ADDR_S(packet));
}

void sdfPrimSetFt4Elem(sdfPrimFt4Elem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2, s32 aU2, s32 aV2,
                   s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0xA400000000008001ULL;
    aElem->gifRegisters = 0x5353535310ULL;
    aElem->prim = (u32)(aPrim | 0x114);
    aElem->rgbaq = (u32)aColor | ((u64)0xFE00 << 46);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->uv2 = (aU2 & 0xFFFF) | (aV2 << 16);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->uv3 = (aU3 & 0xFFFF) | (aV3 << 16);
    aElem->xy3 = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}

void sdfPrimLinkFt4(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aU0,
                   s32 aV0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aZ,
                   s32 (*aAllocator)(s32)) {
    sdfPrimFt4Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimFt4Packet_t *, aAllocator(0x70));
    packet->dmatag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetFt4Elem(&packet->elem, aColor, aPrim, aX0, aY0, aU0, aV0, aX1, aY1, aU1, aV1, aX2, aY2, aU2, aV2, aX3, aY3, aU3, aV3, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetGt3Elem(sdfPrimGt3Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2,
                   s32 aU2, s32 aV2, s32 aColor2, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0xA400000000008001ULL;
    aElem->gifRegisters = 0x5135135130ULL;
    aElem->prim = (u32)(aPrim | 0x11C);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->uv2 = (aU2 & 0xFFFF) | (aV2 << 16);
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
}

void sdfPrimLinkGt3(s32 aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aZ,
                   s32 (*aAllocator)(s32)) {
    sdfPrimGt3Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimGt3Packet_t *, aAllocator(0x70));
    packet->dmatag = 0x20000006;
    packet->vifCommands = (((u64)0x50000006 << 16) | 0x1000) << 16;
    sdfPrimSetGt3Elem(&packet->elem, aPrim, aX0, aY0, aU0, aV0, aColor0, aX1, aY1, aU1, aV1, aColor1, aX2, aY2, aU2, aV2, aColor2, aZ);
    sdfDrawTagLink(EE_ADDR_PTR(aTag), EE_ADDR_S(packet));
}

void sdfPrimSetGt4Elem(sdfPrimGt4Elem_t *aElem, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0, s32 aColor0,
                   s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1, s32 aX2, s32 aY2, s32 aU2,
                   s32 aV2, s32 aColor2, s32 aX3, s32 aY3, s32 aU3, s32 aV3, s32 aColor3, s32 aZ) {
    u64 depthHigh = (u64)aZ << 32;

    aElem->giftag = 0xE400000000008001ULL;
    aElem->gifRegisters = 0xF5135135135130ULL;
    aElem->prim = (u32)(aPrim | 0x11C);
    aElem->uv0 = (aU0 & 0xFFFF) | (aV0 << 16);
    aElem->rgbaq0 = (u32)aColor0 | ((u64)0xFE00 << 46);
    aElem->xy0 = (u32)((aX0 & 0xFFFF) | (aY0 << 16)) | depthHigh;
    aElem->uv1 = (aU1 & 0xFFFF) | (aV1 << 16);
    aElem->rgbaq1 = (u32)aColor1 | ((u64)0xFE00 << 46);
    aElem->xy1 = (u32)((aX1 & 0xFFFF) | (aY1 << 16)) | depthHigh;
    aElem->uv2 = (aU2 & 0xFFFF) | (aV2 << 16);
    aElem->rgbaq2 = (u32)aColor2 | ((u64)0xFE00 << 46);
    aElem->xy2 = (u32)((aX2 & 0xFFFF) | (aY2 << 16)) | depthHigh;
    aElem->uv3 = (aU3 & 0xFFFF) | (aV3 << 16);
    aElem->rgbaq3 = (u32)aColor3 | ((u64)0xFE00 << 46);
    aElem->xy3 = (u32)((aX3 & 0xFFFF) | (aY3 << 16)) | depthHigh;
}

void sdfPrimLinkGt4(sdfDrawTag_t *aTag, s32 aPrim, s32 aX0, s32 aY0, s32 aU0, s32 aV0,
                   s32 aColor0, s32 aX1, s32 aY1, s32 aU1, s32 aV1, s32 aColor1,
                   s32 aX2, s32 aY2, s32 aU2, s32 aV2, s32 aColor2, s32 aX3, s32 aY3,
                   s32 aU3, s32 aV3, s32 aColor3, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimGt4Packet_t *packet;
    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimGt4Packet_t *, aAllocator(0x90));
    packet->dmatag = 0x20000008;
    packet->vifCommands = (((u64)0x50000008 << 16) | 0x1000) << 16;
    sdfPrimSetGt4Elem(&packet->elem, aPrim, aX0, aY0, aU0, aV0, aColor0, aX1, aY1, aU1, aV1, aColor1, aX2, aY2, aU2, aV2, aColor2, aX3, aY3, aU3, aV3, aColor3, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

void sdfPrimSetRectElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ) {
    u64 depthHigh = (u64)(u32)aZ << 32;
    u32 first = ((u32)aX0 & 0xFFFF) | ((u32)aY0 << 16);
    u32 second = ((u32)aX1 & 0xFFFF) | ((u32)aY1 << 16);

    aElem->gifTag = 0x4400000000008001ULL;
    aElem->gifRegisters = 0x5510;
    aElem->primitive = (u32)aPrim | 0x106;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = first | depthHigh;
    aElem->xyz2[1] = second | depthHigh;
}

void sdfPrimLinkRect(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimRectPacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimRectPacket_t *, aAllocator(0x40));
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetRectElem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR(packet));
}

void sdfPrimSetLineElem(sdfPrimRectElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ) {
    u64 depthHigh = (u64)(u32)aZ << 32;
    u32 first = ((u32)aX0 & 0xFFFF) | ((u32)aY0 << 16);
    u32 second = ((u32)aX1 & 0xFFFF) | ((u32)aY1 << 16);

    aElem->gifTag = 0x4400000000008001ULL;
    aElem->gifRegisters = 0x5510;
    aElem->primitive = (u32)aPrim | 0x101;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = first | depthHigh;
    aElem->xyz2[1] = second | depthHigh;
}

void sdfPrimLinkLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimRectPacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimRectPacket_t *, aAllocator(0x40));
    packet->dmaTag = 0x20000003ULL;
    packet->vifCommands = 0x5000000310000000ULL;
    sdfPrimSetLineElem(&packet->elem, aColor, aPrim, aX0, aY0, aX1, aY1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR(packet));
}

void sdfPrimSetBoxLineElem(sdfPrimBoxLineElem_t *aElem, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ) {
    u64 depthHigh = (u64)(u32)aZ << 32;
    u32 x0 = (u32)aX0 & 0xFFFF;
    u32 x1 = (u32)aX1 & 0xFFFF;
    u32 y0 = (u32)aY0 << 16;
    u32 y1 = (u32)aY1 << 16;

    aElem->giftag = 0x8400000000008001ULL;
    aElem->gifRegisterList = (s32)0xF5555510;
    aElem->prim = (u32)aPrim | 0x102;
    aElem->rgbaq = (u32)aColor | 0x3F80000000000000ULL;
    aElem->xyz2[0] = (x0 | y0) | depthHigh;
    aElem->xyz2[1] = (x1 | y0) | depthHigh;
    aElem->xyz2[2] = (x1 | y1) | depthHigh;
    aElem->xyz2[3] = (x0 | y1) | depthHigh;
    aElem->xyz2[4] = (x0 | y0) | depthHigh;
}

void sdfPrimLinkBoxLine(sdfDrawTag_t *aTag, s32 aColor, s32 aPrim, s32 aX0, s32 aY0, s32 aX1, s32 aY1, s32 aZ, s32 (*aAllocator)(s32)) {
    sdfPrimBoxLinePacket_t *packet;

    if (aAllocator == NULL) {
        aAllocator = sdfGrAllocPacketBuff;
    }
    packet = EE_ADDR_AS(sdfPrimBoxLinePacket_t *, aAllocator(0x60));
    packet->dmaTag = 0x20000005ULL;
    packet->vifCommands = 0x5000000510000000ULL;
    sdfPrimSetBoxLineElem(&packet->line, aColor, aPrim, aX0, aY0, aX1, aY1, aZ);
    sdfDrawTagLink(aTag, EE_ADDR_S(packet));
}

