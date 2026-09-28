#include "common.h"

extern s32 func_00264B58(void);

extern s64 func_0026C768(void);

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern s32 kwlnFadeIsActive(void);

extern s32 func_00101958();
extern void func_002619A8(s32, s32);
extern void func_0025FD78(s32);
extern void func_00297320(s32);
extern void func_00297970(s32);
extern s32 D_00435DD0;
extern s32 D_003CE148[];
extern u8 D_003CE4EC[];
extern void func_00297220(s32, u32);
extern s32 func_00261480(s32, s32);

extern void func_002C42B0(s32, s32);

extern u8 D_003CE498[];
extern s32 D_00435E48;
extern char D_00437840[];
extern s32 D_003C9A20[];
extern void func_0026C918();
extern s32 func_0035C860(char *, const char *, ...);
extern void func_0026CA60();
extern void func_0026CA80();
extern void func_00260020();
extern s32 func_0025FE70();
extern void func_002B86E8();
extern s32 D_003CE14C[];
extern u8 D_003CE620[];
extern u8 D_003CE400[];
extern s32 func_00297898();
extern s32 mdlFlagTest();
extern void mdlFlagSet();
extern void func_0026C5B8();
extern u8 D_003CE1A8[];
typedef struct EvtSlot {
    u32 threshold;
    s32 flag;
    struct {
        u8 kind;
        s32 id;
    } sub[8];
} EvtSlot;
extern s32 func_002C5498();
extern u8 D_003CE604[];
extern u16 D_003CE3F8[];
extern void func_00261670();
extern void func_00294930();
extern void func_00298648();
extern void func_002C42C0();

extern void func_00297240(s32, u32);

extern void func_0026C900(void);

extern u8 D_003CE4B4[];

extern void func_00297200(s32, u32);

extern void func_00295D38();

extern u8 D_003CE4D0[];

extern u8 D_003CE508[];

extern s32 func_00261B98(s32);

extern u8 D_003CE690[];

extern void func_002971C0(s32, u32);

extern void func_002958B0();

s32 func_00261E10(void) {
    s32 fadeActive = kwlnFadeIsActive();

    if (fadeActive != 0) {
        return 0;
    }
    return func_0026C768() == 0;
}

void evtInstallStateTable(s32 event) {
    if (*(s32 *)(event + 0x94) == 2) {
        *(s32 *)(event + 0x5c) = (s32)D_003CE498;
        func_002C42B0(event + 0x58, (s32)(D_003CE498 + 0x118));
    }
}

s32 func_00261E80(void) {
    s32 context = func_00101958();
    s32 record;
    s32 slot;
    func_0026CA60(0);
    func_0026CA80(0, 2);
    if (*(s32 *)(context + 0x7c) == 0) {
        func_00260020(context);
    }
    record = *(s32 *)(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18) + 0x30);
    slot = func_0025FE70(context);
    *(s32 *)(context + 0xb0) = *(s32 *)(D_00435DD0 + 0x3c);
    *(u16 *)(record + 0x12) = slot;
    *(u16 *)(context + 0xa6) = slot;
    return 1;
}

s32 func_00261F08(void) {
    s32 context = func_00101958();
    if (*(s32 *)(context + 0xc0) == 1) {
        func_00297240(context, 4);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00261F48);

s64 func_00262190(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSync(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableB(s32 event) {
    if (*(s32 *)(event + 0x94) == 1) {
        *(s32 *)(event + 0x5c) = (s32)D_003CE4B4;
        func_002C42B0(event + 0x58, (s32)(D_003CE4B4 + 0xfc));
    }
}

s32 func_00262270(void) {
    s32 context = func_00101958();
    if (*(s32 *)(context + 0xc0) == 1) {
        func_00261670(context);
    }
    return 1;
}

s32 evtSelectStateAction(void) {
    s32 context = func_00101958();
    s32 list;
    if (*(s32 *)(context + 0xc0) == 5) {
        func_00297240(context, 3);
    } else if (*(s32 *)(context + 0xc0) == 7) {
        func_00297240(context, 9);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_00295D38;
        func_00297200(list, 0xa);
    }
    *(u16 *)(context + 0xc4) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262330);

s64 func_00262598(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSyncB(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableC(s32 event) {
    if (*(s32 *)(event + 0x94) == 1) {
        *(s32 *)(event + 0x5c) = (s32)D_003CE4D0;
        func_002C42B0(event + 0x58, (s32)(D_003CE4D0 + 0xe0));
    }
}

s32 func_00262678(void) {
    s32 context = func_00101958();
    if (*(s32 *)(context + 0xc0) == 1) {
        func_002619A8(context, 1);
    }
    return 1;
}

s32 evtSelectStateActionB(void) {
    s32 context = func_00101958();
    s32 list;
    if (*(s32 *)(context + 0xc0) == 5) {
        func_00297240(context, 3);
    } else if (*(s32 *)(context + 0xc0) == 7) {
        func_00297240(context, 9);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_00295D38;
        func_00297200(list, 0xa);
    }
    *(u16 *)(context + 0xc4) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262740);

s64 func_002629A8(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00262A00(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void func_00262A48(s32 event) {
    if (*(s32 *)(event + 0x94) == 1) {
        *(s32 *)(event + 0x5c) = (s32)D_003CE4EC;
        func_002C42B0(event + 0x58, (s32)(D_003CE4EC + 0xc4));
    }
}

s32 func_00262A88(void) {
    s32 context = func_00101958();
    if (*(s32 *)(context + 0xc0) == 1) {
        func_002619A8(context, 3);
    }
    return 1;
}

s32 func_00262AC8(void) {
    s32 context = func_00101958();
    s32 list;
    if (*(s32 *)(context + 0xc0) == 5) {
        func_00297240(context, 3);
    } else if (*(s32 *)(context + 0xc0) == 7) {
        func_00297240(context, 9);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_00295D38;
        func_00297200(list, 0xa);
    }
    *(u16 *)(context + 0xc4) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262B50);

s64 func_00262DB8(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00262E10(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

void evtInstallStateTableD(s32 event) {
    if (*(s32 *)(event + 0x94) == 2) {
        *(s32 *)(event + 0x5c) = (s32)D_003CE508;
        func_002C42B0(event + 0x58, (s32)(D_003CE508 + 0xa8));
    }
}

s32 evtEnableStateFlag(void) {
    s32 context = func_00101958();
    if (*(s32 *)(context + 0xc0) == 1 && !func_00261B98(context)) {
        *(s32 *)(context + 0x94) = 2;
    }
    return 1;
}

s32 func_00262EF0(void) {
    s32 context = func_00101958();
    s32 list;
    if (*(s32 *)(context + 0xc0) == 5) {
        func_00297240(context, 3);
    } else if (*(s32 *)(context + 0xc0) == 7) {
        func_00297240(context, 9);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_00295D38;
        func_00297200(list, 0xa);
    }
    *(u16 *)(context + 0xc4) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00262F78);

s64 func_00263180(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002631D8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 func_00263220(u32 index) {
    s32 expected;
    s32 delta;
    if (index >= 8) {
        return -1;
    }
    expected = D_003CE148[index * 3];
    if (expected == 0) {
        return -1;
    }
    delta = expected - *(s32 *)(D_00435DD0 + 0x1e654);
    if (delta > 0) {
        return delta;
    }
    return 0;
}

s32 func_00263270(void) {
    char text[0x40];
    func_0026C918(0, D_00435E48 + 0x11);
    func_0035C860(text, D_00437840, *(s32 *)(D_00435DD0 + 0x1e654));
    func_0026C918(1, text);
    func_0026C918(2, D_003C9A20[*(s32 *)(D_00435DD0 + 0x1e658)]);
    func_0035C860(text, D_00437840, func_00263220(*(s32 *)(D_00435DD0 + 0x1e658) + 1));
    func_0026C918(3, text);
    if (func_00263220(*(s32 *)(D_00435DD0 + 0x1e658) + 1) >= 0) {
        func_0026C5B8(*(s32 *)(D_00435DD0 + 0x1e658) + 0x1a);
    } else {
        func_0026C5B8(0x21);
    }
    return 1;
}

u32 func_00263378(void) {
    return 1;
}

s64 func_00263380(s32 callback) {
    s32 context = func_00101958();
    s32 *result = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, result, 0, callback);
    if (state == 0) {
        if (func_0026C768() == 0) {
            func_002C42C0(result, D_003CE498);
        }
        return 0;
    }
    return state;
}

s64 func_002633F0(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00263448(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263490);

u32 func_002635E0(void) {
    return 1;
}

s64 func_002635E8(s32 callback) {
    s32 context = func_00101958();
    s32 *result = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, result, 0, callback);
    if (state == 0) {
        if (func_0026C768() == 0) {
            func_002C42C0(result, D_003CE498);
        }
        return 0;
    }
    return state;
}

s64 func_00263658(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297320(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002636B0(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

u32 func_002636F8(void) {
    s32 temp_v0;

    temp_v0 = func_00101958();
    *(u32 *)(temp_v0 + 0xb8) = 0;
    func_002B8988(*(u32 *)(*(s32 *)(temp_v0 + 0x7c) + 0x18));
    return 1;
}

s64 evtQueryStateProgress(s32 callback) {
    s32 context = func_00101958();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (func_0026C768() == 0) {
                s32 count = *(s32 *)(context + 0xb8);
                if ((f32)count < 20.0f) {
                    *(s32 *)(context + 0xb8) = count + 1;
                } else {
                    func_002C42B0(window, D_003CE690);
                }
            }
        }
        return 0;
    }
    return state;
}

s64 func_002637E0(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00294930(context, *(s32 *)(context + 0xb8));
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 evtSetupDispatchSyncE(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 func_00263880(void) {
    s32 context = func_00101958();
    *(s32 *)(context + 0x90) = 1;
    func_00261480(-1, context);
    return 1;
}

s32 evtAdvanceStateStage(void) {
    s32 context = func_00101958();
    s32 list;
    if (*(s32 *)(context + 0xc0) == 0xa) {
        *(u16 *)(context + 0xc4) = 0xa;
        func_00297240(context, 6);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_002958B0;
        func_002971C0(list, 0);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263920);

s64 func_00263B98(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00298648(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00263BF0(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263C38);

s32 func_00263D40(void) {
    s32 context = func_00101958();
    s32 stage = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x7c) + 0x18) + 0x1c) + 0x60) + 1;
    s32 id = *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x80) + 0x18) + 0x1c) + 0x64);
    if (stage == 4) {
        if (*(u8 *)(id + D_00435DD0 + 0x1340) == 0) {
            func_002B86E8(*(s32 *)(*(s32 *)(context + 0x80) + 0x18));
        }
        if (*(s32 *)(*(s32 *)(*(s32 *)(context + 0x80) + 0x18) + 0x20) == 0) {
            *(s32 *)(context + 0x94) = 2;
        }
    }
    return 1;
}

s32 func_00263DD0(void) {
    s32 context = func_00101958();
    s32 list;
    switch (func_00297898(context)) {
    case 6:
        return 1;
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        return 0;
    case 8:
        func_00297240(context, 6);
        list = *(s32 *)(*(s32 *)(context + 0x80) + 0x18);
        *(s32 *)(list + 0x2c) = (s32)func_002958B0;
        func_002971C0(list, 0);
        *(u16 *)(context + 0xc4) = 0xa;
        return 0;
    default:
        return 0;
    }
}

void func_00263E60(void) {
    s32 context = func_00101958();
    func_00297240(context, 8);
    func_00297220(*(s32 *)(*(s32 *)(context + 0x80) + 0x18), 10);
    *(u8 *)(context + 0x389) = 1;
}

void func_00263EB0(s32 obj) {
    s32 *record = (s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(obj + 0x80) + 0x18) + 0x1c) + 0x60);
    if (func_002C5498(record[1])) {
        *(s32 *)(D_00435DD0 + 0xa50) += record[0] * *(s32 *)(obj + 0x90);
    }
}

s32 func_00263F10(s32 id) {
    u32 i;
    for (i = 0; i < 3; i++) {
        if (D_003CE3F8[i] == id) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263F50);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00263FB0);

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264120);

s64 func_00264240(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    if (*(s8 *)(context + 0x389) == 1) {
        func_00297970(context);
    } else {
        func_00298648(context);
    }
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_002642B8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 func_00264300(void) {
    s32 context = func_00101958();
    func_0026C948(1);
    switch (*(s32 *)(context + 0x94)) {
    case 1:
        func_0026C5B8(5);
        break;
    case 2:
        func_0026C5B8(6);
        break;
    }
    return 1;
}

s32 func_00264378(void) {
    s32 context = func_00101958();
    switch (*(s32 *)(context + 0x94)) {
    case 1:
        func_00297240(context, 6);
        break;
    case 2:
        if (*(s32 *)(context + 0x5c) != (s32)D_003CE498) {
            func_00297240(context, 5);
        }
        break;
    }
    *(s32 *)(context + 0x94) = 0;
    return 1;
}

s64 func_002643F8(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101958();
    piVar3 = (s32 *)(temp_v0 + 0x58);
    temp_v1 = func_002C4038(temp_v0 + 0xc, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0026C768(), temp_v1 == 0)) {
            func_002C42C0(piVar3, *(u32 *)(temp_v0 + 0x5c));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264480);

s64 func_002646C8(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264710);

u32 func_00264848(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00264850);

s64 func_00264AB8(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264B10(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 func_00264B58(void) {
    s32 i = 0;
    while (func_00263220(*(s32 *)(D_00435DD0 + 0x1e658) + i + 1) == 0) {
        mdlFlagSet(D_003CE14C[(*(s32 *)(D_00435DD0 + 0x1e658) + i) * 3 + 3]);
        i++;
    }
    *(s32 *)(D_00435DD0 + 0x1e658) += i;
    return i;
}

u32 func_00264C00(void) {
    u8 temp_v0;
    s32 temp_v1;

    temp_v1 = func_00101958();
    temp_v0 = func_00264B58();
    *(u8 *)(temp_v1 + 0xcc) = temp_v0;
    if ((*(s32 *)(temp_v1 + 200) == 0) && (*(s8 *)(temp_v1 + 0xcd) == '\x01')) {
        func_0026C5B8(0x22);
    }
    return 1;
}

u32 func_00264C58(void) {
    return 1;
}

s64 func_00264C60(s32 callback) {
    s32 context = func_00101958();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (func_0026C768() == 0) {
                func_002C42C0(window, D_003CE604);
            }
        }
        return 0;
    }
    return state;
}

s64 func_00264CE0(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264D38(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

s32 func_00264D80(s32 context) {
    u8 *entry = D_003CE400;
    u32 i;
    for (i = 0; i < 1; i++, entry += 8) {
        if ((u32)(*(s32 *)(D_00435DD0 + 0x1e658) + 1) >= (u32)*(s8 *)entry) {
            u32 flag = *(u32 *)(entry + 4);
            if (mdlFlagTest(flag) == 0) {
                mdlFlagSet(flag);
                func_0026C5B8(*(u16 *)(entry + 2));
                return 1;
            }
        }
    }
    return 0;
}

s32 func_00264E18(void) {
    char text[0x40];
    if (*(s8 *)(func_00101958() + 0xcc) > 0) {
        func_0026C918(0, D_00435E48 + 0x11);
        func_0035C860(text, D_00437840, D_003CE148[*(s32 *)(D_00435DD0 + 0x1e658) * 3]);
        func_0026C918(1, text);
        func_0026C918(2, D_003C9A20[*(s32 *)(D_00435DD0 + 0x1e658)]);
        if (func_00263220(*(s32 *)(D_00435DD0 + 0x1e658) + 1) >= 0) {
            func_0026C5B8(0x24);
        } else {
            func_0026C5B8(0x25);
        }
    }
    return 1;
}

u32 func_00264EF8(void) {
    return 1;
}

s64 func_00264F00(s32 callback) {
    s32 context = func_00101958();
    s32 *window = (s32 *)(context + 0x58);
    s64 state = func_002C4038(context + 0xc, window, 0, callback);
    if (state == 0) {
        if (*window == 0) {
            if (func_0026C768() == 0) {
                if (func_00264D80(context) == 0) {
                    func_002C42C0(window, D_003CE620);
                }
            }
        }
        return 0;
    }
    return state;
}

s64 func_00264F98(s32 callback) {
    s32 context = func_00101958();
    func_0025FD78(context);
    func_00297970(context);
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 1, callback);
}

s64 func_00264FF0(s32 callback) {
    s32 context = func_00101958();
    func_0026C900();
    return func_002C4038(context + 0xc, (s32 *)(context + 0x58), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_00261E10", func_00265038);

s32 func_00265130(s32 count) {
    s32 active = 0;
    u32 i;
    for (i = 0; i < 8; i++) {
        if (((EvtSlot *)D_003CE1A8)[i].flag != 0) {
            active++;
        }
    }
    return count + 1 == active;
}

INCLUDE_RODATA(const s32, "game/code_00261E10", D_00424D08);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437840);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437848);

INCLUDE_SDATA(const s32, "game/code_00261E10", D_00437850);

