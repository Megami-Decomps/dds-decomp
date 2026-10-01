#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

extern BillDispatch D_0034E658[];

extern BillDispatch D_0034E654[];
extern BillDispatch D_0034E650[];
extern void sdfComposeVuMatrixFromRegisters(void);
extern void *sdfConsInitPacketHeader(void *, s32, s32, s64, s32);
extern void *sdfAllocPacketAligned(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void (*D_0034E680[])();
extern void (*D_0034E690[])();
extern void func_00160B00(void *);
extern void func_00161790(void *);
extern void func_002D0918(void *);
extern void func_00160690();
extern void func_001054D0(s32, s32, f32);
extern s32 kwlnTextureSetReferenceFlagIfPresent();
extern void func_003003F0(const char *);
extern f32 D_00324850[];
extern f32 D_00324860[];
extern u8 D_00325870[];
extern s32 D_003BA900;
extern s8 D_003BB018;
extern s32 D_003BB01C;
extern s32 D_003BB020;

typedef struct EffectDispatchState {
    u8 pad00[0xB0];
    u16 handler;
    u16 valueB2;
} EffectDispatchState;

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

typedef struct BillWork {
    u8 pad00[0x64];
    u8 currentValue;
    u8 pad65[0x4B];
    u16 pendingCount;
    u16 queuedCount;
    u8 padB4[0xC];
    u8 stagedValue;
} BillWork;

typedef struct BillEntry {
    u8 pad00[0x10];
    u32 value;
} BillEntry;

typedef struct BillEntryOwner {
    u8 pad00[0x14];
    BillEntry *entries;
    u8 pad18[4];
    u32 value1C; /* matches DDS2 EffectDispatchState at +0x1C */
} BillEntryOwner;

void *effBillCreateDispatch(s32 index, void *arg) {
    EffectDispatchState *effect = D_0034E650[index].func(arg);

    effect->handler = index;
    effect->valueB2 = 1;
    return effect;
}

void *effCreateDispatchStateForHandler(EffectDispatchState *effect) {
    EffectDispatchState *result = D_0034E650[effect->handler].func();

    result->handler = effect->handler;
    result->valueB2 = 1;
    return result;
}

void billDispatchIndexedObjectCallback(EffectDispatchState *effect) {
    D_0034E658[effect->handler].func();
}

void func_0015F5B0(EffectDispatchState *effect) {
    D_0034E654[effect->handler].func();
}

void effInvokeHandlerAndMarkActive(EffectDispatchState *effect) {
    D_0034E680[effect->handler]();
    effect->valueB2 = 1;
}

void effRunPreHandlerAndActivate(EffectDispatchState *effect) {
    D_0034E690[effect->handler]();
    effInvokeHandlerAndMarkActive(effect);
}

u16 effBillGetQueuedCount(BillWork *work) {
    return work->queuedCount;
}

void func_0015F678(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* vu0 routine: effect->matrix20 = matrix * effect->matrix70 via sdfComposeVuMatrixFromRegisters */
void effComposeBillboardTransformMatrix(u8 *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect + 0x70);
    sdfComposeVuMatrixFromRegisters();
    VU0_STORE_MATRIX(effect + 0x20);
}

void func_0015F6E8(BillObj *effect, void *value) {
    effect->unk60 = value;
}

/* A value staged with no pending work is immediately mirrored to the active slot. */
void effBillSetWorkValue(BillWork *work, u8 value) {
    if (work->pendingCount == 0) {
        work->stagedValue = value;
    }
    work->currentValue = value;
}

u8 billGetCurrentValue(BillWork *work) {
    return work->currentValue;
}

void func_0015F710(BillEntryOwner *owner, u32 value) {
    owner->value1C = value;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F718);

void effBillSetEntryValue(BillEntryOwner *owner, s32 index, u32 value) {
    owner->entries[index].value = value;
}

s32 func_0015F810(s32 arg0) {
    return arg0 + 0x20;
}

void *effCreateSizedDrawPacket(s32 height, s32 flags) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(9, height));

    sdfConsInitPacketHeader(packet, flags | 0x54, 9, 0x525252521, height);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F890);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F9C8);

void func_0015FB88(u64 *packet, s32 color, s32 primitive, s32 x0, s32 y0,
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

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FC48);

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

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FE20);

void func_00160210(s32 frames) {
    u8 *entry;
    s32 i;
    u64 clearValue;

    if (D_003BB018 != 0) {
        func_00160690();
    }
    if (frames != 0) {
        D_00324850[0] = 0.0f;
        D_00324850[1] = 0.0f;
        D_00324850[2] = 0.0f;
        D_00324860[0] = 0.0f;
        D_00324860[1] = 0.0f;
        D_00324860[2] = 0.0f;
        D_003BA900 = 0x80808080;
        func_001054D0(0x100, 0xE0, D_00324850[0]);
        kwlnTextureSetReferenceFlagIfPresent();
        i = 0;
        entry = D_00325870;
        clearValue = 0x80008000ULL;
        entry += 0x1b70;
        do {
            i++;
            *(u64 *)(entry - 0x10) = clearValue;
            *(u64 *)entry = clearValue;
            entry += 0x1f40;
        } while (i != 2);
        D_003BB020 = frames;
        D_003BB018 = 1;
        D_003BB01C = 0;
        func_003003F0("dds3CrossfadeStart\n");
    }
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001602F8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160690);

extern u32 func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(u32 handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern void func_00161650(SoundMixer *dst, SoundMixer *src);

/* Clone a mixer: copy its banks, rebuild the voice state from the original and start with no voices. */
SoundMixer *func_001606C0(SoundMixer *src) {
    u32 handle = func_002D03F8(sizeof(SoundMixer));
    SoundMixer *mixer = sdfResourceRetainAddress(handle);

    mixer->resource = (void *)handle;
    memcpy(mixer, src, 0xC38);
    func_00161650(mixer, src);
    mixer->unk0C3C = 0;
    mixer->voiceList = NULL;
    return mixer;
}

void sndReleaseAllVoices(SoundMixer *mixer) {
    SoundVoice *voice = mixer->voiceList;
    SoundVoice *next;

    while (voice != NULL) {
        next = voice->next;
        func_00160B00(voice);
        voice = next;
    }
    func_00161790(mixer);
    func_002D0918(mixer->resource);
}

s32 func_00160858(SoundMixer *mixer, u16 kind) {
    int bank = kind >= 2;
    return mixer->banks[bank].value50;
}

s32 func_00160888(SoundMixer *mixer, u16 kind) {
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

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160958);

INCLUDE_RODATA(const s32, "game/code_0015F4D0", D_003A0DB8);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB018);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB01C);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB020);

