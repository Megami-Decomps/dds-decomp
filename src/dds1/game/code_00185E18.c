#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    f32 startVec[4];
    u8 pad10[0x54];
    s32 total;       /* 0x64 */
    s32 fadeIn;      /* 0x68 */
    s32 fadeOut;     /* 0x6C */
    u32 resource[2]; /* 0x70 */
    s32 frame;       /* 0x78 */
    u32 value7C;
} EffectPair;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);
extern void func_00167A78(u32 handle);
extern void btlSetActorEffectParameterOrMuzzlePosition(u32 unit, s32 arg1);

void effFreePairedResources(EffectPair *pair) {
    func_00167350(pair->resource[1]);
    func_00167350(pair->resource[0]);
    sdfReleaseChipBlock(pair);
}

void effUpdatePairedResources(EffectPair *work) {
    s32 total = work->total;
    s32 frame = work->frame;
    s32 fadeIn = work->fadeIn;
    s32 fadeOut = work->fadeOut;
    u32 obj = func_00161858();
    s32 remain;
    f32 t;
    u32 color;
    u32 i;
    u8 *record;
    f32 vec[4];
    if (obj != 0) {
        if (frame < total) {
            remain = total - frame;
            if (frame < fadeIn && fadeIn != 0) {
                t = (f32)frame / (f32)fadeIn;
            } else if (remain <= fadeOut && fadeOut != 0) {
                t = (f32)remain / (f32)fadeOut;
            } else {
                t = 1.0f;
            }
            color = effBlendColor(work->value7C & 0xFFFFFF, work->value7C, t);
            i = 0;
            do {
                if (*(u16 *)(obj + 0x124) == 0x109) {
                    btlSetActorEffectParameterOrMuzzlePosition(obj, i + 0x14);
                } else {
                    btlSetActorEffectParameterOrMuzzlePosition(obj, 0xB);
                }
                VU0_STORE_VF(vf10, vec);
                record = (u8 *)func_001673C0(work->resource[i]);
                VU0_LOAD_VF(vf10, vec);
                VU0_MOVE_VF(vf12, vf10);
                VU0_STORE_VF(vf10, record);
                VU0_LOAD_VF(vf11, work);
                VU0_LERP_VF10(0.33f);
                VU0_STORE_VF(vf10, record + 0x10);
                VU0_MOVE_VF(vf10, vf12);
                VU0_LERP_VF10(0.66f);
                VU0_STORE_VF(vf10, record + 0x20);
                VU0_STORE_VF(vf11, record + 0x30);
                func_001673C8(work->resource[i], color);
                func_00167A78(work->resource[i]);
                i++;
            } while (i < 2);
        }
        work->frame++;
    }
}

void func_00186030(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00186040(EffectPair *pair, u32 value) {
    pair->value7C = value;
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186048);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186398);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186498);

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186718);

extern void *effCreateSizedDrawPacket();
extern void *func_0015F810();
extern void func_00186398();
extern void func_00186048();
extern void sdfAppendPacket();

/* Build the two draw packets for `source` and append them to `list`. */
void func_001869F0(void *list, void *source, u8 flag) {
    void *packet;

    packet = effCreateSizedDrawPacket(1, 0x200);
    func_00186398(source, func_0015F810(packet), flag);
    sdfAppendPacket(list, packet);
    packet = effCreateSizedDrawPacket(1, 0);
    func_00186048(source, func_0015F810(packet), flag);
    sdfAppendPacket(list, packet);
}

typedef struct BlurFilterOps {
    u8 pad00[0x10];
    void (*draw)(struct BlurFilterOps *self, void *list); /* 0x10 */
} BlurFilterOps;

extern BlurFilterOps D_003253E8;
extern u8 D_00326ED0[];
extern void *sdfAllocPacketAligned(s32);
extern u32 func_00100518(void);
extern void func_002D4CC8(const void *, void *, s32);
extern void sdfAppendDmaTagToList(void *, void *);

/* Queue a 0x40-byte textured packet for the current frame buffer onto `list`, then let the filter ops draw it. */
void func_00186A98(void *list) {
    void *packet = sdfAllocPacketAligned(0x40);

    func_002D4CC8(D_00326ED0 + func_00100518() * 0x1F40, packet, 1);
    sdfAppendDmaTagToList(list, packet);
    D_003253E8.draw(&D_003253E8, list);
}

extern s32 func_0011E278();
extern void sdfInitPacketList();
extern void func_00186718();

void func_00186B20(void *source, s32 arg1, u8 flag) {
    void *list;

    if (func_0011E278(source) == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        func_00186718(list, *(s32 *)((u8 *)source + 4), arg1);
        func_001869F0(list, source, flag);
        func_00186A98(list);
    }
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186BC8);

typedef struct EffBlurTemplateBody {
    u32 words[11];
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;  /* 0x00: copied from the source template */
    u32 resourceWord;          /* 0x2C */
} EffBlurTemplate;

extern void *func_002CFEB8(s32 size);
extern u32 effGetResourceFirstWord(s32 index);

/* Clone a blur template into a fresh allocation. */
EffBlurTemplate *func_00186C18(EffBlurTemplate *src) {
    EffBlurTemplate *dst = func_002CFEB8(sizeof(EffBlurTemplate));

    dst->resourceWord = effGetResourceFirstWord(2);
    dst->body = src->body;
    return dst;
}

void func_00186CB8(void) {
    sdfReleaseChipBlock();
}

INCLUDE_ASM(const s32, "game/code_00185E18", func_00186CD0);

typedef struct BlurRect {
    s32 extent;     /* 0x00 half-size source */
    u8 pad04[0x10];
    s32 centerX;    /* 0x14 */
    s32 centerY;    /* 0x18 */
    s32 left;       /* 0x1C */
    s32 top;        /* 0x20 */
    s32 right;      /* 0x24 */
    s32 bottom;     /* 0x28 */
    u32 resource;   /* 0x2C */
} BlurRect;

void func_00186D48(BlurRect *rect) {
    s32 x, y, w;

    if (func_0011E278(rect) == 0) {
        x = rect->centerX + 0x1000;
        y = (rect->centerY + 0xE00) >> 1;
        w = rect->extent;
        rect->left = x - w;
        rect->right = x + w;
        w >>= 1;
        rect->top = y - w;
        rect->bottom = y + w;
        func_00186B20((u8 *)rect + 4, rect->resource, 1);
    }
}
