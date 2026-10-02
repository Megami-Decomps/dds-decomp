#include "common.h"
extern s32 func_003292A8(s32 size);
extern s8 *sdfMemoryGetBlockAddress();


extern void evtPrintDeveloperConsoleMessage();

extern void mdlFlagClear(s32 flag);

extern void mdlFlagSet(s32 flag);

extern s32 mdlFlagTest(s32 flag);

extern void mnuClearCampResourceFlagEntries(void);

extern void func_00290E48(s32 arg);

extern void func_00286BA8(void *record);

extern void func_00308808(s32, s32, s32, s32, s32, s32, s32);

/* Mantra record zeroed before each update call (0x1C4). */
typedef struct {
    u16 flags;      /* 0x0: low bit marks an active unit */
    u16 pad2;       /* 0x2 */
    u16 unk4;       /* 0x4 */
    u8 pad6[0x1BE]; /* 0x6 */
} MtrRecord;

/* Sprite grid geometry: cell counts at +0x1C/+0x1E. */
typedef struct {
    u8 pad0[0x1C]; /* 0x0 */
    s16 unk1C;     /* 0x1C */
    s16 unk1E;     /* 0x1E */
} MtrGrid;


INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285CE8);
INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285D78);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00285E98);

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002860D8);

void mnuDrawCellScaledGrid(s32 arg0, s32 arg1, s32 arg2, s32 arg3, MtrGrid *arg4, s32 arg5, s32 arg6) {
    func_00308808(arg0 << 4, arg1 << 3, arg2, arg4->unk1C << 4, arg4->unk1E << 3, arg3 | 0x80808000, arg6);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_002862B0);

INCLUDE_RODATA(const s32, "game/code_00285CE8", D_00425DD8);

s32 mtrMantraEventBitPush(void) {
    s8 *data;
    s32 handle;
    s32 i;

    handle = func_003292A8(0x76);
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
extern void func_003297C8(s32 arg);

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
    func_003297C8(arg);
    evtPrintDeveloperConsoleMessage("*****************[mtrMantraEventBitPop()]*****************\n");
}

extern void scrSetEntryLowFlags(u32 context, u16 entryId, u16 lowFlags);
void mtrMantraBitResetUnit(MtrRecord *record) {
    s32 i;

    for (i = 0; i < 0xB0; i++) {
        scrSetEntryLowFlags((u32)record, i, 0);
    }
    evtPrintDeveloperConsoleMessage(
        "*****************[mtrMantraBitReset_Unit():[0x%x]]*****************\n", record->unk4);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", mtrMantraBitReset);

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
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 2;
    func_00286BA8(&record);
    func_00290E48(0);
    if (mdlFlagTest(0x9A0)) {
        mdlFlagSet(0x9A1);
    }
}

void func_00286670(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 1;
    func_00286BA8(&record);
    func_00290E48(1);
    if (mdlFlagTest(0x9A1)) {
        mdlFlagSet(0x9A0);
    }
}

void func_002866C8(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 3;
    func_00286BA8(&record);
}

void func_00286700(void) {
    MtrRecord record;

    memset(&record, 0, sizeof(record));
    record.unk4 = 7;
    func_00286BA8(&record);
}

INCLUDE_ASM(const s32, "game/code_00285CE8", func_00286738);
