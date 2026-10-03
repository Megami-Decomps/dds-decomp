#include "common.h"
#include "fpu.h"

/* Sliding menu bar: direction flag and 0..max position */
typedef struct { s32 active; s32 pos; } SlideBar;

extern s32 mnuGetSlidePathSegmentWeight(s32);
extern s32 D_003E38A0[];

extern s32 mnuMovieMenuState;

extern void mnuReleaseMenuResourceSlots(void);

extern void mnuDestroyMovieMenuSelectionList(void);

extern s16 D_003E3792[];

typedef struct { s32 unk0; s32 gfx[1]; } SprWork;

/* View of the sprite work area used by the list and handle helpers. */
typedef struct SpriteMenuState {
    u8 pad00[8];
    s32 spriteHandle; /* 0x08 */
    u8 pad0C[0x18];
    s32 list;         /* 0x24 */
} SpriteMenuState;

typedef struct SpriteMenuList {
    u8 pad00[0x1C];
    u32 *selected;    /* 0x1C */
    u8 pad20[0xC];
    void (*callback)(void); /* 0x2C */
} SpriteMenuList;

/* Sprite rows: graphic slot, draw parameter, x offset, y offset. */
extern s16 D_003E3790[][4];

s32 mnuSpriteGraphicHandle(u32 index) {
    SprWork *work = (SprWork *)mnuMovieMenuState;
    return work->gfx[D_003E3790[index][0]];
}

s16 mnuSpriteDrawParam(u32 index) {
    return D_003E3792[index * 4];
}

extern void func_00306CD0(s32, s32, s32, u32, s32, s32, s32, s32);

void mnuDrawSprite(s32 x, s32 y, s32 depth, s32 alpha, s32 drawMode,
                   s32 spriteIndex, s32 drawContext) {
    func_00306CD0((x + D_003E3790[spriteIndex][2]) << 4,
                  (y + D_003E3790[spriteIndex][3]) << 3,
                  depth,
                  (u32)((f32)alpha * 256.0f * 0.0078125f),
                  drawMode,
                  ((SprWork *)mnuMovieMenuState)
                      ->gfx[D_003E3790[spriteIndex][0]],
                  D_003E3790[spriteIndex][1],
                  drawContext);
}

extern s32 mnuCreateListState(s32, s32, s32);

extern void mnuDestroyListState(s32);

extern void mnuListAppendNode(s32, s32);

extern void func_002A5A78();

void mnuRecreateMenuSelectionList(void) {
    s32 i;
    s32 node;
    if (((SpriteMenuState *)mnuMovieMenuState)->list != 0) {
        mnuDestroyListState(((SpriteMenuState *)mnuMovieMenuState)->list);
    }
    node = mnuCreateListState(0, 3, 0);
    ((SpriteMenuState *)mnuMovieMenuState)->list = node;
    ((SpriteMenuList *)node)->callback = func_002A5A78;
    for (i = 0; i < 3; i++) {
        mnuListAppendNode(((SpriteMenuState *)mnuMovieMenuState)->list, 0);
    }
}

void mnuDestroyMovieMenuSelectionList(void) {
    mnuDestroyListState(((SpriteMenuState *)mnuMovieMenuState)->list);
}

u32 func_002A3C78(void) {
    return *((SpriteMenuList *)((SpriteMenuState *)mnuMovieMenuState)->list)->selected;
}

void mnuSelectMenuListCursorByAdvance(s32 advanceCount) {
    mnuSelectFirstListNode(((SpriteMenuState *)mnuMovieMenuState)->list);
    if (0 < advanceCount) {
        do {
            advanceCount = advanceCount - 1;
            mnuAdvanceListCursorDefault(((SpriteMenuState *)mnuMovieMenuState)->list);
        } while (advanceCount != 0);
    }
}

extern s8 D_0037F510[];

s32 mnuIsAnyMenuInputPressed(void) {
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
extern u32 effLoadIndexedResource(const char *path, const char *name, u32 mode);
extern void effRequestResourceByMode(const char *path, const char *name, u32 mode, u32 *handle);


void func_002A3D70(void) {
    u32 *state = (u32 *)mnuMovieMenuState;
    u32 resource;

    if (state[1] == 0) {
        resource = effLoadIndexedResource((const char *)D_00437A48,
            (const char *)D_003E3760, 0);
        state = (u32 *)mnuMovieMenuState;
        state[1] = resource;
    }
    if (state[3] == 0) {
        const char *name = (const char *)D_003E3760;
        name += 0x20;
        resource = effLoadIndexedResource((const char *)D_00437A48, name, 0);
        state = (u32 *)mnuMovieMenuState;
        state[3] = resource;
    }
}

void mnuReleaseMenuResourceSlots(void) {
    u32 *state = (u32 *)mnuMovieMenuState;
    if (state[1] != 0) {
        effDestroyResourceSlotSet(state[1]);
        state = (u32 *)mnuMovieMenuState;
        state[1] = 0;
    }
    if (state[3] != 0) {
        effDestroyResourceSlotSet(state[3]);
        state = (u32 *)mnuMovieMenuState;
        state[3] = 0;
    }
}

void func_002A3E38(s32 mode) {
    u32 *handle = (u32 *)&((SpriteMenuState *)mnuMovieMenuState)->spriteHandle;

    if (*handle == 0) {
        if (mode != 0) {
            const char *name = (const char *)D_003E3760;
            name += 0x10;
            ((SpriteMenuState *)mnuMovieMenuState)->spriteHandle =
                effLoadIndexedResource((const char *)D_00437A48, name, 0);
        } else {
            const char *name = (const char *)D_003E3760;
            name += 0x10;
            effRequestResourceByMode((const char *)D_00437A48, name, 0,
                                     handle);
        }
    }
}

u8 mnuHasSpriteHandle(void) {
    return ((SpriteMenuState *)mnuMovieMenuState)->spriteHandle != 0;
}

void mnuReleaseSpriteHandle(void) {
    if (((SpriteMenuState *)mnuMovieMenuState)->spriteHandle != 0) {
        effDestroyResourceSlotSet(((SpriteMenuState *)mnuMovieMenuState)->spriteHandle);
        ((SpriteMenuState *)mnuMovieMenuState)->spriteHandle = 0;
    }
}

void mnuStartMovieMenuSfx16(s32 work) {
    mnuDrawSprite(0, 0, 0, work, 0, 0x1F, 0x53);
}

void mnuStartMovieMenuSfx17(u32 work) {
    mnuDrawSprite(0, 0, 0, work, 0, 0x20, 0x53);
}

void mnuStartMovieMenuSfx18(u32 work) {
    mnuDrawSprite(0, 0, 0, work, 0, 0x21, 0x53);
}

void mnuSlideBarSetState(u32 *work, s32 state) {
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

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

void mnuDrawAndAdvanceMovieMenuBar(SlideBar *bar, s32 value) {
    s32 offset;

    if (bar->active != 0 || bar->pos != 0) {
        offset = bar->pos / 4;
        mnuDrawSprite(0, 0, 0, offset, 0, 0, value);
        mnuDrawSprite(0, 0, 0, offset, 0, 3, value);
        if (bar->active != 0) {
            bar->pos += 27;
        } else {
            bar->pos -= 27;
        }
        if (bar->pos < 0) {
            bar->pos = 0;
        }
        if (bar->pos > 512) {
            bar->pos = 512;
        }
    }
}

void mnuSlideBarSetStateB(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[1] = 0; mode = 0; break;
    case 3: mode = 1; state[1] = 0x200; break;
    }
    state[0] = mode;
}


void mnuAdvanceSlideBarValue(SlideBar *bar, s32 value) {
    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    mnuDrawSprite(0, 0, 0, bar->pos / 4, 0, 2, value);
    if (bar->active != 0) {
        bar->pos += 0x20;
    } else {
        bar->pos -= 0x20;
    }
    if (bar->pos < 0) {
        bar->pos = 0;
    }
    if (bar->pos > 0x200) {
        bar->pos = 0x200;
    }
}

void mnuSlideBarSetStateSmall(u32 *state, u32 mode) {
    switch (mode) {
    case 2: state[2] = 0; mode = 0; break;
    case 3: mode = 1; state[2] = 0x80; break;
    }
    state[0] = mode;
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4208);

void func_002A4380(s32 bar) {
    *(u8 *)(bar + 8) = 0;
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

void mnuTitlePickRandomSlot(list)
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

void mnuPairedSlideBarSetState(u32 *work, s32 state) {
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

s32 mnuGetSlidePathSegmentWeight(s32 segment) {
    s32 x0 = D_003E38A0[segment + 8];
    s32 x1 = D_003E38A0[segment + 9];
    s32 y0 = D_003E38A0[segment + 12];
    s32 y1 = D_003E38A0[segment + 13];
    s32 dx = x1 - x0;
    s32 dy = y1 - y0;

    return (s32)fsqrtf((f32)(dx * dx + dy + dy));
}

s32 mnuGetSlidePathTotalWeight(void) {
    s32 index;
    s32 next;
    s32 total;

    total = 0;
    index = 0;
    do {
        next = index + 1;
        index = mnuGetSlidePathSegmentWeight(index);
        total = total + index;
        index = next;
    } while (next < 3);
    return total;
}

s32 mnuGetSlidePathSegmentStart(s32 segment) {
    s32 i = 0;
    s32 total = 0;
    s32 duration = mnuGetSlidePathTotalWeight();

    while (i < segment) {
        total += mnuGetSlidePathSegmentWeight(i++);
    }
    return (total << 12) / duration;
}

s32 mnuFindSlidePathSegmentAtPosition(s32 position) {
    s32 segment = 0;
    s32 total = 0;
    s32 duration = mnuGetSlidePathTotalWeight();

    while (segment < 3) {
        total += mnuGetSlidePathSegmentWeight(segment);
        if (position < (total << 12) / duration) {
            return segment;
        }
        segment++;
    }
    return 3;
}

s32 mnuGetSlidePathSegmentEnd(s32 segment) {
    s32 i = 0;
    s32 total = 0;
    s32 duration = mnuGetSlidePathTotalWeight();

    while (i < segment + 1) {
        total += mnuGetSlidePathSegmentWeight(i++);
    }
    return (total << 12) / duration;
}

void mnuSlidePathPoint(s32 position, s32 *outX, s32 *outY) {
    s32 segment = mnuFindSlidePathSegmentAtPosition(position);
    s32 start = mnuGetSlidePathSegmentStart(segment);
    s32 end = mnuGetSlidePathSegmentEnd(segment);
    /* Consecutive table words give each segment's start/end X at +8/+9
       and start/end Y at +12/+13; the last X shares the first Y word. */
    s32 x0 = D_003E38A0[segment + 8];
    s32 y0 = D_003E38A0[segment + 12];
    s32 dx = D_003E38A0[segment + 9] - x0;
    s32 dy = D_003E38A0[segment + 13] - y0;
    s32 offset = position - start;
    s32 span = end - start;

    *outX = x0 + dx * offset / span;
    *outY = y0 + dy * offset / span;
}

typedef struct MovieMenuEffectSlot {
    s32 active;
    s32 counter;
} MovieMenuEffectSlot;

typedef struct MovieMenuEffectState {
    u8 pad00[0x74];
    MovieMenuEffectSlot slots[4];
} MovieMenuEffectState;

void func_002A49C0(MovieMenuEffectState *work) {
    s32 selected = -1;
    s32 i;

    if (effMiscRand(0) % 60u == 0) {
        for (i = 0; i < 4; i++) {
            if (work->slots[i].active == 0) {
                selected = i;
                break;
            }
        }
        if (selected >= 0) {
            work->slots[selected].active = 1;
            work->slots[selected].counter = 0;
        }
    }
}

extern s16 D_003E38E0[][2];

void func_002A4A68(u8 *menu, s32 drawContext) {
    s32 i;
    s32 off = 0x70;
    u8 *flags = menu + 4;
    u8 *positions = menu + 8;

    for (i = 0; i < 4; i++, off += 8) {
        if (*(s32 *)(flags + off) != 0) {
            s32 pos = *(s32 *)(positions + off);

            if (pos > 0x100) {
                pos = 0x200 - pos;
            }
            mnuDrawSprite(D_003E38E0[i][0], D_003E38E0[i][1], 0, pos / 2, 0, 0x1E, drawContext);
            if (*(s32 *)(positions + off) == 0x200) {
                *(s32 *)(flags + off) = 0;
            } else {
                *(s32 *)(positions + off) += 4;
                if (*(s32 *)(positions + off) > 0x200) {
                    *(s32 *)(positions + off) = 0x200;
                }
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002A3AE8", func_002A4B70);

extern void mnuTitlePickRandomSlot();

extern void func_002A44C0(SlideBar *, s32);

extern void func_002A4B70(SlideBar *, s32);

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceSlideBar(SlideBar *bar, s32 drawContext) {
    if (bar->active != 0 || bar->pos != 0) {
        mnuTitlePickRandomSlot();
        func_002A44C0(bar, drawContext);
        mnuDrawSprite(0, 0, 0, bar->pos, 0, 0x16, drawContext);
        func_002A4B70(bar, drawContext);
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

void mnuTimedSlideBarSetState(SlideBarTimed *bar, s32 mode, s32 timer) {
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

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

void mnuAdvanceTimedSlideBar(SlideBarTimed *bar, s32 drawContext) {
    if (bar->timer > 0) {
        bar->timer--;
        if (bar->timer == 0) {
            mnuTimedSlideBarSetState(bar, bar->id, 0);
        }
    }
    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    mnuDrawSprite(0, 0, 0, bar->pos / 4, 0, 0x12, drawContext);
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

void mnuAdvanceMultiSpriteSlideBar(SlideBar *bar, s32 drawContext) {
    s32 half;

    if (bar->active == 0 && bar->pos == 0) {
        return;
    }
    half = bar->pos / 4;
    mnuDrawSprite(0, 0, 0, half, 0, 0x13, drawContext);
    mnuDrawSprite(0, 0, 0, half, 0, 0x1A, drawContext);
    mnuDrawSprite(0, 0, 0, half, 0, 0x1B, drawContext);
    mnuDrawSprite(0, 0, 0, half, 0, 0x1C, drawContext);
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

void func_002A50E8(s32 buffer, s32 index, u8 value) {
    *(u8 *)(buffer + index) = value;
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

void mnuAdvancePairedSlideBars(SlideBarPair *bars, s32 drawContext) {
    u32 i;

    for (i = 0; i < 2; i++) {
        s32 pos = bars->pos[i];

        switch (i) {
        case 0:
            mnuDrawSprite(0, 0, 0, pos, 0, 0xF, drawContext);
            mnuDrawSprite(0, 0, 0, pos, 0, 0x10, drawContext);
            break;
        case 1:
            mnuDrawSprite(0, 0, 0, pos, 0, 0x11, drawContext);
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
