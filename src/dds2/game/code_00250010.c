#include "common.h"

extern u32 D_004373CC;

extern u16 D_004373C8;

extern void func_00135588(s32 arg0);

extern s16 func_00135598(void);

extern u16 D_00438FB0;

extern u16 D_00438FB2;

extern s16 D_00438FB4;

extern s16 D_00438FB6;

extern void func_00134A18(void);

extern void func_0012BC38(s32 arg0);

extern void func_0012D3E0(void);

extern void sdfAppendPacket(s32 arg0, s32 arg1);

extern s32 func_0033D810();

typedef struct {
    s16 unk0;
    s8 unk2;
    u8 pad3[7];
} EvtTblEntry; /* 0xA bytes */

extern EvtTblEntry D_003C9730[];

extern s8 D_003C9732[];

extern void *func_00101958();

extern s32 func_001979E0(void);

typedef struct EvtRuntimeChild {
    u8 pad00[2];
    u16 unk02;
    u8 pad04[6];
    u16 unk0A;
    u8 pad0C[6];
    u16 unk12;
    u8 pad14[0x18];
    void *payload; /* 0x2C: serialized child data */
    struct EvtRuntimeChild *next; /* 0x30 */
} EvtRuntimeChild;

typedef struct EvtRuntimeGroup {
    s32 type;
    u8 value04;
    u8 pad05[3];
    s32 value08;
    u8 pad0C[0x10];
    u16 value1C;
    u8 value1E;
    u8 value1F;
    u8 pad20[0x34];
    EvtRuntimeChild *children;
    u8 pad58[0x24];
    struct EvtRuntimeGroup *next;
} EvtRuntimeGroup;

typedef struct EvtRuntime {
    u8 pad00[0x2034];
    EvtRuntimeGroup *groups;
} EvtRuntime;

extern void func_003421E8(s32 arg0);

extern s32 func_00342168(s32 arg0);

extern void func_00341BB8(s32 arg0);

extern void func_0035B6E0(char *fmt, ...);

extern void func_00342538(s32 arg0);

extern void func_00342580(u32 arg0);

extern void sndSetSequenceVolumePan(s32 arg0, s32 arg1, s32 arg2);

extern void func_00341C78(u32 arg0);

extern s32 (*D_003C9928[])(s32, s32, void *);

extern char D_004373C0[];
extern void func_0024FF80();
extern void func_0024FFC8();
extern u8 *func_0024FE98();
extern void func_0024FEC0();

void evtCreateTask(s32 arg0, s32 arg1) {
    s32 data = (s32)func_0024FE98();
    func_0024FEC0(data, arg1);
    kwlnTaskCreate(D_004373C0, arg0, 1, 1, (s32)func_0024FF80, (s32)func_0024FFC8, data);
}

extern s32 kwlnTaskCreate(char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern char D_004373C0[];
extern void func_0024FF80();
extern void func_0024FFC8();
extern u8 *func_0024FE98();

void evtCreateTaskWithValue(s32 arg0, s32 arg1) {
    u8 *data = func_0024FE98();
    *(s32 *)(data + 4) = arg1;
    kwlnTaskCreate(D_004373C0, arg0, 1, 1, (s32)func_0024FF80, (s32)func_0024FFC8, (s32)data);
}

void evtSetSkyOverlayEnabled(u32 arg0) {
    D_004373CC = arg0;
}

void evtStartSkyAlphaTransition(s32 duration, s32 target) {
    s16 current;

    current = func_00135598();
    if (current != target) {
        if (duration == 0) {
            func_00135588(target);
            D_004373C8 = 0;
        } else {
            D_00438FB2 = duration;
            D_00438FB4 = current;
            D_00438FB6 = target;
            D_004373C8 = 1;
            D_00438FB0 = 0;
        }
    }
}

u16 evtIsSkyAlphaTransitionActive(void) {
    return D_004373C8;
}

void evtUpdateSkyAlphaTransition(void) {
    if (D_004373C8 != 0) {
        D_00438FB0 += 1;
        func_00135588(D_00438FB4 + (s32)((f32)(D_00438FB6 - D_00438FB4) * ((f32)D_00438FB0 / (f32)D_00438FB2)));
        if (D_00438FB0 >= D_00438FB2) {
            D_004373C8 = 0;
        }
    }
}

s32 evtUpdateSkyTask(void) {
    evtUpdateSkyAlphaTransition();
    func_00134A18();
    if (D_004373CC != 0) {
        func_0012BC38(0x53);
        func_0012D3E0();
    }
    return 0;
}

void evtResetSkyTaskFlags(void) {
    D_004373C8 = 0;
    D_004373CC = 0;
}

extern char D_004373D0[];

void evtDestroySkyTask(void) {
    s32 task = func_00101740(D_004373D0);
    if (task != 0) {
        kwlnTaskDestroyWithHierarchy(task, 1);
    }
}

extern void func_00135568();
extern void func_00135578();
extern void func_00135588();
extern void fldSetFadeTarget();

void evtCreateSkyTask(void) {
    func_00135568(0);
    func_00135578(0x80);
    func_00135588(0);
    fldSetFadeTarget(0, 1, 0);
    kwlnTaskCreate(D_004373D0, 0x2B0E, 1, 1, (s32)evtUpdateSkyTask, (s32)evtResetSkyTaskFlags, 0);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002502E0);

extern s32 kwlnTaskCreate(char *name, s32 arg1, s32 arg2, s32 arg3, s32 update, s32 destroy, s32 data);
extern s32 func_002502E0();
extern char D_00423380[]; /* "FrameVar" */

void evtCreateFrameVariableTask(void) {
    kwlnTaskCreate(D_00423380, 0x2AF9, 1, 1, (s32)func_002502E0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250338);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423380);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423390);

s32 func_00250508(s32 arg0, s32 arg1, s32 arg2) {
    sdfAppendPacket(arg0, func_0033D810(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250558);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250718);

s32 func_00250880(s32 arg0, s32 arg1, s32 arg2) {
    sdfAppendPacket(arg0, func_0033D810(arg1, arg2, 0xFEFFFF, 0, "VALUE CHANGE."));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004233F0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423400);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423410);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423428);

INCLUDE_ASM(const s32, "game/code_00250010", func_002508D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250A08);

s32 mnuDrawFrameChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "FRAME CHANGE."));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00250BA0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250D68);

INCLUDE_ASM(const s32, "game/code_00250010", func_00250F20);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251180);

extern char D_004374D0[];

s32 evtDrawStringEntry(s32 list, s32 x, s32 y, u8 *ctx) {
    s32 text = *(s32 *)(ctx + 0x22C4);
    if (text == 0) {
        return 0;
    }
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002512B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251340);

extern char D_004374E0[];
extern char D_004374D0[];

s32 func_00251440(s32 list, s32 x, s32 y, u8 *ctx) {
    char text[16];
    func_0035C860(text, D_004374E0, (s32)ctx + 0x22D4, (s32)ctx + 0x22E0);
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_004374D0, text));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002514C8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002515C8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423668);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423688);

INCLUDE_ASM(const s32, "game/code_00250010", func_002517E0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251868);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251B20);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251DE8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00251ED0);

INCLUDE_ASM(const s32, "game/code_00250010", func_002520E8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002521C8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423AF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423B70);

INCLUDE_ASM(const s32, "game/code_00250010", func_00252378);

INCLUDE_ASM(const s32, "game/code_00250010", func_002534B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253590);

INCLUDE_ASM(const s32, "game/code_00250010", func_002537A8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253938);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253A98);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253C08);

extern char D_00423EC0[]; /* "MESSAGE MENU (MESMAX %3d)" */

s32 func_00253D08(s32 list, s32 x, s32 y, u8 *ctx) {
    sdfAppendPacket(list, func_0033D810(x, y, 0xFEFFFF, 0, D_00423EC0, itfMesGetEntryCount(*(s32 *)(*(u8 **)(ctx + 8) + 0x104))));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EC0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EE0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423EF0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F00);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F10);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F20);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F30);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F40);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F50);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F60);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F70);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423F80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253D80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00253FF8);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00423FE0);

INCLUDE_RODATA(const s32, "game/code_00250010", jtbl_00423FF0);

s32 mnuDrawCutFlagLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "CUTFLAG MENU"));
    return 2;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424020);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254250);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254458);

s32 func_00254620(s32 *arg0) {
    return D_003C9730[*arg0].unk0 != 0;
}

s32 mnuGetSelectedTableValue(s32 arg0) {
    return D_003C9732[*(s32 *)(arg0 + 0x23C8) + *(s32 *)(*(s32 *)(arg0 + 0x2308)) * 10];
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254678);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424080);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424090);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254940);

s32 mnuDrawMotionChangeLabel(s32 target, s32 x, s32 y) {
    sdfAppendPacket(target, func_0033D810(x, y, 0xFEFFFF, 0, "MOTION CHANGE MENU"));
    return 2;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00254CE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00254F80);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255360);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255538);

void func_00255648(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00255650);

INCLUDE_ASM(const s32, "game/code_00250010", func_00255818);

void func_002560A8(void) {
}

INCLUDE_ASM(const s32, "game/code_00250010", func_002560B0);

INCLUDE_ASM(const s32, "game/code_00250010", func_002566F8);

INCLUDE_ASM(const s32, "game/code_00250010", func_002567A8);

void evtSetRuntimeCommandValues(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    *(s32 *)(arg0 + 0x23E4) = arg1;
    *(s32 *)(arg0 + 0x23E8) = arg2;
    *(s32 *)(arg0 + 0x23EC) = arg3;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241A0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256898);

INCLUDE_ASM(const s32, "game/code_00250010", func_002569D0);

INCLUDE_RODATA(const s32, "game/code_00250010", D_004241C0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256AE0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00256CF0);

INCLUDE_ASM(const s32, "game/code_00250010", evtSynchronizeSelectedEntry);

s32 func_002570B8(void) {
    void *runtime;

    runtime = func_00101958();
    if (func_001979E0() == 0) {
        *(s32 *)((u8 *)runtime + 0x228C) = 0;
        return -1;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424210);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424220);

INCLUDE_RODATA(const s32, "game/code_00250010", D_00424230);

INCLUDE_ASM(const s32, "game/code_00250010", func_002570F8);

INCLUDE_ASM(const s32, "game/code_00250010", func_00257910);

INCLUDE_ASM(const s32, "game/code_00250010", func_002582D0);

INCLUDE_ASM(const s32, "game/code_00250010", func_00258700);

s32 evtDispatchActionByIndex(s32 index, s32 x, s32 y, void *runtime) {
    s32 mode = *(s32 *)((u8 *)runtime + 0x2280);
    if (mode == 11 && index != mode) {
        return 0;
    }
    return D_003C9928[index](x, y, runtime);
}

void func_002588A0(s32 arg0, s32 arg1, s32 arg2) {
    func_0036A420();
}

s32 func_002588B8(EvtRuntime *runtime) {
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

s32 func_00258920(EvtRuntime *runtime) {
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

s32 func_00258988(EvtRuntime *runtime) {
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

s32 func_002589F0(EvtRuntime *runtime) {
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

s32 func_00258A58(EvtRuntime *runtime) {
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

s32 func_00258AC0(EvtRuntime *runtime) {
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

s32 func_00258B28(EvtRuntime *runtime) {
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

s32 func_00258B90(EvtRuntime *runtime) {
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

s32 func_00258BF8(EvtRuntime *runtime) {
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

s32 func_00258C60(EvtRuntime *runtime) {
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

INCLUDE_ASM(const s32, "game/code_00250010", func_00258CC8);

void evtWriteRuntimeHeaderValues(s32 arg0, u8 *arg1) {
    s32 buffer[4];
    buffer[0] = *(s32 *)(arg1 + 0x10);
    buffer[1] = *(s32 *)(arg1 + 0x14);
    buffer[2] = *(s32 *)(arg1 + 0xC);
    buffer[3] = *(s32 *)(arg1 + 0x243C);
    func_002588A0(arg0, buffer, 0x10);
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00259298);

void evtWriteFixedSizeEntries(s32 arg0, u8 *arg1) {
    s32 i;
    u8 *entry;
    i = 0;
    if (*(s32 *)(arg1 + 0x20) > 0) {
        entry = arg1 + 0x24;
        do {
            func_002588A0(arg0, entry, 0x20);
            entry += 0x20;
            i++;
        } while (i < *(s32 *)(arg1 + 0x20));
    }
}

void evtWriteGroupHeader(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    s32 header[4];
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 2) {
            header[0] = group->value08;
            header[1] = 0;
            header[2] = 0;
            header[3] = 0;
            func_002588A0(output, header, 0x10);
        }
    }
}

void func_00259518(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xA) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x30);
            }
        }
    }
}

void func_002595A0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xB) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x20);
            }
        }
    }
}

void func_00259628(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xD) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x28);
            }
        }
    }
}

void func_002596B0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xE) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_00259738(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0xF) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_002597C0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x17) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x2C);
            }
        }
    }
}

void func_00259848(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x1B) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x28);
            }
        }
    }
}

void func_002598D0(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x10) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x18);
            }
        }
    }
}

void func_00259958(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x11) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x24);
            }
        }
    }
}

void evtWriteGroupMetadata(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    u8 record[8];
    for (group = runtime->groups; group != 0; group = group->next) {
        record[0] = *(u8 *)&group->type;
        record[1] = group->value04;
        *(u16 *)(record + 2) = *(u16 *)&group->value08;
        *(u16 *)(record + 4) = group->value1C;
        record[6] = group->value1E;
        record[7] = group->value1F;
        func_002588A0(output, record, 8);
    }
}

void func_00259A60(s32 output, EvtRuntime *runtime) {
    EvtRuntimeGroup *group;
    EvtRuntimeChild *child;
    for (group = runtime->groups; group != 0; group = group->next) {
        if (group->type == 0x19) {
            for (child = group->children; child != 0; child = child->next) {
                func_002588A0(output, (s32)child->payload, 0x40);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_00259AE8);

u16 func_00259FF8(s32 group, s32 index) {
    if (*(s32 *)(*(s32 *)(group + 0x74) + 0x14) == 4) {
        return *(u16 *)(index * 0x10 + *(s32 *)(group + 0x88));
    }
    return *(u16 *)(index * 0x2c + *(s32 *)(group + 0x8c));
}

s16 func_0025A048(s32 group, s32 index) {
    if (*(s32 *)(*(s32 *)(group + 0x74) + 0x14) == 4) {
        return *(s16 *)(index * 0x10 + *(s32 *)(group + 0x88) + 6);
    }
    return *(s16 *)(index * 0x2c + *(s32 *)(group + 0x8c) + 6);
}

u16 func_0025A098(s32 group, s32 index) {
    if (*(s32 *)(*(s32 *)(group + 0x74) + 0x14) == 4) {
        return *(u16 *)(index * 0x10 + *(s32 *)(group + 0x88) + 2);
    }
    return *(u16 *)(index * 0x2c + *(s32 *)(group + 0x8c) + 2);
}

u16 func_0025A0E8(s32 group, s32 index) {
    if (*(s32 *)(*(s32 *)(group + 0x74) + 0x14) == 4) {
        return *(u16 *)(index * 0x10 + *(s32 *)(group + 0x88) + 4);
    }
    return *(u16 *)(index * 0x2c + *(s32 *)(group + 0x8c) + 4);
}

s32 func_0025A138(s32 group, s32 index) {
    if (*(s32 *)(*(s32 *)(group + 0x74) + 0x14) == 4) {
        return *(s32 *)(group + 0x88) + index * 0x10 + 8;
    }
    return *(s32 *)(group + 0x8c) + index * 0x2c + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025A188);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025A280);

INCLUDE_ASM(const s32, "game/code_00250010", func_0025CAF8);

s32 evtEncodeBgmSoundCode(s32 arg0, s32 arg1) {
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

s32 evtPreloadBgm(s32 id) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, 0);
    func_003421E8(sound);
    return sound;
}

s32 evtIsBgmLoaded(s32 id) {
    if ((u32)(id - 0x258) >= 0x100) {
        return 1;
    }
    return func_00342168(evtEncodeBgmSoundCode(id, 0)) == 1;
}

s32 evtPlayBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_0035B6E0("Event BGM play :%08X\n", sound);
    func_00341BB8(sound);
    return sound;
}

extern char D_004247F8[]; /* "Event BGM trans :%08X\n" */
extern s32 evtEncodeBgmSoundCode();
extern void func_00342600();

void evtTransitionBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) < 0x100U) {
        sound = -1;
        if (fade >= 0) {
            sound = evtEncodeBgmSoundCode(id, fade);
        }
        func_0035B6E0(D_004247F8, sound);
        func_00342600(sound);
    }
}

INCLUDE_RODATA(const s32, "game/code_00250010", D_004247F8);

s32 evtFadeInBgm(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_0035B6E0("Event BGM fade in play :%08X\n", sound);
    func_00342538(sound);
    return sound;
}

s32 func_0025CE68(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_00342580(sound);
    return sound;
}

s32 evtSetBgmVolumePan(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    sndSetSequenceVolumePan(sound, 0x7F, 0x3F);
    return sound;
}

s32 func_0025CF00(s32 id, s32 fade) {
    s32 sound;
    if ((u32)(id - 0x258) >= 0x100) {
        return 0;
    }
    sound = evtEncodeBgmSoundCode(id, fade);
    func_00341C78(sound);
    return sound;
}

extern char D_004377D0[];

void evtFormatTaskName(s32 id, char *buffer) {
    func_0035C860(buffer, D_004377D0, id);
}

s32 evtFindTaskById(u32 arg0) {
    u8 temp_v0 [32];

    evtFormatTaskName(arg0, temp_v0);
    return func_00101740(temp_v0);
}

s32 evtGetTaskValueWord(u32 id) {
    s32 task = evtFindTaskById(id);
    if (task == 0) {
        return -1;
    }
    return *(s32 *)((u8 *)func_00101958(task) + 4);
}

void *evtGetTaskData(u32 id) {
    s32 task = evtFindTaskById(id);
    if (task != 0) {
        return func_00101958(task);
    }
    return (void *)task;
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025D008);

extern void effSetCh72Id();

void evtRefreshTaskData(s32 arg0, s32 arg1) {
    u8 *task = evtGetTaskData(arg0);
    s32 resource = func_0025D008(arg0, arg1);
    s32 handle;
    if (resource != 0) {
        if (*(s32 *)(task + 0x38) != 0) {
            func_0032BBB0(*(s32 *)(task + 0x38));
            *(s32 *)(task + 0x38) = 0;
        }
        handle = func_0032C138(resource);
        effSetCh72Id(handle);
        *(s32 *)(task + 0x38) = handle;
    }
}

INCLUDE_ASM(const s32, "game/code_00250010", func_0025D140);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373CC);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004373F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437400);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437408);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437410);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437418);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437420);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437428);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437430);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437438);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437440);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437448);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437450);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437458);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437460);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437468);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437470);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437478);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437480);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437488);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437490);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437498);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004374F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437500);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437508);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437510);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437518);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437520);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437528);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437530);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437538);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437540);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437548);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437550);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437558);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437560);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437568);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437570);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437578);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437580);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437588);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437590);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437598);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004375F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437600);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437608);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437610);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437618);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437620);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437628);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437630);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437638);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437640);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437648);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437650);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437658);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437660);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437668);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437670);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437678);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437680);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437688);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437690);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437698);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376D8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376E8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004376F8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437700);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437708);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437710);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437718);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437720);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437728);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437730);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437738);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437740);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437748);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437750);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437758);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437760);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437768);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437770);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437778);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437780);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437788);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437790);

INCLUDE_SDATA(const s32, "game/code_00250010", D_00437798);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377A8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377B8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C0);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377C8);

INCLUDE_SDATA(const s32, "game/code_00250010", D_004377D0);

