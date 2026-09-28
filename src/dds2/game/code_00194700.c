#include "common.h"
#include "eff.h"

extern u32 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

extern void *func_00328D68(s32 arg0);
extern EffHandler32 D_003B2060[];

extern EffHandler D_003B2064[];

extern EffHandler D_003B206C[];

extern EffHandler D_003B2074[];

extern EffHandler D_003B2070[];

extern u8 D_00438B66;

extern u32 D_00438F08;

extern char D_00436448[];

extern void func_0035C860();

extern s32 sceDopen(void *arg0);

extern void func_00328E48(void *arg0);

extern char D_00436450[];

EffResult *effAllocDispatch(s32 arg0, s32 arg1) {
    EffResult *mem = func_00328D68(8);
    s32 ret = D_003B2060[arg0].handler(arg1);

    mem->unk0 = arg0;
    mem->unk4 = ret;
    return mem;
}

void effTypeDispatch(EffWork *arg0) {
    D_003B2064[arg0->type].handler(arg0->unk4);
}

INCLUDE_ASM(const s32, "game/code_00194700", effTypeDispatchFree);

u32 effGetHandlerArg(s32 arg0) {
    return *(u32 *)(arg0 + 4);
}

u32 func_001947F8(u32 *arg0) {
    return *arg0;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

void effTypeDispatchGuardedA(EffWork *arg0) {
    void (*handler)(void *) = D_003B206C[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void effTypeDispatchGuardedB(EffWork *arg0) {
    void (*handler)(void *) = D_003B2074[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void effTypeDispatchGuardedC(EffWork *arg0) {
    void (*handler)(void *) = D_003B2070[arg0->type].handler;

    if (handler != NULL) {
        handler(arg0->unk4);
    }
}

void effSetSubSlot(EffWork *arg0, s32 arg1) {
    u8 v = arg1;
    u32 t = arg0->type;

    switch (t) {
    case 0:
        ((EffSub *)arg0->unk4)->unk20 = v;
        return;
    case 1:
        ((EffSub *)arg0->unk4)->unk40 = v;
        return;
    case 2:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    case 3:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    case 4:
        ((EffSub *)arg0->unk4)->unk50 = v;
        return;
    default:
        return;
    }
}

u32 effGetSubSlot(EffWork *arg0, s32 arg1) {
    u8 v = arg1;
    u32 t = arg0->type;

    switch (t) {
    case 0:
        return ((EffSub *)arg0->unk4)->unk20;
    case 1:
        return ((EffSub *)arg0->unk4)->unk40;
    case 2:
        return ((EffSub *)arg0->unk4)->unk50;
    case 3:
        return ((EffSub *)arg0->unk4)->unk50;
    case 4:
        return ((EffSub *)arg0->unk4)->unk50;
    default:
        break;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00194700", effAllocSubWork);

void func_001949D8(void) {
}

void func_001949E0(void) {
    func_001027D8(0, 0, 0, 0);
}

u32 func_00194A08(u32 arg0) {
    return arg0;
}

void func_00194A10(void) {
}

void func_00194A18(void) {
}

void func_00194A20(void) {
}

void func_00194A28(void) {
}

void func_00194A30(void) {
}

void func_00194A38(void) {
}

void func_00194A40(void) {
}

void func_00194A48(void) {
}

void func_00194A50(void) {
}

void func_00194A58(void) {
}

void func_00194A60(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A68);

void func_00194A70(void) {
}

u32 func_00194A78(void) {
    return 0;
}

u32 func_00194A80(void) {
    return 0;
}

void func_00194A88(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194A90);

void func_00194A98(void) {
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194AA0);

s32 effOpenDataDir(void *arg0) {
    u8 buf[0x70];

    if (D_00438B66 != 0) {
        func_0035C860(buf, D_00436448, arg0);
        return sceDopen(buf);
    } else {
        D_00438F08 = 0;
        return 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00194700", effRunIfEnabled);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194B28);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

void effFreeWorkList(EffWork *arg0) {
    EffWork *p = (EffWork *)arg0->unk8;

    if (p != NULL) {
        do {
            EffWork *next = p->unk38;
            func_00328E48(p);
            p = next;
        } while (p != NULL);
    }
    func_00328E48(arg0->unk4);
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195060);

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void effFreeWork(u32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)((s32)arg0 + 0x3c);
    if (temp_v0 != 0) {
        func_0032BBB0(temp_v0);
        *(u32 *)((s32)arg0 + 0x3c) = 0;
    }
    func_00328E48(arg0);
}

void effSetMsgHeader(EffMsg *arg0, s32 arg1, s32 arg2) {
    arg0->unk0 = arg1;
    arg0->unk4 = arg2;
}

u32 effGetWorkParam(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 effGetWorkLink(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

u32 effFormatMsgNames(EffMsg *arg0, void *arg1) {
    func_0035C860(arg1, D_00436450, arg0->unk40[1], arg0->unk34 + 1);
    return *arg0->unk34;
}

void effSetWorkFirst(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void effSetWorkSecond(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

void effSetMsgPair(EffMsg *arg0, u32 arg1, u32 arg2) {
    arg0->unk28 = arg1;
    arg0->unk2C = arg2;
}

void effSetupWorkSound(s32 arg0, u64 arg1) {
    u32 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    if (*(s32 *)(arg0 + 0x3c) != 0) {
        func_0032BBB0(*(s32 *)(arg0 + 0x3c));
        *(u32 *)(arg0 + 0x3c) = 0;
    }
    temp_v1 = func_00343ED0(arg1, temp_v2, 0);
    temp_v0 = func_0032C138(temp_v2[0]);
    *(u32 *)(arg0 + 0x3c) = temp_v0;
    func_003297C8(temp_v1);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_001956A8);

void func_001957C0(void) {
    func_001027D8(0, 0, 0, 0);
}

void func_001957E8(void) {
    func_001027D8(0, 0, 0, 0);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195810);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195890);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195978);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195A30);

INCLUDE_ASM(const s32, "game/code_00194700", func_00195AB0);

void *effAllocSlotArray(s32 n) {
    void *mem1 = func_003292A8(n * 0x38 + 0xC);
    void *mem2 = sdfResourceRetainAddress(mem1);
    u32 i = 0;
    EffSlot38 *r = mem2;
    u8 *end = (u8 *)r + n * 0x38;

    ((EffArrHdr *)end)->unk8 = mem1;
    ((EffArrHdr *)end)->unk0 = mem2;
    ((EffArrHdr *)end)->unk4 = n;
    if (n != 0) {
        do {
            i++;
            r->unk30 = 0;
            r->unk34 = 0.05f;
            r = (EffSlot38 *)((u8 *)r + 0x38);
        } while (i < n);
    }
    return end;
}

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004146F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414708);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414718);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414728);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414738);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414748);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414758);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414768);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414778);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414788);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414798);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004147F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414808);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414818);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414828);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414838);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414848);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414858);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414868);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414878);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414888);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414898);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148A8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148B8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148C8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148D8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148E8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004148F8);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414908);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414918);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414928);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414938);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414948);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414958);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414968);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414978);

INCLUDE_RODATA(const s32, "game/code_00194700", D_00414990);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149A0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149B0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149C0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149D0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149E0);

INCLUDE_RODATA(const s32, "game/code_00194700", D_004149F0);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436440);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436448);

INCLUDE_SDATA(const s32, "game/code_00194700", D_00436450);

