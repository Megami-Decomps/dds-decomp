#include "common.h"

#include "pcp_vu0.h"
#include "eff.h"

extern BillDispatch D_003AAF88[];

extern BillDispatch D_003AAF84[];

extern void (*D_003AAFB0[])();

extern void (*D_003AAFC0[])();

extern BillDispatch D_003AAF80[];

extern void sdfComposeVuMatrixFromRegisters(void);
extern void *sdfAllocPacketAligned(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void *sdfConsInitPacketHeader(void *, s32, s32, s64, s32);
extern void func_001686F0(void *);
extern void func_00169370(void *);
extern void func_003297C8(void *);
extern void func_00168280();
extern void func_001053F0(s32, s32, f32);
extern s32 kwlnTextureSetReferenceFlagIfPresent();
extern void func_0035B6E0(const char *);
extern f32 D_0037F850[];
extern f32 D_0037F860[];
extern u8 D_00380870[];
extern s32 D_00435CD0;
extern s8 D_00436408;
extern s32 D_0043640C;
extern s32 D_00436410;

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

typedef struct SoundBank {
    u8 pad00[8];
    s32 value08;
    u8 pad0C[0x44];
    s32 value50;
    u8 pad54[0x24];
    s32 value78;
    u8 pad7C[0x59C];
} SoundBank;

typedef struct SoundVoice SoundVoice;

typedef struct SoundMixer {
    SoundBank banks[2];
    u8 pad0C30[8];
    void *resource;
    u32 unk0C3C;
    SoundVoice *voiceList;
} SoundMixer;

struct SoundVoice {
    u8 pad00[4];
    SoundMixer *mixer;
    u8 pad08[0x11C];
    SoundVoice *next;
};

void *effBillCreateDispatch(s32 index, void *arg) {
    EffectDispatchState *effect = D_003AAF80[index].func(arg);

    effect->handler = index;
    effect->valueB2 = 1;
    return effect;
}

void *effCreateDispatchStateForHandler(EffectDispatchState *effect) {
    EffectDispatchState *result = D_003AAF80[effect->handler].func();

    result->handler = effect->handler;
    result->valueB2 = 1;
    return result;
}

void billDispatchIndexedObjectCallback(EffectDispatchState *effect) {
    D_003AAF88[effect->handler].func();
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

void func_00167268(void *dst, void *src) {
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

s32 func_00167400(EffectDispatchState *effect) {
    return (s32)effect + 0x20;
}

void *effCreateSizedDrawPacket(s32 height, s32 flags) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(9, height));
    sdfConsInitPacketHeader(packet, flags | 0x54, 9, 0x525252521, height);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167480);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001675B8);

void func_00167778(u64 *packet, s32 color, s32 primitive, s32 x0, s32 y0,
                   s32 x1, s32 y1, s32 x2, s32 y2, s32 depth,
                   f32 uFirst, f32 vFirst, f32 uSecond, f32 vSecond,
                   f32 uThird, f32 vThird) {
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

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167838);

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

void func_00167E00(s32 frames) {
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
        func_001053F0(0x100, 0xE0, D_0037F850[0]);
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

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167EE8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168280);

extern u32 func_003292A8(s32 size);
extern void *sdfResourceRetainAddress(u32 handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void func_00169230(SoundMixer *dst, SoundMixer *src);

/* Clone a mixer: copy its banks, rebuild the voice state from the original and start with no voices. */
SoundMixer *func_001682B0(SoundMixer *src) {
    u32 handle = func_003292A8(sizeof(SoundMixer));
    SoundMixer *mixer = sdfResourceRetainAddress(handle);

    mixer->resource = (void *)handle;
    memcpy(mixer, src, 0xC38);
    func_00169230(mixer, src);
    mixer->unk0C3C = 0;
    mixer->voiceList = NULL;
    return mixer;
}

void sndReleaseAllVoices(SoundMixer *mixer) {
    SoundVoice *voice = mixer->voiceList;
    SoundVoice *next;

    while (voice != NULL) {
        next = voice->next;
        func_001686F0(voice);
        voice = next;
    }
    func_00169370(mixer);
    func_003297C8(mixer->resource);
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

void sndUnlinkVoice(SoundVoice *voice) {
    SoundVoice **link = &voice->mixer->voiceList;
    SoundVoice *cur;

    while ((cur = *link) != NULL) {
        if (cur == voice) {
            *link = cur->next;
            return;
        }
        link = &cur->next;
    }
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168548);

INCLUDE_RODATA(const s32, "game/code_001670C0", D_00414478);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436408);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_0043640C);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436410);

