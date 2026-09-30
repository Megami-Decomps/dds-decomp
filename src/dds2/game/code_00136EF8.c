#include "common.h"
#include "fpu.h"

extern u64 dds3GetWorldObject(void);

extern s32 mdlFlagTest(s32);
extern int strcmp(const char *, const char *);

extern void evtSetDrawSurfaceIndex();
extern void evtSubmitGsRegister47();
extern void func_00108BD8();
extern void func_00108EC0();

extern s32 D_004360F8;

extern u32 D_004360FC;

extern s32 D_00436100;

extern u32 D_00436178;

extern u32 D_00436180;

extern u32 D_0043618C;

extern u32 D_00436190;

extern s32 D_004361F8;

extern void func_003298C0(u32 arg0);

extern void *memset(void *s, s32 c, u32 n);

extern void *func_003292A8(s32 size);

extern void *sdfResourceRetainAddress(void *p);

extern u32 D_0038BD50[];

extern s32 func_0010C100(u32 arg0);

extern s32 func_00110FB0(u64 arg0, u32 arg1);

extern s32 kwlnTaskIsRegistered(u32 arg0);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 D_004361A4;

extern u32 *D_0038BC50[];

extern void fldDrawMarkerQuad(u32 value);

extern s32 D_004361CC;

extern s32 D_004361D0;

extern u8 *func_001404E0(u32);

extern s16 D_00444C68[];

extern u32 D_004361F4;

extern s16 D_003932B2[];

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;

extern FldIndexPair D_0038A3B8[];

extern FldIndexPair D_0038A480[];

extern s32 D_004361E4;

extern s32 D_004361EC;

extern s32 D_004361F0;

extern s32 D_00435F28;

extern s32 D_00389784[];

extern u8 D_003932A0[];

extern u8 D_00391F30[];

extern s32 D_00438ECC;

extern u32 func_0032C138(void *);

extern char D_00413350[];

extern u32 D_00438ED0;

extern u32 D_004360F4;

extern u8 D_00444990[];

extern u32 func_00343ED0(const char *, u32 *, s32);

extern u32 func_002DEB80(const void *);

extern void func_001379C0(u32);

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern u32 sdfDevCreateCommandState(const char *);

extern u32 func_0033EB10(u32, void *, u32);

extern void func_0033EAE0(u32);

extern s32 D_00436184;

typedef struct FldRecE4 {
    u8 pad0[0xCC];
    s32 id;
    s32 value;
    u8 padD4[0x10];
} FldRecE4; /* 0xE4 bytes */

extern s32 D_00436188;

extern s32 D_0043617C;

extern s32 D_00389770[];

extern u32 D_00444A30[];

extern s32 D_004361AC;

extern u64 dds3GetWorldSecondaryObject(void);

extern s32 D_004361A8;

extern s32 D_004361B0;

extern s32 D_004361B4;

extern s32 func_00110F28();

extern s32 D_004361BC;

typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;

extern s32 func_00110C70(u64, u32, s32);

extern u32 dds3GetPathState(s32);

typedef struct FldRoomPlanes {
    f32 plane[6][4];
    f32 limit[6];
    u8 pad78[0x140 - 0x78];
} FldRoomPlanes;

extern FldRoomPlanes D_00444BC0[];

extern f32 fldDotVector(f32 *, f32 *);

typedef struct FldRoomState {
    f32 corner[8][4]; /* 0x00 */
    f32 center[4];    /* 0x80 */
    f32 plane[6][4];  /* 0x90 */
    f32 limit[6];     /* 0xF0 */
    s32 unk108;
    s32 unk10C;
    s32 unk110;
    s32 unk114;
    s32 unk118;
    s32 unk11C;
    s32 unk120;
    u8 pad124[0xC];
    s16 unk130;
    s16 roomId; /* 0x132: returned by fldFindRoomByTask */
    s16 unk134;
    s16 mode;
    s16 unk138;
    s16 axisMode;
    s32 unk13C;
} FldRoomState; /* 0x140 bytes */

extern FldRoomState D_00444B30[];

extern u8 D_0038E2D0[];

extern u8 D_0038E2D0[];

extern char D_00413448[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */

extern s32 *func_001111A8();

typedef struct FldNpcMotion {
    s32 unk0;
    u8 unk4[0x10];
    u8 unk14[0x10];
    u8 unk24[0x10];
    u8 unk34[0x10];
    s32 unk44;
} FldNpcMotion; /* 0x48 bytes */

extern FldNpcMotion D_00391FA0[];

extern void *D_004361D4;

extern void *D_004361D8;

extern u32 D_004361DC;

extern s32 D_004361E0;

typedef struct {
    s16 data[12];
} FldRowData; /* 0x18 bytes */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 count;
    FldRowData body;
} FldS16Row; /* 0x20 bytes */

extern FldS16Row D_003931A0[];

extern u32 D_00449B30[][23];

typedef struct FldActorEntry {
    /* 0x00 */ u8 kind;
    /* 0x01 */ u8 pad01;
    /* 0x02 */ s16 modelId;
    /* 0x04 */ s16 area;
    /* 0x06 */ char name[0xC];
    /* 0x12 */ s16 state;
    /* 0x14 */ s16 state2;
    /* 0x16 */ s16 state3;
    /* 0x18 */ u8 name0[0xC];
    /* 0x24 */ u8 name1[0xC];
    /* 0x30 */ s8 locked;
    /* 0x31 */ u8 pad31;
    /* 0x32 */ s16 warpArea;
    /* 0x34 */ s16 warpEntry;
    /* 0x36 */ u8 pad36[0xE];
    /* 0x44 */ s8 linkKind;
    /* 0x45 */ u8 pad45;
    /* 0x46 */ u8 linkName[0x26];
} FldActorEntry; /* 0x6C bytes */

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00136EF8);

void func_00137818(void) {
    D_00438ECC = func_00343ED0(D_00413350, &D_00438ED0, 0);
    D_00436100 = func_0032C138((void *)D_00438ED0);
    D_004360F8 = func_002DEB80(D_00444990);
    if (D_00438ECC != 0) {
        func_003298C0(D_00438ECC);
        D_00438ECC = 0;
    }
    func_001379C0(D_004360F4);
    D_004360FC = 1;
}

void func_00137888(void) {
    func_00136EF8();
}

void func_001378A0(void) {
    D_004360FC = 0;
    if (D_00436100 != 0) {
        sdfTexReleaseReferenceViaHandler(D_00436100);
        D_00436100 = 0;
    }
    if (D_004360F8 != 0) {
        func_002DEBB0(D_004360F8);
        D_004360F8 = 0;
    }
}

typedef struct FldSaveHeader {
    u32 word[0x54 / 4];
} FldSaveHeader;

extern u8 D_004449D0[];

void fldCopyCameraSetting(FldSaveHeader *dst) {
    *dst = *(FldSaveHeader *)D_004449D0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001379C0);

/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocateRecordStorage(void) {
    u8 *storage = func_003292A8(0x72000);

    D_00436190 = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    D_00436180 = (u32)storage;
    memset(storage, 0, 0x72000);
    storage = func_003292A8(0x4A00);
    D_0043618C = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    D_00436178 = (u32)storage;
    memset(storage, 0, 0x4A00);
}

void func_00137BC8(void) {
    func_00329910(D_00436190);
    func_003298C0(D_00436190);
    D_00436180 = 0;
    func_00329910(D_0043618C);
    func_003298C0(D_0043618C);
    D_00436178 = 0;
}

float fldDotVector(float *left, float *right) {
    return *left * *right + left[1] * right[1] + left[2] * right[2];
}

f32 fldCalculateVectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

void func_00137C68(f32 *points, f32 *nx, f32 *ny, f32 *nz, f32 *planeD) {
    f32 p0[4];
    f32 p1[4];
    f32 p2[4];
    f32 inv;

    p0[0] = points[0];
    p0[1] = points[1];
    p0[2] = points[2];
    p1[0] = points[4];
    p1[1] = points[5];
    p1[2] = points[6];
    p2[0] = points[8];
    p2[1] = points[9];
    p2[2] = points[10];
    *nx = (p1[1] - p0[1]) * (p2[2] - p1[2]) - (p1[2] - p0[2]) * (p2[1] - p1[1]);
    *ny = (p1[2] - p0[2]) * (p2[0] - p1[0]) - (p1[0] - p0[0]) * (p2[2] - p1[2]);
    *nz = (p1[0] - p0[0]) * (p2[1] - p1[1]) - (p1[1] - p0[1]) * (p2[0] - p1[0]);
    inv = 1.0f / fsqrtf(*nx * *nx + *ny * *ny + *nz * *nz);
    *nx = *nx * inv;
    *ny = *ny * inv;
    *nz = *nz * inv;
    if (*nx > -0.0001f && *nx < 0.0001f) {
        *nx = 0.0f;
    }
    if (*ny > -0.0001f && *ny < 0.0001f) {
        *ny = 0.0f;
    }
    if (*nz > -0.0001f && *nz < 0.0001f) {
        *nz = 0.0f;
    }
    *planeD = -(*nx * p2[0] + *ny * p2[1] + *nz * p2[2]);
}

s32 fldGetRecordValueById(s32 key) {
    s32 i = 0;

    if (D_00436184 > 0) {
        FldRecE4 *record = (FldRecE4 *)D_00436180;
        do {
            if (record->id == key) {
                return record->value;
            }
            i++;
            record++;
        } while (i < D_00436184);
    }
    return 0;
}

void fldSetRecordValueById(s32 id, s32 value) {
    s32 i;

    for (i = 0; i < D_00436184; i++) {
        if (((FldRecE4 *)D_00436180)[i].id == id) {
            ((FldRecE4 *)D_00436180)[i].value = value;
        }
    }
}

void fldResetRecordState(void) {
    s32 count = D_00436184;
    if (count > 0) {
        /* Required to match: advance a pointer to the value field, not the record base. */
        u8 *record = (u8 *)D_00436180 + 0xd0;
        do {
            count--;
            *(s32 *)record = 0;
            record += 0xe4;
        } while (count != 0);
    }
    D_00436184 = 0;
    D_00389770[0x28] = -1;
    D_00389770[0x29] = -1;
    D_00389770[0x2b] = -1;
    D_00436188 = 0;
    D_0043617C = 0;
    if (D_00436180 != 0) {
        func_00137BC8();
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137F10);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139400);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139628);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139950);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139B98);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00139EC0);

void func_0013AA78(void) {
}

void func_0013AA80(void) {
}

void func_0013AA88(void) {
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013AA90);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013AC40);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B0D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B4F8);

void func_0013B810(void) {
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B818);

void fldResetTaskSlots(void) {
    s32 i;
    u64 world;
    u32 id;
    FldTaskInfo *info;

    D_004361BC = 1;
    for (i = 0; i < D_004361A4; i++) {
        D_0038BD50[i] = 0;
    }
    D_004361A8 = -1;
    D_004361AC = -1;
    D_004361B0 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (i = 0; i < D_004361A4; i++) {
            info = *(FldTaskInfo **)(D_0038BC50[i] + 8);
            if (info->slot >= 0) {
                id = dds3GetPathState(func_00110C70(world, *(u32 *)D_00444A30[info->slot], 0xD));
                if (func_0010C100(id) != 0) {
                    func_00110FB0(dds3GetWorldObject(), id);
                }
            }
        }
    }
}

extern s32 D_004361B8;

s32 fldPushDisplayValue(u32 value) {
    s32 index = D_004361B8;
    D_00444A30[index] = value;
    D_004361B8 = index + 1;
    return index;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013BAB8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D308);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D598);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D7F8);

s32 func_0013DA10(f32 *direction, s32 index) {
    f32 probe[3];
    f32 planar[2];
    s32 i;

    if (D_00444B30[index].axisMode == 0) {
        probe[0] = 0.0f;
        probe[1] = direction[1];
        probe[2] = direction[2];
        planar[0] = direction[1];
        planar[1] = direction[2];
    } else {
        probe[0] = direction[0];
        probe[1] = direction[1];
        probe[2] = 0.0f;
        planar[0] = direction[0];
        planar[1] = direction[1];
    }
    for (i = 0; i < 4; i++) {
        if (fldDotVector(probe, D_00444BC0[index].plane[i + 1]) - D_00444BC0[index].limit[i + 1] < 0.0f) {
            return -1;
        }
    }
    return 0;
}

s32 fldRoomContainsPoint(f32 *direction, s32 index) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fldDotVector(direction, D_00444BC0[index].plane[i]) - D_00444BC0[index].limit[i] > 0.0f) {
            return 0;
        }
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DBB0);

s32 func_0013DC70(u32 flag, u32 slot) {
    D_0038BD50[slot] = 0;
    if (func_0010C100(flag) != 0) {
        func_00110FB0(dds3GetWorldObject(), flag);
        return 0;
    }
    return -1;
}

u32 fldDestroyTaskSlot(u32 index) {
    u32 *taskSlot = &D_0038BD50[index];

    if (kwlnTaskIsRegistered(*taskSlot) != 0) {
        kwlnTaskDestroyWithHierarchy(*taskSlot, 0);
    }
    *taskSlot = 0;
    return 0;
}

extern s8 D_00387D60[];
extern u8 D_00435F24;
extern u8 *func_001406E8(void);

s32 func_0013DD18(void) {
    u8 *object;
    u32 state = D_00435F24;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_00387D60[0] != 0) {
        func_00110FB0(dds3GetWorldObject(), D_00387D60);
    }
    D_00387D60[0] = 0;
    D_00435F24 = 0;
    object = func_001406E8();
    if (func_0010C100((u32)object) == 0) {
        func_00110F28(dds3GetWorldObject(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DDC0);

void func_0013E958(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_0038BC50;
        do {
            fldDrawMarkerQuad((*entry)[4]);
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

void fldClearInactiveTaskSlots(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_0038BD50;
        do {
            if (kwlnTaskIsRegistered(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

s32 fldFindTaskRecordId(u32 task) {
    s32 i;
    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][0];
        }
    }
    return -1;
}

s32 fldFindRoomByTask(u32 task) {
    s32 i;

    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] == task) {
            return D_00444B30[i].roomId;
        }
    }
    return -1;
}

s32 fldGetTaskRecordValue(u32 task) {
    s32 i;
    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][2];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EB30);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013ED20);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EEA0);

extern s32 D_004361C0;

s32 fldCheckEntryActive(s32 value) {
    s32 blocked = D_004361C0;
    s32 index = D_004361AC;

    if (blocked != 0) {
        return 1;
    }
    if (index == -1) {
        return 0;
    }
    if (D_00444B30[index].mode == 1 && D_00444B30[index].unk138 == value) {
        return 1;
    }
    return 0;
}

s32 fldHasActiveTasks(void) {
    s32 i;
    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_0013F1B8(void) {
    s32 index = D_004361CC;

    if (index < 0) {
        return -1;
    }
    return D_00444C68[index * 160];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F1E8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F3E0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F5C8);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004133A0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F790);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013FA98);

extern s32 D_004361C8;

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013FFF8);

void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        func_0035C860(path, D_00413448, directory, field);
        command = sdfDevCreateCommandState(path);
        func_0033EB10(command, D_0038E2D0, 0x3B80);
        func_0033EAE0(command);
    }
}

typedef struct FldSaveBlock {
    u32 word[0x3B80 / 4];
} FldSaveBlock;

void func_00140180(FldSaveBlock *src) {
    *(FldSaveBlock *)D_0038E2D0 = *src;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140238);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_00413448);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001402B8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001404E0);

u8 *func_001406E8(void) {
    s32 index = D_00435F28;
    u8 *entry = D_003932A0 + index * 108;
    if (*(s16 *)(entry + 4) == D_00389784[0] + 1 && *(s8 *)entry == 10) {
        D_004361F4 = index;
        D_004361F8 = 8;
        return D_00391F30;
    }
    return 0;
}

s32 func_00140750(void) {
    return D_003932B2[D_004361F4 * 54];
}

s32 func_00140780(s32 mode) {
    u8 *entry = D_003932A0 + D_004361F4 * 0x6C;
    s16 a;
    u32 result;
    if (mode == 0 && *(s8 *)entry == 1) {
        a = *(s16 *)(entry + 0x12);
        if (a == 5 || *(s16 *)(entry + 0x14) == 5 || a == 6 || *(s16 *)(entry + 0x14) == 6 || a == 7 ||
            *(s16 *)(entry + 0x14) == 7 || a == 8 || *(s16 *)(entry + 0x14) == 8) {
            return 0x28;
        }
        return 0x14;
    }
    if (mode == 1) {
        result = *(u8 *)(entry + 0x54) & 8;
        return result != 0;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140830);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140A58);

u8 func_00140B80(void) {
    return D_004361F8 == 8;
}

void func_00140B90(s8 *arg0) {
    if (arg0[0x53] != 0) {
        D_00389770[0x22] = arg0[0x53] - 1;
    }
    D_00389770[0x16] = arg0[0x45];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140BC8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001411F8);

void func_00141840(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_004361EC = 3;
        D_004361E4 = D_0038A3B8[index].unk0;
        D_004361F0 = index;
        break;
    case 1:
        D_004361F0 = index;
        D_004361E4 = D_0038A480[index].unk0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141898);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141B20);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141CF0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00141F58);

s32 fldGetActorStat0(s32 mode) {
    u8 *actor = D_003932A0 + D_004361F4 * 108;
    s32 *entry;
    s32 flags;

    switch (mode) {
    case 0:
        return *(s16 *)(actor + 0x12);
    case 1:
        entry = func_001111A8(dds3GetWorldObject(), actor + 0x18);
        if (entry != NULL) {
            return entry[1];
        }
    case 2:
        entry = func_001111A8(dds3GetWorldObject(), actor + 0x24);
        if (entry != NULL) {
            return entry[1];
        }
    case 3:
        flags = *(u16 *)(actor + 0x14);
        if (flags & 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}

extern s32 D_003897C0[];

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134D0);

s32 func_001421C0(u32 mode) {
    FldActorEntry *actor = (FldActorEntry *)D_003932A0 + D_004361F4;
    s32 *entry;

    switch (mode) {
    case 0:
        switch (actor->state) {
        case 0:
            return 0xC;
        case 1:
            return 0xF;
        case 2:
            return 0x10;
        case 3:
            return 0xD;
        case 4:
            return 0xE;
        }
        return 0;
    case 1:
        entry = func_001111A8(dds3GetWorldObject(), actor->name0);
        if (entry != NULL) {
            return entry[1];
        }
    case 2:
        entry = func_001111A8(dds3GetWorldObject(), actor->name1);
        if (entry != NULL) {
            return entry[1];
        }
        return actor->state2;
    case 3:
        return actor->state2;
    case 4:
        return actor->state3;
    case 5:
        return D_003897C0[0] != 1;
    }
    return 0;
}

s32 fldGetActorMotionEntry(u32 kind) {
    u8 *entry = D_003932A0 + D_004361F4 * 108;
    s16 index = *(s16 *)(entry + 0x12);
    s32 *found;
    s32 flags;

    switch (kind) {
    case 0:
        return D_00391FA0[index].unk0;
    case 1:
        found = func_001111A8(dds3GetWorldObject(), D_00391FA0[index].unk4);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = func_001111A8(dds3GetWorldObject(), D_00391FA0[index].unk14);
        if (found != NULL) {
            return found[1];
        }
    case 3:
        D_004361D4 = D_00391FA0[index].unk24;
        return 0;
    case 4:
        D_004361D8 = D_00391FA0[index].unk34;
        return 0;
    case 5:
        return D_00391FA0[index].unk44;
    case 6:
        flags = entry[0x64];
        if (flags & 1) {
            return *(s8 *)(entry + 0x67);
        }
        return -1;
    }
    return 0;
}

s32 fldGetRowValue(u32 kind) {
    s32 slot = D_004361DC;

    switch (kind) {
    case 0:
        return D_003931A0[slot].count;
    case 1:
        return D_003931A0[slot].unk4;
    case 2:
        return D_003931A0[slot].body.data[0];
    case 3:
        return D_003931A0[slot].body.data[1];
    case 4:
        return D_003931A0[slot].body.data[2];
    case 5:
        return D_003931A0[slot].body.data[3];
    case 6:
        return D_003931A0[slot].body.data[4];
    case 7:
        return D_003931A0[slot].body.data[5];
    case 8:
        return D_003931A0[slot].body.data[6];
    case 9:
        return D_003931A0[slot].body.data[7];
    case 10:
        return D_003931A0[slot].body.data[8];
    case 11:
        return D_003931A0[slot].body.data[9];
    case 12:
        return D_003931A0[slot].body.data[10];
    case 13:
        return D_003931A0[slot].body.data[11];
    case 14:
        return D_003931A0[slot].count - D_004361E0 - 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", fldFindTableEntry);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142670);

void fldResetActorSlots(void) {
    s32 i;

    for (i = 0; i < 256; i++) {
        D_00449B30[i][0] = 0;
        D_00449B30[i][1] = 0;
        D_00449B30[i][2] = 0;
        D_00449B30[i][3] = -1;
        D_00449B30[i][4] = 0;
        D_00449B30[i][5] = -1;
        D_00449B30[i][6] = -1;
        D_00449B30[i][7] = 0;
        D_00449B30[i][8] = 0;
        D_00449B30[i][9] = 0;
        D_00449B30[i][11] = 0;
        D_00449B30[i][12] = 0;
        D_00449B30[i][13] = 0;
        D_00449B30[i][14] = 0;
        D_00449B30[i][15] = 0;
        D_00449B30[i][16] = 0;
        D_00449B30[i][17] = 0;
        D_00449B30[i][18] = 0;
        D_00449B30[i][19] = 0;
        D_00449B30[i][20] = 0;
        D_00449B30[i][21] = 0;
        D_00449B30[i][22] = 0;
    }
    D_004361F4 = -1;
}

void fldLoadActorWaypointTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field >= 100) {
        memset(D_003931A0, 0, 0x6D00);
    } else {
        fldFormatAreaDirectory(directory, field, 1);
        func_0035C860(path, "%sF%03d.WAP", directory, field);
        command = sdfDevCreateCommandState(path);
        func_0033EB10(command, D_003931A0, 0x6D00);
        func_0033EAE0(command);
    }
}

typedef struct FldWaypointBlock {
    u32 word[0x6D00 / 4];
} FldWaypointBlock;

void fldCopyActorWaypointTable(FldWaypointBlock *src) {
    *(FldWaypointBlock *)D_003931A0 = *src;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142B70);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143768);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004135D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143910);

void func_00143C90(void) {
}

extern void func_00155D48(s32, s32, void *, s32);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143C98);

extern s32 D_00399F90[];

void func_00143D90(s32 mode) {
    evtSetDrawSurfaceIndex(0x53);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(0x97, 0x128, 0x3A, 0x24, 1, 2, 0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  D_00389770[0x7F]);
    func_00108EC0(0xD1, 0x128, 0x5E, 0x24, 0x3A, 2, 1, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  D_00389770[0x7F]);
    func_00108EC0(0x12F, 0x128, 0x3A, 0x24, 0x3B, 2, -0x3A, 0x1D, 0x80808080, 0x80808080, 0x80808080, 0x80808080,
                  D_00389770[0x7F]);
    if (mode < 0x18) {
        func_00108BD8(1);
        func_00108EC0(0xA0, 0x12B, 0x22, 0x24, 1, 0x21, 0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], D_00389770[0x7F]);
        func_00108EC0(0x13E, 0x12B, 0x22, 0x24, 0x23, 0x21, -0x22, 0x1D, D_00399F90[mode], D_00399F90[mode],
                      D_00399F90[mode], D_00399F90[mode], D_00389770[0x7F]);
        func_00108BD8(0);
    }
}

extern s32 D_00389978[];

void func_00143F78(s32 arg0, s32 arg1) {
    evtSetDrawSurfaceIndex(0x53);
    func_00108BD8(0);
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108EC0(arg0, arg1, 0x12, 0x13, 1, 0x25, 0x12, 0x13, 0x80808080, 0x80808080, 0x80808080, 0x80808080, D_00389978[0]);
    func_00108BD8(0);
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00144028);

extern void *func_00328D68(s32 size);
extern void func_00101950(s32, void *);
extern void func_00144028();

void *func_00144178(s32 arg0) {
    s16 *node = func_00328D68(8);
    node[1] = 1;
    node[0] = 0;
    node[2] = 0;
    node[3] = 0;
    func_00101950(arg0, node);
    return func_00144028;
}

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436174);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436178);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043617C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436180);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436184);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436188);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043618C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436190);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436194);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436198);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_0043619C);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361A0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361A4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361A8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361AC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361B8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361BC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361C8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361CC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361D8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361DC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361E8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361EC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361F0);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361F4);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361F8);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_004361FC);

INCLUDE_SDATA(const s32, "game/code_00136EF8", D_00436200);

