#include "common.h"

/* Particle/effect work layouts shared by the matched functions of this TU.
 * Every effect in ../effect/src/effPCPMisc.c keeps its own allocation size,
 * but the small PCP effects share a common header: a task link area followed
 * by resource handles and spawn parameters. Offsets below were recovered
 * from the matched C functions; fields the C code never touches are padding.
 */
typedef struct {
    u8 pad00[0x10];  /* 0x00 task header */
    u32 unk10;       /* 0x10 resource handle */
    u32 unk14;       /* 0x14 resource handle */
    u32 unk18;       /* 0x18 resource handle */
    u32 unk1C;       /* 0x1C resource released on destroy */
    u32 unk20;       /* 0x20 spawn parameter */
    u32 unk24;       /* 0x24 spawn parameter */
    u8 pad28[0x8];   /* 0x28 */
    u32 unk30;       /* 0x30 resource released on destroy */
    u32 unk34;       /* 0x34 resource released on destroy */
    u32 unk38;       /* 0x38 resource released on destroy */
    u8 pad3C[0x18];  /* 0x3C */
    u32 unk54;       /* 0x54 spawn parameter */
    u8 pad58[0x4];   /* 0x58 */
    u32 unk5C;       /* 0x5C nested work handle */
    u32 unk60;       /* 0x60 resource handle */
    u32 unk64;       /* 0x64 resource handle */
    u8 pad68[0xC];   /* 0x68 */
    u32 unk74;       /* 0x74 optional handle (freed if != 0) */
    u8 pad78[0x4];   /* 0x78 */
    u32 unk7C;       /* 0x7C nested work handle */
    u8 pad80[0x1C];  /* 0x80 */
    u32 unk9C;       /* 0x9C spawn parameter */
    u8 padA0[0x4];   /* 0xA0 */
    u32 unkA4;       /* 0xA4 spawn parameter */
    u32 unkA8;       /* 0xA8 resource released on destroy */
    u32 unkAC;       /* 0xAC resource released on destroy */
    u8 padB0[0x8];   /* 0xB0 */
    u32 unkB8;       /* 0xB8 spawn parameter */
    u32 unkBC;       /* 0xBC mode set through the singleton accessor */
    u8 padC0[0x54];  /* 0xC0 */
    u32 unk114;      /* 0x114 spawn parameter */
    u8 pad118[0x58]; /* 0x118 */
    u32 unk170;      /* 0x170 spawn parameter */
} EffPCPWork;

/* Large charge-style effect work (allocation 0x1354). Only the tail is
 * touched by the matched C functions: a colour initialised to grey plus
 * handles released on destroy.
 */
typedef struct {
    u8 pad000[0x1334]; /* 0x0000 */
    u32 unk1334;       /* 0x1334 spawn parameter */
    u8 pad1338[0x4];   /* 0x1338 */
    u32 unk133C;       /* 0x133C cleared on init */
    u32 unk1340;       /* 0x1340 cleared on init */
    u32 color1344;     /* 0x1344 initialised to grey 0x80808080 */
    u32 unk1348;       /* 0x1348 resource released on destroy */
    u32 unk134C;       /* 0x134C resource released on destroy */
    u32 unk1350;       /* 0x1350 resource released on destroy */
} EffPCPChargeWork;

/* Round-robin selector work behind func_0017ED98: three key/ID pairs plus a
 * counter at 0x108. Each call fires the IDs whose key has caught up.
 */
typedef struct {
    s32 keys[3];    /* 0x00 compared against count */
    u8 pad0C[0xF0]; /* 0x0C */
    s32 ids[3];     /* 0xFC fired through func_0017DCF8 */
    s32 count;      /* 0x108 round-robin counter */
} EffPCPRotateWork;

extern u8 D_003BB04C;

extern EffPCPWork *func_0017D7A8(void);

/* Block `index` of a packed effect parameter set: data + offset table entry. */
extern void *func_00163258(void *data, s32 index);

extern EffPCPWork *D_003BD7FC;

extern void *func_002CFEB8(s32 size);

void func_00177190(EffPCPWork *work) {
    func_00151F00(work->unk1C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001771C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001772F8);

void func_00177308(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177310);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177318);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177418);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177590);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177608);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177700);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177850);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177860);

void func_00177868(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177870);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177990);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177A68);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177AC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177B70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CE0);

void func_00177CE8(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177CF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177E38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00177F90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178028);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178130);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178260);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178270);

void func_00178278(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178280);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178320);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001783E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178448);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178500);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001785E8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001785F8);

void func_00178600(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

void func_00178608(EffPCPChargeWork *work) {
    work->unk133C = 0;
    work->unk1340 = 0;
    work->color1344 = 0x80808080;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178620);

void func_001786C0(EffPCPChargeWork *work) {
    func_001629F0(work->unk134C);
    func_001629F0(work->unk1348);
    func_002D0918(work->unk1350);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001786F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178790);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178AF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178B08);

void func_00178B10(EffPCPChargeWork *work, u32 val) {
    work->unk1334 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178B18);

void func_00178B88(EffPCPWork *work) {
    func_001629F0(work->unk14);
    func_001629F0(work->unk18);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178BC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178C28);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CA8);

void func_00178CB8(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178CC0);

void *func_00178F80(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00178CC0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00178FB8);

void *func_00179010(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00178CC0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179048);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179138);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179148);

void func_00179150(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179158);

void *func_001793F8(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179158(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179430);

void *func_00179488(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179158(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001794C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795C0);

void func_001795C8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001795D0);

void *func_00179890(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_001795D0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001798C8);

void *func_00179920(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_001795D0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179958);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A58);

void func_00179A60(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179A68);

void *func_00179D08(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179A68(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179D40);

void *func_00179D98(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179A68(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179DD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179ED0);

void func_00179ED8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00179EE0);

void *func_0017A078(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179EE0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A0B0);

void *func_0017A108(void) {
    void *work;

    work = func_002CFEB8(0x4c);
    func_00179EE0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A140);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A230);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A240);

void func_0017A248(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A250);

void *func_0017A3E8(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    func_0017A250(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A420);

void *func_0017A478(void) {
    void *work;

    work = func_002CFEB8(0x3c);
    func_0017A250(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A4B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5B0);

void func_0017A5B8(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A5C0);

void *func_0017A5F8(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017A5C0(work);
    return work;
}

void func_0017A630(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017A660(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017A5C0(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A698);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A7F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A800);

void func_0017A808(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017A810);

void *func_0017AAD0(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_0017A810(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AB08);

void *func_0017AB60(void) {
    void *work;

    work = func_002CFEB8(0x94);
    func_0017A810(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AB98);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AC88);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AC98);

void func_0017ACA0(EffPCPWork *work, u32 val) {
    work->unk18 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017ACA8);

void *func_0017ACE0(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017ACA8(work);
    return work;
}

void func_0017AD18(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017AD48(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017ACA8(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AD80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AED0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEE0);

void func_0017AEE8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AEF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017AFB8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B008);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B148);

void func_0017B160(u32 unused, u32 val) {
    D_003BD7FC->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B170);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B180);

void func_0017B218(EffPCPWork *work) {
    func_00186CB8(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B248);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B328);

void func_0017B338(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B340);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B348);

void func_0017B3D8(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B348(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B3F8);

void func_0017B498(EffPCPWork *work) {
    func_00188050(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B4C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B670);

void func_0017B680(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B688);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B690);

void func_0017B720(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B690(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B740);

void func_0017B7F0(EffPCPWork *work) {
    func_00186CB8(work->unk38);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B820);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9C8);

void func_0017B9D8(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017B9E8);

void func_0017BA78(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017B9E8(param0);
}

void func_0017BA98(void) {
    func_0017B9E8();
}

void func_0017BAB0(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BAC8);

void func_0017BBD8(EffPCPWork *work, u32 val) {
    work->unk24 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BBE0);

void func_0017BC90(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017BBE0(param0);
}

void func_0017BCB0(void) {
    func_0017BBE0();
}

void func_0017BCC8(void) {
    func_002CFF98();
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BCE0);

void func_0017BDF8(EffPCPWork *work, u32 val) {
    work->unk34 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BE00);

void func_0017BE90(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017BE00(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BEB0);

void func_0017BF60(EffPCPWork *work) {
    func_00187080(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017BF90);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C158);

void func_0017C168(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C170);

void func_0017C200(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017C170(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C220);

void func_0017C2D0(EffPCPWork *work) {
    func_00187580(work->unk34);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C300);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C4F0);

void func_0017C500(EffPCPWork *work, u32 val) {
    work->unk20 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C508);

void *func_0017C540(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C508(work);
    return work;
}

void func_0017C578(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017C5A8(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C508(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C5E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C798);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C7A8);

void func_0017C7B0(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C7B8);

void *func_0017C7F0(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C7B8(work);
    return work;
}

void func_0017C828(EffPCPWork *work) {
    func_001634D8(work->unk1C);
    func_002CFF98(work);
}

void *func_0017C858(void) {
    void *work;

    work = func_002CFEB8(0x20);
    func_0017C7B8(work);
    return work;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017C890);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA40);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA50);

void func_0017CA58(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CA60);

void func_0017CB88(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_0017CA60(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CBD0);

void func_0017CC28(EffPCPWork *work) {
    s32 handle;

    handle = work->unk74;
    if (handle != 0) {
        func_0014FAB8(handle);
    }
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CC60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CEB8);

void func_0017CED0(EffPCPWork *work, u32 val) {
    work->unk64 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CED8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017CF38);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D0A0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D288);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D2F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4A8);

void func_0017D4B8(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D4C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D7A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017D8A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DA58);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DBB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017DCF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E4A8);

void func_0017E4C0(EffPCPWork *work, u32 val) {
    work->unkB8 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E4C8);

void func_0017E4F0(void) {
    EffPCPWork *work;

    work = func_0017D7A8();
    work->unkBC = 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E518);

void func_0017E668(void) {
    EffPCPWork *work;

    work = func_0017D7A8();
    work->unkBC = 2;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E690);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E7E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017E978);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EAB0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017ED38);

void func_0017ED98(EffPCPRotateWork *work) {
    s32 limit;
    s32 *key;
    u32 i;

    i = 0;
    limit = work->count;
    key = work->keys;
    do {
        if (*key <= limit) {
            func_0017DCF8(key[0x3F]);
            limit = work->count;
        }
        i = i + 1;
        key = key + 1;
    } while (i < 3);
    work->count = limit + 1;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE10);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EE70);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EED0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017EF30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F008);

void func_0017F0E8(EffPCPWork *work) {
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F118);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1C0);

void func_0017F1D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F1E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F210);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F2D0);

void func_0017F388(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F3C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4C0);

void func_0017F4D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F4E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F510);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F5D0);

void func_0017F688(EffPCPWork *work) {
    func_001629F0(work->unk64);
    func_001629F0(work->unk60);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F6C0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7C0);

void func_0017F7D0(EffPCPWork *work, u32 val) {
    work->unk5C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F7E0);

u32 func_0017F810(void) {
    D_003BB04C = 1;
    return 0;
}

u32 func_0017F820(void) {
    D_003BB04C = 1;
    return 0;
}

void func_0017F830(void) {
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F838);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F850);

void func_0017F898(void) {
    func_0017F850(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F8B0);

void func_0017F900(void) {
    func_0017F8B0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F918);

void func_0017F960(void) {
    func_0017F918(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F978);

void func_0017F9C8(void) {
    func_0017F978(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017F9E0);

void func_0017FA28(void) {
    func_0017F9E0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FA40);

void func_0017FA90(void) {
    func_0017FA40(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FAA8);

void func_0017FAF0(void) {
    func_0017FAA8(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FB08);

void func_0017FB58(void) {
    func_0017FB08(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FB70);

void func_0017FBB8(void) {
    func_0017FB70(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FBD0);

void func_0017FC20(void) {
    func_0017FBD0(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC38);

void func_0017FC80(void) {
    func_0017FC38(0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FC98);

void func_0017FCE8(void) {
    func_0017FC98(0);
}

void func_0017FD00(EffPCPWork *work) {
    func_001655D0(work->unk20);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FD30);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE38);

void func_0017FE48(EffPCPWork *work, u32 val) {
    work->unk10 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FE50);

void func_0017FF10(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_0017FE50(param0);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_0017FF30);

void func_00180050(EffPCPWork *work) {
    func_001634D8(work->unk30);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180080);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801D8);

void func_001801E8(EffPCPWork *work, u32 val) {
    work->unk24 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801F0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001801F8);

void func_00180338(EffPCPWork *work) {
    func_002DAA68(work->unkA8);
    func_002D0918(work->unkAC);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180370);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180540);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806C8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001806F8);

void func_00180890(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_001806F8(param0);
}

void func_001808B0(void) {
    func_001806F8();
}

void func_001808C8(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk5C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001808F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B30);

void func_00180B40(EffPCPWork *work, u32 val) {
    work->unk54 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180B78);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180BD0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E18);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00180E78);

void func_00181020(void *args) {
    void *param0;

    param0 = func_00163258(args, 0);
    func_00180E78(param0);
}

void func_00181040(void) {
    func_00180E78();
}

void func_00181058(EffPCPWork *work) {
    func_00180338((EffPCPWork *)work->unk7C);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181088);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811B0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811C0);

void func_001811D0(EffPCPWork *work, u32 val) {
    work->unk60 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001811D8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181208);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181490);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181538);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181650);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181708);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001818A8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C60);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C70);

void func_00181C78(EffPCPWork *work, u32 val) {
    work->unk170 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181C80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181D80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181E80);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181EF0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00181F48);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182170);

void func_00182180(EffPCPWork *work, u32 val) {
    work->unk14 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182188);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182190);

void func_00182398(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_00182190(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001823E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001825E0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182660);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001827D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A30);

void func_00182A38(EffPCPWork *work, u32 val) {
    work->unk114 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182A40);

void func_00182C90(void *args) {
    void *param0;
    void *param1;
    void *param2;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    param2 = func_00163258(args, 2);
    func_00182A40(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182CF8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182F20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00182FC8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001830F8);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834C0);

void func_001834D0(EffPCPWork *work, u32 val) {
    work->unk9C = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001834D8);

void func_001836F8(void *args) {
    void *param0;
    void *param1;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    func_001834D8(param0, param1);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183740);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183950);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_001839D0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183B20);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DA0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DB0);

void func_00183DB8(EffPCPWork *work, u32 val) {
    work->unkA4 = val;
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183DC0);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00183EE0);

void func_00183FD0(void *args) {
    void *param0;
    void *param1;
    void *param2;

    param0 = func_00163258(args, 0);
    param1 = func_00163258(args, 1);
    param2 = func_00163258(args, 2);
    func_00183EE0(param0, param1, param2);
}

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184038);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184090);

INCLUDE_ASM(const s32, "effect/effPCPMisc", func_00184130);

INCLUDE_RODATA(const s32, "effect/effPCPMisc", D_003A0EF0);

