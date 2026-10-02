#include "common.h"
#include "eff.h"
#include "pcp_vu0.h"



/* Packet-source layouts and concrete blur owners mirror their constructors.
 * Equal-sized parameter prefixes do not make the blur variants interchangeable. */
typedef struct BlurSource {
    u8 color[4];
    s32 blendControl;
    f32 rotation;
    f32 scale;
    s32 centerX;
    s32 centerY;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} BlurSource;

typedef struct EffScreenDrawParams {
    BlurSource source;
    u8 pad28[8];
} EffScreenDrawParams;

typedef struct EffSolidRectParams {
    u32 color;
    s32 blendControl;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} EffSolidRectParams;

typedef struct EffBlurTemplateBody {
    s32 extent;
    BlurSource source;
} EffBlurTemplateBody;

typedef struct EffBlurTemplate {
    EffBlurTemplateBody body;
    u32 resourceWord;
} EffBlurTemplate;

typedef struct EffBlurScatterParams {
    s32 count;
    s32 delaySpread;
    f32 angleStep;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    s32 x;
    s32 y;
    s32 positionSpread;
    s32 size;
} EffBlurScatterParams;

typedef struct EffBlurScatterSlot EffBlurScatterSlot;
typedef struct EffBlurScatterWork {
    EffBlurScatterParams params;
    u32 sourceHandle;
    u32 allocation;
    EffBlurScatterSlot *slots;
} EffBlurScatterWork;

typedef struct EffBlurScaleParams {
    s32 count;
    f32 phaseStep;
    f32 spacing;
    u32 color;
    s32 unk10;
    f32 unk14;
    f32 unk18;
    f32 angleStep;
    s32 x;
    s32 y;
    s32 size;
} EffBlurScaleParams;

typedef struct EffBlurScaleSlot EffBlurScaleSlot;
typedef struct EffBlurScaleWork {
    EffBlurScaleParams params;
    u32 sourceHandle;
    u32 allocation;
    EffBlurScaleSlot *slots;
} EffBlurScaleWork;

typedef struct EffTemplateBody {
    u32 words[9];
} EffTemplateBody;

typedef struct EffTemplate {
    EffTemplateBody body;
    u32 resourceWord;
} EffTemplate;

typedef struct EffBezierPoint {
    f32 x;
    f32 y;
    f32 z;
} EffBezierPoint;

/* Each slot has a 0x60-byte stride: seven control points, the current segment, the curve parameter t and its step. */
typedef struct EffBezierSlot {
    EffBezierPoint point[7];
    u32 segment; /* 0x54 */
    f32 t;       /* 0x58 */
    f32 step;    /* 0x5C */
} EffBezierSlot;



extern EffScreenDrawParams effBlurRectangleParameters;
extern EffScreenDrawParams D_00355908;
extern EffSolidRectParams effColorRectangleParameters;
extern EffScreenDrawParams D_003559A0;
extern EffBlurTemplateBody D_00355AF8;
extern EffBlurScatterParams D_00355C70;
extern EffScreenDrawParams D_00355E48;
extern EffSolidRectParams D_00355F88;
extern EffTemplateBody D_00356088;
extern EffBlurScaleParams D_003561C8;
extern void *memcpy(void *, const void *, u32);

extern EffTemplate *effTexturedSquareWork;

extern s8 effTexturedSquareEnabled;

extern s8 effColorRectangleEnabled;

extern s8 D_003BB073;

extern EffBlurScaleWork *effStaggeredBlurWork;

extern s8 effStaggeredBlurEnabled;

extern EffBlurScatterWork *effFilterBlurWork;

extern s8 effFilterBlurEnabled;

extern EffBlurTemplate *effBlurPixelWork;

extern s8 effTexturedBlurEnabled;

extern s8 effRectangleBlurEnabled;
extern u32 effGetResourceFirstWord(s32 arg);
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];
extern u8 kwlnPositionedTextSurface[];
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket();
extern s32 sdfCreateFormattedSifCommand();

extern EffBlurTemplate *effCloneBlurTemplate(void *arg);
extern EffBlurScatterWork *func_00186F90(void *arg);
extern EffTemplate *effCloneResourceTemplate(void *arg);
extern EffBlurScaleWork *effCloneBlurWorkWithSlots(void *arg);
extern void effDrawBlurRectangle(EffScreenDrawParams *arg);
extern void effDrawBlurPixelRectWithResource(EffBlurTemplate *arg);
extern void func_00187098(EffBlurScatterWork *arg);
extern void func_00187598(EffBlurScaleWork *arg);
extern void effBlurDrawFramebufferQuad(EffScreenDrawParams *arg);
extern void func_00187C08(EffSolidRectParams *arg);
extern void effResourceRectDrawPixels(EffTemplate *arg);
extern s32 sdfAllocGeneralBlock(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
EffArrHdr *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    s32 handle = sdfAllocGeneralBlock(slotBytes + 0xC);
    EffBezierSlot *slot = (EffBezierSlot *)sdfResourceRetainAddress(handle);
    EffArrHdr *table = (EffArrHdr *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->allocation = (void *)handle;
    table->slots = slot;
    table->unk4 = count; /* The shared array header's slot count. */
    if (count != 0) {
        do {
            index++;
            slot->segment = 0;
            slot->t = 0;
            slot->step = 0.05f;
            slot++;
        } while (index < count);
    }
    return table;
}

void effReleaseSlotArrayAllocation(EffArrHdr *header) {
    sdfReleaseResourceAllocation((u32)header->allocation);
}

/* Evaluate the active slot's Bezier into out and advance it; a slot whose segment has reached 7 is finished and returns 0. A t that passes 1 clamps to 1 and moves on to the next curve segment. */
s32 effStepActiveBezierSlot(EffArrHdr *table, s32 index, f32 *out) {
    EffBezierSlot *slot = &((EffBezierSlot *)table->slots)[index];
    u32 segment = slot->segment;
    f32 w[4];
    f32 t;
    f32 u;
    EffBezierPoint *p;

    if (segment == 7) {
        return 0;
    }
    t = slot->t;
    u = 1.0f - t;
    p = &slot->point[segment];
    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = p[0].x * w[0] + p[1].x * w[1] + p[2].x * w[2] + p[3].x * w[3];
    out[1] = p[0].y * w[0] + p[1].y * w[1] + p[2].y * w[2] + p[3].y * w[3];
    out[2] = p[0].z * w[0] + p[1].z * w[1] + p[2].z * w[2] + p[3].z * w[3];
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        t = 1.0f;
        segment += 3;
    }
    slot->t = t;
    slot->segment = segment;
    return 1;
}

/* Evaluate the cubic Bezier at t into out (xyz, w = 1), advance t, and step to the next curve segment when t passes 1; returns 0 once the last segment is finished. */
s32 effStepBezierSlotSegment(EffBezierSlot *slot, f32 *out) {
    f32 w[4];
    u32 segment = slot->segment;
    f32 t = slot->t;
    EffBezierPoint *p = &slot->point[segment];
    f32 u = 1.0f - t;

    w[0] = u * u * u;
    w[1] = t * (u * u) * 3.0f;
    w[2] = t * t * u * 3.0f;
    w[3] = t * t * t;
    out[0] = p[0].x * w[0] + p[1].x * w[1] + p[2].x * w[2] + p[3].x * w[3];
    out[1] = p[0].y * w[0] + p[1].y * w[1] + p[2].y * w[2] + p[3].y * w[3];
    out[2] = p[0].z * w[0] + p[1].z * w[1] + p[2].z * w[2] + p[3].z * w[3];
    out[3] = 1.0f;
    t += slot->step;
    if (t > 1.0f) {
        if (segment < 3) {
            t -= 1.0f;
            segment += 3;
        } else {
            slot->t = 1.0f;
            return 0;
        }
    }
    slot->segment = segment;
    slot->t = t;
    return 1;
}

/* Evaluate the cubic Bezier made of control points segment..segment+3 at t into out (xyz, w = 1). */
void effEvaluateSlotBezierPosition(EffBezierSlot *slot, f32 *out) {
    EffBezierPoint *p = &slot->point[slot->segment];
    f32 t = slot->t;
    f32 u = 1.0f - t;
    f32 w[4]; /* never read; gcc drops the stores but keeps the frame slot */
    f32 w0 = u * u * u;
    f32 w1 = t * (u * u) * 3.0f;
    f32 w2 = t * t * u * 3.0f;
    f32 w3 = t * t * t;

    out[0] = p[0].x * w0 + p[1].x * w1 + p[2].x * w2 + p[3].x * w3;
    out[1] = p[0].y * w0 + p[1].y * w1 + p[2].y * w2 + p[3].y * w3;
    out[2] = p[0].z * w0 + p[1].z * w1 + p[2].z * w2 + p[3].z * w3;
    out[3] = 1.0f;
}

void effInitSlotTail(EffArrHdr *table, s32 index) {
    EffBezierSlot *slot = &((EffBezierSlot *)table->slots)[index];

    slot->step = 0.05f;
    slot->segment = slot->t = 0;
}

s32 effGetSlotAt(EffArrHdr *table, s32 index) {
    return (s32)&((EffBezierSlot *)table->slots)[index];
}

extern void *func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);
extern void sdfProjectVuVectorToScreen();

/* Draw a 32x16 box with the fixed marker colour at the screen position of `position`. */
void effDrawMarkerBoxAtPoint(f32 *position) {
    void *list = sdfAllocPacketAligned(0x20);
    f32 screen[4];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    u8 *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - 0x700;
    y = (s32)screen[1] * 2 - 0xF20;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, func_0011D3E8((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, 0x20, 0x10, 0x60008080, 0x60008080));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

/* Draw a 32x16 box with the given colour at the screen position of `position`. */
void effDrawColoredBoxAtPoint(f32 *position, s32 color) {
    void *list = sdfAllocPacketAligned(0x20);
    f32 screen[4];
    s32 pixel[2]; /* written, never read; retail keeps the frame slot */
    s32 x;
    s32 y;
    u8 *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, position);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, screen);
    x = (s32)screen[0] - 0x700;
    y = (s32)screen[1] * 2 - 0xF20;
    pixel[0] = x;
    pixel[1] = y;
    sdfAppendPacket(list, func_0011D3E8((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, 0x20, 0x10, color, color));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

extern void *func_0011D570(s32, s32, s32, s32, s32, s32, s32, s32, s32);

/* Draw a box between the screen positions of two points in the fixed marker colour. */
void effDrawMarkerLineBetweenPoints(f32 *from, f32 *to) {
    void *list = sdfAllocPacketAligned(0x20);
    f32 start[4];
    f32 end[4];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    u8 *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, from);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, start);
    VU0_LOAD_VF(vf10, to);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, end);
    x0 = (s32)start[0] - 0x700;
    y0 = (s32)start[1] * 2 - 0xF20;
    x1 = (s32)end[0] - 0x700;
    y1 = (s32)end[1] * 2 - 0xF20;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfAppendPacket(list, func_0011D570((x0 << 4) + 0x7000, (y0 << 3) + 0x7900, 0xFF0000, 0x60008080, (x1 << 4) + 0x7000, (y1 << 3) + 0x7900, 0xFF0000, 0x60008080, 0));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

/* Draw a box between the screen positions of two points in the given colour. */
void effDrawColoredLineBetweenPoints(f32 *from, f32 *to, s32 color) {
    void *list = sdfAllocPacketAligned(0x20);
    f32 start[4];
    f32 end[4];
    s32 pixel[4]; /* written, never read; retail keeps the frame slot */
    s32 x0;
    s32 y0;
    s32 x1;
    s32 y1;
    u8 *scene;

    sdfInitPacketList(list);
    VU0_LOAD_VF(vf10, from);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, start);
    VU0_LOAD_VF(vf10, to);
    sdfProjectVuVectorToScreen();
    VU0_STORE_VF(vf10, end);
    x0 = (s32)start[0] - 0x700;
    y0 = (s32)start[1] * 2 - 0xF20;
    x1 = (s32)end[0] - 0x700;
    y1 = (s32)end[1] * 2 - 0xF20;
    pixel[0] = x0;
    pixel[1] = y0;
    pixel[2] = x1;
    pixel[3] = y1;
    sdfAppendPacket(list, func_0011D570((x0 << 4) + 0x7000, (y0 << 3) + 0x7900, 0xFF0000, color, (x1 << 4) + 0x7000, (y1 << 3) + 0x7900, 0xFF0000, color, 0));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *task = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(task);
    sdfAppendPacket(task, sdfCreateFormattedSifCommand((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, arg2, arg3));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_0018EB08);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018ED80);

INCLUDE_ASM(const s32, "effect/effEvent", func_0018EED0);


void effSubmitSizedDrawPacket(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5) {
    void *list = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011D3E8(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, w * 0x10, h * 8, arg4, arg5));
    scene = kwlnPositionedTextSurface;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

void effEnableRectangleBlur(void) {
    effRectangleBlurEnabled = 1;
}

void effDisableRectangleBlur(void) {
    effRectangleBlurEnabled = 0;
}

void effCopyRectangleBlurParameters(void *src) {
    memcpy(&effBlurRectangleParameters, src, 0x28);
}

EffScreenDrawParams *effGetCh70Params(void) {
    return &effBlurRectangleParameters;
}

void effEnableTexturedBlur(void) {
    effTexturedBlurEnabled = 1;
}

void effDisableTexturedBlur(void) {
    effTexturedBlurEnabled = 0;
}

/* Copy the pixel rectangle only; retain its selected source resource. */
void effCopyCh71Common(EffBlurTemplateBody *src) {
    effBlurPixelWork->body = *src;
}

EffBlurTemplate *effGetCh71Work(void) {
    return effBlurPixelWork;
}

void effSetCh71Id(u32 id) {
    effBlurPixelWork->resourceWord = id;
}

void effInitCh71Id(void) {
    effBlurPixelWork->resourceWord = effGetResourceFirstWord(2);
}

void effEnableFilterBlur(void) {
    effFilterBlurEnabled = 1;
}

void effDisableFilterBlur(void) {
    effFilterBlurEnabled = 0;
}

/* Update scatter parameters without replacing the owned allocation or slots. */
void effCopyCh72Common(EffBlurScatterParams *src) {
    effFilterBlurWork->params = *src;
}

EffBlurScatterWork *effGetCh72Work(void) {
    return effFilterBlurWork;
}

void effSetCh72Id(u32 id) {
    effFilterBlurWork->sourceHandle = id;
}

void effInitCh72Id(void) {
    effFilterBlurWork->sourceHandle = effGetResourceFirstWord(2);
}

void effEnableStaggeredBlur(void) {
    effStaggeredBlurEnabled = 1;
}

void effDisableStaggeredBlur(void) {
    effStaggeredBlurEnabled = 0;
}

/* Update scale parameters without replacing the owned allocation or slots. */
void effCopyCh76Common(EffBlurScaleParams *src) {
    effStaggeredBlurWork->params = *src;
}

EffBlurScaleWork *effGetCh76Work(void) {
    return effStaggeredBlurWork;
}

void effSetCh76Id(u32 id) {
    effStaggeredBlurWork->sourceHandle = id;
}

void effInitCh76Id(void) {
    effStaggeredBlurWork->sourceHandle = effGetResourceFirstWord(3);
}

void func_0018F650(void) {
    D_003BB073 = 1;
}

void func_0018F660(void) {
    D_003BB073 = 0;
}

void func_0018F668(void *src) {
    memcpy(&D_00355908, src, 0x28);
}

EffScreenDrawParams *effGetCh73Params(void) {
    return &D_00355908;
}

void effEnableColorRectangle(void) {
    effColorRectangleEnabled = 1;
}

void effDisableColorRectangle(void) {
    effColorRectangleEnabled = 0;
}

void effCopyColorRectangleParameters(EffSolidRectParams *src) {
    effColorRectangleParameters = *src;
}

EffSolidRectParams *effGetCh74Params(void) {
    return &effColorRectangleParameters;
}

void effEnableTexturedSquare(void) {
    effTexturedSquareEnabled = 1;
}

void effDisableTexturedSquare(void) {
    effTexturedSquareEnabled = 0;
}

/* Copy the resource template body while preserving its selected resource. */
void effCopyCh75Common(EffTemplateBody *src) {
    effTexturedSquareWork->body = *src;
}

EffTemplate *effGetCh75Work(void) {
    return effTexturedSquareWork;
}

void effSetCh75Id(u32 id) {
    effTexturedSquareWork->resourceWord = id;
}

void effInitCh75Id(void) {
    effTexturedSquareWork->resourceWord = effGetResourceFirstWord(0);
}

void effInitWorks(void) {
    effBlurPixelWork = effCloneBlurTemplate(D_003558D8);
    effFilterBlurWork = func_00186F90(D_003558A8);
    effTexturedSquareWork = effCloneResourceTemplate(D_00355948);
    effStaggeredBlurWork = effCloneBlurWorkWithSlots(D_00355970);
    effGetCh76Work()->params.count = 4;
}

void effDispatchActive(void) {
    if (effRectangleBlurEnabled) {
        effDrawBlurRectangle(&effBlurRectangleParameters);
    }
    if (effTexturedBlurEnabled) {
        effDrawBlurPixelRectWithResource(effBlurPixelWork);
    }
    if (effFilterBlurEnabled) {
        func_00187098(effFilterBlurWork);
    }
    if (effStaggeredBlurEnabled) {
        func_00187598(effStaggeredBlurWork);
    }
    if (D_003BB073) {
        effBlurDrawFramebufferQuad(&D_00355908);
    }
    if (effColorRectangleEnabled) {
        func_00187C08(&effColorRectangleParameters);
    }
    if (effTexturedSquareEnabled) {
        effResourceRectDrawPixels(effTexturedSquareWork);
    }
}

typedef struct ChState {
    u8 pad00[0x1C];
    void *src;
    void *dst;
    u32 size;
    u8 pad28[4];
    s32 (*getter)(void *);
    u8 pad30[8];
    s32 *result;
} ChState;

extern ChState D_00355AB8;
extern s8 D_003BB0BD;
extern s8 D_0039862B[];
extern void func_0018CDD0(void *);
extern void func_0018CDF8(void);
extern void func_0018CDF0(void *);
extern void func_0018CE00(void);

s32 effUpdateCh72Params(void) {
    u8 ready = D_003BB0BD;

    if (D_003BB0BD == 0) {
        ChState *state = &D_00355AB8;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0BD = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355AB8);
        func_0018CDF8();
        func_0018CDF0(&D_00355AB8);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0BD = 0;
    }
    return D_003BB0BD;
}

EffScreenDrawParams *effGetLoadDescA(void) {
    return &D_003559A0;
}

void func_0018F9C0(void *src) {
    memcpy(&D_003559A0, src, 0x28);
}

extern ChState D_00355C30;
extern s8 D_003BB0CD;

s32 func_0018FA20(void) {
    u8 ready = D_003BB0CD;

    if (D_003BB0CD == 0) {
        ChState *state = &D_00355C30;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00355AF8);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0CD = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355C30);
        func_0018CDF8();
        func_0018CDF0(&D_00355C30);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0CD = 0;
    }
    return D_003BB0CD;
}

EffBlurTemplateBody *effGetLoadDescB(void) {
    return &D_00355AF8;
}

void effEventSetBlurTemplateParameters(EffBlurTemplateBody *src) {
    D_00355AF8 = *src;
}

extern ChState D_00355E08;
extern s8 D_003BB0FF;

s32 func_0018FB50(void) {
    u8 ready = D_003BB0FF;

    if (D_003BB0FF == 0) {
        ChState *state = &D_00355E08;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00355C70);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB0FF = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355E08);
        func_0018CDF8();
        func_0018CDF0(&D_00355E08);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB0FF = 0;
    }
    return D_003BB0FF;
}

EffBlurScatterParams *effGetLoadDescC(void) {
    return &D_00355C70;
}

void effEventSetScatterBlurParameters(EffBlurScatterParams *src) {
    D_00355C70 = *src;
}

extern ChState D_00355F48;
extern s8 D_003BB114;

s32 func_0018FC80(void) {
    u8 ready = D_003BB114;

    if (D_003BB114 == 0) {
        ChState *state = &D_00355F48;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB114 = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00355F48);
        func_0018CDF8();
        func_0018CDF0(&D_00355F48);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB114 = 0;
    }
    return D_003BB114;
}

EffScreenDrawParams *effGetLoadDescD(void) {
    return &D_00355E48;
}

void func_0018FD48(void *src) {
    memcpy(&D_00355E48, src, 0x28);
}

extern ChState D_00356048;
extern s8 D_003BB127;

s32 func_0018FDA8(void) {
    u8 ready = D_003BB127;

    if (D_003BB127 == 0) {
        ChState *state = &D_00356048;

        if (state->getter != NULL) {
            *state->result = state->getter(state->src);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB127 = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356048);
        func_0018CDF8();
        func_0018CDF0(&D_00356048);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB127 = 0;
    }
    return D_003BB127;
}

EffSolidRectParams *effGetLoadDescE(void) {
    return &D_00355F88;
}

void func_0018FE70(EffSolidRectParams *src) {
    D_00355F88 = *src;
}

extern ChState D_00356188;
extern s8 D_003BB12C;

s32 func_0018FEB0(void) {
    u8 ready = D_003BB12C;

    if (D_003BB12C == 0) {
        ChState *state = &D_00356188;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_00356088);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB12C = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356188);
        func_0018CDF8();
        func_0018CDF0(&D_00356188);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB12C = 0;
    }
    return D_003BB12C;
}

EffTemplateBody *effGetLoadDescF(void) {
    return &D_00356088;
}

void func_0018FF78(EffTemplateBody *src) {
    D_00356088 = *src;
}

extern ChState D_00356360;
extern s8 D_003BB13F;

s32 effAdvancePendingChannelState(void) {
    u8 ready = D_003BB13F;

    if (D_003BB13F == 0) {
        ChState *state = &D_00356360;

        if (state->getter != NULL) {
            *state->result = state->getter(&D_003561C8);
        }
        if (state->dst != NULL) {
            if (state->src != NULL) {
                memcpy(state->dst, state->src, state->size);
            }
        }
        ready = 1;
        D_003BB13F = ready;
    }
    if (ready != 0) {
        func_0018CDD0(&D_00356360);
        func_0018CDF8();
        func_0018CDF0(&D_00356360);
        func_0018CE00();
    }
    if (D_0039862B[0] < 0) {
        D_003BB13F = 0;
    }
    return D_003BB13F;
}

EffBlurScaleParams *effGetLoadDescG(void) {
    return &D_003561C8;
}

void effEventSetScaleBlurParameters(EffBlurScaleParams *src) {
    D_003561C8 = *src;
}

u32 func_00190100() {
    return sndMixerClone();
}

void func_00190118() {
    sndReleaseAllVoices();
}
INCLUDE_ASM(const s32, "effect/effEvent", func_00190130);


extern u8 D_003563F0[];

extern void func_00190118();
extern void *func_002CFEB8(s32 size);

extern s32 D_003BB140;

extern u32 D_003BB144;

extern u32 D_003BB148;

/* Event work shared by resource setup, teardown and state updates. */
typedef struct {
    u8   pad_0x00[0x04]; /* 0x00 */
    void *owner;         /* 0x04: file-record header destination */
    u8   initBlock[0x28];/* 0x08: file-record header source */
    u32  state;          /* 0x30 */
    void *effect;        /* 0x34 */
    u8   pad_0x38[0x48]; /* 0x38 */
    u8   flag;           /* 0x80 */
    u8   pad_0x81[0x03]; /* 0x81 */
    void *resource;      /* 0x84: released on teardown */
} EffEventWork; /* 0x88 */

typedef struct {
    u8 bytes[0x30];
} __attribute__((packed)) FileRecordHeader;

/* Release the attached effect before freeing the event work. */
void effEventReleaseNode(EffEventWork *work) {
    effReleaseBattleVoiceOwner(work->effect);
    sdfReleaseChipBlock(work);
}

void effEventCopyFileRecordHeader(FileRecordHeader *destination, const FileRecordHeader *source) {
    *destination = *source;
}

void effEventCopyBillParticle(const FileRecordHeader *source, FileRecordHeader *destination) {
    *destination = *source;
}

void func_00190308(EffEventWork *work) {
    func_00161588(work->effect);
}

void effEventSetState(EffEventWork *work, u32 value) {
    work->state = value;
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00190328);

struct EffEventWork;
/* Init block of the event holder (0x30 bytes, copied to the event's owner record). */
typedef struct {
    f32 pos[3];           /* 0x00 */
    f32 unk0C;            /* 0x0C */
    u32 unk10;            /* 0x10 */
    u32 unk14;            /* 0x14 */
    u32 unk18;            /* 0x18 */
    f32 scale;            /* 0x1C */
    f32 rangeNear;        /* 0x20 */
    f32 rangeFar;         /* 0x24 */
    f32 param;            /* 0x28 */
    u32 color;            /* 0x2C */
} __attribute__((packed)) EffEventInit; /* 0x30 */


/* 0x3C-byte event holder: a handle, the event it owns, an init block copied to the event. */
typedef struct EffEventLight {
    u32 handle;           /* 0x00 */
    struct EffEventWork *owner; /* 0x04 */
    EffEventInit init;    /* 0x08 */
    u8 active;            /* 0x38 */
} EffEventLight; /* 0x3C */
extern struct EffEventWork *func_00190130();



EffEventLight *effEventLightCreate(u32 arg, f32 param) {
    EffEventLight *work = func_002CFEB8(sizeof(EffEventLight));

    work->init.scale = 1.0f;
    work->init.rangeNear = 50.0f;
    work->init.rangeFar = 180.0f;
    work->init.pos[1] = -90.0f;
    work->init.param = param;
    work->init.color = 0x80808080;
    work->init.unk10 = 0;
    work->init.unk14 = 0;
    work->init.unk18 = 0;
    work->init.pos[0] = 0;
    work->init.pos[2] = 0;
    work->init.unk0C = 0;
    work->handle = func_00190100(arg);
    work->owner = func_00190130(work->handle, 0, &work->init);
    work->active = 1;
    return work;
}

void effEventLightDestroy(EffEventLight *work) {
    effEventReleaseNode(work->owner);
    if (work->active != 0) {
        func_00190118(work->handle);
    }
    sdfReleaseChipBlock(work);
}

EffEventLight *effEventLightClone(EffEventLight *src) {
    EffEventInit *block = &src->init;
    EffEventLight *work = func_002CFEB8(sizeof(EffEventLight));

    work->owner = func_00190130(src->handle, 0, block);
    work->handle = src->handle;
    work->init = *block;
    work->active = 0;
    return work;
}

void func_00190810(EffEventWork *work) {
    func_00190328(work->owner);
}

void effEventLightSetPosition(EffEventLight *work, f32 *vec) {
    work->init.pos[0] = vec[0];
    work->init.pos[1] = vec[1] - work->init.rangeFar * 0.5f;
    work->init.pos[2] = vec[2];
    work->init.unk0C = 0;
    effEventCopyFileRecordHeader((FileRecordHeader *)work->owner, (FileRecordHeader *)&work->init);
}

/* Attach the effect and copy the initial file-record header to its owner. */
void effEventBindEffect(EffEventWork *work, void *value) {
    work->effect = value;
    effEventCopyFileRecordHeader(work->owner, work->initBlock);
}

typedef struct EffAimSource {
    f32 pos[4];           /* 0x00 */
    f32 rot[4];           /* 0x10 */
    f32 range;            /* 0x20 */
    f32 height;           /* 0x24 */
} EffAimSource;

typedef struct EffAimParams {
    u8 pad0;
    u8 mode;              /* 0x01 */
    u8 sub;               /* 0x02 */
    u8 pad3;
    s32 range;            /* 0x04 */
} EffAimParams;

extern f32 D_003563A0[];
extern f32 D_003563B8[];
extern u8 sdfViewTargetVector[];
extern u8 sdfViewUpVector[];
extern void sdfVuBuildLookAtBasis(void *, void *, void *);
extern void sdfInvertRigidVuTransform(void);
extern void effMiscQuaternionToMatrixVU(void);
extern void func_002DD968(f32 angle);
extern void sdfComposeVuMatrixFromRegisters(void);

/* vu0 routine: vf10 = the aimed offset point computed from the source and parameters */
void effEventLoadSelectedAimPositionVu(EffAimSource *src, EffAimParams *param) {
    f32 out[4];
    f32 dir[4];
    f32 pos[4];
    f32 radius;
    f32 half;
    f32 y;
    s32 range = param->range;
    u8 sub = param->sub;
    u8 mode = param->mode;

    if (range == 0) {
        radius = src->range;
    } else {
        radius = (f32)range;
    }
    half = src->height * 0.5f;
    PCP_COPY_VECTOR(pos, src->pos);
    if (mode == 5) {
        if (sub == 8 || sub == 10) {
            y = -1.0f;
            if (range != 0) {
                y = -radius;
            }
        } else {
            y = -1.0f;
        }
    } else {
        y = pos[1] - D_003563A0[mode] * half;
        if (sub == 8 || sub == 10) {
            if (range != 0) {
                y -= radius;
            }
        }
    }
    if (sub == 9 || mode == 4) {
        dir[2] = half < radius ? -radius : -half;
        dir[0] = dir[1] = 0.0f;
        pos[1] = y;
        sdfVuBuildLookAtBasis(pos, sdfViewTargetVector, sdfViewUpVector);
        sdfInvertRigidVuTransform();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_LOAD_VF(vf11, pos);
        VU0_ADD(vf10, vf10, vf11);
        return;
    }
    if (sub == 8 || sub == 10) {
        out[0] = pos[0];
        out[1] = y;
        out[2] = pos[2];
    } else {
        dir[0] = 0.0f;
        dir[2] = 1.0f;
        dir[1] = 0.0f;
        VU0_LOAD_VF(vf10, src->rot);
        effMiscQuaternionToMatrixVU();
        func_002DD968(D_003563B8[sub]);
        sdfComposeVuMatrixFromRegisters();
        VU0_LOAD_VF(vf10, dir);
        VU0_CLEAR_W(vf10);
        VU0_APPLY_MATRIX(vf10, vf10);
        VU0_STORE_VF(vf10, dir);
        out[0] = pos[0] + radius * dir[0];
        out[1] = y + radius * dir[1];
        out[2] = pos[2] + radius * dir[2];
    }
    VU0_LOAD_VF(vf10, out);
}

/* Billboard emitter parameters (0x7C). Creation/copy clamps startDelaySpread,
   fadeIn and fadeOut to at least one; motionDelaySpread is left unchanged. */
typedef struct {
    f32 position[4];
    u32 count;
    s32 duration;
    s32 startDelaySpread;
    s32 motionDelaySpread;
    f32 spawnRadius;
    u32 color;
    s16 drawMode; /* Passed to the billboard mode setter in the direct-draw path. */
    u8 pad2A[2];
    s32 fadeIn;
    s32 fadeOut;
    u8 repeat;
    u8 pad35[3];
    f32 baseScale;
    f32 scaleRandomness;
    f32 scaleAmplitudeX;
    f32 scaleAmplitudeY;
    f32 scalePhaseStep;
    f32 angularSpeed;
    f32 verticalSpeed;
    f32 verticalSpeedRandomness;
    f32 lateralSpeed;
    f32 lateralSpeedRandomness;
    f32 accelerationPercent; /* Speeds multiply by 1 + this / 100 each motion step. */
    f32 swayPhaseStep;
    f32 swayAmplitude;
    f32 swayAmplitudeStep;
    f32 swayRandomness;
    s32 colorStartAge; /* Batched path: primary texture RGB fades from black. */
    s32 colorTransitionFrames;
} EffEventBillParams;

typedef struct {
    f32 position[3];
    u8 pad0C[4];
    f32 motion[3]; /* X/Z: unit sway direction; Y: accumulated vertical offset. */
    u8 pad1C[4];
    s32 age;
    s32 motionDelay;
    f32 baseScale;
    f32 scalePhaseX;
    f32 scalePhaseStepX;
    f32 scaleAmplitudeX;
    f32 scalePhaseY;
    f32 scalePhaseStepY;
    f32 scaleAmplitudeY;
    f32 rotation;
    f32 angularSpeed;
    f32 swayAmplitude;
    f32 swayPhase;
    f32 verticalSpeed;
    f32 lateralSpeed;
    u8 pad5C[4];
} EffEventBillParticle; /* 0x60 */

typedef struct {
    EffEventBillParams head;
    EffEventBillParticle *particles; /* 0x7C */
    u8 flag;              /* 0x80 */
    u8 pad81[3];
    u32 allocationHandle; /* Owner follows its particles in this allocation. */
} EffEventBillSet; /* 0x88 */

extern void *billCreateFromResource(s32 kind, const char *path);

/* Allocate particles plus their owner, copy parameters and acquire shared textures. */
INCLUDE_RODATA(const s32, "effect/effEvent", D_003A12E0);

EffEventBillSet *effEventBillSetCreate(EffEventBillParams *src) {
    u32 count = src->count;
    u32 size = count * sizeof(EffEventBillParticle);
    u32 handle = sdfAllocGeneralBlock(size + sizeof(EffEventBillSet));
    EffEventBillParticle *particle = (EffEventBillParticle *)sdfResourceRetainAddress(handle);
    EffEventBillSet *work = (EffEventBillSet *)((u8 *)particle + size);
    u32 i;

    work->head = *src;
    work->allocationHandle = handle;
    work->particles = particle;
    work->flag = 0;
    if (work->head.fadeIn <= 0) {
        work->head.fadeIn = 1;
    }
    if (work->head.fadeOut <= 0) {
        work->head.fadeOut = 1;
    }
    if (work->head.startDelaySpread <= 0) {
        work->head.startDelaySpread = 1;
    }
    D_003BB140 = D_003BB140 + 1;
    if (D_003BB140 == 1) {
        D_003BB144 = (u32)billCreateFromResource(0, "/efftool/bill/dbball01.tmx");
        D_003BB148 = (u32)billCreateFromResource(0, "/efftool/bill/dbball02.tmx");
    }
    count = work->head.count;
    for (i = 0; i < count; i++) {
        particle->age = 0;
        particle++;
    }
    return work;
}

void effEventReleaseSharedResources(EffEventWork *work) {
    D_003BB140 = D_003BB140 - 1;
    if (D_003BB140 == 0) {
        billDispatchByKind(D_003BB144);
        billDispatchByKind(D_003BB148);
    }
    sdfReleaseResourceAllocation(work->resource);
}

extern u32 effMiscRand(void *state);
extern f32 effMiscRandUnitFloat(void *state);
extern u8 D_0034DF38[];

/* Randomize delays, scale oscillations, spin and motion. Scale amplitudes share
   the base-scale random factor; motion's Y component begins as zero height. */
void effEventRandomizeBillboardParticle(EffEventBillSet *work, s32 index) {
    EffEventBillParticle *p = &work->particles[index];
    f32 dir[4];
    f32 scale;
    f32 range;
    s32 startDelaySpread = work->head.startDelaySpread;
    s32 motionDelaySpread = work->head.motionDelaySpread;

    p->age = -(effMiscRand(D_0034DF38) % startDelaySpread);
    p->motionDelay = -(effMiscRand(D_0034DF38) % motionDelaySpread);
    scale = effMiscRandUnitFloat(D_0034DF38) * work->head.scaleRandomness + (1.0f - work->head.scaleRandomness);
    p->baseScale = work->head.baseScale * scale;
    p->scalePhaseX = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    p->scalePhaseY = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    p->scalePhaseStepX = work->head.scalePhaseStep * (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f);
    p->scalePhaseStepY = work->head.scalePhaseStep * (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f);
    p->scaleAmplitudeX = work->head.scaleAmplitudeX * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f) * scale;
    p->scaleAmplitudeY = work->head.scaleAmplitudeY * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f) * scale;
    p->rotation = effMiscRandUnitFloat(D_0034DF38) * (3.14159265f * 2.0f);
    if (effMiscRand(D_0034DF38) & 1) {
        p->angularSpeed = work->head.angularSpeed * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f);
    } else {
        p->angularSpeed = -(work->head.angularSpeed * (effMiscRandUnitFloat(D_0034DF38) * 0.3f + 0.7f));
    }
    range = work->head.spawnRadius;
    dir[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    dir[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, dir);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, dir);
    p->position[0] = range * effMiscRandUnitFloat(D_0034DF38) * dir[0];
    p->position[1] = range * effMiscRandUnitFloat(D_0034DF38) * dir[1];
    p->position[2] = range * effMiscRandUnitFloat(D_0034DF38) * dir[2];
    p->motion[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    p->motion[1] = 0;
    p->motion[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
    VU0_LOAD_VF(vf10, p->motion);
    VU0_NORMALIZE_VF10();
    VU0_STORE_VF(vf10, p->motion);
    p->swayAmplitude = work->head.swayAmplitude * (effMiscRandUnitFloat(D_0034DF38) * work->head.swayRandomness + (1.0f - work->head.swayRandomness));
    p->swayPhase = 0;
    p->verticalSpeed = work->head.verticalSpeed * (effMiscRandUnitFloat(D_0034DF38) * work->head.verticalSpeedRandomness + (1.0f - work->head.verticalSpeedRandomness));
    p->lateralSpeed = work->head.lateralSpeed * (effMiscRandUnitFloat(D_0034DF38) * work->head.lateralSpeedRandomness + (1.0f - work->head.lateralSpeedRandomness));
}

INCLUDE_ASM(const s32, "effect/effEvent", func_001910C8);

INCLUDE_ASM(const s32, "effect/effEvent", func_001914E0);

void effEventCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void effEventSetWorkFlag(EffEventWork *work, u8 value) {
    work->flag = value;
}

/* Copy the serialized emitter parameters without changing their values. */
void func_00192030(const EffEventBillParams *source, EffEventBillParams *destination) {
    *destination = *source;
}

/* Copy parameters and normalize only start-delay spread and the two fade divisors. */
void func_00192110(EffEventBillParams *destination, const EffEventBillParams *source) {
    *destination = *source;
    if (destination->fadeIn <= 0) {
        destination->fadeIn = 1;
    }
    if (destination->fadeOut <= 0) {
        destination->fadeOut = 1;
    }
    if (destination->startDelaySpread <= 0) {
        destination->startDelaySpread = 1;
    }
}

void effEventInstallBillParticleSet(void) {
    effEventBillSetCreate(D_003563F0);
}

INCLUDE_ASM(const s32, "effect/effEvent", func_00192250);

extern void *effParamTableGetBlock(void *data, s32 index);
extern u32 effParamTableGetWord2(void *data, s32 index);
extern void func_00192250(void *src, u16 kind, void *params);

void effEventParticleSetCreateFromTable(void *data) {
    void *block0 = effParamTableGetBlock(data, 0);
    void *block1 = effParamTableGetBlock(data, 1);

    func_00192250(block0, effParamTableGetWord2(data, 1), block1);
}

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB068);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB06C);

INCLUDE_SDATA(const s32, "effect/effEvent", effRectangleBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effFilterBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB073);

INCLUDE_SDATA(const s32, "effect/effEvent", effColorRectangleEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effTexturedSquareEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", effStaggeredBlurEnabled);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB078);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB080);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB088);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB090);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB098);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0A8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0B8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0C8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0D8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0E8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F0);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB0F8);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB100);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB108);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB110);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB114);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB118);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB120);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB128);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB12C);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB130);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB138);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB140);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB144);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB148);

INCLUDE_SDATA(const s32, "effect/effEvent", D_003BB14C);

