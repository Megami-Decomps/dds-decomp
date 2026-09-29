#include "common.h"

typedef struct SoundSlot {
    u32 remainingFrames;
    u32 sequence;
} SoundSlot;

typedef struct SoundSlotPool {
    u32 handle;
    SoundSlot *slots;
    s32 count;
} SoundSlotPool;

/* Nodes passed to the menu model helpers are 0x50-byte records. */
typedef struct MnuModelNode {
    u8 pad00[0x30];
    f32 x;           /* 0x30 */
    f32 y;           /* 0x34 */
    f32 z;           /* 0x38 */
    u32 positionFlag; /* 0x3C */
    u32 *model;      /* 0x40 */
    u32 flags;       /* 0x44 */
    u16 value48;     /* 0x48 */
    u16 value4A;     /* 0x4A */
    f32 modelZ;      /* 0x4C */
} MnuModelNode;

extern void mdlBroadcastMasked(u32 sprite);

extern u32 *D_00438940;

void mnuClearNodeBroadcastFlag(u8 *node);
void dds3ReleaseSoundSlotPool(void);

void func_0031C578(s32 node);

extern u8 *func_00232198(s32 first, s32 second);
extern void mdlAddEntryFlaggedEx(u8 *model, s32 entry, s32 flags, f32 x, f32 y);
extern u32 func_003292A8(s32 bytes);
extern u32 *sdfMemoryGetBlockAddress(u32 handle);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B188);

void dds3InitSoundSlotPool(void) {
    u32 handle;
    SoundSlotPool *pool;
    if (D_00438940 != 0) {
        dds3ReleaseSoundSlotPool();
    }
    handle = func_003292A8(0x32c);
    D_00438940 = sdfMemoryGetBlockAddress(handle);
    memset(D_00438940, 0, 0x32c);
    pool = (SoundSlotPool *)D_00438940;
    pool->handle = handle;
    pool->slots = (SoundSlot *)(pool + 1);
    pool->count = 100;
}

void dds3ReleaseSoundSlotPool(void) {
    if (D_00438940 != (u32 *)0x0) {
        func_003297C8(*D_00438940);
        D_00438940 = (u32 *)0x0;
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B290);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B2E0);

/* Returns occupied sound slots to their default volume and pan when they expire. */
void dds3UpdateSoundSlots(void) {
    SoundSlotPool *pool = (SoundSlotPool *)D_00438940;
    if (pool != 0) {
        s32 index = 0;
        SoundSlot *slot = pool->slots;
        if (pool->count > 0) {
            do {
                if (slot->sequence != 0) {
                    if (slot->remainingFrames == 0 || --slot->remainingFrames == 0) {
                        sndSetSequenceVolumePan(slot->sequence, 0x7f, 0x3f);
                        slot->sequence = 0;
                    }
                }
                index++;
                slot++;
            } while (index < (s32)D_00438940[2]);
        }
    }
}

void func_0031B3B0(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 0;
}

void func_0031B3B8(s32 arg0) {
    *(u16 *)(arg0 + 0x1da) = 1;
}

void func_0031B3C8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B3D0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B4F0);

void mnuClearNodeRecords(s32 *list) {
    s32 record;
    u32 index;

    index = 0;
    record = *list;
    if (0 < list[1]) {
        do {
            memset(record, 0, 0x20);
            index = (index + 1) & 0xffff;
            record = record + 0x20;
        } while ((s32)index < list[1]);
    }
}

void func_0031B668(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            mnuClearNodeBroadcastFlag(node);
            node += 0x20;
            index++;
        } while (list[1] > index);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B6D0);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B748);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B838);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031B960);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BA28);

void mnuClearNodeBroadcastFlag(u8 *node) {
    *(u32 *)(node + 4) &= ~1U;
    func_002D46A0(*(u32 *)node);
}

/* Resolves a model from a resource and releases its temporary resource data. */

u32 func_0031BBB0(u32 *owner, u32 resource) {
    u32 handle;
    u32 other;
    u32 data = func_00343ED0(resource, &handle, &other);
    *owner = func_002D4138(handle);
    func_002D46A0(*owner);
    func_003297C8(data);
    return *owner;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BC10);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BDE8);

void func_0031BFA0(void) {
    func_002D2C80(2);
}


void func_0031BFC0(void) {
    func_002D2C90(2);
}


INCLUDE_ASM(const s32, "game/code_0031B188", func_0031BFE0);

void mnuInitializeNodeTransforms(u32 *group, f32 x, f32 y, f32 z, f32 w) {
    u16 index = 0;
    u8 *node = (u8 *)group[0];
    if ((s32)group[1] > 0) {
        do {
            memset(node, 0, 0x50);
            *(f32 *)(node + 0x20) = x;
            index++;
            *(f32 *)(node + 0x24) = y;
            *(f32 *)(node + 0x28) = z;
            *(f32 *)(node + 0x2c) = w;
            node += 0x50;
        } while (index < (s32)group[1]);
    }
}

void func_0031C1A0(s32 *list) {
    u8 *node = (u8 *)list[0];
    s32 index = 0;
    if (list[1] > 0) {
        do {
            func_0031C578((s32)node);
            node += 0x50;
            index++;
        } while (index < list[1]);
    }
}

void func_0031C208(s32 *list) {
    u8 *node = (u8 *)list[0];
    s16 index = 0;
    if (list[1] > 0) {
        do {
            u32 model = *(u32 *)(node + 0x40);
            node += 0x50;
            if (model != 0) {
                func_002322E8(model);
            }
            index++;
        } while (index < list[1]);
    }
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C280);

/* Claims the first inactive node whose model pointer is already populated. */
u8 *mnuAcquireUnusedModelNode(u32 *group) {
    s32 index = 0;
    u8 *node = (u8 *)group[0];
    if ((s32)group[1] > 0) {
        do {
            MnuModelNode *entry = (MnuModelNode *)node;
            if ((entry->flags & 1) == 0) {
                u32 *model = entry->model;
                if (model != 0) {
                    *model &= ~1U;
                    entry->value48 = 0;
                    entry->value4A = 0;
                    entry->flags = 1;
                    return node;
                }
                return 0;
            }
            node += 0x50;
            index++;
        } while (index < (s32)group[1]);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C3C8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C458);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C4A0);

void mnuCreateNodeModelEntry(u8 *node, s32 first, s32 second, s32 flag, f32 x, f32 y, f32 z) {
    u8 *model = func_00232198(first, second);
    ((MnuModelNode *)node)->model = (u32 *)model;
    if (flag != -1) {
        *(f32 *)(*(u8 **)(model + 0x1c) + 0x20) = z;
        ((MnuModelNode *)node)->modelZ = z;
        mdlAddEntryFlaggedEx(model, 0, flag, x, y);
    }
}

void func_0031C578(s32 arg0) {
    MnuModelNode *node = (MnuModelNode *)arg0;
    node->flags = 0;
    *node->model = *node->model | 1;
}

void func_0031C590(u8 *node, s32 value) {
    value &= 0xFFFF;
    ((MnuModelNode *)node)->value48 = value;
    ((MnuModelNode *)node)->value4A = value;
}

void func_0031C5A0(u8 *node, f32 x, f32 y, f32 z) {
    ((MnuModelNode *)node)->x = x;
    ((MnuModelNode *)node)->y = y;
    ((MnuModelNode *)node)->z = z;
    ((MnuModelNode *)node)->positionFlag = 0;
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C5B8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C5E8);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C630);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C688);

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C850);

void func_0031C888(u8 *node) {
    mdlBroadcastMasked((u32)((MnuModelNode *)node)->model);
}


void func_0031C8A8(void) {
}

INCLUDE_ASM(const s32, "game/code_0031B188", func_0031C8B0);

void func_0031C900(u8 *node, s8 selector) {
    MnuModelNode *entry = (MnuModelNode *)node;
    if (selector == 1) {
        *entry->model &= ~1U;
    } else {
        *entry->model |= 1U;
    }
}

INCLUDE_SDATA(const s32, "game/code_0031B188", D_00438950);

