#include "common.h"
#include "eff.h"

extern void *memcpy(void *, const void *, u32);

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

typedef struct Work30 {
    u8 data[0x30];
} Work30;

extern Work30 D_003B21B0;

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

extern BDWork2C *D_00438F10;

extern u32 func_00159BB8(s32 arg);

extern BDWork2C *D_00438F0C;

extern BDWork2C *D_00438F18;

extern Work30 D_003B2238;

/* Parameter blocks have distinct 0x18, 0x24, 0x2C and 0x30 byte layouts. */
typedef struct Work18 {
    u8 data[0x18];
} Work18;

extern Work18 D_003B2260;

typedef struct BDCommon24 {
    u8 data[0x20];
    u32 unk20;
} BDCommon24;

typedef struct BDWork24 {
    BDCommon24 common;
    u32 id;
} BDWork24;

extern BDWork24 *D_00438F14;

extern u8 D_003B2208[];

extern u8 D_003B21D8[];

extern u8 D_003B2278[];

extern u8 D_003B22A0[];

extern BDWork2C *func_0018E850(void *arg);

extern BDWork2C *func_0018EBC8(void *arg);

extern BDWork24 *func_0018FBF8(void *arg);

extern BDWork2C *func_0018F098(void *arg);

extern Work30 D_003B22D0;

typedef struct Work2C {
    u8 data[0x28];
    u32 unk28;
} Work2C;

extern Work2C D_003B2428;

extern Work2C D_003B25A0;

extern Work30 D_003B2778;

extern Work18 D_003B28B8;

typedef struct Work24 {
    u8 data[0x20];
    u32 unk20;
} Work24;

extern Work24 D_003B29B8;

extern Work2C D_003B2AF8;

extern s8 D_00436465;

extern s8 D_00436464;

extern s8 D_00436463;

extern s8 D_00436466;

extern s8 D_00436462;

extern s8 D_00436461;

extern s8 D_00436460;

extern void func_0018E0D0(Work30 *arg);

extern void func_0018E908(BDWork2C *arg);

extern void func_0018ECD0(BDWork2C *arg);

extern void func_0018F1D0(BDWork2C *arg);

extern void func_0018F5C0(Work30 *arg);

extern void func_0018F840(Work18 *arg);

extern void func_0018FCA0(BDWork24 *arg);

extern s32 func_003292A8(s32);
extern u8 *sdfResourceRetainAddress(s32);

/* Allocate contiguous slots followed by their count and allocation handle. */
SlotTab *effCreateSlotArray(u32 count) {
    s32 slotBytes = count * 0x60;
    s32 handle = func_003292A8(slotBytes + 0xC);
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
    func_003297C8(header->unk8);
}

INCLUDE_ASM(const s32, "game/code_00195E50", func_00195EF8);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196040);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196180);

void effInitSlotTail(SlotTab *table, s32 index) {
    Slot60 *slot = &table->slots[index];

    slot->unk5C = 0.05f;
    slot->unk54 = slot->unk58 = 0;
}

s32 effGetSlotAt(SlotTab *table, s32 index) {
    return (s32)&table->slots[index];
}

INCLUDE_ASM(const s32, "game/code_00195E50", func_001962B0);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196378);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196448);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196570);

typedef struct GsSurface {
    u8 pad00[0x10];
    void (*submit)(struct GsSurface *, void *);
} GsSurface;

extern GsSurface D_00380748;
extern void *sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(void *);
extern void *func_0033D810(s32, s32, s32, s32, s32);
extern void sdfAppendPacket(void *, void *);

void func_001966A0(s32 x, s32 y, s32 arg2, s32 arg3) {
    void *list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0033D810(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, arg2, arg3));
    D_00380748.submit(&D_00380748, list);
}

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196740);

INCLUDE_ASM(const s32, "game/code_00195E50", func_001969B8);

INCLUDE_ASM(const s32, "game/code_00195E50", func_00196B08);

extern void *func_0011F250(s32, s32, s32, s32, s32, s32, s32);

void func_00196F18(s32 x, s32 y, s32 w, s32 h, s32 arg4, s32 arg5) {
    void *list = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(list);
    sdfAppendPacket(list, func_0011F250(x * 0x10 + 0x7000, y * 8 + 0x7900, 0xFF0000, w * 0x10, h * 8, arg4, arg5));
    D_00380748.submit(&D_00380748, list);
}

void func_00196FD8(void) {
    D_00436460 = 1;
}

void func_00196FE8(void) {
    D_00436460 = 0;
}

void func_00196FF0(void *src) {
    memcpy(&D_003B21B0, src, 0x28);
}

Work30 *effGetCh70Params(void) {
    return &D_003B21B0;
}

void func_00197060(void) {
    D_00436461 = 1;
}

void func_00197070(void) {
    D_00436461 = 0;
}

void effCopyCh71Common(BDCommon2C *src) {
    D_00438F10->common = *src;
}

u32 effGetCh71Work(void) {
    return D_00438F10;
}

void effSetCh71Id(u32 id) {
    D_00438F10->id = id;
}

void effInitCh71Id(void) {
    D_00438F10->id = func_00159BB8(2);
}

void func_00197118(void) {
    D_00436462 = 1;
}

void func_00197128(void) {
    D_00436462 = 0;
}

void effCopyCh72Common(BDCommon2C *src) {
    D_00438F0C->common = *src;
}

u32 effGetCh72Work(void) {
    return D_00438F0C;
}

void effSetCh72Id(u32 id) {
    D_00438F0C->id = id;
}

void effInitCh72Id(void) {
    D_00438F0C->id = func_00159BB8(2);
}

void func_001971D0(void) {
    D_00436466 = 1;
}

void func_001971E0(void) {
    D_00436466 = 0;
}

void effCopyCh76Common(BDCommon2C *src) {
    D_00438F18->common = *src;
}

u32 effGetCh76Work(void) {
    return D_00438F18;
}

void effSetCh76Id(u32 id) {
    D_00438F18->id = id;
}

void effInitCh76Id(void) {
    D_00438F18->id = func_00159BB8(3);
}

void func_00197288(void) {
    D_00436463 = 1;
}

void func_00197298(void) {
    D_00436463 = 0;
}

void func_001972A0(void *src) {
    memcpy(&D_003B2238, src, 0x28);
}

Work30 *effGetCh73Params(void) {
    return &D_003B2238;
}

void func_00197310(void) {
    D_00436464 = 1;
}

void func_00197320(void) {
    D_00436464 = 0;
}

void func_00197328(Work18 *src) {
    D_003B2260 = *src;
}

Work18 *effGetCh74Params(void) {
    return &D_003B2260;
}

void func_00197378(void) {
    D_00436465 = 1;
}

void func_00197388(void) {
    D_00436465 = 0;
}

void effCopyCh75Common(BDCommon24 *src) {
    D_00438F14->common = *src;
}

u32 effGetCh75Work(void) {
    return D_00438F14;
}

void effSetCh75Id(u32 id) {
    D_00438F14->id = id;
}

void effInitCh75Id(void) {
    D_00438F14->id = func_00159BB8(0);
}

void effInitWorks(void) {
    D_00438F10 = func_0018E850(D_003B2208);
    D_00438F0C = func_0018EBC8(D_003B21D8);
    D_00438F14 = func_0018FBF8(D_003B2278);
    D_00438F18 = func_0018F098(D_003B22A0);
    *(s32 *)effGetCh76Work() = 4;
}

void effDispatchActive(void) {
    if (D_00436460) {
        func_0018E0D0(&D_003B21B0);
    }
    if (D_00436461) {
        func_0018E908(D_00438F10);
    }
    if (D_00436462) {
        func_0018ECD0(D_00438F0C);
    }
    if (D_00436466) {
        func_0018F1D0(D_00438F18);
    }
    if (D_00436463) {
        func_0018F5C0(&D_003B2238);
    }
    if (D_00436464) {
        func_0018F840(&D_003B2260);
    }
    if (D_00436465) {
        func_0018FCA0(D_00438F14);
    }
}

typedef struct EffLoader {
    u8 pad00[0x1C];
    void *source;
    void *dest;
    u32 size;
    u8 pad28[4];
    s32 (*load)(void *);
    u8 pad30[8];
    s32 *result;
} EffLoader;

extern EffLoader D_003B23E8;
extern s8 D_004364AD;
extern s8 D_0040B7DB[];
extern void func_00194A08();
extern void func_00194A28();
extern void func_00194A30();
extern void func_00194A38();

s8 effUpdateCh72Params(void) {
    if (D_004364AD == 0) {
        if (D_003B23E8.load != 0) {
            *D_003B23E8.result = D_003B23E8.load(D_003B23E8.source);
        }
        if (D_003B23E8.dest != 0 && D_003B23E8.source != 0) {
            memcpy(D_003B23E8.dest, D_003B23E8.source, D_003B23E8.size);
        }
        D_004364AD = 1;
    }
    if ((u8)D_004364AD != 0) {
        func_00194A08(&D_003B23E8);
        func_00194A30();
        func_00194A28(&D_003B23E8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364AD = 0;
    }
    return D_004364AD;
}

Work30 *effGetLoadDescA(void) {
    return &D_003B22D0;
}

void func_001975F8(void *src) {
    memcpy(&D_003B22D0, src, 0x28);
}

extern EffLoader D_003B2560;
extern s8 D_004364BD;

s8 func_00197658(void) {
    if (D_004364BD == 0) {
        if (D_003B2560.load != 0) {
            *D_003B2560.result = D_003B2560.load(&D_003B2428);
        }
        if (D_003B2560.dest != 0 && D_003B2560.source != 0) {
            memcpy(D_003B2560.dest, D_003B2560.source, D_003B2560.size);
        }
        D_004364BD = 1;
    }
    if ((u8)D_004364BD != 0) {
        func_00194A08(&D_003B2560);
        func_00194A30();
        func_00194A28(&D_003B2560);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364BD = 0;
    }
    return D_004364BD;
}

Work2C *effGetLoadDescB(void) {
    return &D_003B2428;
}

void func_00197720(Work2C *src) {
    D_003B2428 = *src;
}

extern EffLoader D_003B2738;
extern s8 D_004364EF;

s8 func_00197788(void) {
    if (D_004364EF == 0) {
        if (D_003B2738.load != 0) {
            *D_003B2738.result = D_003B2738.load(&D_003B25A0);
        }
        if (D_003B2738.dest != 0 && D_003B2738.source != 0) {
            memcpy(D_003B2738.dest, D_003B2738.source, D_003B2738.size);
        }
        D_004364EF = 1;
    }
    if ((u8)D_004364EF != 0) {
        func_00194A08(&D_003B2738);
        func_00194A30();
        func_00194A28(&D_003B2738);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_004364EF = 0;
    }
    return D_004364EF;
}

Work2C *effGetLoadDescC(void) {
    return &D_003B25A0;
}

void func_00197850(Work2C *src) {
    D_003B25A0 = *src;
}

extern EffLoader D_003B2878;
extern s8 D_00436504;

s8 func_001978B8(void) {
    if (D_00436504 == 0) {
        if (D_003B2878.load != 0) {
            *D_003B2878.result = D_003B2878.load(D_003B2878.source);
        }
        if (D_003B2878.dest != 0 && D_003B2878.source != 0) {
            memcpy(D_003B2878.dest, D_003B2878.source, D_003B2878.size);
        }
        D_00436504 = 1;
    }
    if ((u8)D_00436504 != 0) {
        func_00194A08(&D_003B2878);
        func_00194A30();
        func_00194A28(&D_003B2878);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436504 = 0;
    }
    return D_00436504;
}

Work30 *effGetLoadDescD(void) {
    return &D_003B2778;
}

void func_00197980(void *src) {
    memcpy(&D_003B2778, src, 0x28);
}

extern EffLoader D_003B2978;
extern s8 D_00436517;

s8 func_001979E0(void) {
    if (D_00436517 == 0) {
        if (D_003B2978.load != 0) {
            *D_003B2978.result = D_003B2978.load(D_003B2978.source);
        }
        if (D_003B2978.dest != 0 && D_003B2978.source != 0) {
            memcpy(D_003B2978.dest, D_003B2978.source, D_003B2978.size);
        }
        D_00436517 = 1;
    }
    if ((u8)D_00436517 != 0) {
        func_00194A08(&D_003B2978);
        func_00194A30();
        func_00194A28(&D_003B2978);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_00436517 = 0;
    }
    return D_00436517;
}

Work18 *effGetLoadDescE(void) {
    return &D_003B28B8;
}

void func_00197AA8(Work18 *src) {
    D_003B28B8 = *src;
}

extern EffLoader D_003B2AB8;
extern s8 D_0043651C;

s8 func_00197AE8(void) {
    if (D_0043651C == 0) {
        if (D_003B2AB8.load != 0) {
            *D_003B2AB8.result = D_003B2AB8.load(&D_003B29B8);
        }
        if (D_003B2AB8.dest != 0 && D_003B2AB8.source != 0) {
            memcpy(D_003B2AB8.dest, D_003B2AB8.source, D_003B2AB8.size);
        }
        D_0043651C = 1;
    }
    if ((u8)D_0043651C != 0) {
        func_00194A08(&D_003B2AB8);
        func_00194A30();
        func_00194A28(&D_003B2AB8);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043651C = 0;
    }
    return D_0043651C;
}

Work24 *effGetLoadDescF(void) {
    return &D_003B29B8;
}

void func_00197BB0(Work24 *src) {
    D_003B29B8 = *src;
}

extern EffLoader D_003B2C90;
extern s8 D_0043652F;

s8 func_00197C08(void) {
    if (D_0043652F == 0) {
        if (D_003B2C90.load != 0) {
            *D_003B2C90.result = D_003B2C90.load(&D_003B2AF8);
        }
        if (D_003B2C90.dest != 0 && D_003B2C90.source != 0) {
            memcpy(D_003B2C90.dest, D_003B2C90.source, D_003B2C90.size);
        }
        D_0043652F = 1;
    }
    if ((u8)D_0043652F != 0) {
        func_00194A08(&D_003B2C90);
        func_00194A30();
        func_00194A28(&D_003B2C90);
        func_00194A38();
    }
    if (D_0040B7DB[0] < 0) {
        D_0043652F = 0;
    }
    return D_0043652F;
}

Work2C *effGetLoadDescG(void) {
    return &D_003B2AF8;
}

void func_00197CD0(Work2C *src) {
    D_003B2AF8 = *src;
}

void func_00197D38(void) {
    func_001682B0();
}

void func_00197D50(void) {
    sndReleaseAllVoices();
}

INCLUDE_ASM(const s32, "game/code_00195E50", func_00197D68);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436458);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_0043645C);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436460);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436461);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436462);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436463);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436464);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436465);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436466);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436468);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436470);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436478);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436480);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436488);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436490);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436498);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364A0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364A8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364B0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364B8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364C0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364C8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364D0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364D8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364E0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364E8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364F0);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_004364F8);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436500);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436504);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436508);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436510);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436518);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_0043651C);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436520);

INCLUDE_SDATA(const s32, "game/code_00195E50", D_00436528);

