#include "common.h"
extern char D_00425118[];
extern char D_004378A8[]; /* "%d" */
extern void func_00311DB0(s32, s32, s32, s32, s32, char *, s32, s32);

extern u64 func_00279DC8(u32, u64, u64, u64, u64, u64);

extern u64 func_0027A628(u32, u64, u64);

extern u32 func_00284AE0(void);

extern u32 func_002843C0(u32);

extern s32 mnuFindMantraDrawItemByKind(u32, u32);

extern s32 func_00328D68(u32);

void func_003297C8(u32 sprite);

void func_002844E8(u32 sprite);

void func_00284B48(u32 *obj);
u32 func_002850B8(void);
extern void func_00286270();
u32 func_0026F700(u32);
u32 func_0026F778(u32);
extern u32 func_0026FC88(u32, u32, void (*)(), void (*)(), u32 (*)(), void (*)(), s16, s16, u32);
extern s32 mnuUpdateMantraFadeA();
extern u32 func_00279440(u32, u32);
extern u32 func_00279180();
extern u32 func_00279488();
extern u32 func_00279628();
extern void func_002792D8(s32);
s32 func_00271368();
void func_00271510();
u32 func_002712E0();
void mantraSetupSlot(u32);
void func_00271348();
void func_00275358(u32, u32, u32, u32);
extern s32 func_00270210();
extern void func_00270848();
extern u32 func_00270160();
extern void func_002701D0();
extern s32 func_00270DD8();
extern s32 func_00270F10();
extern u32 func_00270D60();
extern void func_00270DB0();
extern s32 func_00272DA8();
extern void func_00272F08();
extern u32 func_00272D20();
extern void func_00272D80();
extern void func_00273668();
extern void func_00273828();
extern u32 func_002735F0();
extern void func_00273640();
extern void func_002741D0();
extern void func_002743B8();
extern u32 func_00274158();
extern void func_002741A8();
extern s32 func_00274EA8();
extern void func_00274FF8();
extern u32 mnuCreateTypeOneRecord(void);
extern void func_00274E88();
extern u32 D_00453D00[12];
extern u8 *D_00435DD0;

typedef struct MenuRecord {
    u16 type;
    u16 flags;
    u8 unk_04[12];
    u32 unk_10;
    u8 unk_14[4];
} MenuRecord;

typedef struct MantraDisplayNode {
    u8 pad00[0x10];
    struct MantraDisplayNode *next;
} MantraDisplayNode;

typedef struct MantraDrawItem {
    u32 kind;
    u32 flags;
    u8 pad8[4];
    void (*release)(void);
    u8 pad10[0x10];
    void *data;
} MantraDrawItem;

typedef struct MantraDrawPool {
    u32 handle;
    MantraDrawItem *items;
    s32 count;
} MantraDrawPool;

typedef struct MantraFadeData {
    u8 *iconList;
    u16 state;
    u16 elapsed;
    f32 scale;
    s16 x;
    s16 y;
} MantraFadeData;

typedef struct MantraEffectResource {
    u8 pad00[0x6C];
    u32 handle;
} MantraEffectResource;

typedef struct MantraFadeState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 timer;
    /* 0x04 */ u32 unk4;
    /* 0x08 */ f32 value;
    /* 0x0C */ u32 flagsC;
    /* 0x10 */ u32 flags10;
    /* 0x14 */ u32 armed : 1;
    u32 countdown : 31;
    /* 0x18 */ s32 clock;
    /* 0x1C */ u16 queuedState;
    /* 0x1E */ u16 delay;
    /* 0x20 */ u32 queuedFlags;
} MantraFadeState;

typedef struct MantraSourceEntry {
    u32 unk0;
    u32 flags;
    u32 unk8;
    s32 value;
    u32 unk10;
} MantraSourceEntry;

typedef struct MantraListState {
    u32 unk0;
    MantraDisplayNode *head;
    u32 entries[8];
    s16 count;
    s16 index;
    u32 unk2C;
} MantraListState;

typedef struct MantraFileEntry {
    struct MantraFileEntry *next;
    u8 pad04[4];
    u32 handle;
    u8 pad0C[4];
    u8 kind;
} MantraFileEntry;

typedef struct MantraFileRequest {
    u8 pad00[0x60];
    MantraFileEntry *entries;
} MantraFileRequest;
extern u32 func_002C7FF0(const char *);
extern void *kwlnTaskCreate(const char *, s32, s32, s32, s32 (*)(void),
                            void (*)(), void *);
s32 func_0026E6E0(void);
extern u32 func_0026E788(u32, u32, u32, u32, u32, u32, u32);
void func_00284508(u32, u32, u32, u32, u32, u32);
extern void func_002758B8(s16, s16, u32, s32, u32);
extern char D_004250D8[];
extern s32 func_002748D0();
extern void func_00274A70();
extern u32 func_00274820();
extern void func_00274890();
void mnuStorePanelEntry(u32, u32);
void func_003054E8(u32);
extern u32 func_003292A8(u32);
extern u32 sdfMemoryGetBlockAddress(u32);

s32 func_0026DBF8(void) {
    s32 result = 0;

    if (mdlFlagTest(0x920)) {
        result = 1;
    }
    if (mdlFlagTest(0x921)) {
        result = 2;
    }
    if (mdlFlagTest(0x922)) {
        result = 3;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DC48);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026DE08);

void func_0026DF48(u32 arg0, u32 arg1, u32 arg2, u32 arg3, u16 arg4, u32 arg5) {
    u8 buffer[0x20];
    u32 flags = (arg3 & 0xFF) | 0xA09DC300;

    func_0026E788(arg0, arg1, arg2, arg3, 0x4C, 0, arg5);
    if (arg4 != 0) {
        memset(buffer, 0, 0x20);
        func_00314500(arg4, 1, buffer);
        func_00311C50(arg0 + 0x27, arg1 + 0x146, arg2, flags, 4, buffer, 0x101, arg5);
        func_0026E788(arg0, arg1, arg2, arg3, 0x50, 0, arg5);
    } else {
        func_0026E788(arg0, arg1, arg2, arg3, 0x54, 0, arg5);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E060);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E2D8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E470);

void mnuMergeMantraSpriteSlots(u32 *values) {
    s32 i;
    for (i = 0; i < 12; i++) {
        if (values[i] != 0) {
            D_00453D00[i] = values[i];
        }
    }
}

void mnuReleaseFirstMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 1; i >= 0; i--, slot++) {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378A8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378B0);

void func_0026E560(void) {
    s32 slots[2] = {2, 3};
    u32 i;

    for (i = 0; i < 2; i++) {
        if (D_00453D00[slots[i]] != 0) {
            func_003054E8(D_00453D00[slots[i]]);
        }
        D_00453D00[slots[i]] = 0;
    }
}

void mnuStartMantraSpriteLoad(void) {
    if (D_00453D00[4] == 0) {
        u32 resource = func_002C7FF0("/facility/spr/mantra/sprite_a.lb");
        kwlnTaskCreate(D_004250D8, 0x402, 1, 1, func_0026E6E0, 0,
                       (void *)resource);
    }
}

s32 mnuHasMantraSpriteTaskFinished(void) {
    if (D_00453D00[4] != 0) {
        return 1;
    }
    return func_00101740(D_004250D8) == 0;
}

void mnuReleaseMantraSpriteSlots(void) {
    s32 i;
    u32 *slot = D_00453D00;
    for (i = 11; i >= 0; i--, slot++) {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E6E0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E788);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026E998);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EBA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EDB0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026EEE8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004250D8);

void mnuDrawMantraCostBadge(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4, s32 arg5) {
    char buttons[9] = {0, 'n', 's', 'o', 'q', 'p', 'r', 't', 'u'};
    char text[8];
    s32 color = arg4 | 0xA09DC300;

    func_0026E788(arg0, arg1, arg2, arg4, buttons[*(u16 *)(arg3 + 4)], 0, arg5);
    func_0026E788(arg0, arg1, arg2, arg4, 0x76, 0, arg5);
    memset(text, 0, sizeof(text));
    func_0035C860(text, D_004378A8, *(u16 *)(arg3 + 0x14));
    if (strlen(text) > 1) {
        func_00311DB0(arg0 + 0x1E0, arg1 + 0x173, arg2, color, 0, text, 0, arg5);
    } else {
        func_00311DB0(arg0 + 0x1E4, arg1 + 0x173, arg2, color, 0, text, 0, arg5);
    }
}

u32 func_0026F138(u32 unused1, u32 unused2, u32 third, u32 record,
                  u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = *(u16 *)(record + 4);
    return func_0026E788(0, 0, third, position, markers[index], 0, packet);
}

u32 func_0026F190(u32 unused1, u32 unused2, u32 third, u32 record,
                  u32 position, u32 packet) {
    char markers[9] = { '\0', '/', '4', '0', '2', '1', '3', '5', '6' };
    u16 index = *(u16 *)(record + 4);
    return func_0026E788(0, 0, third, position, markers[index] + 8, 0,
                          packet);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F1F0);

MantraDisplayNode *mnuAllocateDisplayListNode(void) {
    MantraDisplayNode *node = (MantraDisplayNode *)func_00328D68(sizeof(MantraDisplayNode));

    memset(node, 0, sizeof(MantraDisplayNode));
    return node;
}

u32 mnuReleaseDisplayListNodeAndGetNext(MantraDisplayNode *node) {
    u32 next;

    next = (u32)node->next;
    // Required to match: the node remains in $a0 for the allocator's release call.
    func_00328E48();
    return next;
}

void func_0026F5D8(MantraListState *list, u32 *entries, s32 count, s32 index) {
    s32 n = 8;
    s32 i;

    memset(list, 0, sizeof(MantraListState));
    if (count < 8) {
        n = count;
    }
    for (i = 0; i < n; i++) {
        list->entries[i] = entries[i];
    }
    list->count = n;
    list->index = index;
    func_0010AE38("[MaxNum %d][CurrentIndex %d]\n", n, index);
}

u32 func_0026F680(u32 state, s8 selection) {
    u32 item;
    u32 *entries;
    if (*(s16 *)(state + 0x2a) == selection) {
        return 0;
    }
    item = mnuAppendDisplayListNode(state);
    if (item != 0) {
        u32 *selected;
        u32 *previous;
        entries = (u32 *)(state + 8);
        *(u16 *)(item + 0xc) = 2;
        selected = entries + selection;
        previous = entries + *(s16 *)(state + 0x2a);
        *(s16 *)(state + 0x2a) = selection;
        *(u32 *)(item + 4) = *previous;
        *(u32 *)(item + 8) = *selected;
    }
    return item;
}

u32 func_0026F700(u32 state) {
    u32 item = mnuAppendDisplayListNode(state);
    u32 *entries = (u32 *)(state + 8);
    if (item != 0) {
        s16 index = *(s16 *)(state + 0x2a);
        s16 count = *(s16 *)(state + 0x28);
        s32 next = index + 1;
        u32 *current;
        u32 *upcoming;
        *(u16 *)(item + 0xc) = 2;
        if (index >= count - 1) {
            next = 0;
        }
        current = entries + *(s16 *)(state + 0x2a);
        upcoming = entries + next;
        *(s16 *)(state + 0x2a) = next;
        *(u32 *)(item + 4) = *current;
        *(u32 *)(item + 8) = *upcoming;
    }
    return item;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F778);

u32 mnuAppendDisplayListNode(u32 state) {
    MantraDisplayNode *node = *(MantraDisplayNode **)(state + 4);
    if (node == 0) {
        node = mnuAllocateDisplayListNode();
        *(MantraDisplayNode **)(state + 4) = node;
    } else {
        while (node->next != 0) {
            node = node->next;
        }
        node->next = mnuAllocateDisplayListNode();
        node = node->next;
    }
    return (u32)node;
}

void mnuReleaseDisplayListNodes(u32 state) {
    MantraDisplayNode *node = *(MantraDisplayNode **)(state + 4);
    while (node != 0) {
        node = (MantraDisplayNode *)mnuReleaseDisplayListNodeAndGetNext(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0026F8A0);

void func_0026FAA8(s32 arg0) {
    *(u32 *)(arg0 + 0x2c) = 0x1e;
}

u32 func_0026FAB8(void) {
    return 0;
}

u32 func_0026FAC0(void) {
    return 0;
}

void func_0026FAC8(void) {
}

u32 mnuCreateMantraDrawPool(u32 count) {
    u32 size = count * 0x24 + 0xc;
    u32 handle = func_003292A8(size);
    MantraDrawPool *pool = (MantraDrawPool *)sdfMemoryGetBlockAddress(handle);
    memset(pool, 0, size);
    pool->handle = handle;
    pool->count = count;
    pool->items = (MantraDrawItem *)((u8 *)pool + 0xc);
    func_0010AE38("mtrDrawProcessCreate!! num[%d]\n", count);
    return (u32)pool;
}

void mnuDestroyMantraDrawPool(u32 address) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    MantraDrawItem *item = pool->items;
    s32 i;
    for (i = 0; i < count; i++, item++) {
        if (item->flags & 1) {
            mantraSetupSlot((u32)item);
        }
    }
    func_003297C8(pool->handle);
}

void mantraSetupSlot(u32 item) {
    MantraDrawItem *entry = (MantraDrawItem *)item;
    if (!((entry->flags >> 11) & 1)) {
        if (entry->release != 0) {
            entry->release();
        }
        entry->flags |= 0x800;
    }
    entry->flags &= ~1;
}

s32 mnuFindFreeMantraDrawItem(s32 address) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    s32 i;
    MantraDrawItem *item = pool->items;
    for (i = 0; i < count; i++, item++) {
        if ((item->flags & 1) == 0) {
            return (s32)item;
        }
    }
    return 0;
}

u32 func_0026FC88(u32 pool, u32 kind, void (*update)(), void (*draw)(), u32 (*poll)(), void (*release)(), s16 arg6, s16 arg7, u32 data) {
    u32 *item = (u32 *)mnuFindFreeMantraDrawItem(pool);

    memset(item, 0, 0x24);
    item[1] |= 1;
    item[0] = kind;
    item[7] = data;
    *(s16 *)((u8 *)item + 0x1A) = arg7;
    *(s16 *)((u8 *)item + 0x18) = arg6;
    item[2] = (u32)poll;
    if (update != 0) {
        item[4] = (u32)update;
    } else {
        item[4] = (u32)func_0026FAB8;
    }
    if (draw != 0) {
        item[5] = (u32)draw;
    } else {
        item[5] = (u32)func_0026FAB8;
    }
    item[3] = (u32)release;
    if (arg6 > 0) {
        item[1] = (item[1] & 0xFFFFF807) | 8;
    } else if (poll != 0) {
        item[1] = (item[1] & 0xFFFFF807) | 0x18;
    } else {
        item[1] = (item[1] & 0xFFFFF807) | 0x20;
    }
    return (u32)item;
}

s32 mnuFindMantraDrawItemByKind(u32 address, u32 kind) {
    MantraDrawPool *pool = (MantraDrawPool *)address;
    s32 count = pool->count;
    s32 i;
    MantraDrawItem *item = pool->items;
    for (i = 0; i < count; i++, item++) {
        if ((item->flags & 0x7f9) == 0x21 && item->kind == kind) {
            return (s32)item;
        }
    }
    return 0;
}

typedef struct DrawItem {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u32 active : 1;
    u32 unk_bits : 2;
    u32 state : 8;
    u32 started : 1;
    u32 unk_hi : 20;
    /* 0x08 */ s32 (*onStart)(struct DrawItem *, s32);
    /* 0x0C */ void (*onEnd)(struct DrawItem *);
    /* 0x10 */ s32 (*isReady)(void *, struct DrawItem *);
    /* 0x14 */ void (*update)(void *, struct DrawItem *);
    /* 0x18 */ s16 timer0;
    /* 0x1A */ s16 timer1;
    /* 0x1C */ s32 arg;
    /* 0x20 */ s32 result;
} DrawItem;

void mnuUpdateMantraDrawPool(u8 *pool) {
    s32 i;
    s32 ready = 0;
    s32 count = *(s32 *)(pool + 8);
    DrawItem *item = *(DrawItem **)(pool + 4);

    for (i = 0; i < count; i++, item = (DrawItem *)((u8 *)item + 0x24)) {
        if (!item->active) {
            continue;
        }
        switch (item->state) {
        case 1:
            item->timer0 -= 1;
            if (item->timer0 == 0) {
                if (item->onStart != 0) {
                    item->state = 3;
                } else {
                    item->state = 4;
                }
            }
            break;
        case 2:
            item->timer1 -= 1;
            if (item->timer1 == 0) {
                if (item->onEnd != 0) {
                    item->state = 5;
                } else {
                    item->state = 6;
                }
            }
            break;
        case 3:
            item->state = 4;
            item->result = item->onStart(pool, item->arg);
            break;
        case 4:
            if (item->isReady(pool, item) == 1) {
                ready = 1;
            }
            item->update(pool, item);
            if (ready != 0) {
                if (item->timer1 > 0) {
                    item->state = 2;
                } else {
                    item->state = 5;
                }
            }
            break;
        case 5:
            if (!item->started) {
                item->onEnd(item);
            }
            item->started = 1;
            item->active = 0;
            break;
        case 6:
            item->active = 0;
            break;
        case 7:
            break;
        }
    }
}

u32 func_00270008(u32 arg0) {
    return func_0026FC88(arg0, 0, func_00270210, func_00270848,
                         func_00270160, func_002701D0, 0, 0, 0);
}

void func_00270050(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    *(u16 *)item->data = 3;
}

void func_00270078(u32 pool, s8 variant) {
    MantraDrawItem *item;
    u8 *data;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    data = (u8 *)item->data;
    *(u32 *)(data + 4) = (*(u32 *)(data + 4) & 0xffffff0f) | (((s32)variant & 0xfU) << 4);
    *(u8 *)(data + 5) = 5;
}

void func_002700D0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        *(u32 *)((u8 *)item->data + 0x10) = 1;
    }
}

void func_00270100(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        *(u32 *)((u8 *)item->data + 0x10) = 0;
    }
}

void func_00270128(u32 pool, u32 value) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 0);
    if (item != 0) {
        *(u32 *)((u8 *)item->data + 0x14) = value;
    }
}


u32 func_00270160(void) {
    u32 data = func_00328D68(0x1c);
    memset((void *)data, 0, 0x1c);
    *(u16 *)data = 1;
    *(u32 *)(data + 4) &= ~0xf;
    *(u8 *)(data + 5) = 0;
    *(u32 *)(data + 0x18) = func_002850B8();
    func_0010AE38("BG Draw Init\n");
    return data;
}

void func_002701D0(u32 obj) {
    s32 data = *(s32 *)(obj + 0x20);
    func_00285120(*(u32 **)(data + 0x18));
    func_00328E48(data);
    func_0010AE38("BG Draw Release\n");
}

typedef struct MantraCursorFade {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u32 cur : 4;
    u32 next : 4;
    u32 unk4_hi : 24;
    /* 0x08 */ u16 timer;
    /* 0x0A */ u16 clock;
    /* 0x0C */ f32 value;
} MantraCursorFade;

s32 func_00270210(s32 arg0, s32 arg1) {
    MantraCursorFade *fade = *(MantraCursorFade **)(arg1 + 0x20);

    fade->clock += 1;
    if ((s16)fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (*(s8 *)((u8 *)fade + 5) > 0) {
        *(s8 *)((u8 *)fade + 5) -= 1;
        if (*(s8 *)((u8 *)fade + 5) == 0) {
            if (fade->cur != fade->next) {
                fade->cur = fade->next;
                *(s8 *)((u8 *)fade + 5) = 5;
            }
        }
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->value = (f32)fade->timer / 22.0f;
        fade->timer += 1;
        if (fade->timer >= 22) {
            fade->state = 2;
            fade->value = 1.0f;
            fade->timer = 0;
        }
        break;
    case 2:
        fade->value = 1.0f;
        break;
    case 3:
    case 5:
        fade->value = 1.0f - (f32)fade->timer / 10.0f;
        fade->timer += 1;
        if (fade->timer >= 10) {
            fade->timer = 0;
            fade->value = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270390);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270568);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270848);

u32 func_00270C78(u32 arg0) {
    return func_0026FC88(arg0, 4, func_00270DD8, func_00270F10,
                         func_00270D60, func_00270DB0, 0, 0, 0);
}

void func_00270CC0(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 3;
}

void func_00270CE8(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 6;
}

void func_00270D10(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 5;
}

void func_00270D38(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 4);
    *(u16 *)item->data = 2;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004251C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425258);

u32 func_00270D60(void) {
    u32 data = func_00328D68(0xc);
    memset((void *)data, 0, 0xc);
    *(u16 *)data = 1;
    func_0010AE38("BGMask Draw Init\n");
    return data;
}

void func_00270DB0(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("BGMask Draw Release\n");
}

s32 func_00270DD8(s32 arg0, s32 arg1) {
    u8 *fade = *(u8 **)(arg1 + 0x20);

    *(u16 *)(fade + 4) += 1;
    if ((s16)*(u16 *)(fade + 4) >= 0x79) {
        *(u16 *)(fade + 4) = 0;
    }
    switch (*(u16 *)fade) {
    case 1:
    case 6:
        *(f32 *)(fade + 8) = (f32)*(u16 *)(fade + 2) / 22.0f;
        *(u16 *)(fade + 2) += 1;
        if (*(u16 *)(fade + 2) >= 22) {
            *(u16 *)fade = 2;
            *(f32 *)(fade + 8) = 1.0f;
            *(u16 *)(fade + 2) = 0;
        }
        break;
    case 2:
        *(f32 *)(fade + 8) = 1.0f;
        break;
    case 3:
    case 5:
        *(f32 *)(fade + 8) = 1.0f - (f32)*(u16 *)(fade + 2) / 10.0f;
        *(u16 *)(fade + 2) += 1;
        if (*(u16 *)(fade + 2) >= 10) {
            *(u16 *)(fade + 2) = 0;
            *(f32 *)(fade + 8) = 0.0f;
            if (*(u16 *)fade == 5) {
                *(u16 *)fade = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        *(f32 *)(fade + 8) = 0.0f;
        break;
    }
    return 0;
}

extern f32 func_003406A0(f32);
extern void func_00270390(s32, s32, f32);

s32 func_00270F10(s32 arg0, s32 arg1) {
    u8 *data = *(u8 **)(arg1 + 0x20);
    s32 amount = *(f32 *)(data + 8) * 128.0f;
    f32 wave = func_003406A0((f32)*(s16 *)(data + 4) / 120.0f * (3.14159265f * 2.0f) + (-3.14159265f / 2.0f));

    func_00270390(amount, 0x53, (wave + 1.0f) * 0.5f);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00270FA8);

void func_00271020(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        if (*(u16 *)data == 4) {
            *(u16 *)(data + 2) = 5;
        }
        *(u16 *)data = 3;
    }
}

void mnuShowMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        *(u16 *)item->data = 6;
        func_0010AE38("LimitLine Draw Show\n");
    }
}

void mnuHideMantraLimitLine(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 1);
    if (item != 0) {
        *(u16 *)item->data = 5;
        func_0010AE38("LimitLine Draw Hide\n");
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002710D8);

void func_002711F8(s16 x, s16 y, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(s16 *)(data + 4) = x;
        *(s16 *)(data + 6) = y;
    }
}

void func_00271250(u32 arg0, u32 value) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 1);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(u32 *)(data + 0x10) = value;
        *(u32 *)(data + 0x14) = 0x15;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271290);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002712E0);

void func_00271348(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
}


s32 func_00271368(s32 arg0, s32 arg1) {
    MantraFadeState *fade = *(MantraFadeState **)(arg1 + 0x20);

    fade->clock += 1;
    if (fade->clock >= 0x79) {
        fade->clock = 0;
    }
    if (fade->delay != 0) {
        fade->delay -= 1;
        if (fade->delay == 0) {
            fade->state = fade->queuedState;
            fade->flagsC = fade->queuedFlags;
        }
    }
    if (fade->armed) {
        fade->countdown -= 1;
        if (fade->countdown == 0) {
            fade->armed = 0;
            fade->flagsC |= fade->flags10;
            fade->flags10 = 0;
        }
    }
    switch (fade->state) {
    case 1:
    case 6:
        fade->value = (f32)fade->timer / 20.0f;
        fade->timer += 1;
        if (fade->timer >= 20) {
            fade->state = 2;
            fade->value = 1.0f;
            fade->timer = 0;
        }
        break;
    case 2:
        fade->value = 1.0f;
        break;
    case 3:
    case 5:
        fade->value = 1.0f - (f32)fade->timer / 5.0f;
        fade->timer += 1;
        if (fade->timer >= 5) {
            fade->timer = 0;
            fade->value = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00271510);

u32 func_00272BD0(u32 arg0) {
    return func_0026FC88(arg0, 6, func_00272DA8, func_00272F08,
                         func_00272D20, func_00272D80, 0, 0, 0);
}

void func_00272C18(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 6);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425338);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425348);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425370);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425380);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004253B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425408);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425420);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425448);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425458);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425490);

void mnuHideMantraTitle(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    if (item != 0) {
        *(u16 *)item->data = 5;
        func_0010AE38("Title Draw Hide\n");
    }
}

void mnuShowMantraTitle(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 6);
    if (item != 0) {
        *(u16 *)item->data = 6;
        func_0010AE38("Title Draw Show\n");
    }
}

void func_00272CB0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 2) = *(u16 *)(temp_v0 + 2) ^ 1;
}

void func_00272CE8(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 6);
    temp_v0 = *(s32 *)(temp_v0 + 0x20);
    *(u16 *)(temp_v0 + 0xc) = 10;
    *(u16 *)(temp_v0 + 0xe) = *(u16 *)(temp_v0 + 0xe) ^ 1;
}

u32 func_00272D20(void) {
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)(data + 2) = 0;
    *(u16 *)(data + 4) = 0;
    *(u16 *)data = 1;
    *(u16 *)(data + 6) = 0;
    func_0010AE38("Title Draw Init\n");
    return data;
}

void func_00272D80(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("Title Draw Release\n");
}

typedef struct MantraBlinkState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ s16 timer;
    /* 0x06 */ s16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ s16 delay;
} MantraBlinkState;

s32 func_00272DA8(s32 arg0, s32 arg1) {
    MantraBlinkState *blink = *(MantraBlinkState **)(arg1 + 0x20);

    blink->clock += 1;
    if (blink->clock >= 0x25) {
        blink->clock = 0;
    }
    if (blink->delay > 0) {
        blink->delay -= 1;
    }
    switch (blink->state) {
    case 1:
    case 6:
        blink->value = (f32)blink->timer / 20.0f;
        blink->timer += 1;
        if (blink->timer >= 20) {
            blink->state = 2;
            blink->value = 1.0f;
            blink->timer = 0;
        }
        break;
    case 2:
        blink->value = 1.0f;
        break;
    case 3:
    case 5:
        blink->value = 1.0f - (f32)blink->timer / 10.0f;
        blink->timer += 1;
        if (blink->timer >= 10) {
            blink->timer = 0;
            blink->value = 0.0f;
            if (blink->state == 5) {
                blink->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        blink->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00272F08);

u32 func_00273450(u32 arg0) {
    return func_0026FC88(arg0, 7, func_00273668, func_00273828,
                         func_002735F0, func_00273640, 10, 0, 0);
}

void func_00273498(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 7);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void mnuHideMantraInfo(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 7);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 6) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 5;
        }
        func_0010AE38("Info Draw Hide\n");
    }
}

void mnuShowMantraInfo(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 7);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 5) {
            *(u32 *)data = ((*(u32 *)data | 0x10000) & 0x1ffff) | 0xa0000;
        } else {
            *(u16 *)data = 6;
        }
        func_0010AE38("Info Draw Show\n");
    }
}

void func_002735A0(u32 arg0, s16 x, u16 y) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 7);
    if (obj != 0) {
        s32 data = *(s32 *)(obj + 0x20);
        *(u16 *)(data + 8) = x;
        *(u16 *)(data + 0xa) = y;
    }
}

u32 func_002735F0(void) {
    u32 data = func_00328D68(0x10);
    memset((void *)data, 0, 0x10);
    *(u16 *)data = 1;
    func_0010AE38("Info Draw Init\n");
    return data;
}

void func_00273640(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("Info Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273668);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00273828);

u32 func_00273F88(u32 arg0) {
    return func_0026FC88(arg0, 8, func_002741D0, func_002743B8,
                         func_00274158, func_002741A8, 0, 0, 0);
}

void func_00273FD0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 8);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void mnuHideMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        s32 data = (s32)item->data;
        u16 state = *(u16 *)data;
        if (state == 6) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 4) {
            *(u16 *)data = 5;
        }
        func_0010AE38("ScrollCursor Draw Hide\n");
    }
}

void mnuShowMantraScrollCursor(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 8);
    if (item != 0) {
        s32 data = (s32)item->data;
        u16 state = *(u16 *)data;
        if (state == 5) {
            *(u32 *)(data + 8) =
                ((*(u32 *)(data + 8) | 0x10000) & 0xfe01ffff) | 0xa0000;
        } else if (state != 2) {
            *(u16 *)data = 6;
        }
        func_0010AE38("ScrollCursor Draw Show\n");
    }
}

void func_002740E8(u32 ctx, u16 flags) {
    s32 obj = mnuFindMantraDrawItemByKind(ctx, 8);
    if (obj != 0) {
        u8 *entry = (u8 *)(*(u32 *)(obj + 0x20) + 2);
        s32 i;
        for (i = 0; i < 4; i++, entry += 2) {
            if ((flags >> i) & 1) {
                *entry = 1;
            } else {
                *entry = 0;
            }
        }
    }
}

u32 func_00274158(void) {
    u32 data = func_00328D68(0x18);
    memset((void *)data, 0, 0x18);
    *(u16 *)data = 4;
    func_0010AE38("ScrollCursor Draw Init\n");
    return data;
}

void func_002741A8(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
    func_0010AE38("ScrollCursor Draw Release\n");
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002741D0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002743B8);

u32 func_002745E0(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 9, func_002748D0, func_00274A70,
                         func_00274820, func_00274890, 10, 0, arg1);
}

void func_00274628(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 9);
    **(u16 **)(temp_v0 + 0x20) = 3;
}

void func_00274650(u32 arg0, s16 value) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 9);
    if (obj != 0) {
        *(s16 *)(*(s32 *)(obj + 0x20) + 2) = value;
    }
}

void mnuHideMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 6) {
            *(u32 *)(data + 0x3c) =
                ((*(u32 *)(data + 0x3c) | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 5;
        }
        func_0010AE38("UnitPanel Draw Hide\n");
    }
}

void mnuShowMantraUnitPanel(u32 ctx) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(ctx, 9);
    if (item != 0) {
        s32 data = (s32)item->data;
        if (*(u16 *)data == 5) {
            *(u32 *)(data + 0x3c) =
                ((*(u32 *)(data + 0x3c) | 1) & 0xffff0001) | 0xa;
        } else {
            *(u16 *)data = 6;
        }
        func_0010AE38("UnitPanel Draw Show\n");
    }
}


void func_00274760(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 9);
    func_0026F778(*(u32 *)(obj + 0x20) + 0xc);
}

void func_00274788(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 9);
    func_0026F700(*(u32 *)(obj + 0x20) + 0xc);
}

u32 func_002747B0(u32 arg0, s8 value) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 9);
    return func_0026F680(*(u32 *)(obj + 0x20) + 0xc, value) != 0;
}

u32 func_002747F0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = mnuFindMantraDrawItemByKind(arg0, 9);
    func_0026FAA8(*(s32 *)(temp_v0 + 0x20) + 0xc);
    return 0;
}

u32 func_00274820(u32 ctx, u32 resources) {
    u32 data = func_00328D68(0x40);
    memset((void *)data, 0, 0x40);
    *(u16 *)data = 1;
    func_0026F5D8(data + 0xc, resources,
                  *(u32 *)(resources + 0x24), *(u32 *)(resources + 0x20));
    func_0010AE38("UnitPanel Draw Init\n");
    return data;
}

void func_00274890(u32 obj) {
    s32 data = *(s32 *)(obj + 0x20);
    mnuReleaseDisplayListNodes(data + 0xc);
    func_00328E48(data);
    func_0010AE38("UnitPanel Draw Release\n");
}

typedef struct MantraLampState {
    /* 0x00 */ u16 state;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 timer;
    /* 0x06 */ u16 clock;
    /* 0x08 */ f32 value;
    /* 0x0C */ u8 unkC[0x30];
    /* 0x3C */ u32 bits;
} MantraLampState;

s32 func_002748D0(s32 arg0, s32 arg1) {
    MantraLampState *lamp = *(MantraLampState **)(arg1 + 0x20);
    u32 bits;
    u16 hold;

    lamp->clock += 1;
    if ((s16)lamp->clock >= 0x3D) {
        lamp->clock = 0;
    }
    if (lamp->bits & 0xFFFE) {
        hold = (lamp->bits >> 1) & 0x7FFF;
        hold -= 1;
        lamp->bits = (lamp->bits & 0xFFFF0001) | ((hold & 0x7FFF) << 1);
    }
    switch (lamp->state) {
    case 1:
    case 6:
        lamp->value = (f32)lamp->timer / 20.0f;
        lamp->timer += 1;
        if (lamp->timer >= 20) {
            bits = lamp->bits;
            lamp->state = (bits & 1) ? 4 : 2;
            lamp->value = 1.0f;
            lamp->timer = 0;
            lamp->bits = bits & ~1;
        }
        break;
    case 2:
        lamp->value = 1.0f;
        break;
    case 3:
    case 5:
        lamp->value = 1.0f - (f32)lamp->timer / 10.0f;
        lamp->timer += 1;
        if (lamp->timer >= 10) {
            lamp->timer = 0;
            lamp->value = 0.0f;
            if (lamp->state == 5) {
                bits = lamp->bits;
                lamp->state = (bits & 1) ? 2 : 4;
                lamp->bits = bits & ~1;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        lamp->value = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274A70);

u32 func_00274D88(u32 arg0) {
    return func_0026FC88(arg0, 10, func_00274EA8, func_00274FF8,
                         mnuCreateTypeOneRecord, func_00274E88, 0, 0, 0);
}

void func_00274DD0(u32 pool) {
    MantraDrawItem *item;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    *(u16 *)item->data = 3;
}

void func_00274DF8(u32 pool) {
    MantraDrawItem *item;
    MenuRecord *record;

    item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 10);
    record = (MenuRecord *)item->data;
    *(u32 *)((u8 *)record + 0xc) = 0xf;
    record->flags = record->flags ^ 1;
}

u32 mnuCreateTypeOneRecord(void) {
    MenuRecord *record = (MenuRecord *)func_00328D68(sizeof(MenuRecord));
    memset(record, 0, sizeof(MenuRecord));
    record->type = 1;
    record->flags = 0;
    record->unk_10 = *(u32 *)(D_00435DD0 + 0x3c);
    return (u32)record;
}

void func_00274E88(u32 obj) {
    func_00328E48(*(u32 *)(obj + 0x20));
}

s32 func_00274EA8(s32 arg0, s32 arg1) {
    u8 *fade = *(u8 **)(arg1 + 0x20);

    *(u16 *)(fade + 6) += 1;
    if ((s16)*(u16 *)(fade + 6) >= 0x3D) {
        *(u16 *)(fade + 6) = 0;
    }
    if (*(s32 *)(fade + 0xC) > 0) {
        *(s32 *)(fade + 0xC) -= 1;
    }
    switch (*(u16 *)fade) {
    case 1:
    case 6:
        *(f32 *)(fade + 8) = (f32)*(u16 *)(fade + 4) / 20.0f;
        *(u16 *)(fade + 4) += 1;
        if (*(u16 *)(fade + 4) >= 20) {
            *(u16 *)fade = 2;
            *(f32 *)(fade + 8) = 1.0f;
            *(u16 *)(fade + 4) = 0;
        }
        break;
    case 2:
        *(f32 *)(fade + 8) = 1.0f;
        break;
    case 3:
    case 5:
        *(f32 *)(fade + 8) = 1.0f - (f32)*(u16 *)(fade + 4) / 10.0f;
        *(u16 *)(fade + 4) += 1;
        if (*(u16 *)(fade + 4) >= 10) {
            *(u16 *)(fade + 4) = 0;
            *(f32 *)(fade + 8) = 0.0f;
            if (*(u16 *)fade == 5) {
                *(u16 *)fade = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        *(f32 *)(fade + 8) = 0.0f;
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00274FF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275218);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275358);

u32 func_00275510(u32 count) {
    u32 size = count * 12 + 0x14;
    u32 handle = func_003292A8(size);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    *(u32 *)(block + 0x10) = count;
    *(u32 *)(block + 4) = block + 0x14;
    return block;
}

void func_00275598(u32 *sprite) {
    func_003297C8(*sprite);
}

u32 func_002755B8(u32 *pool, u32 flags) {
    s32 i;
    u32 *item;
    u32 *tail;
    u32 header;

    if (flags & 0x20) {
        if (pool[3] != 0) {
            return pool[3];
        }
    }
    if (flags & 0xF) {
        if (pool[1] == 0) {
            return 0;
        }
        return pool[2];
    }
    item = (u32 *)pool[1];
    for (i = 0; i < (s32)pool[4]; i++, item += 3) {
        if ((item[0] & 1) == 0) {
            memset(item, 0, 0xC);
            header = item[0] | 1;
            item[0] = header;
            if (flags & 0x10) {
                tail = (u32 *)pool[2];
                if (tail != 0) {
                    if (((tail[0] >> 1) & 0xF) == 9) {
                        tail[0] = (tail[0] & 0xFFFFFFE1) | 8;
                    } else {
                        tail[0] = (tail[0] & 0xFFFFFFE1) | 6;
                    }
                    tail[2] = 0;
                }
                pool[2] = (u32)item;
            } else {
                pool[3] = (u32)item;
                item[0] = header | 0x40;
            }
            return (u32)item;
        }
    }
    return 0;
}

typedef struct MantraIconEntry {
    /* 0x0 */ u32 active : 1;
    u32 state : 4;
    u32 leaving : 1;
    u32 shortLoop : 1;
    u32 unk_bits : 25;
    /* 0x4 */ s16 x;
    s16 y;
    /* 0x8 */ s32 timer;
} MantraIconEntry;

s32 mnuUpdateMantraIconList(u8 *list) {
    MantraIconEntry *entry;
    s32 i;

    entry = *(MantraIconEntry **)(list + 4);
    if (entry == 0) {
        return 0;
    }
    for (i = 0; i < *(s32 *)(list + 0x10); i++, entry++) {
        if (!entry->active) {
            continue;
        }
        switch (entry->state) {
        case 1:
        case 5:
            entry->timer += 1;
            if (entry->timer >= 11) {
                entry->timer = 0;
                if (entry->leaving) {
                    entry->state = 3;
                } else {
                    entry->state = 2;
                }
            }
            break;
        case 2:
            entry->timer += 1;
            if (entry->shortLoop) {
                if (entry->timer >= 26) {
                    entry->timer = 0;
                }
            } else if (entry->timer >= 41) {
                entry->timer = 0;
            }
            if (entry->leaving) {
                entry->timer = 0;
                entry->state = 3;
            }
            break;
        case 3:
        case 6:
            entry->timer += 1;
            if (entry->timer >= 6) {
                entry->timer = 0;
                if (entry->state == 6) {
                    entry->state = 9;
                } else {
                    entry->state = 4;
                }
            }
            break;
        case 4:
            entry->active = 0;
            break;
        case 7:
        case 10:
            entry->timer += 1;
            if (entry->timer >= 31) {
                entry->timer = 0;
                if (entry->state == 10) {
                    entry->state = 12;
                } else {
                    entry->state = 9;
                }
            }
            break;
        case 8:
        case 11:
            entry->timer += 1;
            if (entry->timer >= 31) {
                entry->timer = 0;
                entry->state = 2;
            }
            break;
        case 9:
        case 12:
            break;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002758B8);

s32 mnuDrawMantraFadeIcon(s32 x, s32 y, s32 depth, s32 amount, s32 unused, MantraIconEntry *icon) {
    s32 count;
    f32 ratio;

    switch (icon->state) {
    case 1:
    case 5:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        amount = (f32)amount * ratio;
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 2:
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 3:
    case 6:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        ratio = 1.0f - ratio;
        amount = (f32)amount * ratio;
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10B, 0, 0x53);
        break;
    case 4:
    case 7:
    case 8:
    case 9:
        break;
    }
    return 0;
}

s32 mnuDrawMantraFadeIcon2(s32 x, s32 y, s32 depth, s32 amount, s32 unused, MantraIconEntry *icon) {
    s32 count;
    f32 ratio;

    switch (icon->state) {
    case 1:
    case 5:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        amount = (f32)amount * ratio;
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 2:
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 3:
    case 6:
        count = icon->timer;
        if (count < 5) {
            ratio = (f32)count / 5.0f;
        } else {
            ratio = 1.0f;
        }
        ratio = 1.0f - ratio;
        amount = (f32)amount * ratio;
        func_0026E788(x + icon->x, y + icon->y, depth, amount, 0x10C, 0, 0x53);
        break;
    case 4:
    case 7:
    case 8:
    case 9:
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425828);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00275CE8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00277F38);

u32 func_00278D80(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 2, mnuUpdateMantraFadeA, func_00279440,
                         func_00279180, func_002792D8, 0, 0, arg1);
}

void func_00278DC8(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void func_00278DF8(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 2);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void func_00278E50(u32 a, u32 b, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x10);
    }
}

void func_00278EA8(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x18);
    }
}

void func_00278EE0(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x14);
    }
}

void func_00278F18(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x11);
        mnuStorePanelEntry(0x20003, 5);
    }
}

void func_00278F60(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x91);
        mnuStorePanelEntry(0x20003, 5);
    }
}

void func_00278FA8(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x92);
        mnuStorePanelEntry(0x20002, 0);
    }
}

void func_00278FF0(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x12);
    }
}

void func_00279028(u32 a, u32 b, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x20);
    }
}

void func_00279080(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x28);
    }
}

void func_002790B8(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x24);
    }
}

void func_002790F0(u32 a, u32 b, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x21);
    }
}

void func_00279148(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 2);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x22);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279180);

void func_002792D8(s32 obj) {
    MantraFadeData *fade = (MantraFadeData *)((MantraDrawItem *)obj)->data;
    func_00275598((u32 *)fade->iconList);
    func_00328E48(fade);
}

s32 mnuUpdateMantraFadeA(s32 unused, s32 arg1) {
    MantraFadeData *fade = (MantraFadeData *)((MantraDrawItem *)arg1)->data;

    mnuUpdateMantraIconList(fade->iconList);
    switch (fade->state) {
    case 1:
    case 6:
        fade->scale = (f32)fade->elapsed / 20.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 20) {
            fade->state = 2;
            fade->scale = 1.0f;
            fade->elapsed = 0;
        }
        break;
    case 2:
        fade->scale = 1.0f;
        break;
    case 3:
    case 5:
        fade->scale = 1.0f - (f32)fade->elapsed / 10.0f;
        fade->elapsed += 1;
        if (fade->elapsed >= 10) {
            fade->elapsed = 0;
            fade->scale = 0.0f;
            if (fade->state == 5) {
                fade->state = 4;
            } else {
                return 1;
            }
        }
        break;
    case 4:
        fade->scale = 0.0f;
        break;
    }
    return 0;
}

u32 func_00279440(u32 unused, u32 obj) {
    u32 data = (u32)((MantraDrawItem *)obj)->data;
    s32 scale = (s32)(((MantraFadeData *)data)->scale * 128.0f);
    func_002758B8(((MantraFadeData *)data)->x, ((MantraFadeData *)data)->y, 0,
                  scale, *(u32 *)data);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279488);

u32 func_002795E0(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 3, mnuUpdateMantraFadeA, func_00279440,
                         func_00279488, func_002792D8, 0, 0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279628);

u32 func_00279778(u32 arg0, u32 arg1) {
    return func_0026FC88(arg0, 3, mnuUpdateMantraFadeA, func_00279440,
                         func_00279628, func_002792D8, 0, 0, arg1);
}

void func_002797C0(u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        ((MantraFadeData *)item->data)->state = 3;
    }
}

void func_002797F0(s16 x, s16 y, u32 pool) {
    MantraDrawItem *item = (MantraDrawItem *)mnuFindMantraDrawItemByKind(pool, 3);
    if (item != 0) {
        MantraFadeData *fade = (MantraFadeData *)item->data;
        fade->x = x;
        fade->y = y;
    }
}

void func_00279848(u32 a, u32 b, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x50);
    }
}

void func_002798A0(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x58);
    }
}

void func_002798D8(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x54);
    }
}

void func_00279910(u32 a, u32 b, u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(a, b, *(u32 *)*(s32 *)(obj + 0x20), 0x60);
    }
}

void func_00279968(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x68);
    }
}

void func_002799A0(u32 arg0) {
    s32 obj = mnuFindMantraDrawItemByKind(arg0, 3);
    if (obj != 0) {
        func_00275358(0, 0, *(u32 *)*(s32 *)(obj + 0x20), 0x64);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002799D8);

void func_00279C38(u32 *sprite) {
    if (sprite != 0) {
        func_003297C8(*sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279C58);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00279DC8);

typedef struct MantraPanelAnimation {
    u32 flags;
    u8 pad4[8];
    u16 x;
    u16 y;
    u32 visualParameters[3];
    u8 byte1C;
    u8 byte1D;
    u16 frame;
    u8 pad20[0xE];
    s16 transitionDelay;
} MantraPanelAnimation;

void func_00279F30(MantraPanelAnimation *panel, s32 dx, s32 dy, u32 a, u32 b, u32 c, u8 d, u8 e) {
    dx = (s16)dx + panel->x;
    dy = (s16)dy + panel->y;
    panel->x = dx;
    panel->y = dy;
    panel->visualParameters[0] = a;
    panel->visualParameters[1] = b;
    panel->visualParameters[2] = c;
    panel->byte1C = d;
    panel->byte1D = e;
}

void func_00279F70(MantraPanelAnimation *panel) {
    panel->flags = (panel->flags & 0xfffc03ff) | 0x800;
}

typedef struct MantraPanelPool {
    u32 handle;
    MantraPanelAnimation *items;
    s32 count;
} MantraPanelPool;

void func_00279F90(MantraPanelPool *pool, s32 mode) {
    s32 i;
    u32 *item = (u32 *)pool->items;

    for (i = 0; i < pool->count; i++, item += 12) {
        if ((((MantraPanelAnimation *)item)->flags >> 8) & 1) {
            if (((((MantraPanelAnimation *)item)->flags >> 19) & 0xF) == 2) {
                func_0027A798((MantraPanelAnimation *)item, 5, 0);
            } else if (mode == 1) {
                func_0027A798((MantraPanelAnimation *)item, 7, 0);
            } else if (mode == 0) {
                func_0027A798((MantraPanelAnimation *)item, 9, 0);
            } else {
                func_00279F70((MantraPanelAnimation *)item);
            }
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258B8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004258F0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425928);

s32 mnuAdvanceMantraPanelAnim(s32 unused, MantraPanelAnimation *panel) {
    s32 result;

    if (panel->transitionDelay > 0) {
        panel->transitionDelay -= 1;
        if (panel->transitionDelay == 0) {
            panel->frame = 0;
            panel->flags = (panel->flags & 0xFF87FFFF) | (((panel->flags >> 23) & 0xF) << 19);
        }
    }
    result = 0;
    switch ((panel->flags >> 19) & 0xF) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        break;
    case 3:
        break;
    case 4:
        break;
    case 5:
        break;
    case 6:
        panel->frame += 1;
        if ((s16)panel->frame >= 4) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 8:
        panel->frame += 1;
        if ((s16)panel->frame >= 10) {
            panel->frame = 0;
            panel->flags = panel->flags & 0xFF87FFFF;
        }
        break;
    case 7:
        panel->frame += 1;
        if ((s16)panel->frame >= 4) {
            result = 1;
        }
        break;
    case 9:
        panel->frame += 1;
        result = (s16)panel->frame > 9;
        break;
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A198);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A4C0);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A628);

u32 func_0027A798(MantraPanelAnimation *panel, u32 bits, s16 length) {
    u32 flags = (panel->flags & 0xF87FFFFF) | ((bits & 0xF) << 23);
    panel->transitionDelay = length;
    panel->flags = flags;
    if (length == 0) {
        panel->frame = 0;
        panel->flags = (flags & 0xFF87FFFF) | ((bits & 0xF) << 19);
    }
    return 0;
}

void func_0027A7F0(void) {
}

void func_0027A7F8(void) {
}

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425988);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259C0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_004259F8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A30);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027A800);

extern f32 func_00341240(void *state);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378C8);

void func_0027AF40(s32 arg0, u8 *object) {
    u8 table[4] = {0, 20, 40, 60};
    u8 index;

    object[0x20] = 0;
    object[0x21] = 0;
    index = func_00341240(0) * 3.0f;
    object[0x22] = table[index];
}

void func_0027AFD8(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027AFE0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425A98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AB8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425AC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027B678);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378D8);

void func_0027C078(s32 arg0, u8 *object) {
    u8 table[4] = {0, 10, 20, 30};
    u8 index;

    index = func_00341240(0) * 3.0f;
    object[0x20] = table[index];
}

void func_0027C108(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C110);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C2F0);

void func_0027C418(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

void func_0027C448(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

s32 btlDrawPanelA(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x89, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    func_00308380(0x3000DL, packet);
    func_00308DB0(packet);
    func_0026E788(x, y, 0, 0x80, 0x91, 0x60, packet);
    func_00308E60(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027C558);

void func_0027CD78(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(2);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_0027CDB0(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027CDD0);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B78);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425B88);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027D3D8);

void func_0027DE30(void) {
}

void func_0027DE38(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027DE40);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425BC8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E0E0);

void func_0027E208(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(0);
    *(u32 *)(arg1 + 0x24) = temp_v0;
    temp_v0 = func_00284AE0();
    *(u32 *)(arg1 + 0x28) = temp_v0;
}

void func_0027E240(s32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
    func_00284B48(*(u32 **)(obj + 0x28));
}

s32 btlDrawPanelB(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x87, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0xffffff, 0x1000, 0x800, 0, packet);
    func_00308380(0x3000DL, packet);
    func_00308DB0(packet);
    func_0026E788(x, y, 0, 0x80, 0x91, 0x60, packet);
    func_00308E60(packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027E360);

void func_0027FB60(void) {
}

void func_0027FB68(void) {
}

s32 btlDrawPanelC(s32 x, s32 y, u32 a2, u32 a3, u32 a4, u32 a5, u32 packet) {
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    func_00308380(0x5100DL, packet);
    func_0026E788(x, y, 0, a3, 0x77, 0, packet);
    func_0026E788(x, y, 0, a3, 0x9A, 0, packet);
    func_00308380(0x30000, packet);
    func_00308808((x - 0x80) << 4, (y - 0x80) << 3, 0, 0x1000, 0x800, 0, packet);
    func_00308380(0x5100DL, packet);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FC90);

void func_0027FDB8(void) {
}

void func_0027FDC0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_0027FDC8);

void func_00280390(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
    *(u8 *)(arg1 + 0x22) = 0;
}

void func_002803A0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002803A8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425C98);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CA8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CB8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002805E0);

void func_002817B8(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
}

void func_002817C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002817C8);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425CF8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281B60);

void func_00281C88(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002843C0(1);
    *(u32 *)(arg1 + 0x24) = temp_v0;
}

void func_00281CB8(u32 obj) {
    func_002844E8(*(u32 *)(obj + 0x24));
}

u32 func_00281CD8(u32 ctx, u32 x, u32 y, u32 direction, u32 unused,
                  u32 sprite, u32 animation) {
    func_0026E788(ctx, x, y, direction, 0x77, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf7, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf8, 0, animation);
    func_0026E788(ctx, x, y, direction, 0xf9, 0, animation);
    func_00284508(ctx, x, 0, direction, *(u32 *)(sprite + 0x24), animation);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00281DC0);

void func_00282B50(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_00282B60(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00282B68);

INCLUDE_RODATA(const s32, "game/code_0026DBF8", D_00425D68);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283090);

void func_00283F90(u32 arg0, s32 arg1) {
    *(u8 *)(arg1 + 0x20) = 0;
    *(u8 *)(arg1 + 0x21) = 0;
}

void func_00283FA0(void) {
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00283FA8);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284298);
INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002843C0);


void func_002844E8(u32 sprite) {
    if (sprite != 0) {
        func_00328E48(sprite);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284508);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284818);

u32 func_00284AE0(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x10;
    *(u32 *)(block + 8) = 0x64;
    return block;
}

void func_00284B48(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284B70);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00284F00);

u32 func_002850B8(void) {
    u32 handle = func_003292A8(0x650);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, 0x650);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x10;
    *(u32 *)(block + 8) = 0x64;
    return block;
}

void func_00285120(u32 *obj) {
    if (*obj != 0) {
        func_003297C8(*obj);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285148);

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285500);

u32 mnuRequestEffectResource(u32 context, u32 config) {
    MantraEffectResource *resource = (MantraEffectResource *)func_00328D68(sizeof(MantraEffectResource));
    memset(resource, 0, sizeof(MantraEffectResource));
    effRequestResourceByMode(context, config, 0, &resource->handle);
    return (u32)resource;
}

u8 mnuHasEffectResourceHandle(MantraEffectResource *resource) {
    return resource->handle != 0;
}

void mnuReleaseEffectResource(MantraEffectResource *resource) {
    func_003054E8(resource->handle);
    func_00328E48(resource);
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285788);

void func_00285800(s32 *state) {
    u8 *particle;
    s32 value;
    s32 i;

    state[0] = state[0] - 1;
    if (state[0] < 0) {
        value = func_00341240(0) * 60.0f + 60.0f;
        state[1] = value;
        state[0] = value;
        *(f32 *)(state + 2) = func_00341240(0) * 0.20000005f + 0.4f;
    }
    particle = (u8 *)state + 0xC;
    for (i = 7; i >= 0; i--) {
        func_002858A0(particle);
        particle += 0xC;
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_002858A0);


void func_00285A60(u8 *object) {
    if (*(s8 *)(object + 0xA) != 0) {
        func_003406A0((1.0f - (f32)*(s16 *)(object + 4) / (f32)*(s16 *)(object + 6)) * 3.14159265f);
    }
}

INCLUDE_ASM(const s32, "game/code_0026DBF8", func_00285AC0);

u32 func_00285BD8(u32 count) {
    u32 size = count * 12 + 0x28;
    u32 handle = func_003292A8(size);
    u32 block = sdfMemoryGetBlockAddress(handle);
    memset((void *)block, 0, size);
    *(u32 *)block = handle;
    *(u32 *)(block + 4) = block + 0x28;
    *(u32 *)(block + 8) = count;
    *(u16 *)(block + 0x16) = 10;
    *(u16 *)(block + 0x18) = 1;
    *(u16 *)(block + 0x1a) = 60;
    *(u8 *)(block + 0x22) = 5;
    *(u16 *)(block + 0x20) = 5;
    *(u16 *)(block + 0x10) = 0x200;
    *(u16 *)(block + 0x12) = 0x1c0;
    *(u16 *)(block + 0x1c) = 13;
    *(u16 *)(block + 0x1e) = 9;
    *(void (**)(void))(block + 0x24) = func_00286270;
    *(u16 *)(block + 0xc) = 0;
    *(u16 *)(block + 0xe) = 0;
    return block;
}

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378E8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F0);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_004378F8);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437900);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437908);

INCLUDE_SDATA(const s32, "game/code_0026DBF8", D_00437910);

