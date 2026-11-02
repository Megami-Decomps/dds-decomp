#ifndef BILLBOARD_H
#define BILLBOARD_H

/* Camera-facing quads: the batch dds3BillboardTagLink fills a quad at a time,
 * the resource it draws from, and the VIF packets the sdfBillBoard* builders
 * hand to VU1. See include/README.md for the evidence per struct.
 *
 * The HD metadata keeps dds3BillResource_tag in effect.h and Sprite3D_t in
 * field.h; they are together here because one allocation holds both halves. */

#include "common.h"
#include "libsdf.h"

typedef struct dds3BillResource_tag dds3BillResource_tag;

/* One GS texture coordinate pair, in 1/16 texel. */
typedef struct
{
    u16 u;
    u16 v;
} UV_t;

/* A quad's four GS texture coordinate pairs, as the billboard code stores
 * them: one flat array, not four UV_t. */
typedef struct billUvQuad_t {
    u16 components[8];
} billUvQuad_t;

/* One corner offset from a quad's centre, in screen units. */
typedef struct
{
    f32 x;
    f32 y;
} VTX_t;

/* One quad with its own Z rotation, which drawSprite3D turns into a transform
 * packet before handing the rest to sdfBillBoardMake1. */
typedef struct
{
    sdfVector4_t pos;
    UV_t uv[4];
    VTX_t vt[4];
    s32 col;
    f32 rot;
} Sprite3D_t;

/* Up to 15 quads as four parallel arrays, which is what lets
 * sdfBillBoardMakeN take four pointers into one block. */
typedef struct
{
    sdfVector4_t centre[15];
    u32 color[15];
    UV_t uv[15][4];
    f32 offset[15][8]; /* Four flat X/Y corner pairs per quad. */
    s32 count;
} sdfBillBoardBatch_t;

/* The tail of BillboardLoadTex's 1100-byte allocation, whose first 1024 bytes
 * are the batch. uv, cx, cy, width and height come from the texture's size. */
struct dds3BillResource_tag
{
    sdfTexHandle_t *hTex;
    u16 type;
    u8 pad06[2];
    u32 count;
    /* Eight halfwords, not four UV_t: the flat array is what the stores in
     * BillboardLoadTex and the single 16-byte copy in dds3BillboardTagLink
     * reproduce. */
    billUvQuad_t uv;
    f32 cx;
    f32 cy;
    f32 width;
    f32 height;
    /* One tag per ordering table; current_mode picks the one a batch links
     * into, and dds3BillboardLinkOt flushes all four. */
    sdfDrawTag_t *pBillTag[4];
    u16 current_mode;
    u8 pad42[2];
    sdfMemHandle_t *hMem;
    sdfBillBoardBatch_t *batch;
    /* The pending list dds3BillboardLinkOt walks from dds3BillboardTag; set to
     * self while the resource is not queued. */
    dds3BillResource_tag *next;
};

/* The DMA tag at the head of each packet below carries two VIF codes in the
 * words a chain tag would use for dat0 and dat1, so the fields are spelled out
 * rather than nesting an sdfDmaTag_t. */

/* sdfBillBoardMakeTMPacket: six floats of the rotation, then MSCAL 8. */
typedef struct QWORD_ALIGNED
{
    u16 dma_qwc;
    u8 dma_pad;
    u8 dma_id;
    u32 dma_addr;
    u32 vif_unpack;
    f32 tm[6];
    u32 vif_itop;
    u32 vif_mscal;
    u32 pad[3];
} sdfBillBoardTMPacket_t;

/* sdfBillBoardMake1: one quad, one flat colour. sdfBillBoardGetPacketSize is
 * 64 * n + 64, so one quad is 128 bytes. */
typedef struct QWORD_ALIGNED
{
    u16 dma_qwc;
    u8 dma_pad;
    u8 dma_id;
    u32 dma_addr;
    u32 vif_unpack1;
    u32 count;
    u32 giftag_h;
    u32 giftag_reg_l;
    u32 giftag_reg_h;
    u32 vif_unpack2;
    u32 prim;
    u32 vif_unpack3;
    f32 center[3];
    u32 vif_unpack4;
    u32 color;
    u32 vif_unpack5;
    u32 uv[4];
    u32 vif_unpack6;
    f32 xyOffset[8];
    u32 vif_itop;
    u32 vif_mscal;
    u32 pad;
} sdfBillBoard1Packet_t;

/* sdfBillBoardVcMake1: one quad, one colour per corner. */
typedef struct QWORD_ALIGNED
{
    u16 dma_qwc;
    u8 dma_pad;
    u8 dma_id;
    u32 dma_addr;
    u32 vif_unpack1;
    u32 count;
    u32 giftag_h;
    u32 giftag_reg_l;
    u32 giftag_reg_h;
    u32 vif_unpack2;
    u32 prim;
    u32 vif_unpack3;
    f32 center[3];
    u32 vif_unpack4;
    u32 color[4];
    u32 vif_unpack5;
    u32 uv[4];
    u32 vif_unpack6;
    f32 xyOffset[8];
    u32 vif_itop;
    u32 vif_mscal;
    u32 pad[2];
} sdfBillBoardVc1Packet_t;

/* sdfMultiBillBoardMake1: one quad drawn from two textures, so two flat
 * colours and two interleaved UV sets. */
typedef struct QWORD_ALIGNED
{
    u16 dma_qwc;
    u8 dma_pad;
    u8 dma_id;
    u32 dma_addr;
    u32 vif_unpack1;
    u32 count;
    u32 giftag_h;
    u32 giftag_reg_l;
    u32 giftag_reg_h;
    u32 vif_unpack2;
    u32 prim;
    u32 vif_unpack3;
    f32 center[3];
    u32 vif_unpack4;
    u32 color[2];
    u32 vif_unpack5;
    u32 uv[8];
    u32 vif_unpack6;
    f32 xyOffset[8];
    u32 vif_itop;
    u32 vif_mscal;
} sdfMultiBillBoard1Packet_t;

/* sdfMultiBillBoardVcMake1: two textures and a colour per corner of each. */
typedef struct QWORD_ALIGNED
{
    u16 dma_qwc;
    u8 dma_pad;
    u8 dma_id;
    u32 dma_addr;
    u32 vif_unpack1;
    u32 count;
    u32 giftag_h;
    u32 giftag_reg_l;
    u32 giftag_reg_h;
    u32 vif_unpack2;
    u32 prim;
    u32 vif_unpack3;
    f32 center[3];
    u32 vif_unpack4;
    u32 color[8];
    u32 vif_unpack5;
    u32 uv[8];
    u32 vif_unpack6;
    f32 xyOffset[8];
    u32 vif_itop;
    u32 vif_mscal;
    u32 pad[2];
} sdfMultiBillBoardVc1Packet_t;

/* dds3BillResource_tag.current_mode, the pBillTag slot a batch links into.
 * Names and values from the IDB; not checked against the binary. */
enum BILL_DRAW
{
    BILL_DRAW_MODE0 = 0,
    BILL_DRAW_MODE1 = 1,
    BILL_DRAW_MAX = 2
};

typedef struct billRenderPair_t {
    dds3BillResource_tag *children[2];
    u16 cameraFacingMode;  
    u16 kind;
    u32 colors[2];
    sdfDrawTag_t *packetList;  
    struct billRenderPair_t *next;  
} billRenderPair_t;

typedef struct billHandle_t {
    f32 pos[4];  
    f32 childScaleX;   
    f32 childScaleY;   
    f32 unk18;
    f32 unk1C;
    f32 rot;   
    u32 abgr;    
    void (*callback)();  
    u16 type;          
    u16 mode;  
    union {
        void *payload;  
        dds3BillResource_tag *child;  
        struct billData_t *animationData;  
    } u;
    billRenderPair_t pair;  
    u16 animationActive;  
    u8 pad52[2];
    u32 modeFlags;  
    u32 animationEntryIndex;  
    s32 entryCount;
    struct billOut_t *resolvedEntries;  
} billHandle_t;
#endif /* BILLBOARD_H */
