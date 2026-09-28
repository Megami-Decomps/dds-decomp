#include "common.h"

extern void *func_00101A70();
extern s32 func_0018FDA8(void);
extern s32 func_002E92C0(s32 arg0);
extern s32 func_002350F8(void);
extern void func_00235120(s32 arg0, s32 arg1);
extern void func_002351E0(void);
extern void func_00235228(void);
extern s32 func_00241B58();
extern void func_003014F0(void *arg0, void *arg1, s32 arg2);
extern void func_00241B30(s32 arg0, void *arg1);
extern void kwlnTaskCreate(void *name, s32 arg1, s32 arg2, s32 arg3, void *update, void *destroy, void *data);
extern s32 kwlnTaskGetTaskByName(void *name);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern void func_00132B60(s32 arg0);
extern void func_00132B70(s32 arg0);
extern void func_00132B80(s32 arg0);
extern s16 func_00132B90(void);
extern void func_00132AE8(s32 arg0, s32 arg1, s32 arg2);
extern void func_00132010(void);
extern void func_00129720(s32 arg0);
extern void func_0012AEB0(void);
extern void func_002D4038(s32 arg0, s32 arg1);
extern s32 func_002E4960();
extern u8 D_003BBF80[];
extern u8 D_003BBF90[];
extern u8 D_003BC360[];
extern u16 D_003BD898;
extern u16 D_003BD89A;
extern s16 D_003BD89C;
extern s16 D_003BD89E;

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */
extern EvtTblEntry D_00368950[];
extern s8 D_00368952[];

extern u16 D_003BBF88;

extern u32 D_003BBF8C;

void func_00235270(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002350F8();
    func_00235120(temp_v0, arg1);
    kwlnTaskCreate(D_003BBF80, arg0, 1, 1, func_002351E0, func_00235228, (void *)temp_v0);
}


void func_002352E0(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002350F8();
    *(s32 *)(temp_v0 + 4) = arg1;
    kwlnTaskCreate(D_003BBF80, arg0, 1, 1, func_002351E0, func_00235228, (void *)temp_v0);
}

void func_00235340(u32 arg0) {
    D_003BBF8C = arg0;
}


void func_00235348(s32 arg0, s32 arg1) {
    s16 temp_v0;

    temp_v0 = func_00132B90();
    if (temp_v0 != arg1) {
        if (arg0 == 0) {
            func_00132B80(arg1);
            D_003BBF88 = 0;
        } else {
            D_003BD89A = arg0;
            D_003BD89C = temp_v0;
            D_003BD89E = arg1;
            D_003BBF88 = 1;
            D_003BD898 = 0;
        }
    }
}

u16 func_002353B0(void) {
    return D_003BBF88;
}


void func_002353B8(void) {
    if (D_003BBF88 != 0) {
        D_003BD898 += 1;
        func_00132B80(D_003BD89C + (s32)((f32)(D_003BD89E - D_003BD89C) * ((f32)D_003BD898 / (f32)D_003BD89A)));
        if (D_003BD898 >= D_003BD89A) {
            D_003BBF88 = 0;
        }
    }
}


s32 func_00235440(void) {
    func_002353B8();
    func_00132010();
    if (D_003BBF8C != 0) {
        func_00129720(0x53);
        func_0012AEB0();
    }
    return 0;
}

void func_00235488(void) {
    D_003BBF88 = 0;
    D_003BBF8C = 0;
}


void func_00235498(void) {
    s32 temp_v0;

    temp_v0 = kwlnTaskGetTaskByName(D_003BBF90);
    if (temp_v0 != 0) {
        kwlnTaskDestroyWithHierarchy(temp_v0, 1);
    }
}


void func_002354D8(void) {
    func_00132B60(0);
    func_00132B70(0x80);
    func_00132B80(0);
    func_00132AE8(0, 1, 0);
    kwlnTaskCreate(D_003BBF90, 0x2B0E, 1, 1, func_00235440, func_00235488, 0);
}


INCLUDE_ASM(const s32, "game/code_00235270", func_00235540);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235560);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235598);


s32 func_00235768(s32 arg0, s32 arg1, s32 arg2) {
    func_002D4038(arg0, func_002E4960(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDE0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDF0);

INCLUDE_ASM(const s32, "game/code_00235270", func_002357B8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235978);


s32 func_00235AE0(s32 arg0, s32 arg1, s32 arg2) {
    func_002D4038(arg0, func_002E4960(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE40);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE50);

INCLUDE_RODATA(const s32, "game/code_00235270", jtbl_003ADE60);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADE78);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235B30);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235C68);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235DB0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235E00);

INCLUDE_ASM(const s32, "game/code_00235270", func_00235FC8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236180);

INCLUDE_ASM(const s32, "game/code_00235270", func_002363E0);


INCLUDE_ASM(const s32, "game/code_00235270", func_002364B0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236510);

INCLUDE_ASM(const s32, "game/code_00235270", func_002365A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_002366A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236728);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236828);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0B8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE0D8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236A40);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236AC8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00236D80);

INCLUDE_ASM(const s32, "game/code_00235270", func_00237048);

INCLUDE_ASM(const s32, "game/code_00235270", func_00237130);

INCLUDE_ASM(const s32, "game/code_00235270", func_00237348);

INCLUDE_ASM(const s32, "game/code_00235270", func_00237428);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE530);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE540);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE550);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE560);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE570);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE580);

INCLUDE_ASM(const s32, "game/code_00235270", func_002375D8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238600);

INCLUDE_ASM(const s32, "game/code_00235270", func_002386E0);

INCLUDE_ASM(const s32, "game/code_00235270", func_002388F8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238A88);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238BE8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238D58);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238E58);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8D0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8E0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE8F0);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE900);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE910);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE920);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE930);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE940);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE950);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE960);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE970);

INCLUDE_ASM(const s32, "game/code_00235270", func_00238ED0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239148);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AE9D0);

INCLUDE_RODATA(const s32, "game/code_00235270", jtbl_003AE9E0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239350);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA10);

INCLUDE_ASM(const s32, "game/code_00235270", func_002393A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_002395A8);


s32 func_00239770(s32 *arg0) {
    return D_00368950[*arg0].unk0 != 0;
}


s32 func_00239798(s32 arg0) {
    return D_00368952[*(s32 *)(arg0 + 0x23C8) + *(s32 *)(*(s32 *)(arg0 + 0x2308)) * 10];
}

INCLUDE_ASM(const s32, "game/code_00235270", func_002397C8);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA70);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEA80);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239A90);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239DE0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00239E30);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A0D0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A4B0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A688);

void func_0023A798(void) {
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A7A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023A968);

void func_0023B1F8(void) {
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023B200);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023B848);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023B8F8);


void func_0023B9D8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)(arg0 + 0x23E4) = arg1;
    *(s32 *)(arg0 + 0x23E8) = arg2;
    *(s32 *)(arg0 + 0x23EC) = arg3;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEB90);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023B9E8);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BB20);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEBB0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BC30);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023BE40);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023C178);


s32 func_0023C208(void) {
    void *temp_v0;

    temp_v0 = func_00101A70();
    if (func_0018FDA8() == 0) {
        *(s32 *)((u8 *)temp_v0 + 0x228C) = 0;
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC00);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC10);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003AEC20);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023C248);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023CA60);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D420);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D560);

void func_0023D5B0(s32 arg0, void *arg1, s32 arg2) {
    func_0030F190(arg0, arg1, arg2);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D5C8);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D630);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D698);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D700);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D768);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D7D0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D838);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D8A0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D908);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D970);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023D9D8);


void func_0023DF60(s32 arg0, s32 *arg1) {
    s32 temp_10 = arg1[4];
    s32 temp_14 = arg1[5];
    s32 temp_C = arg1[3];
    s32 temp_243C = *(s32 *)((u8 *)arg1 + 0x243C);
    s32 buf[4];

    buf[0] = temp_10;
    buf[1] = temp_14;
    buf[2] = temp_C;
    buf[3] = temp_243C;
    func_0023D5B0(arg0, buf, 0x10);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023DFA8);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E138);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E1B0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E228);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E2B0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E338);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E3C0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E448);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E4D0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E558);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E5E0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E668);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E6F0);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E770);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023E7F8);

u16 func_0023ED08(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88));
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c));
}


s16 func_0023ED58(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(s16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 6);
    }
    return *(s16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 6);
}

u16 func_0023EDA8(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 2);
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 2);
}

u16 func_0023EDF8(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(u16 *)(arg1 * 0x10 + *(s32 *)(arg0 + 0x88) + 4);
    }
    return *(u16 *)(arg1 * 0x2c + *(s32 *)(arg0 + 0x8c) + 4);
}

s32 func_0023EE48(s32 arg0, s32 arg1) {
    if (*(s32 *)(*(s32 *)(arg0 + 0x74) + 0x14) == 4) {
        return *(s32 *)(arg0 + 0x88) + arg1 * 0x10 + 8;
    }
    return *(s32 *)(arg0 + 0x8c) + arg1 * 0x2c + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_0023EE98);

INCLUDE_ASM(const s32, "game/code_00235270", func_0023EF90);

INCLUDE_ASM(const s32, "game/code_00235270", func_002416E0);


s32 func_00241878(s32 arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = 0xC7;
    if (arg0 != 0x31F) {
        temp_v0 = arg0 - 0x259;
        if (arg0 >= 0x320) {
            temp_v0 = (arg0 < 0x384) ? (arg0 - 0x258) : (arg0 - 0x29E);
        }
    }
    return ((temp_v0 + 0x100) << 0x10) + arg1;
}


INCLUDE_ASM(const s32, "game/code_00235270", func_002418B0);


INCLUDE_ASM(const s32, "game/code_00235270", func_002418F8);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241938);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241990);

INCLUDE_ASM(const s32, "game/code_00235270", func_002419F8);


INCLUDE_ASM(const s32, "game/code_00235270", func_00241A50);


INCLUDE_ASM(const s32, "game/code_00235270", func_00241A98);


INCLUDE_ASM(const s32, "game/code_00235270", func_00241AE8);


void func_00241B30(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BC360, arg0);
}

s32 func_00241B58(u32 arg0) {
    u8 temp_v0[32];

    func_00241B30(arg0, temp_v0);
    return kwlnTaskGetTaskByName(temp_v0);
}


s32 func_00241B80(u32 arg0) {
    s32 task;

    task = func_00241B58(arg0);
    if (task != 0) {
        return *(s32 *)(func_00101A70(task) + 4);
    } else {
        return -1;
    }
}


s32 func_00241BB8(u32 arg0) {
    s32 task;

    task = func_00241B58(arg0);
    if (task != 0) {
        return (s32)func_00101A70(task);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00241BF0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241CA0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241D28);
