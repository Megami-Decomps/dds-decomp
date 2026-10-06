#include "common.h"
#include "dat_state.h"
extern s32 sdfAllocGeneralBlock(s32 size);
extern s8 *sdfMemoryGetBlockAddress();


extern void evtPrintDeveloperConsoleMessage();

extern void mdlFlagClear(s32 flag);

extern void mdlFlagSet(s32 flag);

extern s32 mdlFlagTest(s32 flag);

extern void mnuClearCampResourceFlagEntries(void);

extern void mnuSynchronizeMantraModelFlags(s32 arg);

extern void func_00286BA8(void *record);

extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);




typedef struct MtrCell {
    s16 x;
    s16 y;
    s16 unk04;
    s16 unk06;
    s8 direction;
    s8 state;
    s8 remainingDepth;
    u8 unk0B;
} MtrCell;

/* Native cell storage and callback; geometry is shared with the draw callback. */
typedef struct MtrGrid {
    u8 pad00[4];
    MtrCell *cells;
    s32 count;
    u8 pad0C[0x10];
    s16 unk1C;
    s16 unk1E;
    u8 pad20[4];
    void (*draw)(s32, s32, s32, s32, struct MtrGrid *, s32, s32);
} MtrGrid;

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285CE8);
extern f32 sdfSinPoly(f32);

void func_00285D78(s32 x, s32 y, s32 z, s32 alpha, MtrGrid *grid, s32 flags, s32 context) {
    MtrCell *cell = grid->cells;
    s32 i;

    for (i = 0; i < grid->count; i++, cell++) {
        if (cell->state != 0) {
            f32 fade = sdfSinPoly((f32)cell->unk04 / (f32)cell->unk06 * 3.141592503f);
            grid->draw(x + cell->x, y + cell->y, z, (s32)((f32)alpha * fade), grid, flags, context);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285E98);

extern MtrCell *func_00285E98(MtrGrid *, MtrCell *, s8);
extern f32 effMiscRandUnitFloat(void *);

void func_002860D8(MtrGrid *grid, MtrCell *source, s8 mode) {
    MtrCell *cell;
    f32 random;
    s32 i;

    switch (mode) {
    case 1:
        cell = func_00285E98(grid, NULL, 0);
        if (cell != NULL) {
            cell->state = 4;
        }
        break;
    case 2:
        random = effMiscRandUnitFloat(0);
        if (random < 0.8f) {
            cell = func_00285E98(grid, source, 0);
            if (cell == NULL) {
                return;
            }
            cell->state = mode;
        }
        if (random < 0.9f) {
            cell = func_00285E98(grid, source, 1);
            if (cell != NULL) {
                cell->state = 3;
            }
        }
        break;
    case 3:
        if (effMiscRandUnitFloat(0) < 0.8f) {
            cell = func_00285E98(grid, source, 0);
            if (cell != NULL) {
                cell->state = mode;
            }
        }
        break;
    case 4:
        for (i = 0; i < 6; i++) {
            cell = func_00285E98(grid, source, i);
            if (cell == NULL) {
                break;
            }
            cell->state = 2;
        }
        break;
    }
}

void mnuDrawCellScaledGrid(s32 arg0, s32 arg1, s32 arg2, s32 arg3, MtrGrid *arg4, s32 arg5, s32 arg6) {
    uiDrawUniformColorRect(arg0 << 4, arg1 << 3, arg2, arg4->unk1C << 4, arg4->unk1E << 3, arg3 | 0x80808000, arg6);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002862B0);

INCLUDE_RODATA(const s32, "game/code_00285CE8", D_00425DD8);

s32 mtrMantraEventBitPush(void) {
    s8 *data;
    s32 handle;
    s32 i;

    handle = sdfAllocGeneralBlock(0x76);
    data = sdfMemoryGetBlockAddress(handle);
    memset(data, 0, 0x76);
    for (i = 0; i < 0x70; i++) {
        if (mdlFlagTest(i + 0x920)) {
            *data = 1;
        }
        data++;
    }
    for (i = 0; i < 6; i++) {
        if (mdlFlagTest(i + 0x9A0)) {
            *data = 1;
        }
        data++;
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitPush()]*****************\n");
    return handle;
}

extern s8 *sdfMemoryGetBlockAddress(void);
extern void sdfReleaseResourceAllocation(s32 arg);

void mtrMantraEventBitPop(s32 arg) {
    s32 i;
    s8 *data = sdfMemoryGetBlockAddress();

    for (i = 0; i < 0x70; i++) {
        if (*data != 0) {
            mdlFlagSet(i + 0x920);
        } else {
            mdlFlagClear(i + 0x920);
        }
        data++;
    }
    for (i = 0; i < 6; i++) {
        if (*data != 0) {
            mdlFlagSet(i + 0x9A0);
        } else {
            mdlFlagClear(i + 0x9A0);
        }
        data++;
    }
    sdfReleaseResourceAllocation(arg);
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitPop()]*****************\n");
}

extern void scrSetEntryLowFlags(DatPartyRecord *context, u16 entryId, u16 lowFlags);
void mtrMantraBitResetUnit(DatPartyRecord *record) {
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        scrSetEntryLowFlags(record, i, 0);
    }
    evtPrintDeveloperConsoleMessage(
        "*****************[mtrMantraBitReset_Unit():[0x%x]]*****************\n", record->unitId);
}

void mtrMantraBitReset(void) {
    s32 i;

    for (i = 0; i < 32; i++) {
        if (datGameState->party[i].flags & 1) {
            mtrMantraBitResetUnit(&datGameState->party[i]);
        }
    }
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraBitReset()]*****************\n");
}

void mtrMantraEventBitReset(void) {
    s32 i;

    for (i = 0; i < 0x70; i++) {
        mdlFlagClear(i + 0x920);
    }
    for (i = 0; i < 6; i++) {
        mdlFlagClear(i + 0x9A0);
    }
    mnuClearCampResourceFlagEntries();
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitReset()]*****************\n");
}

void func_00286618(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 2;
    func_00286BA8(&record);
    mnuSynchronizeMantraModelFlags(0);
    if (mdlFlagTest(0x9A0)) {
        mdlFlagSet(0x9A1);
    }
}

void func_00286670(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 1;
    func_00286BA8(&record);
    mnuSynchronizeMantraModelFlags(1);
    if (mdlFlagTest(0x9A1)) {
        mdlFlagSet(0x9A0);
    }
}

void func_002866C8(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 3;
    func_00286BA8(&record);
}

void func_00286700(void) {
    DatPartyRecord record;

    memset(&record, 0, sizeof(record));
    record.unitId = 7;
    func_00286BA8(&record);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286738);
