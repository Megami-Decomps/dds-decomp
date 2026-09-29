#include "common.h"
extern s8 D_0043643D;

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

extern EffHandler D_003B2068[];

extern void func_0036B420(void);

EffResult *effAllocDispatch(s32 kind, s32 input) {
    EffResult *result = func_00328D68(8);
    s32 value = D_003B2060[kind].handler(input);

    result->unk0 = kind;
    result->unk4 = value;
    return result;
}

void effTypeDispatch(EffWork *work) {
    D_003B2064[work->type].handler(work->unk4);
}

void effTypeDispatchFree(EffWork *work) {
    D_003B2068[work->type].handler(work->unk4);
    func_00328E48(work);
}

u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->unk4;
}

u32 func_001947F8(u32 *arg0) {
    return *arg0;
}

void func_00194800(void) {
}

u32 func_00194808(void) {
    return 1;
}

void effTypeDispatchGuardedA(EffWork *work) {
    void (*handler)(void *) = D_003B206C[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
    }
}

void effTypeDispatchGuardedB(EffWork *work) {
    void (*handler)(void *) = D_003B2074[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
    }
}

void effTypeDispatchGuardedC(EffWork *work) {
    void (*handler)(void *) = D_003B2070[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
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

void effAllocSubWork(EffWork *arg0) {
    u32 t = arg0->type;
    u32 v = 0;

    switch (t) {
    case 0:
        v = arg0->unk4;
        break;
    case 1:
        v = arg0->unk4;
        break;
    case 2:
        v = arg0->unk4 + 0x40;
        break;
    case 3:
        v = arg0->unk4 + 0x40;
        break;
    case 4:
        v = arg0->unk4 + 0x40;
        break;
    default:
        break;
    }
    effAllocDispatch((EffWork *)t, v);
}

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

s32 func_00194A68(s32 arg0) {
    return arg0;
}

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

s32 func_00194A90(void) {
    return 0;
}

void func_00194A98(void) {
}

s8 func_00194AA0(void) {
    return D_0043643D;
}

/* An enabled disc directory uses sceDopen; otherwise iteration uses
 * the built-in name table and starts over at entry zero. */
s32 effOpenDataDir(void *name) {
    u8 path[0x70];

    if (D_00438B66 != 0) {
        func_0035C860(path, D_00436448, name);
        return sceDopen(path);
    } else {
        D_00438F08 = 0;
        return 0;
    }
}

void effRunIfEnabled(void) {
    if (D_00438B66 != 0) {
        func_0036B420();
    }
}

INCLUDE_ASM(const s32, "game/code_00194700", effNextDataDirEntry);

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

void effFreeWorkList(EffWork *root) {
    EffWork *node = (EffWork *)root->unk8;

    if (node != NULL) {
        do {
            EffWork *next = node->unk38;
            func_00328E48(node);
            node = next;
        } while (node != NULL);
    }
    func_00328E48(root->unk4);
    func_00328E48(root);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195060);

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void effFreeWork(EffWork *work) {
    s32 soundHandle;

    soundHandle = work->unk3C;
    if (soundHandle != 0) {
        func_0032BBB0(soundHandle);
        work->unk3C = 0;
    }
    func_00328E48(work);
}

void effSetMsgHeader(EffMsg *message, s32 first, s32 second) {
    message->unk0 = first;
    message->unk4 = second;
}

u32 effGetWorkParam(EffWork *work) {
    return work->unk14;
}

u32 effGetWorkLink(EffWork *work) {
    return work->unk8;
}

u32 effFormatMsgNames(EffMsg *message, void *destination) {
    func_0035C860(destination, D_00436450, message->unk40[1], message->unk34 + 1);
    return *message->unk34;
}

void effSetWorkFirst(EffWork *work, u32 value) {
    work->unk20 = value;
}

void effSetWorkSecond(EffWork *work, u32 value) {
    work->unk24 = value;
}

void effSetMsgPair(EffMsg *arg0, u32 arg1, u32 arg2) {
    arg0->unk28 = arg1;
    arg0->unk2C = arg2;
}

void effSetupWorkSound(EffWork *work, u64 resource) {
    u32 soundHandle;
    u64 allocation;
    u32 resourceData[4];

    if (work->unk3C != 0) {
        func_0032BBB0(work->unk3C);
        work->unk3C = 0;
    }
    allocation = func_00343ED0(resource, resourceData, 0);
    soundHandle = func_0032C138(resourceData[0]);
    work->unk3C = soundHandle;
    func_003297C8(allocation);
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

INCLUDE_ASM(const s32, "game/code_00194700", effBlendColor);

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

