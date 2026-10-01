#include "common.h"
#include "ee_mmi.h"
#include "pcp_vu0.h"
#include "eff.h"

/* Directory entry filled by the effect data directory iterator. */
typedef struct EffDirEnt {
    u32 flags;      /* 0x00: bit 12 cleared */
    u8 pad04[0x3C]; /* 0x04 */
    char name[0x40]; /* 0x40 */
} EffDirEnt;

extern EffHandler D_00355734[];
extern EffHandler D_00355738[];
extern EffHandler D_0035573C[];
extern EffHandler D_00355740[];
extern EffHandler D_00355744[];
extern u8 D_003BD476;
extern s8 D_003BB04D;
extern u32 D_003BD800;
extern char D_003BB058[];
extern char D_003BB060[];
extern char *D_003557A8[];
extern s32 func_00310320(void);
extern u32 func_002D3288(u32);
extern u64 func_002EB028(u64, u32 *, u64);
extern void sdfReleaseChipBlock(void *arg0);
extern void sdfTexReleaseReferenceViaHandler(s32 arg0);
extern void func_002D0918(u64 arg0);
extern void func_001028E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_003101B8(void);
extern void func_003014F0();
extern s32 sceDopen(void *arg0);
extern void *func_002CFEB8(s32 arg0);
extern void sdfInitPacketList(s32 arg0);
extern void sdfConsCreateDrawPacket(s32 arg0, void *arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern void *func_002E1420(s32 arg0);
extern s32 sdfConsAllocateColumnPacket(s32 arg0);
extern void sdfAppendPacket(s32 arg0, s32 arg1);

extern EffHandler32 D_00355730[];

extern u8 D_003296F0[];

extern u8 D_00324610[];

extern u8 D_00324660[];

extern void func_002DDD60(void *);


EffResult *effAllocDispatch(s32 type, s32 handlerArg) {
    EffResult *result = func_002CFEB8(8);
    s32 handlerResult = D_00355730[type].handler(handlerArg);

    result->unk0 = type;
    result->unk4 = handlerResult;
    return result;
}

void effTypeDispatch(EffWork *work) {
    D_00355734[work->type].handler(work->unk4);
}

void effTypeDispatchFree(EffWork *work) {
    D_00355738[work->type].handler(work->unk4);
    sdfReleaseChipBlock(work);
}

u32 effGetHandlerArg(EffWork *work) {
    return (u32)work->unk4;
}

u32 func_0018CBC0(u32 *value) {
    return *value;
}

void func_0018CBC8(void) {
}

u32 func_0018CBD0(void) {
    return 1;
}

void effTypeDispatchGuardedA(EffWork *work) {
    void (*handler)(void *) = D_0035573C[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
    }
}

void effTypeDispatchGuardedB(EffWork *work) {
    void (*handler)(void *) = D_00355744[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
    }
}

void effTypeDispatchGuardedC(EffWork *work) {
    void (*handler)(void *) = D_00355740[work->type].handler;

    if (handler != NULL) {
        handler(work->unk4);
    }
}

/* Each effect type keeps its mutable subslot at a different byte offset
 * within the callback payload at EffWork +4. */
void effSetSubSlot(EffWork *work, s32 slotValue) {
    u8 value = slotValue;
    u32 type = work->type;

    switch (type) {
    case 0:
        ((EffSub *)work->unk4)->unk20 = value;
        return;
    case 1:
        ((EffSub *)work->unk4)->unk40 = value;
        return;
    case 2:
        ((EffSub *)work->unk4)->unk50 = value;
        return;
    case 3:
        ((EffSub *)work->unk4)->unk50 = value;
        return;
    case 4:
        ((EffSub *)work->unk4)->unk50 = value;
        return;
    default:
        return;
    }
}

u32 effGetSubSlot(EffWork *work, s32 unusedSlotValue) {
    u8 unusedValue = unusedSlotValue;
    u32 type = work->type;

    switch (type) {
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
    u32 type = work->type;
    u32 handlerArg = 0;

    switch (type) {
    case 0:
        handlerArg = work->unk4;
        break;
    case 1:
        handlerArg = work->unk4;
        break;
    case 2:
        handlerArg = work->unk4 + 0x40;
        break;
    case 3:
        handlerArg = work->unk4 + 0x40;
        break;
    case 4:
        handlerArg = work->unk4 + 0x40;
        break;
    default:
        break;
    }
    /* Required to match: the effect type is passed in the handler's pointer-shaped slot. */
    effAllocDispatch((EffWork *)type, handlerArg);
}

void func_0018CDA0(void) {
}

void func_0018CDA8(void) {
    func_001028E8(0, 0, 0, 0);
}

u32 func_0018CDD0(u32 arg0) {
    return arg0;
}

void func_0018CDD8(void) {
}

void func_0018CDE0(void) {
}

void func_0018CDE8(void) {
}

void func_0018CDF0(void) {
}

void func_0018CDF8(void) {
}

void func_0018CE00(void) {
}

void func_0018CE08(void) {
}

void func_0018CE10(void) {
}

void func_0018CE18(void) {
}

void func_0018CE20(void) {
}

void func_0018CE28(void) {
}

u32 func_0018CE30(u32 arg0) {
    return arg0;
}

void func_0018CE38(void) {
}

u32 func_0018CE40(void) {
    return 0;
}

u32 func_0018CE48(void) {
    return 0;
}

void func_0018CE50(void) {
}

u32 func_0018CE58(void) {
    return 0;
}

void func_0018CE60(void) {
}

s32 func_0018CE68(void) {
    return D_003BB04D;
}

/* An enabled disc directory uses sceDopen; otherwise iteration uses
 * the built-in name table and starts over at entry zero. */
s32 effOpenDataDir(void *name) {
    u8 path[0x70];

    if (D_003BD476 != 0) {
        func_003014F0(path, D_003BB058, name);
        return sceDopen(path);
    } else {
        D_003BD800 = 0;
        return 0;
    }
}


void effRunIfEnabled(void) {
    if (D_003BD476 != 0) {
        func_003101B8();
    }
}

/* In built-in mode the name table supplies entries instead of the
 * directory iterator; clearing 0x1000 marks a synthesized entry. */
s32 effNextDataDirEntry(s32 unused, EffDirEnt *entry) {
    if (D_003BD476 != 0) {
        return func_00310320();
    }
    if ((u32)D_003BD800 >= 0x2F) {
        return 0;
    }
    strcpy(entry->name, D_003557A8[D_003BD800]);
    entry->flags &= ~0x1000;
    D_003BD800++;
    return strlen(entry->name);
}


INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018CF98);

/* The root's +8 points to the first node; each node's +0x38 points to the
 * next. The root also owns the separate allocation stored at +4. */
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

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D428);

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018D4B8);

void effFreeWork(EffWork *work) {
    if (work->unk3C != 0) {
        sdfTexReleaseReferenceViaHandler(work->unk3C);
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
    func_003014F0(destination, D_003BB060, message->unk40[1], message->unk34 + 1);
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

/* Replace the work's sound handle, keeping the newly acquired voice handle
 * but releasing the temporary resource returned by the loader. */
void effSetupWorkSound(EffWork *work, u64 soundResource) {
    u32 voiceHandle;
    u64 loadedResource;
    u32 resourceWords[4];

    if (work->unk3C != 0) {
        sdfTexReleaseReferenceViaHandler(work->unk3C);
        work->unk3C = 0;
    }
    loadedResource = func_002EB028(soundResource, resourceWords, 0);
    voiceHandle = func_002D3288(resourceWords[0]);
    work->unk3C = voiceHandle;
    func_002D0918(loadedResource);
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DA70);

void func_0018DB88(void) {
    func_001028E8(0, 0, 0, 0);
}

void func_0018DBB0(void) {
    func_001028E8(0, 0, 0, 0);
}

void func_0018DBD8(void) {
    u8 *matrix;
    VU0_LOAD_MATRIX(D_003296F0);
    matrix = D_00324610;
    func_002DDD60(matrix);
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
        : : "r"(D_00324660) : "memory");
}

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DC58);

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
u32 func_0018DDF8(u32 colorA, u32 colorB) {
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

INCLUDE_ASM(const s32, "game/code_0018CAC8", func_0018DE78);

void *effAllocSlotArray(s32 count) {
    void *allocation = func_002D03F8(count * 0x38 + 0xC);
    void *retainedAddress = sdfResourceRetainAddress(allocation);
    u32 index = 0;
    EffSlot38 *slot = retainedAddress;
    u8 *headerAddress = (u8 *)slot + count * 0x38;

    ((EffArrHdr *)headerAddress)->unk8 = allocation;
    ((EffArrHdr *)headerAddress)->unk0 = retainedAddress;
    ((EffArrHdr *)headerAddress)->unk4 = count;
    if (count != 0) {
        do {
            index++;
            slot->unk30 = 0;
            slot->unk34 = 0.05f;
            slot = (EffSlot38 *)((u8 *)slot + 0x38);
        } while (index < count);
    }
    return headerAddress;
}

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F88);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0F98);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FA8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FB8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FC8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FD8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FE8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A0FF8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1008);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1018);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1028);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1038);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1048);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1058);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1068);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1078);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1088);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1098);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A10F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1108);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1118);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1128);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1138);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1148);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1158);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1168);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1178);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1188);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1198);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11A8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11B8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11C8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11D8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11E8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A11F8);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1208);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1218);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1228);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1238);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1248);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1258);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1270);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1280);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A1290);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12A0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12B0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12C0);

INCLUDE_RODATA(const s32, "game/code_0018CAC8", D_003A12D0);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB050);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB058);

INCLUDE_SDATA(const s32, "game/code_0018CAC8", D_003BB060);

