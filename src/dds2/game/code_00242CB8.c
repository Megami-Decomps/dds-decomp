#include "common.h"

extern s64 func_001018B0(u64);

extern u64 func_0010D650(u64);

extern s32 D_00435DD0;

extern u32 D_004371F8;

extern u32 D_00437200;

extern u64 func_00101958(void);

void func_00101968(s32 arg0, s32 arg1);

s32 func_0010D8C8(void);

char *func_0010D7D0(s32 idx);

void func_0010D818(s32 value);

extern char D_00422050[];

void *func_00328D68(s32 size);

void initializeEventVisualData(s32 arg0);

void func_00101950(s32 arg0, void *arg1);

void func_00243430(void);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242CB8);

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00421FE8);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00242D10);

u32 func_00242DE0(void) {
    s32 v0;
    s32 v1;

    v0 = func_0010D8C8();
    if (v0 == 0) {
        return 1;
    }
    if (*(s32 *)(v0 + 0xe4) == 0) {
        func_0035B6E0(D_00422050);
        return 1;
    }
    v1 = evtCreateTask(0x2afe, func_0010D7D0(0));
    func_00101968(*(s32 *)(v0 + 0xe4), v1);
    func_0010D818(v1);
    return 1;
}

u32 func_00242E70(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE28(temp_v0);
    }
    return 1;
}

u32 func_00242EB8(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE50(temp_v0);
    }
    return 1;
}

u32 func_00242F00(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_001018B0(temp_v0);
    if (temp_v1 != 0) {
        func_0024FE80(temp_v0);
    }
    return 1;
}

u32 func_00242F48(void) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    func_00135568(0);
    func_00135578(0x80);
    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    temp_v2 = func_0010D650(2);
    func_001354F0(temp_v0, temp_v1, temp_v2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00242CB8", D_00422050);

u32 func_00242FB0(void) {
    s64 v;

    v = func_0010D650(1);
    if (v < -255) {
        func_0010AE38("warning : SET_SKY_A alpha < -255\n");
        v = -255;
    }
    if (v > 255) {
        func_0010AE38("warning : SET_SKY_A alpha > 255\n");
        v = 255;
    }
    func_002500E8(func_0010D650(0), v);
    return 1;
}

u32 func_00243028(void) {
    func_002500E0(1);
    return 1;
}

u32 func_00243048(void) {
    func_002500E0(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243068);

u32 func_002430C0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    evtPlayBgm(temp_v0, temp_v1);
    return 1;
}

u32 func_00243100(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    evtTransitionBgm(temp_v0, temp_v1);
    return 1;
}

u32 func_00243140(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CE68(temp_v0, temp_v1);
    return 1;
}

u32 func_00243180(void) {
    func_003425B0();
    return 1;
}

u32 func_002431A0(void) {
    func_003425D8();
    return 1;
}

u32 func_002431C0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    evtSetBgmVolumePan(temp_v0, temp_v1);
    return 1;
}

u32 func_00243200(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025CF00(temp_v0, temp_v1);
    return 1;
}

u32 func_00243240(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D650(0);
    temp_v1 = func_0010D650(1);
    func_0025D0B8(temp_v0, temp_v1);
    return 1;
}

u32 func_00243280(void) {
    effInitCh72Id();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002432A0);

typedef struct {
    u8 pad00[0xA40];
    u8 flags;          /* 0xA40 */
    u8 phase;          /* 0xA41 */
    u8 padA42[2];      /* DDS2 clears +0xA43, unlike the DDS1 phase byte. */
    u32 phaseCounter;  /* 0xA44 */
} SolarWorldState;

void func_00243310(void) {
    *(u8 *)(D_00435DD0 + 0xa43) = 0;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243320);

s32 func_00243330(void) {
    s32 v;

    v = ((SolarWorldState *)D_00435DD0)->phase;
    if (v >= 9) {
        v = 8 - (v & 7);
    }
    return v;
}

u8 func_00243358(void) {
    return ((SolarWorldState *)D_00435DD0)->phase;
}

void func_00243368(u8 phase) {
    ((SolarWorldState *)D_00435DD0)->phase = phase & 0xf;
    ((SolarWorldState *)D_00435DD0)->phaseCounter = 0;
}

void func_00243380(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags | 1;
}

void func_00243398(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfe;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002433B0);

void func_002433E8(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfd;
    D_004371F8 = 0;
}

void func_00243400(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags | 2;
}

void func_00243418(void) {
    ((SolarWorldState *)D_00435DD0)->flags = ((SolarWorldState *)D_00435DD0)->flags & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243430);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243568);

void *func_002436A8(s32 arg0) {
    s32 p;

    p = (s32)func_00328D68(0x104);
    initializeEventVisualData(p);
    func_002437F0((u32 *)p);
    func_00101950(arg0, (void *)p);
    return (void *)func_00243430;
}

void func_00243700(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    releaseSolarNoiseSprite(temp_v0);
    func_00328E48(temp_v0);
    D_00437200 = 0;
}

INCLUDE_ASM(const s32, "game/code_00242CB8", func_00243740);

INCLUDE_ASM(const s32, "game/code_00242CB8", func_002437B8);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371F8);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_004371FC);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437200);

INCLUDE_SDATA(const s32, "game/code_00242CB8", D_00437208);

