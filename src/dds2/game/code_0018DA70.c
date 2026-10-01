#include "common.h"
#include "pcp_vu0.h"

typedef struct PairedEffectResources {
    f32 startVec[4];
    u8 pad10[0x54];
    s32 total;       /* 0x64 */
    s32 fadeIn;      /* 0x68 */
    s32 fadeOut;     /* 0x6C */
    u32 resource[2]; /* 0x70 */
    s32 frame;       /* 0x78 */
    u32 colorWithAlpha; /* 0x7C: blend starts with the top byte masked off */
} PairedEffectResources;

/* The four interpolated VU0 vectors written into each effect resource. */
typedef struct {
    f32 start[4];   /* 0x00: sampled actor position */
    f32 oneThird[4];/* 0x10 */
    f32 twoThirds[4];/* 0x20 */
    f32 end[4];     /* 0x30: work->startVec */
} BlurVectorRecord;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void func_0016F6D0(u32 handle);
extern void btlSetActorEffectParameterOrMuzzlePosition(u32 unit, s32 arg1);

void effFreePairedResources(PairedEffectResources *pair) {
    func_0016EFA8(pair->resource[1]);
    func_0016EFA8(pair->resource[0]);
    sdfReleaseChipBlock(pair);
}

void effUpdatePairedResources(PairedEffectResources *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 actor = func_00169438();
    s32 remain;
    f32 t;
    u32 blendColor;
    u32 resourceIndex;
    BlurVectorRecord *record;
    f32 actorPosition[4];
    if (actor != 0) {
        if (frame < total) {
            remain = total - frame;
            if (frame < fadeIn && fadeIn != 0) {
                t = (f32)frame / (f32)fadeIn;
            } else if (remain <= fadeOut && fadeOut != 0) {
                t = (f32)remain / (f32)fadeOut;
            } else {
                t = 1.0f;
            }
            blendColor = effBlendColor(work->colorWithAlpha & 0xFFFFFF, work->colorWithAlpha, t);
            resourceIndex = 0;
            do {
                btlSetActorEffectParameterOrMuzzlePosition(actor, 0xB);
                VU0_STORE_VF(vf10, actorPosition);
                record = func_0016F018(work->resource[resourceIndex]);
                VU0_LOAD_VF(vf10, actorPosition);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF(vf10, record->start);
                VU0_LOAD_VF(vf11, work->startVec);
                VU0_LERP_VF10(0.33f);
                VU0_STORE_VF(vf10, record->oneThird);
                VU0_MOVE_VF(vf10, vf12);
                VU0_LERP_VF10(0.66f);
                VU0_STORE_VF(vf10, record->twoThirds);
                VU0_STORE_VF(vf11, record->end);
                func_0016F020(work->resource[resourceIndex], blendColor);
                func_0016F6D0(work->resource[resourceIndex]);
                resourceIndex++;
            } while (resourceIndex < 2);
        }
        work->frame++;
    }
}

/* Copy the endpoint vector; the remaining paired-resource state is unchanged. */
void func_0018DC68(PairedEffectResources *dst, PairedEffectResources *src) {
    PCP_COPY_VECTOR(dst->startVec, src->startVec);
}

void func_0018DC78(PairedEffectResources *pair, u32 colorWithAlpha) {
    pair->colorWithAlpha = colorWithAlpha;
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DC80);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DFD0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E0D0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E350);

/* Packet builders read RGBA, a blend control word, and a rotated/scaled rectangle. */
typedef struct {
    u8 color[4];      /* 0x00: individual RGBA channels */
    s32 blendControl; /* 0x04: forwarded to GS ALPHA packet setup */
    f32 rotation;     /* 0x08: sine/cosine input */
    f32 scale;        /* 0x0C: applied about the rectangle center */
    s32 centerX;      /* 0x10 */
    s32 centerY;      /* 0x14 */
    s32 left;         /* 0x18 */
    s32 top;          /* 0x1C */
    s32 right;        /* 0x20 */
    s32 bottom;       /* 0x24 */
} BlurSource;

extern void *effCreateSizedDrawPacket();
extern void *func_00167400();
extern void func_0018DFD0();
extern void func_0018DC80();
extern void sdfAppendPacket();

/* Build the two draw packets for `source` and append them to `list`. */
void func_0018E628(void *list, BlurSource *source, u8 fixedPointCoordinates) {
    void *packet;

    packet = effCreateSizedDrawPacket(1, 0x200);
    func_0018DFD0(source, func_00167400(packet), fixedPointCoordinates);
    sdfAppendPacket(list, packet);
    packet = effCreateSizedDrawPacket(1, 0);
    func_0018DC80(source, func_00167400(packet), fixedPointCoordinates);
    sdfAppendPacket(list, packet);
}

typedef struct BlurFilterOps {
    u8 pad00[0x10];
    void (*draw)(struct BlurFilterOps *self, void *list); /* 0x10 */
} BlurFilterOps;

/* Per-frame packet storage; only its leading 0x40-byte DMA packet is copied here. */
typedef struct {
    u8 dmaPacket[0x40];
    u8 pad40[0x1F00];
} BlurFramePacketRecord;

extern BlurFilterOps D_003803E8;
extern BlurFramePacketRecord D_00381ED0[];
extern void *sdfAllocPacketAligned(s32);
extern u32 func_00100400(void);
extern void func_0032DB78(const void *, void *, s32);
extern void sdfAppendDmaTagToList(void *, void *);

/* Queue a 0x40-byte textured packet for the current frame buffer onto `list`, then let the filter ops draw it. */
void func_0018E6D0(void *list) {
    void *packet = sdfAllocPacketAligned(0x40);

    func_0032DB78(D_00381ED0[func_00100400()].dmaPacket, packet, 1);
    sdfAppendDmaTagToList(list, packet);
    D_003803E8.draw(&D_003803E8, list);
}

extern s32 func_001200E0();
extern void sdfInitPacketList();
extern void func_0018E350();

/* Queue blend setup and both rectangle packets, then finish with the filter draw. */
void func_0018E758(BlurSource *source, s32 resource, u8 fixedPointCoordinates) {
    void *list;

    if (func_001200E0(source) == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        func_0018E350(list, source->blendControl, resource);
        func_0018E628(list, source, fixedPointCoordinates);
        func_0018E6D0(list);
    }
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E800);

typedef struct EffBlurTemplateBody {
    s32 extent;        /* 0x00 */
    BlurSource source; /* 0x04: same source view used for draw packet construction */
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;  /* 0x00: copied from the source template */
    u32 resourceWord;          /* 0x2C */
} EffBlurTemplate;

extern void *func_00328D68(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone a blur template into a fresh allocation. */
EffBlurTemplate *func_0018E850(EffBlurTemplate *src) {
    EffBlurTemplate *dst = func_00328D68(sizeof(EffBlurTemplate));

    dst->resourceWord = effGetResourceFirstWord(2);
    dst->body = src->body;
    return dst;
}
