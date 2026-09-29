#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

/* Per-effect work areas. Only the fields touched by the matched spawn,
   teardown and accumulator helpers are known; the update bodies are still
   assembly. Each work area belongs to the effect whose initializer is noted. */
typedef struct PcpFlashWork1 PcpFlashWork1;

/* func_0016A088 */
struct PcpFlashWork1 {
    u8 pad00[0x20];
    u32 colorA;
    u32 colorB;
    u8 pad28[0x10];
    u32 unk38;
    f32 unk3C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

extern s32 func_00177E90(s32 base, s32 index);

extern s32 func_00195A30(s32 color, s32 param);

typedef struct PcpFlashWork2 PcpFlashWork2;

/* func_0016A6C0 */
struct PcpFlashWork2 {
    u8 pad00[0x24];
    u32 unk24;
    u32 unk28;
    u8 pad2C[0x1C];
    u32 unk48;
    f32 unk4C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc14 PcpFlashPtc14;

struct PcpFlashPtc14 {
    u8 pad00[0x10];
    f32 unk10;
};

typedef struct PcpFlashWork3 PcpFlashWork3;

/* func_0016AFF0 */
struct PcpFlashWork3 {
    u8 pad00[0x40];
    f32 unk40;
    u32 unk44;
    PcpFlashPtc14 *parts;
    u32 unk4C;
    u32 unk50;
    f32 unk54;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc1C PcpFlashPtc1C;

struct PcpFlashPtc1C {
    u8 pad00[0x18];
    f32 unk18;
};

typedef struct PcpFlashWork4 PcpFlashWork4;

/* func_0016B800 */
struct PcpFlashWork4 {
    u8 pad00[0x44];
    f32 unk44;
    u32 unk48;
    u32 unk4C;
    PcpFlashPtc1C *parts;
    u32 unk54;
    u32 unk58;
    f32 unk5C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

/* Particle elements. Only the fields touched by the matched accumulators are
   known; each struct's size is the element stride used to index its array. */
typedef struct PcpFlashPtc10 PcpFlashPtc10;

struct PcpFlashPtc10 {
    u8 pad00[0x08];
    f32 unk08;
    u8 pad0C[0x04];
};

typedef struct PcpFlashWork5 PcpFlashWork5;

/* func_0016C0E8 */
struct PcpFlashWork5 {
    u8 pad00[0x50];
    f32 unk50;
    u32 unk54;
    PcpFlashPtc10 *parts;
    u32 unk5C;
    u32 unk60;
    f32 unk64;
    u8 pad68[0x10];
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20A PcpFlashPtc20A;

struct PcpFlashPtc20A {
    u8 pad00[0x08];
    f32 unk08;
    u8 pad0C[0x04];
    f32 unk10;
    u8 pad14[0x0C];
};

typedef struct PcpFlashWork6 PcpFlashWork6;

/* func_0016C9F0 */
struct PcpFlashWork6 {
    u8 pad00[0x4C];
    PcpFlashPtc20A *parts;
    u32 unk50;
    u32 unk54;
    f32 unk58;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork7 PcpFlashWork7;

/* func_0016D2A8 */
struct PcpFlashWork7 {
    u8 pad00[0x24];
    u32 unk24;
    u32 unk28;
    u8 pad2C[0x18];
    u32 unk44;
    f32 unk48;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashPtc20B PcpFlashPtc20B;

struct PcpFlashPtc20B {
    u8 pad00[0x08];
    f32 unk08;
    u8 pad0C[0x0C];
    f32 unk18;
    u8 pad1C[0x04];
};

typedef struct PcpFlashWork8 PcpFlashWork8;

/* func_0016D940 */
struct PcpFlashWork8 {
    u8 pad00[0xD0];
    PcpFlashPtc20B *parts;
    u32 unkD4;
    u32 unkD8;
    f32 unkDC;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork9 PcpFlashWork9;

/* func_0016E290 */
struct PcpFlashWork9 {
    u8 pad00[0x48];
    f32 unk48;
    u32 unk4C;
    PcpFlashPtc14 *parts;
    u32 unk54;
    u32 unk58;
    f32 unk5C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

typedef struct PcpFlashWork10 PcpFlashWork10;

/* func_0016EB00 */
struct PcpFlashWork10 {
    u8 pad00[0x24];
    u32 unk24;
    u32 unk28;
    u8 pad2C[0x1C];
    u32 unk48;
    f32 unk4C;
    u32 ownedBuffer;
    u32 resourceHandle;
};

void func_00171E00(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00171CE0(temp_v0);
}

void func_00171E20(void) {
    func_00171CE0();
}

void func_00171E38(PcpFlashWork1 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171E68);

void func_00171E78(PcpFlashWork1 *work, u32 value) {
    work->unk38 = value;
}

void func_00171E80(PcpFlashWork1 *work, f32 value)
{
    work->unk3C = value;
}

void func_00171E88(PcpFlashWork1 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00177E90(work->resourceHandle, index);
    rgb1 = work->colorA & 0xFFFFFF;
    rgb2 = work->colorB & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 4) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 8) = func_00195A30(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00171F28);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001720A8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172318);

void func_001724B0(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00172318(temp_v0);
}

void func_001724D0(void) {
    func_00172318();
}

void func_001724E8(PcpFlashWork2 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172518);

void func_00172528(PcpFlashWork2 *work, u32 value) {
    work->unk48 = value;
}

void func_00172530(PcpFlashWork2 *work, f32 value)
{
    work->unk4C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172538);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172628);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001727A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", effRotateFlashParticlePosition);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001729B0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172C48);

void func_00172E68(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00172C48(temp_v0);
}

void func_00172E88(void) {
    func_00172C48();
}

void func_00172EA0(PcpFlashWork3 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172ED0);

void func_00172EE0(PcpFlashWork3 *work, u32 value) {
    work->unk50 = value;
}

void func_00172EE8(PcpFlashWork3 *work, f32 value)
{
    work->unk54 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172EF0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00172FE0);

void func_001731C8(PcpFlashWork3 *work, s32 index) {
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->unk10 += work->unk40;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001731F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173458);

void func_00173690(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00173458(temp_v0);
}

void func_001736B0(void) {
    func_00173458();
}

void func_001736C8(PcpFlashWork4 *work) {
    func_00177FA8(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001736F8);

void func_00173708(PcpFlashWork4 *work, u32 value) {
    work->unk58 = value;
}

void func_00173710(PcpFlashWork4 *work, f32 value)
{
    work->unk5C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173718);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173808);

void func_00173A78(PcpFlashWork4 *work, s32 index) {
    PcpFlashPtc1C *part;

    part = &work->parts[index];
    part->unk18 += work->unk44;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173AA0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173D40);

void func_00173F90(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00173D40(temp_v0);
}

void func_00173FB0(void) {
    func_00173D40();
}

void func_00173FC8(PcpFlashWork5 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00173FF8);

void func_00174008(PcpFlashWork5 *work, u32 value) {
    work->unk60 = value;
}

void func_00174010(PcpFlashWork5 *work, f32 value)
{
    work->unk64 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174018);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174108);

void func_001742D0(PcpFlashWork5 *work, s32 index) {
    PcpFlashPtc10 *part;

    part = &work->parts[index];
    part->unk08 += work->unk50;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001742F0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174648);

void func_00174808(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00174648(temp_v0);
}

void func_00174828(void) {
    func_00174648();
}

void func_00174840(PcpFlashWork6 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174870);

void func_00174880(PcpFlashWork6 *work, u32 value) {
    work->unk54 = value;
}

void func_00174888(PcpFlashWork6 *work, f32 value)
{
    work->unk58 = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174890);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174980);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174A58);

void func_00174C18(PcpFlashWork6 *work, s32 index) {
    PcpFlashPtc20A *part;

    part = &work->parts[index];
    part->unk10 += part->unk08;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174C38);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00174F00);

void func_00175038(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00174F00(temp_v0);
}

void func_00175058(void) {
    func_00174F00();
}

void func_00175070(PcpFlashWork7 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001750A0);

void func_001750B0(PcpFlashWork7 *work, u32 value) {
    work->unk44 = value;
}

void func_001750B8(PcpFlashWork7 *work, f32 value)
{
    work->unk48 = value;
}

void func_001750C0(PcpFlashWork7 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00177E90(work->resourceHandle, index);
    rgb1 = work->unk24 & 0xFFFFFF;
    rgb2 = work->unk28 & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 4) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 8) = func_00195A30(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175160);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001752F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175598);

void func_00175770(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00175598(temp_v0);
}

void func_00175790(void) {
    func_00175598();
}

void func_001757A8(PcpFlashWork8 *work) {
    func_00177FA8(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757D8);

void func_001757E8(PcpFlashWork8 *work, u32 value) {
    work->unkD8 = value;
}

void func_001757F0(PcpFlashWork8 *work, f32 value)
{
    work->unkDC = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001757F8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001758E8);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001759C0);

void func_00175BE8(PcpFlashWork8 *work, s32 index) {
    PcpFlashPtc20B *part;

    part = &work->parts[index];
    part->unk18 += part->unk08;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175C08);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00175EE8);

void func_00176118(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00175EE8(temp_v0);
}

void func_00176138(void) {
    func_00175EE8();
}

void func_00176150(PcpFlashWork9 *work) {
    func_00177880(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176180);

void func_00176190(PcpFlashWork9 *work, u32 value) {
    work->unk58 = value;
}

void func_00176198(PcpFlashWork9 *work, f32 value)
{
    work->unk5C = value;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001761A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176290);

void func_00176478(PcpFlashWork9 *work, s32 index) {
    PcpFlashPtc14 *part;

    part = &work->parts[index];
    part->unk10 += work->unk48;
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001764A0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176758);

void func_00176898(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00176758(temp_v0);
}

void func_001768B8(void) {
    func_00176758();
}

void func_001768D0(PcpFlashWork10 *work) {
    func_00177CA0(work->resourceHandle);
    func_003297C8(work->ownedBuffer);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176900);

void func_00176910(PcpFlashWork10 *work, u32 value) {
    work->unk48 = value;
}

void func_00176918(PcpFlashWork10 *work, f32 value)
{
    work->unk4C = value;
}

void func_00176920(PcpFlashWork10 *work, s32 index, s32 param)
{
    s32 slot;
    s32 rgb1;
    s32 rgb2;

    slot = func_00177E90(work->resourceHandle, index);
    rgb1 = work->unk24 & 0xFFFFFF;
    rgb2 = work->unk28 & 0xFFFFFF;
    *(s32 *)(slot + 0) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 4) = func_00195A30(rgb2, param);
    *(s32 *)(slot + 8) = func_00195A30(rgb1 | 0xFF000000, param);
}

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_001769C0);

INCLUDE_ASM(const s32, "effect/effPCPFlash", func_00176B58);
