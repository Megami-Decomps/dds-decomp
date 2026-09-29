#include "common.h"

/* Sliding menu bar: direction flag and 0..max position */
typedef struct { s32 active; s32 pos; } SlideBar;

extern s32 func_002A46C8(s32);
extern s32 D_003E38A0[];

extern s32 D_00437A40;

extern void func_002A3DE8(void);

extern void func_002A3C58(void);

extern s16 D_003E3792[];

/* Sprite table entry: graphic slot in the work area, draw parameter, offsets */
typedef struct { s16 gfx; s16 unk2; s16 x; s16 y; } SprEntry;
typedef struct { s32 unk0; s32 gfx[1]; } SprWork;

extern SprEntry D_003E3790[];

s32 func_002A3AE8(u32 index) {
    SprWork *work = (SprWork *)D_00437A40;
    return work->gfx[D_003E3790[index].gfx];
}

s16 func_002A3B10(u32 index) {
    return D_003E3792[index * 4];
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A3B28);

extern s32 func_002B8158(s32, s32, s32);

extern void func_002B81C8(s32);

extern void mnuListAppendNode(s32, s32);

extern void func_002A5A78();

void func_002A3BE0(void) {
    s32 i;
    s32 node;
    if (*(s32 *)(D_00437A40 + 0x24) != 0) {
        func_002B81C8(*(s32 *)(D_00437A40 + 0x24));
    }
    node = func_002B8158(0, 3, 0);
    *(s32 *)(D_00437A40 + 0x24) = node;
    *(void **)(node + 0x2C) = func_002A5A78;
    for (i = 0; i < 3; i++) {
        mnuListAppendNode(*(s32 *)(D_00437A40 + 0x24), 0);
    }
}

void func_002A3C58(void) {
    func_002B81C8(*(s32 *)(D_00437A40 + 0x24));
}

u32 func_002A3C78(void) {
    return **(u32 **)(*(s32 *)(D_00437A40 + 0x24) + 0x1c);
}

void func_002A3C90(s32 arg0) {
    func_002B8968(*(u32 *)(D_00437A40 + 0x24));
    if (0 < arg0) {
        do {
            arg0 = arg0 - 1;
            func_002B8CF0(*(u32 *)(D_00437A40 + 0x24));
        } while (arg0 != 0);
    }
}

extern s8 D_0037F510[];

s32 func_002A3CE0(void) {
    if (D_0037F510[0x21] < 0 || D_0037F510[0x23] < 0 ||
        D_0037F510[0x22] < 0 || D_0037F510[0x20] < 0 ||
        D_0037F510[0x2a] < 0 || D_0037F510[0x2b] < 0 ||
        D_0037F510[0x28] < 0 || D_0037F510[0x29] < 0 ||
        D_0037F510[0x2d] < 0 || D_0037F510[0x2c] < 0) {
        return 1;
    }
    return 0;
}

extern u8 D_00437A48[];

extern u8 D_003E3760[];

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A3D70);

void func_002A3DE8(void) {
    u32 *state = (u32 *)D_00437A40;
    if (state[1] != 0) {
        func_003054E8(state[1]);
        state = (u32 *)D_00437A40;
        state[1] = 0;
    }
    if (state[3] != 0) {
        func_003054E8(state[3]);
        state = (u32 *)D_00437A40;
        state[3] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A3E38);

u8 mnuHasSpriteHandle(void) {
    return *(s32 *)(D_00437A40 + 8) != 0;
}

void mnuReleaseSpriteHandle(void) {
    if (*(s32 *)(D_00437A40 + 8) != 0) {
        func_003054E8(*(s32 *)(D_00437A40 + 8));
        *(u32 *)(D_00437A40 + 8) = 0;
    }
}

s64 mnuStartMovieMenuSfx16(s32 arg0) {
    return func_002A3B28(0, 0, 0, arg0, 0, 0x1F, 0x53);
}

void func_002A3F28(u32 work) {
    func_002A3B28(0, 0, 0, work, 0, 0x20, 0x53);
}

void func_002A3F60(u32 work) {
    func_002A3B28(0, 0, 0, work, 0, 0x21, 0x53);
}

void func_002A3F98(u32 *work, s32 state) {
    switch (state) {
    case 2:
        work[1] = 0;
        state = 0;
        break;
    case 3:
        work[1] = 0x200;
        state = 1;
        break;
    default:
        break;
    }
    work[0] = state;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A3FE0);

void func_002A40C8(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4110);

void func_002A41C0(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[2] = 0; mode = 0; break;
    case 3: mode = 1; state[2] = 0x80; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4208);

void func_002A4380(s32 arg0) {
    *(u8 *)(arg0 + 8) = 0;
}

typedef struct PickEntry {
    s8 id;
    u8 unk1;
    u8 unk2;
} PickEntry;

typedef struct PickList {
    u8 unk0[8];
    u8 count;
    PickEntry entry[32];
} PickList;

void titlePickRandomSlot(list)
    PickList *list;
{
    u32 range = 0x20;
    s32 tries;
    tries = 0;
    do {
        u32 id;
        s32 i;
        s32 found;
        s32 n = list->count;
        if (n >= 0x20) {
            return;
        }
        id = effMiscRand(0) % range;
        found = 0;
        for (i = 0; i < list->count; i++) {
            if (list->entry[i].id == id) {
                found = 1;
                break;
            }
        }
        if (found == 0) {
            list->entry[list->count].id = id;
            list->entry[list->count].unk1 = 0;
            list->entry[list->count].unk2 = 10;
            list->count++;
        }
        tries++;
    } while (tries <= 0);
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A44C0);

void func_002A4670(u32 *work, s32 state) {
    switch (state) {
    case 2:
        work[1] = 0;
        work[0x1c] = 0;
        work[0x1b] = 0;
        state = 0;
        break;
    case 3:
        work[1] = 0x80;
        work[0x1b] = 0;
        work[0x1c] = 0x80;
        state = 1;
        break;
    default:
        break;
    }
    work[0] = state;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A46C8);

s32 func_002A4728(void) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    temp_v0 = 0;
    do {
        temp_v1 = temp_v0 + 1;
        temp_v0 = func_002A46C8(temp_v0);
        temp_v2 = temp_v2 + temp_v0;
        temp_v0 = temp_v1;
    } while (temp_v1 < 3);
    return temp_v2;
}

s32 func_002A4770(s32 segment) {
    s32 i = 0;
    s32 total = 0;
    s32 duration = func_002A4728();

    while (i < segment) {
        total += func_002A46C8(i++);
    }
    return (total << 12) / duration;
}

s32 func_002A47E8(s32 position) {
    s32 segment = 0;
    s32 total = 0;
    s32 duration = func_002A4728();

    while (segment < 3) {
        total += func_002A46C8(segment);
        if (position < (total << 12) / duration) {
            return segment;
        }
        segment++;
    }
    return 3;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4870);

void func_002A48F0(s32 position, s32 *outX, s32 *outY) {
    s32 segment = func_002A47E8(position);
    s32 start = func_002A4770(segment);
    s32 end = func_002A4870(segment);
    s32 x0 = D_003E38A0[segment + 8];
    s32 y0 = D_003E38A0[segment + 12];
    s32 dx = D_003E38A0[segment + 9] - x0;
    s32 dy = D_003E38A0[segment + 13] - y0;
    s32 offset = position - start;
    s32 span = end - start;

    *outX = x0 + dx * offset / span;
    *outY = y0 + dy * offset / span;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A49C0);

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4A68);

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4B70);

extern void titlePickRandomSlot();

extern void func_002A44C0(SlideBar *, s32);

extern void func_002A4B70(SlideBar *, s32);

extern void func_002A3B28(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSlideBar(SlideBar *bar, s32 arg1) {
    if (bar->active != 0 || bar->pos != 0) {
        titlePickRandomSlot();
        func_002A44C0(bar, arg1);
        func_002A3B28(0, 0, 0, bar->pos, 0, 0x16, arg1);
        func_002A4B70(bar, arg1);
        if (bar->active != 0) {
            bar->pos += 8;
        } else {
            bar->pos -= 8;
        }
        if (bar->pos < 0) {
            bar->pos = 0;
        }
        if (bar->pos > 0x80) {
            bar->pos = 0x80;
        }
    }
}

typedef struct { s32 active; s32 pos; s32 id; s32 timer; } SlideBarTimed;

void func_002A4DF0(SlideBarTimed *bar, s32 mode, s32 timer) {
    bar->timer = timer;
    if (timer > 0) {
        bar->id = mode;
        return;
    }
    switch (mode) {
    case 2: bar->pos = 0; mode = 0; break;
    case 3: mode = 1; bar->pos = 0x200; break;
    }
    bar->active = mode;
}

extern void func_002A3B28(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceTimedSlideBar(SlideBarTimed *bar, s32 arg1) {
    if (bar->timer > 0) {
        bar->timer--;
        if (bar->timer == 0) {
            func_002A4DF0(bar, bar->id, 0);
        }
    }
    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    func_002A3B28(0, 0, 0, bar->pos / 4, 0, 0x12, arg1);
    if (bar->active != 0) {
        bar->pos += 0x10;
    } else {
        bar->pos -= 8;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
}

void mnuAdvanceMultiSpriteSlideBar(SlideBar *bar, s32 arg1) {
    s32 half;

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    half = bar->pos / 4;
    func_002A3B28(0, 0, 0, half, 0, 0x13, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1A, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1B, arg1);
    func_002A3B28(0, 0, 0, half, 0, 0x1C, arg1);
    if (bar->active != 0) {
        bar->pos += 0x1B;
    } else {
        bar->pos -= 0x1B;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A5040);

void func_002A50E8(s32 arg0, s32 arg1, u8 arg2) {
    *(u8 *)(arg0 + arg1) = arg2;
}

void func_002A50F8(u8 *work) {
    u32 i;
    for (i = 0; i < 2; i++) {
        work[i] = 0;
        *(u32 *)(work + 4 + i * 4) = 0;
    }
}

typedef struct {
    u8 active[2];
    u8 pad2[2];
    s32 pos[2];
} SlideBarPair;

void mnuAdvancePairedSlideBars(SlideBarPair *bars, s32 arg1) {
    u32 i;

    for (i = 0; i < 2; i++) {
        s32 pos = bars->pos[i];

        switch (i) {
        case 0:
            func_002A3B28(0, 0, 0, pos, 0, 0xF, arg1);
            func_002A3B28(0, 0, 0, pos, 0, 0x10, arg1);
            break;
        case 1:
            func_002A3B28(0, 0, 0, pos, 0, 0x11, arg1);
            break;
        }
        if (bars->active[i] == 0) {
            bars->pos[i] -= 8;
        } else {
            bars->pos[i] += 8;
        }
        if (bars->pos[i] < 0) {
            bars->pos[i] = 0;
        }
        if (bars->pos[i] > 0x80) {
            bars->pos[i] = 0x80;
        }
    }
}
