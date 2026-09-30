#include "common.h"
#include "pcp_vu0.h"
#include "eff.h"

extern BillDispatch D_0034E658[];

extern BillDispatch D_0034E654[];
extern BillDispatch D_0034E650[];
extern void func_002DDBF8(void);
extern void *func_002E1428(void *, s32, s32, s64, s32);
extern void *sdfAllocPacketAligned(s32);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void (*D_0034E680[])();
extern void (*D_0034E690[])();
extern void func_00160B00(void *);
extern void func_00161790(void *);
extern void func_002D0918(void *);

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
    u8 pad0C3C[4];
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

void *func_0015F4D0(s32 index, void *arg) {
    EffectDispatchState *effect = D_0034E650[index].func(arg);

    effect->handler = index;
    effect->valueB2 = 1;
    return effect;
}

void *func_0015F520(EffectDispatchState *effect) {
    EffectDispatchState *result = D_0034E650[effect->handler].func();

    result->handler = effect->handler;
    result->valueB2 = 1;
    return result;
}

void func_0015F578(BillObj *obj) {
    D_0034E658[*(u16 *)((u8 *)obj + 0xB0)].func();
}

void func_0015F5B0(BillObj *obj) {
    D_0034E654[*(u16 *)((u8 *)obj + 0xB0)].func();
}

void func_0015F5E8(EffectDispatchState *effect) {
    D_0034E680[effect->handler]();
    effect->valueB2 = 1;
}

void func_0015F630(EffectDispatchState *effect) {
    D_0034E690[effect->handler]();
    func_0015F5E8(effect);
}

u16 effBillGetQueuedCount(BillWork *work) {
    return work->queuedCount;
}

void func_0015F678(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

/* vu0 routine: effect->matrix20 = matrix * effect->matrix70 via func_002DDBF8 */
void func_0015F688(u8 *effect, void *matrix) {
    VU0_LOAD_MATRIX(matrix);
    VU0_LOAD_MATRIX_B(effect + 0x70);
    func_002DDBF8();
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

u8 func_0015F708(BillWork *work) {
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

void *func_0015F818(s32 height, s32 flags) {
    void *packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(9, height));

    func_002E1428(packet, flags | 0x54, 9, 0x525252521, height);
    return packet;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F890);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F9C8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FB88);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FC48);

u64 *func_0015FD98(u32 flags) {
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

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160210);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001602F8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160690);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001606C0);

void func_00160800(SoundMixer *mixer) {
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

s32 func_001608B8(SoundMixer *mixer, u16 kind) {
    int bank = kind >= 2;
    if (kind == 2) {
        return mixer->banks[bank].value08;
    }
    return mixer->banks[bank].value78;
}

void func_00160910(SoundVoice *voice) {
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

