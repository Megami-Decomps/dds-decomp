#include "common.h"

/* Shared resource handed between scatter effects. func_00173018 creates it,
   func_001730B8 takes a reference, func_00173068 releases it. */
typedef struct PcpScatterRes PcpScatterRes;

struct PcpScatterRes {
    u32 unk00;
    s32 refCount;
};

extern void *func_00163258(void *data, s32 index);

extern void func_00175D88(u32 res);
extern void func_002DAA68(u32 res);
extern void func_002D0918(u32 res);
extern void func_00173068(PcpScatterRes *res);
extern PcpScatterRes *func_001730B8(PcpScatterRes *res);

/* Effect initializers implemented in assembly below (func_001708A0 lives in
   another unit). Each is entered with and without spawn arguments, so they
   are declared unchecked. */
extern void func_001708A0();
extern void func_00171550();
extern void func_00172158();
extern void func_001730D0();
extern void func_00173B48();
extern void func_00174680();
extern void func_00175230();


/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and scale helpers are known; the update bodies are still assembly.
   Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpScatterWork1 PcpScatterWork1;
typedef struct PcpScatterWork2 PcpScatterWork2;
typedef struct PcpScatterWork3 PcpScatterWork3;
typedef struct PcpScatterWork4 PcpScatterWork4;
typedef struct PcpScatterWork5 PcpScatterWork5;
typedef struct PcpScatterWork6 PcpScatterWork6;
typedef struct PcpScatterWork7 PcpScatterWork7;

/* func_001708A0 */
struct PcpScatterWork1 {
    u8 pad00[0x3C];
    f32 unk3C;
    u32 unk40;
    u32 unk44;
    u32 unk48;
    f32 unk4C;
    f32 unk50;
    u8 pad54[0x28];
    u32 unk7C;
};

/* func_00172158 */
struct PcpScatterWork2 {
    u8 pad00[0x40];
    f32 unk40;
    f32 unk44;
    u8 pad48[0x24];
    u32 unk6C;
};

/* Resource-holding effect around func_00172F88 */
struct PcpScatterWork3 {
    u8 pad00[0x20];
    s32 unk20;
    s32 unk24;
    u32 unk28;
    u32 unk2C;
    PcpScatterRes *res;
    u8 pad34[0x20];
    u32 unk54;
};

/* func_001730D0 */
struct PcpScatterWork4 {
    u8 pad00[0x180];
    u32 unk180;
    u32 unk184;
    u32 unk188;
};

/* func_00173B48 */
struct PcpScatterWork5 {
    u8 pad00[0x184];
    u32 unk184;
    u32 unk188;
    u32 unk18C;
    u32 unk190;
};

/* func_00174680 */
struct PcpScatterWork6 {
    u8 pad00[0x18C];
    u32 unk18C;
    u32 unk190;
    u32 unk194;
    u32 unk198;
};

/* func_00175230 */
struct PcpScatterWork7 {
    u8 pad00[0x134];
    u32 unk134;
    u32 unk138;
};

extern PcpScatterRes *func_00173018(u32 resId);

void func_00170B88(void *data)
{
    func_001708A0(func_00163258(data, 0), func_00163258(data, 1), func_00163258(data, 2));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170BF0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170CE0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170D68);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00170F28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171510);

void func_00171520(PcpScatterWork1 *work, u32 value)
{
    work->unk7C = value;
}

void func_00171528(f32 scale, PcpScatterWork1 *work)
{
    work->unk3C *= scale;
    work->unk4C *= scale;
    work->unk50 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171550);

void func_001717E0(void *data)
{
    func_00171550(func_00163258(data, 0), func_00163258(data, 1), func_00163258(data, 2));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171848);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171938);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001719C0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00171B28);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172120);

void func_00172130(PcpScatterWork2 *work, u32 value)
{
    work->unk6C = value;
}

void func_00172138(f32 scale, PcpScatterWork2 *work)
{
    work->unk40 *= scale;
    work->unk44 *= scale;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172158);

void func_00172400(void *data)
{
    func_00172158(func_00163258(data, 0), func_00163258(data, 1), func_00163258(data, 2));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172468);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172568);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001725F0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001726E8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172C48);

void func_00172C58(PcpScatterWork3 *work, u32 value)
{
    work->unk54 = value;
}

void func_00172C60(void)
{
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172C68);

void func_00172D70(PcpScatterWork3 *work)
{
    if (work->res != NULL) {
        func_00173068(work->res);
    }
    func_002DAA68(work->unk28);
    func_002D0918(work->unk2C);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00172DB0);

void func_00172F88(PcpScatterWork3 *work, u32 resId)
{
    PcpScatterRes *res;

    res = func_00173018(resId);
    work->res = res;
}

void func_00172FB8(PcpScatterWork3 *work, PcpScatterWork3 *src)
{
    PcpScatterRes *res;

    res = func_001730B8(src->res);
    work->res = res;
}

s32 func_00172FE8(PcpScatterWork3 *work, s32 index)
{
    return work->unk20 + index * 0x60;
}

s32 func_00173000(PcpScatterWork3 *work, s32 index)
{
    return work->unk24 + index * 0x18;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173018);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173068);

PcpScatterRes *func_001730B8(PcpScatterRes *res)
{
    res->refCount++;
    return res;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001730D0);

void func_001732E8(void *data)
{
    func_001730D0(func_00163258(data, 0), func_00163258(data, 1));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173330);

void func_00173378(PcpScatterWork4 *work)
{
    func_00175D88(work->unk184);
    func_002D0918(work->unk188);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001733A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173738);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001738C8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AC0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AD8);

void func_00173AE0(PcpScatterWork4 *work, u32 value)
{
    work->unk180 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173AE8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173B48);

void func_00173D78(void *data)
{
    func_00173B48(func_00163258(data, 0), func_00163258(data, 1));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173DC0);

void func_00173E08(PcpScatterWork5 *work)
{
    func_00175D88(work->unk18C);
    func_002D0918(work->unk190);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00173E38);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001741B0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174350);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001745F8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174610);

void func_00174618(PcpScatterWork5 *work, u32 value)
{
    work->unk184 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174620);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174680);

void func_00174880(void *data)
{
    func_00174680(func_00163258(data, 0), func_00163258(data, 1));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001748C8);

void func_00174910(PcpScatterWork6 *work)
{
    func_00175D88(work->unk194);
    func_002D0918(work->unk198);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174940);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174D30);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00174ED0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751A8);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751C0);

void func_001751C8(PcpScatterWork6 *work, u32 value)
{
    work->unk18C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001751D0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175230);

void func_00175420(void *data)
{
    func_00175230(func_00163258(data, 0), func_00163258(data, 1));
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175468);

void func_001754B0(PcpScatterWork7 *work)
{
    func_00175D88(work->unk134);
    func_002D0918(work->unk138);
}

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_001754E0);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175780);

INCLUDE_ASM(const s32, "effect/effPCPScatter", func_00175908);
