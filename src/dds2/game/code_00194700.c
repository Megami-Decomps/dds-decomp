#include "common.h"

extern s8 D_0043643D;

#include "eff.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"

extern u32 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

extern void *func_00328D68(s32 size);

extern EffHandler32 D_003B2060[];

extern EffHandler D_003B2064[];

extern EffHandler D_003B206C[];

extern EffHandler D_003B2074[];

extern EffHandler D_003B2070[];

extern u8 D_00438B66;

extern u32 D_00438F08;

extern char D_00436448[];

extern void func_0035C860();

extern s32 sceDopen(void *path);

extern void sdfReleaseChipBlock(void *allocation);

extern char D_00436450[];

extern EffHandler D_003B2068[];

extern void func_0036B420(void);

/* Directory entry filled by the effect data directory iterator. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x40]; /* 0x40 */
} EffDirEnt;

extern char *D_003B20D8[];

extern s32 func_0036B588(void);

extern u8 D_003846F0[];

extern u8 D_0037F610[];

extern u8 D_0037F660[];

extern void func_00336C10(void *);

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
    sdfReleaseChipBlock(work);
}

u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->unk4;
}

u32 func_001947F8(u32 *value) {
    return *value;
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

void effSetSubSlot(EffWork *work, s32 slot) {
    u8 slotByte = slot;
    u32 kind = work->type;

    switch (kind) {
    case 0:
        ((EffSub *)work->unk4)->unk20 = slotByte;
        return;
    case 1:
        ((EffSub *)work->unk4)->unk40 = slotByte;
        return;
    case 2:
        ((EffSub *)work->unk4)->unk50 = slotByte;
        return;
    case 3:
        ((EffSub *)work->unk4)->unk50 = slotByte;
        return;
    case 4:
        ((EffSub *)work->unk4)->unk50 = slotByte;
        return;
    default:
        return;
    }
}

u32 effGetSubSlot(EffWork *work, s32 unused) {
    u32 kind = work->type;

    switch (kind) {
    case 0:
        return ((EffSub *)work->unk4)->unk20;
    case 1:
        return ((EffSub *)work->unk4)->unk40;
    case 2:
        return ((EffSub *)work->unk4)->unk50;
    case 3:
        return ((EffSub *)work->unk4)->unk50;
    case 4:
        return ((EffSub *)work->unk4)->unk50;
    default:
        break;
    }
    return 0;
}

void effAllocSubWork(EffWork *work) {
    u32 kind = work->type;
    u32 handlerInput = 0;

    switch (kind) {
    case 0:
        handlerInput = work->unk4;
        break;
    case 1:
        handlerInput = work->unk4;
        break;
    case 2:
        handlerInput = work->unk4 + 0x40;
        break;
    case 3:
        handlerInput = work->unk4 + 0x40;
        break;
    case 4:
        handlerInput = work->unk4 + 0x40;
        break;
    default:
        break;
    }
    effAllocDispatch((EffWork *)kind, handlerInput);
}

void func_001949D8(void) {
}

void func_001949E0(void) {
    func_001027D8(0, 0, 0, 0);
}

u32 func_00194A08(u32 value) {
    return value;
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

s32 func_00194A68(s32 value) {
    return value;
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

/* In built-in mode the name table supplies entries instead of the
 * directory iterator; clearing 0x1000 marks a synthesized entry. */
s32 effNextDataDirEntry(s32 unused, EffDirEnt *entry) {
    if (D_00438B66 != 0) {
        return func_0036B588();
    }
    if ((u32)D_00438F08 >= 0x2F) {
        return 0;
    }
    strcpy(entry->name, D_003B20D8[D_00438F08]);
    entry->flags &= ~0x1000;
    D_00438F08++;
    return strlen(entry->name);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00194BD0);

void effFreeWorkList(EffWork *root) {
    EffWork *node = (EffWork *)root->unk8;

    if (node != NULL) {
        do {
            EffWork *next = node->unk38;
            sdfReleaseChipBlock(node);
            node = next;
        } while (node != NULL);
    }
    sdfReleaseChipBlock(root->unk4);
    sdfReleaseChipBlock(root);
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195060);

INCLUDE_ASM(const s32, "game/code_00194700", func_001950F0);

void effFreeWork(EffWork *work) {
    s32 soundHandle;

    soundHandle = work->unk3C;
    if (soundHandle != 0) {
        sdfTexReleaseReferenceViaHandler(soundHandle);
        work->unk3C = 0;
    }
    sdfReleaseChipBlock(work);
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

void effSetMsgPair(EffMsg *message, u32 first, u32 second) {
    message->unk28 = first;
    message->unk2C = second;
}

void effSetupWorkSound(EffWork *work, u64 resource) {
    u32 soundHandle;
    u64 allocation;
    u32 resourceData[4];

    if (work->unk3C != 0) {
        sdfTexReleaseReferenceViaHandler(work->unk3C);
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

void func_00195810(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(D_003846F0);
    matrix = D_0037F610;
    func_00336C10(matrix);
    __asm__ volatile (
        ".set noreorder\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        ".set reorder"
        : : : "memory");
    matrix += 0x40;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(matrix) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        "vadd.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "r"(D_0037F660) : "memory");
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195890);

/* vu0 routine: blend two RGBA8888 colours by t (lerp in float, packed back to RGBA8888) */
u32 effBlendColor(u32 colorA, u32 colorB, f32 t) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit;

    if (t >= 1.0f) {
        return colorB;
    }
    unit = 0x3C000000;
    color1[0] = colorB;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorA;
    EE_MMI_RGBA_UNPACK(color2, unit);
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $2, %0\n"
        "qmtc2.ni $2, vf2\n"
        "vmulx.xyzw vf10, vf10, vf2x\n"
        ".set reorder"
        : : "f"(1.0f - t) : "$2");
    __asm__ volatile (
        ".set noreorder\n"
        "mfc1 $3, %0\n"
        "qmtc2.ni $3, vf2\n"
        "vmulx.xyzw vf11, vf11, vf2x\n"
        "vadd.xyzw vf10, vf10, vf11\n"
        ".set reorder"
        : : "f"(t) : "$3");
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;

    return packed;
}

/* vu0 routine: modulate two RGBA8888 colours, (a/128 * b/128) * 128 per channel */
u32 func_00195A30(u32 colorA, u32 colorB) {
    s32 color1[4];
    s32 color2[4];
    s32 blended[4];
    u32 packed;
    u32 unit = 0x3C000000;
    color1[0] = colorA;
    EE_MMI_RGBA_UNPACK(color1, unit);
    VU0_MOVE_VF(vf11, vf10);
    color2[0] = colorB;
    EE_MMI_RGBA_UNPACK(color2, unit);
    VU0_MUL(vf10, vf10, vf11);
    EE_MMI_RGBA_PACK(packed);
    blended[0] = packed;
    return blended[0];
}

INCLUDE_ASM(const s32, "game/code_00194700", func_00195AB0);

void *effAllocSlotArray(s32 count) {
    void *allocation = func_003292A8(count * 0x38 + 0xC);
    void *slotBase = sdfResourceRetainAddress(allocation);
    u32 index = 0;
    EffSlot38 *slot = slotBase;
    u8 *end = (u8 *)(slot + count);

    ((EffArrHdr *)end)->unk8 = allocation;
    ((EffArrHdr *)end)->unk0 = slotBase;
    ((EffArrHdr *)end)->unk4 = count;
    if (count != 0) {
        do {
            index++;
            slot->unk30 = 0;
            slot->unk34 = 0.05f;
            slot++;
        } while (index < count);
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

