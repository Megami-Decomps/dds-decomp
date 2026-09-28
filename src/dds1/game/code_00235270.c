#include "common.h"

extern void func_0023D5B0(s32 arg0, void * arg1, s32 arg2);

extern void *func_00101A70();
extern s32 func_0018FDA8(void);
extern s32 func_002E92C0(s32 arg0);
extern void func_002E9340(s32 arg0);
extern s32 func_00235540(s32 *task);
extern s32 func_002350F8(void);
extern void func_00235120(s32 arg0, s32 arg1);
extern void func_002351E0(void);
extern void func_00235228(void);
extern s32 evtFindTaskById();
extern void func_003014F0(void *arg0, void *arg1, s32 arg2);
extern void evtFormatTaskName(s32 arg0, void *arg1);
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
extern void func_002E8D10(s32 arg0);
extern void func_002E9690(s32 arg0);
extern void func_002E9758(s32 arg0);
extern void func_003003F0(char *fmt, ...);
extern void func_002E96D8(u32 arg0);
extern void func_002E8DD0(u32 arg0);
extern void soundSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);
extern s32 func_002E4960();
extern u8 D_003BBF80[];
extern u8 D_003BBF90[];
extern u8 D_003BC360[];
extern u16 D_003BD898;
extern u16 D_003BD89A;
extern s16 D_003BD89C;
extern s16 D_003BD89E;


typedef struct EvtRuntimeChild {
    u8 pad00[2];
    u16 unk02;
    u8 pad04[6];
    u16 unk0A;
    u8 pad0C[6];
    u16 unk12;
    u8 pad14[0x1C];
    struct EvtRuntimeChild *next;
} EvtRuntimeChild;

typedef struct EvtRuntimeGroup {
    s32 type;
    u8 pad04[0x50];
    EvtRuntimeChild *children;
    u8 pad58[0x24];
    struct EvtRuntimeGroup *next;
} EvtRuntimeGroup;

typedef struct EvtRuntime {
    u8 pad00[0x2034];
    EvtRuntimeGroup *groups;
} EvtRuntime;

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */
extern EvtTblEntry D_00368950[];
extern s8 D_00368952[];

extern u16 D_003BBF88;

extern u32 D_003BBF8C;

void evtCreateTask(s32 arg0, s32 arg1) {
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

void evtDestroySkyTask(void) {
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

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate("FrameVar", 0x2AF9, 1, 1, func_00235540, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00235598);

INCLUDE_RODATA(const s32, "game/code_00235270", D_003ADDE0);

s32 func_00235768(s32 arg0, s32 arg1, s32 arg2) {
    func_002D4038(arg0, func_002E4960(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

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

s32 drawFrameChangeLabel(s32 target, s32 x, s32 y) {
    func_002D4038(target, func_002E4960(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

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

s32 drawCutFlagMenuLabel(s32 target, s32 x, s32 y) {
    func_002D4038(target, func_002E4960(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

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

s32 drawMotionChangeMenuLabel(s32 target, s32 x, s32 y) {
    func_002D4038(target, func_002E4960(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

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

extern s32 func_0018FEB0();
extern s32 kwlnTaskGetTimer(s32 task);
extern u8 *D_003BB128;
extern s32 D_003BD8A0;

s32 evtSynchronizeSelectedEntry(s32 task) {
    u8 *runtime = func_00101A70();
    if (func_0018FEB0() == 0) {
        *(s32 *)(runtime + 0x228C) = 0;
        return -1;
    }
    if (kwlnTaskGetTimer(task) == 0) {
        s32 selected = *(s32 *)(runtime + 0x23E0);
        D_003BD8A0 = selected;
        if (selected != 0) {
            *(s32 *)(D_003BB128 + 0x24) = selected;
        }
    }
    if (*(s32 *)(D_003BB128 + 0x24) != *(s32 *)(runtime + 0x23E0)) {
        s32 selected = *(s32 *)(runtime + 0x23E0);
        if (selected != 0) {
            *(s32 *)(D_003BB128 + 0x24) = selected;
        }
    }
    return 0;
}

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

extern s32 (*D_00368B48[])(s32, s32, void *);

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, void *runtime) {
    s32 mode = *(s32 *)((u8 *)runtime + 0x2280);
    if (mode == 11 && index != mode) {
        return 0;
    }
    return D_00368B48[index](x, y, runtime);
}

void func_0023D5B0(s32 arg0, void *arg1, s32 arg2) {
    func_0030F190(arg0, arg1, arg2);
}

s32 func_0023D5C8(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk12 = index++;
            }
        }
    }
    return index;
}

s32 func_0023D630(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk02 = index++;
            }
        }
    }
    return index;
}

s32 func_0023D698(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D700(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D768(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D7D0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D838(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D8A0(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D908(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

s32 func_0023D970(EvtRuntime *runtime) {
    s32 index = 0;
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                child->unk0A = index++;
            }
        }
    }
    return index;
}

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

void func_0023E138(s32 arg0, s32 arg1) {
    void *pvVar1;
    s32 temp_v0;

    temp_v0 = 0;
    if (0 < *(s32 *)(arg1 + 0x20)) {
        pvVar1 = (void *)(arg1 + 0x24);
        do {
            func_0023D5B0(arg0, pvVar1, 0x20);
            temp_v0 = temp_v0 + 1;
            pvVar1 = (void *)((s32)pvVar1 + 0x20);
        } while (temp_v0 < *(s32 *)(arg1 + 0x20));
    }
}

void writeEventGroupHeader(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 2) {
            s32 header[4];
            header[0] = *(s32 *)((s32)group + 8);
            header[1] = 0;
            header[2] = 0;
            header[3] = 0;
            func_0023D5B0(output, header, 0x10);
        }
    }
}

void func_0023E228(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xA) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x30);
            }
        }
    }
}

void func_0023E2B0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xB) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x20);
            }
        }
    }
}

void func_0023E338(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xD) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x28);
            }
        }
    }
}

void func_0023E3C0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xE) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x2C);
            }
        }
    }
}

void func_0023E448(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0xF) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x2C);
            }
        }
    }
}

void func_0023E4D0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x17) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x2C);
            }
        }
    }
}

void func_0023E558(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x1B) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x28);
            }
        }
    }
}

void func_0023E5E0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x10) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x18);
            }
        }
    }
}

void func_0023E668(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x11) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x24);
            }
        }
    }
}

void writeEventGroupMetadata(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        u8 header[8];
        header[0] = *(u8 *)group;
        header[1] = *(u8 *)((s32)group + 4);
        *(u16 *)&header[2] = *(u16 *)((s32)group + 8);
        *(u16 *)&header[4] = *(u16 *)((s32)group + 0x1C);
        header[6] = *(u8 *)((s32)group + 0x1E);
        header[7] = *(u8 *)((s32)group + 0x1F);
        func_0023D5B0(output, header, sizeof(header));
    }
}

void func_0023E770(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    for (group = runtime->groups; group != NULL; group = group->next) {
        if (group->type == 0x19) {
            EvtRuntimeChild *child;
            for (child = group->children; child != NULL; child = child->next) {
                func_0023D5B0(output, *(void **)((s32)child + 0x2C), 0x40);
            }
        }
    }
}

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

s32 func_00241878(s32 eventId, s32 variation) {
    s32 sequenceId;

    sequenceId = 0xC7;
    if (eventId != 0x31F) {
        sequenceId = eventId - 0x259;
        if (eventId >= 0x320) {
            sequenceId = (eventId < 0x384) ? (eventId - 0x258) : (eventId - 0x29E);
        }
    }
    return ((sequenceId + 0x100) << 0x10) + variation;
}

s32 evtPreloadBgm(s32 id) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, 0);
    func_002E9340(sound);
    return sound;
}

s32 evtIsBgmLoaded(s32 id) {
    if ((u32)(id - 0x258) >= 0x100) {
        return 1;
    }
    return func_002E92C0(func_00241878(id, 0)) == 1;
}

s32 evtPlayBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, fade);
    func_003003F0("Event BGM play :%08X\n", sound);
    func_002E8D10(sound);
    return sound;
}

void evtTransitionBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return;
    }
    sound = -1;
    if (fade >= 0) {
        sound = func_00241878(id, fade);
    }
    func_003003F0("Event BGM trans :%08X\n", sound);
    func_002E9758(sound);
}

s32 evtFadeInBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, fade);
    func_003003F0("Event BGM fade in play :%08X\n", sound);
    func_002E9690(sound);
    return sound;
}

s32 func_00241A50(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, fade);
    func_002E96D8(sound);
    return sound;
}

s32 evtSetBgmVolumePan(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, fade);
    soundSetSequenceVolumePan(sound, 0x7F, 0x3F);
    return sound;
}

s32 func_00241AE8(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = func_00241878(id, fade);
    func_002E8DD0(sound);
    return sound;
}

void evtFormatTaskName(s32 arg0, void *arg1) {
    func_003014F0(arg1, D_003BC360, arg0);
}

s32 evtFindTaskById(u32 arg0) {
    u8 temp_v0[32];

    evtFormatTaskName(arg0, temp_v0);
    return kwlnTaskGetTaskByName(temp_v0);
}

s32 func_00241B80(u32 arg0) {
    s32 task;

    task = evtFindTaskById(arg0);
    if (task != 0) {
        return *(s32 *)(func_00101A70(task) + 4);
    } else {
        return -1;
    }
}

s32 evtGetTaskData(u32 arg0) {
    s32 task;

    task = evtFindTaskById(arg0);
    if (task != 0) {
        return (s32)func_00101A70(task);
    }
    return task;
}

INCLUDE_ASM(const s32, "game/code_00235270", func_00241BF0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241CA0);

INCLUDE_ASM(const s32, "game/code_00235270", func_00241D28);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF80);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF88);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF8C);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBF90);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFA0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFA8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFB0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFB8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFC0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFC8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFD0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFD8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFE0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFE8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFF0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BBFF8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC000);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC008);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC010);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC018);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC020);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC028);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC030);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC038);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC040);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC048);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC050);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC058);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC060);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC068);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC070);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC078);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC080);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC088);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC090);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC098);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC0F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC100);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC108);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC110);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC118);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC120);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC128);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC130);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC138);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC140);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC148);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC150);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC158);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC160);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC168);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC170);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC178);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC180);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC188);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC190);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC198);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC1F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC200);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC208);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC210);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC218);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC220);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC228);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC230);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC238);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC240);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC248);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC250);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC258);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC260);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC270);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC278);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC280);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC288);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC290);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC298);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2A8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2B0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2B8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2C0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2C8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2D0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2D8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2E0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2E8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2F0);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC2F8);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC300);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC308);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC310);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC318);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC320);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC328);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC330);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC338);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC340);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC348);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC350);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC358);

INCLUDE_SDATA(const s32, "game/code_00235270", D_003BC360);

