#include "common.h"

#include "eff.h"

/* Parameter blocks have distinct 0x18, 0x24, 0x2C and 0x30 byte layouts. */
typedef struct Work18 {
    u8 data[0x18];
} Work18;

typedef struct Work24 {
    u8 data[0x20];
    u32 unk20;
} Work24;

typedef struct Work2C {
    u8 data[0x28];
    u32 unk28;
} Work2C;

typedef struct Work30 {
    u8 data[0x30];
} Work30;

/* Each slot has a 0x60-byte stride; only its resettable tail is known. */
typedef struct Slot60 {
    u8 pad[0x54];
    s32 unk54;
    s32 unk58;
    f32 unk5C;
} Slot60;

typedef struct SlotTab {
    Slot60 *slots;
    u32 count;
    s32 handle;
} SlotTab;

/* Per-channel work copies this 0x2C-byte prefix before its separate ID word.
 * The prefix's final word is copied with lw/sw rather than as part of the blob. */
typedef struct BDCommon2C {
    u8 data[0x28];
    u32 unk28;
} BDCommon2C;

typedef struct BDWork2C {
    BDCommon2C common;
    u32 id;
} BDWork2C;

typedef struct BDCommon24 {
    u8 data[0x20];
    u32 unk20;
} BDCommon24;

typedef struct BDWork24 {
    BDCommon24 common;
    u32 id;
} BDWork24;

extern Work30 D_00355880;
extern Work30 D_00355908;
extern Work18 D_00355930;
extern Work30 D_003559A0;
extern Work2C D_00355AF8;
extern Work2C D_00355C70;
extern Work30 D_00355E48;
extern Work18 D_00355F88;
extern Work24 D_00356088;
extern Work2C D_003561C8;
extern void *memcpy(void *, const void *, u32);

extern BDWork24 *D_003BD80C;

extern s8 D_003BB075;

extern s8 D_003BB074;

extern s8 D_003BB073;

extern BDWork2C *D_003BD810;

extern s8 D_003BB076;

extern BDWork2C *D_003BD804;

extern s8 D_003BB072;

extern BDWork2C *D_003BD808;

extern s8 D_003BB071;

extern s8 D_003BB070;
extern u32 effGetResourceFirstWord(s32 arg);
extern u8 D_003558D8[];
extern u8 D_003558A8[];
extern u8 D_00355948[];
extern u8 D_00355970[];
extern u8 D_00325748[];
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void sdfAppendPacket();
extern s32 sdfCreateFormattedSifCommand();

extern BDWork2C *effCloneBlurTemplate(void *arg);
extern BDWork2C *func_00186F90(void *arg);
extern BDWork24 *effCloneResourceTemplate(void *arg);
extern BDWork2C *effCloneBlurWorkWithSlots(void *arg);
extern void effDrawBlurRectangle(Work30 *arg);
extern void effDrawBlurPixelRectWithResource(BDWork2C *arg);
extern void func_00187098(BDWork2C *arg);
extern void func_00187598(BDWork2C *arg);
extern void func_00187988(Work30 *arg);
extern void func_00187C08(Work18 *arg);
extern void func_00188068(BDWork24 *arg);
extern s32 func_002D03F8(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
SlotTab *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    s32 handle = func_002D03F8(slotBytes + 0xC);
    Slot60 *slot = (Slot60 *)sdfResourceRetainAddress(handle);
    SlotTab *table = (SlotTab *)((u8 *)slot + slotBytes);
    u32 index = 0;
    table->handle = handle;
    table->slots = slot;
    table->count = count;
    if (count != 0) {
        do {
            index++;
            slot->unk54 = 0;
            slot->unk58 = 0;
            slot->unk5C = 0.05f;
            slot++;
        } while (index < count);
    }
    return table;
}

void effReleaseSlotArrayAllocation(EffArrHdr *header) {
    func_002D0918((u32)header->unk8);
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E2C0);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E408);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E548);

void effInitSlotTail(SlotTab *table, s32 index) {
    Slot60 *slot = &table->slots[index];

    slot->unk5C = 0.05f;
    slot->unk54 = slot->unk58 = 0;
}

s32 effGetSlotAt(SlotTab *table, s32 index) {
    return (s32)&table->slots[index];
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E678);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E740);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E810);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018E938);

void effSubmitPositionedDrawPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *task = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(task);
    sdfAppendPacket(task, sdfCreateFormattedSifCommand((x << 4) + 0x7000, (y << 3) + 0x7900, 0xFF0000, arg2, arg3));
    scene = D_00325748;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, task);
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018EB08);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018ED80);

INCLUDE_ASM(const s32, "game/code_0018E218", func_0018EED0);

extern void *func_0011D3E8(s32, s32, s32, s32, s32, s32, s32);

void effSubmitSizedDrawPacket(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5) {
    void *list = sdfAllocPacketAligned(0x20);
    u8 *scene;

    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011D3E8(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, w * 0x10, h * 8, arg4, arg5));
    scene = D_00325748;
    (*(void (**)(void *, void *))(scene + 0x10))(scene, list);
}

void func_0018F3A0(void) {
    D_003BB070 = 1;
}

void func_0018F3B0(void) {
    D_003BB070 = 0;
}

void func_0018F3B8(void *src) {
    memcpy(&D_00355880, src, 0x28);
}

Work30 *effGetCh70Params(void) {
    return &D_00355880;
}

void func_0018F428(void) {
    D_003BB071 = 1;
}

void func_0018F438(void) {
    D_003BB071 = 0;
}

void effCopyCh71Common(BDCommon2C *src) {
    D_003BD808->common = *src;
}

BDWork2C *effGetCh71Work(void) {
    return D_003BD808;
}

void effSetCh71Id(u32 id) {
    D_003BD808->id = id;
}

void effInitCh71Id(void) {
    D_003BD808->id = effGetResourceFirstWord(2);
}

void func_0018F4E0(void) {
    D_003BB072 = 1;
}

void func_0018F4F0(void) {
    D_003BB072 = 0;
}

void effCopyCh72Common(BDCommon2C *src) {
    D_003BD804->common = *src;
}

BDWork2C *effGetCh72Work(void) {
    return D_003BD804;
}

void effSetCh72Id(u32 id) {
    D_003BD804->id = id;
}

void effInitCh72Id(void) {
    D_003BD804->id = effGetResourceFirstWord(2);
}

void func_0018F598(void) {
    D_003BB076 = 1;
}

void func_0018F5A8(void) {
    D_003BB076 = 0;
}

void effCopyCh76Common(BDCommon2C *src) {
    D_003BD810->common = *src;
}

BDWork2C *effGetCh76Work(void) {
    return D_003BD810;
}

void effSetCh76Id(u32 id) {
    D_003BD810->id = id;
}

void effInitCh76Id(void) {
    D_003BD810->id = effGetResourceFirstWord(3);
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

Work30 *effGetCh73Params(void) {
    return &D_00355908;
}

void func_0018F6D8(void) {
    D_003BB074 = 1;
}

void func_0018F6E8(void) {
    D_003BB074 = 0;
}

void func_0018F6F0(Work18 *src) {
    D_00355930 = *src;
}

Work18 *effGetCh74Params(void) {
    return &D_00355930;
}

void func_0018F740(void) {
    D_003BB075 = 1;
}

void func_0018F750(void) {
    D_003BB075 = 0;
}

void effCopyCh75Common(BDCommon24 *src) {
    D_003BD80C->common = *src;
}

BDWork24 *effGetCh75Work(void) {
    return D_003BD80C;
}

void effSetCh75Id(u32 id) {
    D_003BD80C->id = id;
}

void effInitCh75Id(void) {
    D_003BD80C->id = effGetResourceFirstWord(0);
}

void effInitWorks(void) {
    D_003BD808 = effCloneBlurTemplate(D_003558D8);
    D_003BD804 = func_00186F90(D_003558A8);
    D_003BD80C = effCloneResourceTemplate(D_00355948);
    D_003BD810 = effCloneBlurWorkWithSlots(D_00355970);
    *(s32 *)effGetCh76Work() = 4;
}

void effDispatchActive(void) {
    if (D_003BB070) {
        effDrawBlurRectangle(&D_00355880);
    }
    if (D_003BB071) {
        effDrawBlurPixelRectWithResource(D_003BD808);
    }
    if (D_003BB072) {
        func_00187098(D_003BD804);
    }
    if (D_003BB076) {
        func_00187598(D_003BD810);
    }
    if (D_003BB073) {
        func_00187988(&D_00355908);
    }
    if (D_003BB074) {
        func_00187C08(&D_00355930);
    }
    if (D_003BB075) {
        func_00188068(D_003BD80C);
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

Work30 *effGetLoadDescA(void) {
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

Work2C *effGetLoadDescB(void) {
    return &D_00355AF8;
}

void func_0018FAE8(Work2C *src) {
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

Work2C *effGetLoadDescC(void) {
    return &D_00355C70;
}

void func_0018FC18(Work2C *src) {
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

Work30 *effGetLoadDescD(void) {
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

Work18 *effGetLoadDescE(void) {
    return &D_00355F88;
}

void func_0018FE70(Work18 *src) {
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

Work24 *effGetLoadDescF(void) {
    return &D_00356088;
}

void func_0018FF78(Work24 *src) {
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

Work2C *effGetLoadDescG(void) {
    return &D_003561C8;
}

void func_00190098(Work2C *src) {
    D_003561C8 = *src;
}

void func_00190100(void) {
    func_001606C0();
}

void func_00190118(void) {
    sndReleaseAllVoices();
}

INCLUDE_ASM(const s32, "game/code_0018E218", func_00190130);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB068);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB06C);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB070);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB071);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB072);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB073);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB074);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB075);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB076);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB078);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB080);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB088);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB090);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB098);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0A0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0A8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0B0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0B8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0C0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0C8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0D0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0D8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0E0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0E8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0F0);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB0F8);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB100);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB108);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB110);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB114);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB118);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB120);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB128);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB12C);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB130);

INCLUDE_SDATA(const s32, "game/code_0018E218", D_003BB138);

