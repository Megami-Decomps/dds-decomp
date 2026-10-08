#include "common.h"
#include "btl_sound.h"
#include "kwln_sprite.h"

#include "pcp_vu0.h"
#include "eff.h"

extern void sdfInitPacketList(SdfListHead *);
extern void sdfAppendPacket(SdfListHead *, u32);
extern void sdfAppendDmaPrimary(s32, u32, SdfDmaNode *);
extern void *sdfConsAllocateColumnPacket(s32);
extern s32 sdfConsMeasurePacketWithHeader(s32);
extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);
extern s32 kwlnGetDrawBufferIndex(void);
extern s32 kwlnTextureGetHeldReference(void);
extern u8 kwlnFrameDrawPacketRecords[];
extern SdfPoolNode D_0037FB88;
extern SdfPoolNode D_00380588;

extern BillDispatch billObjectCallbacks[];

extern BillDispatch D_003AAF84[];

extern void (*D_003AAFB0[])();

extern void (*D_003AAFC0[])();

extern BillDispatch effBillConstructorEntries[];

extern void sdfComposeVuMatrixFromRegisters(void);
extern void *sdfAllocPacketAligned(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(void *, s32, s32, s64, s32);
extern void effBattleReleaseParameterBanks(void *);
extern void sdfReleaseResourceAllocation(void *);
extern void func_00168280();
extern const char D_00414430[];
extern void kwlnCreateHeldTextureBuffer(s32, s32, f32);
extern s32 kwlnTextureSetReferenceFlagIfPresent();
extern void kwlnTextureClearReferenceFlag(void);
extern void kwlnTextureReleaseHeldReference(void);
extern void func_0035B6E0(const char *fmt, ...);
extern f32 D_0037F850[];
extern f32 D_0037F860[];
extern u8 D_00380870[];
extern s32 D_00435CD0;
extern s8 D_00436408;
extern u32 D_0043640C;
extern u32 D_00436410;

typedef struct EffectEntry {
    u8 pad00[0x10];
    u32 value;
} EffectEntry;

typedef struct EffectDispatchState {
    u8 pad00[0x14];
    EffectEntry *entries;
    u8 pad18[4];
    u32 value1C;
    u8 matrix20[0x40];
    u32 value60;
    u8 value64;
    u8 pad65[0xB];
    u8 matrix70[0x40];
    u16 handler;
    u16 valueB2;
} EffectDispatchState;

typedef struct BillWork {
    u8 pad00[0x64];
    u8 currentValue;
    u8 pad65[0x4B];
    u16 pendingCount;
    u16 queuedCount;
    u8 padB4[0xC];
    u8 stagedValue;
} BillWork;





void *effBillCreateDispatch(s32 index, void *arg) {
    EffectDispatchState *effect = effBillConstructorEntries[index].func(arg);

    effect->handler = index;
    effect->valueB2 = 1;
    return effect;
}

void *effCreateDispatchStateForHandler(EffectDispatchState *effect) {
    EffectDispatchState *result = effBillConstructorEntries[effect->handler].func();

    result->handler = effect->handler;
    result->valueB2 = 1;
    return result;
}

void billDispatchIndexedObjectCallback(EffectDispatchState *effect) {
    billObjectCallbacks[effect->handler].func();
}

void func_001671A0(EffectDispatchState *effect) {
    D_003AAF84[effect->handler].func();
}

void effInvokeHandlerAndMarkActive(EffectDispatchState *effect) {
    D_003AAFB0[effect->handler]();
    effect->valueB2 = 1;
}

void effRunPreHandlerAndActivate(EffectDispatchState *effect) {
    D_003AAFC0[effect->handler]();
    effInvokeHandlerAndMarkActive(effect);
}

u16 effBillGetQueuedCount(EffectDispatchState *effect) {
    return effect->valueB2;
}

void billCopyDispatchWorkVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* vu0 routine: effect->matrix20 = matrix * effect->matrix70 via sdfComposeVuMatrixFromRegisters */
void effComposeBillboardTransformMatrix(EffectDispatchState *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect->matrix70);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect->matrix20);
}

void func_001672D8(EffectDispatchState *effect, u32 value) {
    effect->value60 = value;
}

/* A value staged with no pending work is immediately mirrored to the active slot. */
void effBillSetWorkValue(BillWork *work, u8 value) {
    if (work->pendingCount == 0) {
        work->stagedValue = value;
    }
    work->currentValue = value;
}

u8 billGetCurrentValue(EffectDispatchState *effect) {
    return effect->value64;
}

void func_00167300(EffectDispatchState *effect, u32 value) {
    effect->value1C = value;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167308);

void effBillSetEntryValue(EffectDispatchState *effect, s32 index, u32 value) {
    effect->entries[index].value = value;
}

s32 billGetWorkTransformMatrix(EffectDispatchState *effect) {
    return (s32)effect + 0x20;
}

void *effCreateSizedDrawPacket(s32 height, s32 flags) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(9, height));
    sdfConsInitPacketHeader(packet, flags | 0x54, 9, 0x525252521, height);
    return packet;
}

void effWriteGouraudTexturedQuadPacket(u64 *packet, s32 primitive,
                  s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                  s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                  s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                  s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0xE400000000008001ULL;
    packet[1] = 0xF5125125125120ULL;
    packet[2] = (u32)(primitive | 0x1C);
    ((f32 *)packet)[6] = s0;
    ((f32 *)packet)[7] = t0;
    packet[4] = (u32)color0 | ((u64)0xFE00 << 46);
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    ((f32 *)packet)[12] = s1;
    ((f32 *)packet)[13] = t1;
    packet[7] = (u32)color1 | ((u64)0xFE00 << 46);
    packet[8] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    ((f32 *)packet)[18] = s2;
    ((f32 *)packet)[19] = t2;
    packet[10] = (u32)color2 | ((u64)0xFE00 << 46);
    packet[11] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
    ((f32 *)packet)[24] = s3;
    ((f32 *)packet)[25] = t3;
    packet[13] = (u32)color3 | ((u64)0xFE00 << 46);
    packet[14] = (u32)((x3 & 0xFFFF) | (y3 << 16)) | depthHigh;
}

void effAppendGouraudTexturedQuadPacket(s32 chain, s32 primitive,
                 s32 x0, s32 y0, f32 s0, f32 t0, s32 color0,
                 s32 x1, s32 y1, f32 s1, f32 t1, s32 color1,
                 s32 x2, s32 y2, f32 s2, f32 t2, s32 color2,
                 s32 x3, s32 y3, f32 s3, f32 t3, s32 color3, s32 depth) {
    u64 *packet = sdfAllocPacketAligned(0x90);

    packet[0] = 0x20000008;
    packet[1] = 0x5000000810000000ULL;
    effWriteGouraudTexturedQuadPacket(packet + 2, primitive, x0, y0, s0, t0, color0,
                 x1, y1, s1, t1, color1, x2, y2, s2, t2, color2,
                 x3, y3, s3, t3, color3, depth);
    sdfAppendPacket((SdfListHead *)chain, (u32)packet);
}

void billWriteFloatTextureTrianglePacket(u64 *packet, s32 color, s32 primitive,
                  s32 x0, s32 y0, f32 uFirst, f32 vFirst,
                  s32 x1, s32 y1, f32 uSecond, f32 vSecond,
                  s32 x2, s32 y2, f32 uThird, f32 vThird, s32 depth) {
    u64 depthHigh = (u64)depth << 32;

    packet[0] = 0x8400000000008001ULL;
    packet[1] = 0x52525210;
    packet[2] = (u32)(primitive | 0x14);
    packet[3] = (u32)color | ((u64)0xFE00 << 46);
    ((f32 *)packet)[8] = uFirst;
    ((f32 *)packet)[9] = vFirst;
    packet[5] = (u32)((x0 & 0xFFFF) | (y0 << 16)) | depthHigh;
    ((f32 *)packet)[12] = uSecond;
    ((f32 *)packet)[13] = vSecond;
    packet[7] = (u32)((x1 & 0xFFFF) | (y1 << 16)) | depthHigh;
    ((f32 *)packet)[16] = uThird;
    ((f32 *)packet)[17] = vThird;
    packet[9] = (u32)((x2 & 0xFFFF) | (y2 << 16)) | depthHigh;
}

void effAppendTexturedTrianglePacket(s32 chain, s32 color, s32 primitive,
                 s32 x0, s32 y0, f32 u0, f32 v0,
                 s32 x1, s32 y1, f32 u1, f32 v1,
                 s32 x2, s32 y2, f32 u2, f32 v2, s32 depth) {
    u64 *packet = sdfAllocPacketAligned(0x60);

    packet[0] = 0x20000005;
    packet[1] = 0x5000000510000000ULL;
    billWriteFloatTextureTrianglePacket(packet + 2, color, primitive, x0, y0, u0, v0,
                 x1, y1, u1, v1, x2, y2, u2, v2, depth);
    sdfAppendPacket((SdfListHead *)chain, (u32)packet);
}

u64 *effBuildDrawPacketWithFlags(u32 flags) {
    u64 *packet = sdfAllocPacketAligned(0x80);

    packet[0] = 7;
    packet[1] = 0x5000000700000000ULL;
    packet[2] = 0xB400000000008001ULL;
    packet[3] = 0xFF515151510ULL;
    packet[4] = flags | 0x14C;
    packet[15] = 0;
    packet[14] = 0;
    packet[13] = 0;
    return packet;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167A10);

void dds3StartCrossfade(s32 frames) {
    u8 *entry;
    s32 i;
    u64 clearValue;

    if (D_00436408 != 0) {
        func_00168280();
    }
    if (frames != 0) {
        D_0037F850[0] = 0.0f;
        D_0037F850[1] = 0.0f;
        D_0037F850[2] = 0.0f;
        D_0037F860[0] = 0.0f;
        D_0037F860[1] = 0.0f;
        D_0037F860[2] = 0.0f;
        D_00435CD0 = 0x80808080;
        kwlnCreateHeldTextureBuffer(0x100, 0xE0, D_0037F850[0]);
        kwlnTextureSetReferenceFlagIfPresent();
        i = 0;
        entry = D_00380870;
        clearValue = 0x80008000ULL;
        entry += 0x1b70;
        do {
            i++;
            *(u64 *)(entry - 0x10) = clearValue;
            *(u64 *)entry = clearValue;
            entry += 0x1f40;
        } while (i != 2);
        D_00436410 = frames;
        D_00436408 = 1;
        D_0043640C = 0;
        func_0035B6E0("dds3CrossfadeStart\n");
    }
}

/* Capture the first frame, then draw the held texture with a decreasing alpha. */
void func_00167EE8(void) {
    SdfListHead *list;
    SdfDmaNode *reference;
    u64 *texturePacket;
    u64 *blendPacket;
    void *sprite;
    KwlnSpriteVertex *vertex;
    u32 alpha;

    if (D_00436408 == 0) {
        return;
    }
    if (D_0043640C == D_00436410) {
        func_00168280();
        return;
    }
    if (D_0043640C == 0) {
        list = sdfAllocPacketAligned(0x20);
        sdfInitPacketList(list);
        reference = sdfAllocPacketAligned(0x20);
        sdfAppendDmaPrimary((s32)list,
            (u32)(kwlnFrameDrawPacketRecords + kwlnGetDrawBufferIndex() * 0x1F40),
            reference);
        texturePacket = sdfAllocPacketAligned(0x40);
        texturePacket[0] = 3;
        texturePacket[1] = 0x5000000310000000ULL;
        texturePacket[2] = 0x1000000000008002ULL;
        texturePacket[3] = 0xE;
        texturePacket[4] = 0x8000000080ULL;
        texturePacket[5] = 0x3B;
        texturePacket[6] = 0;
        texturePacket[7] = 0x3F;
        sdfAppendPacket(list, (u32)texturePacket);
        blendPacket = sdfAllocPacketAligned(0x40);
        blendPacket[0] = 3;
        blendPacket[1] = 0x5000000310000000ULL;
        blendPacket[2] = 0x1000000000008002ULL;
        blendPacket[3] = 0xE;
        blendPacket[4] = 0x31001;
        blendPacket[5] = 0x47;
        blendPacket[6] = 0x44;
        blendPacket[7] = 0x42;
        sdfAppendPacket(list, (u32)blendPacket);
        sprite = sdfConsAllocateColumnPacket(1);
        vertex = (KwlnSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
        vertex->r = 0x80;
        vertex->g = 0x80;
        vertex->b = 0x80;
        vertex->a = 0x80;
        vertex->corner[0].u = 0;
        vertex->corner[0].v = 0;
        vertex->corner[0].x = 0x77F8;
        vertex->corner[0].y = 0x78F8;
        vertex->corner[0].mask = 0xFF0000;
        vertex->corner[0].flag = 0;
        vertex->corner[1].u = 0x2000;
        vertex->corner[1].v = 0xE00;
        vertex->corner[1].x = 0x87F8;
        vertex->corner[1].y = 0x86F8;
        vertex->corner[1].mask = 0xFF0000;
        vertex->corner[1].flag = 0;
        sdfAppendPacket(list, (u32)sprite);
        D_0037FB88.append((SdfListHead *)&D_0037FB88, list);
    }
    D_0043640C = (u32)D_0043640C + 1;
    alpha = (128 - (u32)((f32)(u32)D_0043640C * 128.0f /
                       (f32)(u32)D_00436410)) & 0xFF;
    list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfConsCreateDrawPacket(list, (SdfTex *)kwlnTextureGetHeldReference(), 0);
    sprite = sdfConsAllocateColumnPacket(1);
    vertex = (KwlnSpriteVertex *)sdfConsMeasurePacketWithHeader((s32)sprite);
    vertex->r = 0x80;
    vertex->g = 0x80;
    vertex->b = 0x80;
    vertex->a = alpha;
    vertex->corner[0].u = 0;
    vertex->corner[0].v = 0;
    vertex->corner[0].x = 0x7000;
    vertex->corner[0].y = 0x7900;
    vertex->corner[0].mask = 0xFF0000;
    vertex->corner[0].flag = 0;
    vertex->corner[1].u = 0x1000;
    vertex->corner[1].v = 0xE00;
    vertex->corner[1].x = 0x9000;
    vertex->corner[1].y = 0x8700;
    vertex->corner[1].mask = 0xFF0000;
    vertex->corner[1].flag = 0;
    sdfAppendPacket(list, (u32)sprite);
    D_00380588.append((SdfListHead *)&D_00380588, list);
}

void func_00168280(void) {
    kwlnTextureClearReferenceFlag();
    kwlnTextureReleaseHeldReference();
    D_00436408 = 0;
    func_0035B6E0(D_00414430);
}

extern u32 sdfAllocGeneralBlock(s32 size);
extern void *sdfResourceRetainAddress(u32 handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void effBattleRebuildClonedParameterBanks(SoundMixer *dst, SoundMixer *src);

/* Clone a mixer: copy its banks, rebuild the voice state from the original and start with no voices. */
SoundMixer *sndMixerClone(SoundMixer *src) {
    u32 handle = sdfAllocGeneralBlock(sizeof(SoundMixer));
    SoundMixer *mixer = sdfResourceRetainAddress(handle);

    mixer->resource = (void *)handle;
    memcpy(mixer, src, 0xC38);
    effBattleRebuildClonedParameterBanks(mixer, src);
    mixer->active = 0;
    mixer->voiceList = NULL;
    return mixer;
}

void sndReleaseAllVoices(SoundMixer *mixer) {
    BattleEffect *voice = mixer->voiceList;
    BattleEffect *next;

    while (voice != NULL) {
        next = voice->next;
        effReleaseBattleVoiceOwner(voice);
        voice = next;
    }
    effBattleReleaseParameterBanks(mixer);
    sdfReleaseResourceAllocation(mixer->resource);
}

s32 func_00168448(SoundMixer *mixer, u16 kind) {
    int bank = kind >= 2;
    return mixer->banks[bank].value50;
}

s32 func_00168478(SoundMixer *mixer, u16 kind) {
    int bank = kind >= 2;
    return mixer->banks[bank].value08;
}

s32 sndReadSelectedMixerBankValue(SoundMixer *mixer, u16 kind) {
    int bank = kind >= 2;
    if (kind == 2) {
        return mixer->banks[bank].value08;
    }
    return mixer->banks[bank].value78;
}

void sndUnlinkVoice(BattleEffect *voice) {
    BattleEffect **link = &voice->state->voiceList;
    BattleEffect *cur;

    while ((cur = *link) != NULL) {
        if (cur == voice) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

INCLUDE_RODATA(const s32, "game/code_001670C0", D_00414430);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168548);

INCLUDE_RODATA(const s32, "game/code_001670C0", D_00414478);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436408);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_0043640C);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436410);

