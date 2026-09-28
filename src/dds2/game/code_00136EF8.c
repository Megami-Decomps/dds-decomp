#include "common.h"

extern u64 func_0010FFA8(void);

extern s32 D_004360F8;

extern u32 D_004360FC;

extern s32 D_00436100;

extern u32 D_00436178;

extern u32 D_00436180;

extern u32 D_0043618C;

extern u32 D_00436190;

extern s32 D_004361F8;

extern u32 D_00389770[];

extern void func_003298C0(u32 arg0);

extern void *memset(void *s, s32 c, u32 n);

extern void *func_003292A8(s32 size);

extern void *func_003298F8(void *p);

extern u32 D_0038BD50[];

extern s32 func_0010C100(u32 arg0);

extern s32 func_00110FB0(u64 arg0, u32 arg1);

extern s32 func_001018B0(u32 arg0);

extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

extern s32 D_004361A4;

extern u32 *D_0038BC50[];

extern void func_0012D9D0(u32 value);

extern s32 D_004361CC;

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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00136EF8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137818);

void func_00137888(void) {
    func_00136EF8();
}

void func_001378A0(void) {
    D_004360FC = 0;
    if (D_00436100 != 0) {
        func_0032BBB0(D_00436100);
        D_00436100 = 0;
    }
    if (D_004360F8 != 0) {
        func_002DEBB0(D_004360F8);
        D_004360F8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001378E8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001379C0);

void func_00137B60(void) {
    u8 *temp_v0 = func_003292A8(0x72000);

    D_00436190 = (u32)temp_v0;
    temp_v0 = func_003298F8(temp_v0);
    D_00436180 = (u32)temp_v0;
    memset(temp_v0, 0, 0x72000);
    temp_v0 = func_003292A8(0x4A00);
    D_0043618C = (u32)temp_v0;
    temp_v0 = func_003298F8(temp_v0);
    D_00436178 = (u32)temp_v0;
    memset(temp_v0, 0, 0x4A00);
}

void func_00137BC8(void) {
    func_00329910(D_00436190);
    func_003298C0(D_00436190);
    D_00436180 = 0;
    func_00329910(D_0043618C);
    func_003298C0(D_0043618C);
    D_00436178 = 0;
}

float func_00137C08(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137C38);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137C68);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137E00);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137E48);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00137E90);

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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013B970);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013BA98);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013BAB8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D308);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D598);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013D7F8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DA10);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DB08);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DBB0);

s32 func_0013DC70(u32 flag, u32 slot) {
    D_0038BD50[slot] = 0;
    if (func_0010C100(flag) != 0) {
        func_00110FB0(func_0010FFA8(), flag);
        return 0;
    }
    return -1;
}

u32 func_0013DCC8(u32 arg0) {
    u32 *temp_v0 = &D_0038BD50[arg0];

    if (func_001018B0(*temp_v0) != 0) {
        kwlnTaskDestroyWithHierarchy(*temp_v0, 0);
    }
    *temp_v0 = 0;
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DD18);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013DDC0);

void func_0013E958(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_0038BC50;
        do {
            func_0012D9D0((*entry)[4]);
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

void func_0013E9B8(void) {
    s32 count = D_004361A4;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_0038BD50;
        do {
            if (func_001018B0(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < D_004361A4);
    }
}

s32 func_0013EA18(u32 task) {
    s32 i;
    for (i = 0; i < D_004361A4; i++) {
        if (D_0038BD50[i] == task) {
            return D_0038BC50[i][0];
        }
    }
    return -1;
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EA78);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EAD0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EB30);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013ED20);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013EEA0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F108);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F168);

s32 func_0013F1B8(void) {
    s32 temp_v0 = D_004361CC;

    if (temp_v0 < 0) {
        return -1;
    }
    return D_00444C68[temp_v0 * 160];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F1E8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F3E0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F5C8);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004133A0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013F790);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013FA98);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_0013FFF8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001400F8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140180);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140238);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001402B8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001404E0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001406E8);

s32 func_00140750(void) {
    return D_003932B2[D_004361F4 * 54];
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00140780);

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

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001420F0);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134C0);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004134D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001421C0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_001422E8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142478);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142618);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142670);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142990);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142A10);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142AB8);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00142B70);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143768);

INCLUDE_RODATA(const s32, "game/code_00136EF8", D_004135D0);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143910);

void func_00143C90(void) {
}

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143C98);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143D90);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00143F78);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00144028);

INCLUDE_ASM(const s32, "game/code_00136EF8", func_00144178);
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

