#include "common.h"
#include "fpu.h"

extern s32 D_00435F14;

extern s32 D_00435F18;

extern s32 D_00435F10;

extern s32 D_00435E04;

extern u32 sdfCreateResetPacketList(void);

extern u64 sdfAllocPacketAligned(u64);

extern u64 func_0019F460(s32, s32, u64, u64, u64, u64);

extern u32 D_00436014;

extern u32 D_00436018;

extern u32 D_00435FC0;

extern u32 D_00436060;

extern s32 D_00436068;

extern s32 func_003283E0(u32);

extern u32 D_00436088;

extern u64 dds3GetWorldObject(void);

extern s64 func_00110C18(u64);

extern s64 func_00125F38(void);

extern u32 D_004360AC;

extern u32 D_004360C4;

extern u32 D_004360C8;

extern u32 D_004360CC;

extern u32 D_004360D0;

extern u32 D_00435F0C;

extern s32 D_004360F8;

extern u32 D_004360FC;

extern s32 D_00436100;

extern s32 D_00438ECC;

extern u32 D_0043610C;

extern u32 D_00436108;

extern u32 D_00435FCC;

extern u32 fileRequestIsReady(u32 arg0);

extern u8 D_00435BB4;

extern u32 D_003897E8[];

extern char D_00444950[];

extern void func_0012A3D8(char *arg0);

extern s32 strcmp(const char *a, const char *b);

extern u32 D_00435FD8;

extern u32 D_00435FC4;

extern u8 D_00436020[];

extern void func_003298C0(u32 arg0);

extern void func_002C7CE8(u32 arg0);

extern u8 D_003846F0[];

extern u8 D_0037F610[];

extern u8 D_0037F660[];

extern void func_00336C10(void *src);

extern u32 D_00389904[];

extern u32 D_00389910[];

extern u32 D_003899C0[];

extern void func_0012D9D0(u32 value);

extern void func_001295E0(u32, u32);

extern void fldCreatePlayerObject(void);

extern void func_00128FE8(u32, u32, s32);

extern u32 D_00436064;

extern u32 D_0043607C;

extern u32 D_00436080;

extern u32 D_00438EC8;

extern u8 D_0038A700[];

extern void *func_003335E0(void);

extern u32 func_0032C138(void *);

extern u32 D_004360B0;

extern u8 D_00444980[];

extern u8 D_00444970[];

extern void func_00113110(s64 arg0, void *arg1, void *arg2);

extern f32 sdfAtan2(f32 arg0, f32 arg1);

extern f32 D_0038BAB0[];

extern f32 D_0038BAC0[];

/* Data transfer descriptor: source-relative byte offset and transfer size. */
typedef struct FldTransferChunk {
    u32 unk0;
    s32 offset;
    u32 size;
} FldTransferChunk;

extern f32 D_003897DC[];

extern s32 D_00389770[];

extern char D_004130D8[]; /* "%sf%03d_%03d.LB" */

extern s32 func_0035C860(char *, const char *, ...);

extern u32 func_002C7FF0(char *);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern s32 func_0035C860(char *, const char *, ...);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern void fldFormatAreaDirectory(char *, s32, s32);

extern s32 func_0035C860(char *, const char *, ...);

extern void *func_003292A8(s32 size);

extern void *sdfResourceRetainAddress(void *p);

extern u32 D_00435FF0, D_00435FF4, D_00435FF8, D_00435FFC;

extern u32 D_00436000, D_00436004, D_00436008, D_0043600C;

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u64);
    u32 unk14[3];
} FieldBufferDescriptor;

extern void sdfResetPacketList(u64);

extern u32 D_0037FB48[];

extern void sdfResetPacketList(u64);

extern void sdfAppendPacket(u64, u64);

extern void sdfAppendPacket(u64, u64);

extern void sdfAppendPacket(u64, u64);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 sdfConsCalculateDrawPacketSize(s32, s32);

extern void func_0033A2D8(u64, s32, s32, s32, s32);

extern u64 *func_0033A2D0(u64);

extern s32 func_00100400(void);

extern u8 D_00381ED0[];

extern void func_0032DB30(const void *, u64, s32);

extern void func_0032CF98(u64, u64);

extern void func_0032DB78(const void *, u64, s32);

extern f32 D_0038A980[];

extern u32 D_0038A9A0[];

extern u64 func_00348158(const void *, const void *, s32, s32);

extern void *memset(void *s, s32 c, u32 n);

typedef struct FldPrimDesc {
    s16 kind;
    s16 count;
    u8 pad4[4];
    s32 color;
    u8 padC[4];
    f32 *verts;
    u8 pad14[0xC];
    s32 *indices;
    u8 pad24[8];
} FldPrimDesc; /* 0x2C bytes */

extern void sdfConsAppendClearPacket(u64, s32);

extern void sdfConsAppendAssetPacket(u64, u32, s32);

extern u64 func_0033B050(FldPrimDesc *);

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u32);
    u32 unk14[3];
} FieldResourceDescriptor;

extern u32 D_0040B2A0[];

extern u32 sdfAllocatePacketList(s32);

extern void sdfCreateResourcePacket(u32, u32, s32, s32, s32, s32, u32, s32, s32, s32);

extern void sdfCreateDescriptorPacket(u32, u32, s32, s32, s32, s32, u32, s32);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 x;          /* 0x1C: quad origin X */
    s32 y;          /* 0x20: quad origin Y */
    s32 unk24;
    s32 packetList; /* 0x28 */
} FldQuadState; /* 0x2C bytes */

typedef struct {
    u8 pad0[0x10];
    void (*invoke)(void *, s32);
} FldGfxCallback;

extern FldGfxCallback D_00380748;

extern void sdfPktInit(void *, s32, s32, s32, s32);

extern s32 func_0033D7B8();

extern FldGfxCallback D_00380708;

extern u8 D_00436070[];

extern u8 D_00436078[];

extern s32 D_00436090;

extern u32 D_0043608C;

extern s32 func_0022E450(void);

extern s32 fldEncProc(void);

extern void func_0022E0E0(void);

extern void mdlSetNodeFloat20(s32, s32, f32);

extern void mdlAddEntryFlagged(s32, s32, s32);

extern f32 D_00436110;

extern s32 D_00436114;

extern f32 func_003406A0(f32);

extern s32 D_00389780[];

extern s32 D_004360E8;

extern s32 D_00436118, D_0043611C;

extern s32 *D_00436104;

extern f32 D_00436120, D_00436124;

extern s32 D_00389780[];

extern s32 D_004360E8;

extern void func_00232E38(s32 arg0);

extern void func_00232E80(s32 arg0);

extern s32 mdlAddEntryPlainEx(s32, s32, s32, f32, f32);

extern u32 D_0043612C;

extern u32 D_00436130;

extern u32 D_00436134;

extern u32 D_003899B4[];

extern void func_00135A68(u32 arg0, s32 arg1);

extern u32 D_00436128;

extern u32 D_00436158;

extern s16 D_00389898[];

extern u8 D_0037F650[];

extern u8 D_0037F9B0[];

extern u8 D_0037F9F0[];

extern u8 D_0037FA00[];

extern u8 D_00384790[];

extern s32 sdfTexGetPrimaryBuffer(s32);

extern s32 func_0032B1B8(s32);

extern void sdfConsInitDmaPacketHeader(u64, s32, s32);

extern void sdfAppendReferencePacket(u64, u64);

extern void func_003365B8(f32);

extern void sdfInitGeometryDmaPacket(u64, f32 *);

extern void func_0033B530(u64, u8 *, s32, u8 *, u8 *);

extern s32 D_00435F30;

extern void func_00139950(f32 *);

typedef struct {
    u8 pad0[0x84];
    s32 unk84;
    u8 pad88[0xB8];
    f32 unk140;
    f32 unk144;
    f32 unk148;
    f32 unk14C;
    f32 unk150;
    f32 unk154;
    f32 unk158;
    f32 unk15C;
    f32 unk160;
    u8 pad164[0x14];
    s32 unk178;
} FldCamState;

typedef struct FldCameraOverrides {
    u8 pad00[0x174];
    f32 currentHeading;      /* 0x174 */
    u8 pad178[0x1C];
    u32 xyPending;           /* 0x194 */
    f32 xyValue0;            /* 0x198 */
    f32 xyValue1;            /* 0x19C */
    u32 headingPending;      /* 0x1A0 */
    f32 targetHeading;       /* 0x1A4 */
} FldCameraOverrides;

typedef struct FldAreaResourceState {
    s32 pad00[30];
    s32 resourceFlag;     /* 0x78 */
    s32 area;             /* 0x7C */
    s32 room;             /* 0x80 */
} FldAreaResourceState;

extern void func_0012B518(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

extern void btlActivateRuntime(s32 mode);

extern void func_00110A88(u64, s8);

extern char D_00436098[];

extern void func_0022E338(void);

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern void func_00131000(s16, s32, f32);

extern s32 fldGetLocationCoordinateValue(s32, s32);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00128FE8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001295E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129660);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129940);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129B40);

void func_00129CC0(u32 *args) {
    s32 state;
    func_001295E0(args[4], args[3]);
    state = D_00389770[4];
    if (state != 1 && state < 200) fldCreatePlayerObject();
    func_00128FE8(args[1], args[0], 0);
    D_00389770[1] = ((u32 *)args[2])[1];
}

void func_00129D40(u32 *arg0) {
    func_00128FE8(arg0[1], *arg0, 1);
}

s32 func_00129D60(s32 arg0) {
    return arg0 + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129D68);

void func_00129E50(u32 buffer, FldTransferChunk *chunk) {
    sdfRelocatePackedResourceWords(buffer, buffer, (s32)buffer + chunk->offset, chunk->size);
}

void func_00129E78(u32 buffer, FldTransferChunk *chunk) {
    func_00129D68(buffer, buffer, (s32)buffer + chunk->offset, chunk->size);
}

void fldSetPendingAreaAndFloor(u32 arg0, u32 arg1) {
    D_00436014 = arg0;
    D_00436018 = arg1;
}

s32 fldLoadAreaResource(void) {
    char directory[64];
    char path[80];
    u32 area = D_00436014;
    u32 floor = D_00436018;

    if (area != 0 || floor != 0) {
        fldFreeDisplayObjects();
        ((FldAreaResourceState *)D_00389770)->area = area;
        ((FldAreaResourceState *)D_00389770)->room = floor;
        fldFormatAreaDirectory(directory, area, 1);
        func_0035C860(path, D_004130D8, directory, area, floor);
        D_00435FCC = func_002C7FF0(path);
        ((FldAreaResourceState *)D_00389770)->resourceFlag = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00129F58);

typedef struct FldDisplayNode {
    struct FldDisplayNode *next;
    u8 pad04[4];
    u32 displayObject;
} FldDisplayNode;

typedef struct FldDisplayWork {
    u8 pad00[0x60];
    FldDisplayNode *objects;
} FldDisplayWork;

void fldFreeDisplayObjects(void) {
    if (D_00435FCC != 0) {
        FldDisplayNode *node = ((FldDisplayWork *)D_00435FCC)->objects;

        if (node != 0) {
            do {
                func_003298C0(node->displayObject);
                node = node->next;
            } while (node != 0);
        }
        func_002C7CE8(D_00435FCC);
        D_00435FCC = 0;
    }
    D_00436014 = 0;
    D_00436018 = 0;
    ((FldAreaResourceState *)D_00389770)->area = 0;
    ((FldAreaResourceState *)D_00389770)->room = 0;
    ((FldAreaResourceState *)D_00389770)->resourceFlag = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 temp_v1 = ((FldAreaResourceState *)D_00389770)->resourceFlag;

    if (temp_v1 != 0) {
        if (temp_v1 == 1) {
            if (fileRequestIsReady(D_00435FCC) != 0) {
                ((FldAreaResourceState *)D_00389770)->resourceFlag = 0;
                D_00435BB4 = 0;
            }
        }
    }
    return 0;
}

u32 fldGetResourceReadyFlag(void) {
    return D_003897E8[0];
}

u8 fldIsAreaResourceReady(void) {
    if (D_00435FCC != 0) {
        if (fileRequestIsReady(D_00435FCC) != 0) {
            return 1;
        }
    }
    return D_003897E8[0] != 0;
}

s32 fldIsAreaFloorResourceReady(s32 area, s32 room) {
    if (((FldAreaResourceState *)D_00389770)->area != area || ((FldAreaResourceState *)D_00389770)->room != room) {
        return 0;
    }
    if (D_00435FCC != 0 && fileRequestIsReady(D_00435FCC) != 0) {
        return 1;
    }
    return ((FldAreaResourceState *)D_00389770)->resourceFlag != 0;
}

void *func_0012A1F8(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)D_00389770;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = func_003292A8(D_00436000);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FF0, D_00436000);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A270(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)D_00389770;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = func_003292A8(D_00436004);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FF4, D_00436004);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A2E8(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)D_00389770;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = func_003292A8(D_00436008);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FF8, D_00436008);
            return buffer;
        }
    }
    return NULL;
}

void *func_0012A360(void **destination, s32 area, s32 room) {
    FldAreaResourceState *state = (FldAreaResourceState *)D_00389770;

    if (state->area == area) {
        if (state->room == room) {
            void *buffer = func_003292A8(D_0043600C);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_00435FFC, D_0043600C);
            return buffer;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004130D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A3D8);

u8 func_0012A4E0(void) {
    char buf[32];

    func_0012A3D8(buf);
    return strcmp(D_00444950, buf) != 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A510);

void fldReleaseAreaResourceCache(void) {
    u32 temp_v0 = D_00435FD8;

    if (temp_v0 != 0) {
        func_003298C0(temp_v0);
        D_00435FD8 = 0;
    }
    temp_v0 = D_00435FC4;
    if (temp_v0 != 0) {
        func_002C7CE8(temp_v0);
        D_00435FC4 = 0;
    }
    D_00444950[0] = D_00436020[0];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012A6F0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012AC90);

extern void fldClearMenuEntries();
extern void fldDestroyTitleTask();
extern void fldFreeSceneResources();
extern void fldPlayPendingSounds();
extern void fldReleaseObjectSlots();
extern void fldReleaseResourceSlots();
extern void fldReleaseTextureSlots();
extern void fldResetObjectSlots();
extern void fldResetRecordState();
extern s32 fldTitleMiniIsActive();
extern void func_0013B818();
extern void func_00144598();
extern void func_001457E0();
extern void func_0014A228();
extern void func_0014B748();
extern void kwlnTaskDestroyWithHierarchyByName();
extern void mnuReleaseResourceEntries();
extern void sdfResourceListRelease();
extern u32 D_00444930[];
extern u32 D_00444940[];
extern s32 D_00435FA0;
extern s32 D_00435FA4;
extern FldTransferChunk *D_00435FA8;
extern u32 D_00435FAC;
extern FldTransferChunk *D_00435FB0;
extern u32 D_00435FB4;
extern FldTransferChunk *D_00435FB8;
extern u32 D_00435FBC;
extern FldDisplayWork *D_00435FC8;
extern s32 D_00435FDC;
extern u32 D_00435FE0;
extern u32 D_00435FE4;
extern u32 D_00435FE8;
extern u32 D_00435FEC;

void func_0012ADA0(void) {
    s32 i;
    FldDisplayNode *node;

    func_0012DC98();
    fldPlayPendingSounds();
    fldDestroyTitleTask();
    if (fldTitleMiniIsActive() != 0) {
        kwlnTaskDestroyWithHierarchyByName("fldTitleMini", 1);
    }
    fldReleaseObjectSlots();
    fldResetObjectSlots();
    if (D_00389770[4] < 0xC8) {
        if (D_00389770[8] != 0) {
            if ((u32)(D_00389770[4] - 0x1B) < 2U) {
                fldReleaseResourceSlots(D_00389770);
                func_0014A228();
                func_001457E0();
                fldReleaseAreaResourceCache();
            }
        } else {
            fldReleaseResourceSlots(D_00389770);
            func_0014A228();
            func_001457E0();
            fldFreeSceneResources();
            fldReleaseAreaResourceCache();
            func_00134790();
        }
        fldResetRecordState();
        func_0013B818();
        func_00144598();
        fldClearMenuEntries();
        mnuReleaseResourceEntries();
        fldReleaseTextureSlots();
        func_0014B748();
    }
    sdfResourceListRelease(D_00435FA4, 1);
    D_00435FA4 = 0;
    for (i = 0; i < 4; i++) {
        if (D_00444930[i] != 0) {
            func_003298C0(D_00444930[i]);
            D_00444930[i] = 0;
            D_00444940[i] = 0;
        }
    }
    if (D_00435FC0 != 0) {
        if (D_00435FA8 != 0) {
            func_00129E78(D_00435FAC, D_00435FA8);
            D_00435FA8 = 0;
            D_00435FAC = 0;
        }
        if (D_00435FB0 != 0) {
            func_00129E78(D_00435FB4, D_00435FB0);
            D_00435FB0 = 0;
            D_00435FB4 = 0;
        }
        if (D_00435FB8 != 0) {
            func_00129E78(D_00435FBC, D_00435FB8);
            D_00435FB8 = 0;
            D_00435FBC = 0;
        }
    }
    D_00435FA0 = 0;
    D_00435FDC = 0;
    if (D_00435FE0 != 0) {
        func_003298C0(D_00435FE0);
        D_00435FE0 = 0;
    }
    if (D_00435FE4 != 0) {
        func_003298C0(D_00435FE4);
        D_00435FE4 = 0;
    }
    if (D_00435FE8 != 0) {
        func_003298C0(D_00435FE8);
        D_00435FE8 = 0;
    }
    if (D_00435FEC != 0) {
        func_003298C0(D_00435FEC);
        D_00435FEC = 0;
    }
    if (D_00389770[4] < 0xC8 && D_00389770[7] != D_00389770[4]) {
        D_00389770[7] = D_00389770[4];
    }
    if (D_00435FC8 != 0) {
        node = D_00435FC8->objects;
        i = 0;
        if (node != 0) {
            do {
                if (i > 0) {
                    func_003298C0(node->displayObject);
                }
                node = node->next;
                i++;
            } while (node != 0);
        }
        func_002C7CE8(D_00435FC8);
        D_00435FC8 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", fldFormatAreaDirectory);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B0D0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413198);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B2B8);

void func_0012B4A0(u32 arg0) {
    D_00435FC0 = arg0;
}

void fldInitDisplayObjects(void) {
    if (D_00436064 == 0) {
        void *object;
        D_00436064 = 1;
        object = func_003335E0();
        D_0043607C = (u32)object;
        *(f32 *)((u8 *)object + 0x1C) = 1.0f;
        D_00438EC8 = (u32)func_003335E0();
        D_00436080 = func_0032C138(D_0038A700);
    }
}

u32 *fldGetDisplayTableRow(void) {
    return &D_0037FB48[D_00436060 * 8];
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B518);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B690);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012B7F8);

void fldProjectPointSetup(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003846F0) : "memory");
    func_00336C10(D_0037F610);
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        ".set reorder"
        : : "r"(vec) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037F650) : "memory");
    __asm__ volatile ("vmul.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037F660) : "memory");
    __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

void fldProjectPointSetupAlt(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];

    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_00384790) : "memory");
    func_00336C10(D_0037F9B0);
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        ".set reorder"
        : : "r"(vec) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037F9F0) : "memory");
    __asm__ volatile ("vmul.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037FA00) : "memory");
    __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

void func_0012BB68(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003846F0) : "memory");
    matrix = D_0037F610;
    func_00336C10(matrix);
    __asm__ volatile (
        ".set noreorder\n"
        "vmove.xyzw vf24, vf28\n"
        "vmove.xyzw vf25, vf29\n"
        "vmove.xyzw vf26, vf30\n"
        "vmove.xyzw vf27, vf31\n"
        ".set reorder"
        : : : "memory");
    matrix += 0x40;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(matrix) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf12, 0(%0)\n"
        ".set reorder"
        : : "r"(D_0037F660) : "memory");
}

void func_0012BBD0(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
    f32 vec[4] = { x, y, z, 1.0f };
    f32 result[4];
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf10, 0(%0)\n"
        "vmulax.xyzw ACC, vf28, vf10x\n"
        "vmadday.xyzw ACC, vf29, vf10y\n"
        "vmaddaz.xyzw ACC, vf30, vf10z\n"
        "vmaddw.xyzw vf10, vf31, vf0w\n"
        "vdiv Q, vf0w, vf10w\n"
        "vmove.w vf10, vf0\n"
        "vwaitq\n"
        "vmulq.xyzw vf10, vf10, Q\n"
        "vmul.xyzw vf10, vf10, vf11\n"
        "vadd.xyzw vf10, vf10, vf12\n"
        ".set reorder"
        : : "r"(vec) : "memory");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

void func_0012BC38(u32 arg0) {
    D_00436060 = arg0;
}

void func_0012BC40(s32 lower, s32 bits, u64 upper) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 packet;
    u64 *entry;
    FieldBufferDescriptor *descriptor;

    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    entry = func_0033A290(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitFrameQuad(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 packet;
    u64 *data;
    FieldBufferDescriptor *descriptor;

    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    data = func_0033A290(packet, 0x30);
    data[4] = (arg7 << 17) | 0x10000 | (arg5 << 15) | (arg4 << 14) | (arg3 << 12) | (arg2 << 4) | (arg1 << 1) | arg0;
    data[5] = 0x47;
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012BE18);

void fldSubmitGsLinesScaled(s32 x0, s32 y0, s32 x1, s32 y1, u32 arg4, u32 arg5, u32 arg6) {
    s32 coords[4];
    u64 command;
    u64 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    FieldBufferDescriptor *descriptor;
    s32 i;

    coords[0] = x0 * 16;
    coords[1] = y0 * 16;
    coords[2] = x1 * 16;
    coords[3] = y1 * 16;
    command = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    func_0033A2D8(packet, 0x49, 2, 0x41, 2);
    dst = func_0033A2D0(packet);
    lo = (u64)arg4 | ((u64)arg5 << 32);
    hi = (u64)arg6 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsLines(u32 x0, u32 y0, u32 x1, u32 y1, u32 arg4, u32 arg5, u32 arg6) {
    u32 coords[4];
    u64 command;
    u64 packet;
    u64 *dst;
    FieldBufferDescriptor *descriptor;
    u32 *pos;
    u64 lo;
    u64 hi;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y1;
    command = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    func_0033A2D8(packet, 0x49, 2, 0x41, 2);
    dst = func_0033A2D0(packet);
    lo = (u64)arg4 | ((u64)arg5 << 32);
    hi = (u64)arg6 | (0x8000LL << 24);
    pos = coords;
    for (i = 0; i < 2; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void fldSubmitGsQuadTagged(s32 x, s32 y, s32 w, s32 h, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    s32 coords[8];
    u64 command;
    u64 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    s32 *pos;
    FieldBufferDescriptor *descriptor;
    s32 i;

    coords[0] = x * 16;
    coords[1] = y * 16;
    coords[2] = (x + w) * 16;
    coords[3] = y * 16;
    coords[4] = (x + w) * 16;
    coords[5] = (y + h) * 16;
    coords[6] = x * 16;
    coords[7] = (y + h) * 16;
    command = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_0033A2D8(packet, 0x4D, 2, 0x41, 4);
    dst = func_0033A2D0(packet);
    lo = (u64)arg4 | ((u64)arg5 << 32);
    hi = (u64)arg6 | ((u64)arg7 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFF;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C360);

void fldSubmitGsRect(s32 x0, s32 y0, s32 x1, s32 y1, u32 arg4, u32 arg5, u32 arg6, u32 arg7) {
    u32 coords[8];
    u64 command;
    u64 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u32 *pos;
    FieldBufferDescriptor *descriptor;
    s32 i;

    coords[0] = x0;
    coords[1] = y0;
    coords[2] = x1;
    coords[3] = y0;
    coords[4] = x1;
    coords[5] = y1;
    coords[6] = x0;
    coords[7] = y1;
    command = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_0033A2D8(packet, 0x4D, 2, 0x41, 4);
    dst = func_0033A2D0(packet);
    lo = (u64)arg4 | ((u64)arg5 << 32);
    hi = (u64)arg6 | ((u64)arg7 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = 0xFFFFFFFFULL;
        dst[0] = (u64)pos[0] | ((u64)pos[1] << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C650);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012C888);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012CB08);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012CDC0);

void fldSubmitModelPacket(s32 arg0, u8 *arg1) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 header;
    u64 packet;
    f32 mat[16];
    FieldBufferDescriptor *descriptor;

    sdfResetPacketList(command);
    header = sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader(header, sdfTexGetPrimaryBuffer(arg0), func_0032B1B8(arg0));
    sdfAppendReferencePacket(command, header);
    func_003365B8(*(f32 *)(arg1 + 0x44));
    __asm__ volatile(
        ".set noreorder\n"
        "sqc2 vf28, 0(%0)\n"
        "sqc2 vf29, 16(%0)\n"
        "sqc2 vf30, 32(%0)\n"
        "sqc2 vf31, 48(%0)\n"
        ".set reorder"
        : : "r"(mat) : "memory");
    packet = sdfAllocPacketAligned(0x38);
    sdfInitGeometryDmaPacket(packet, mat);
    sdfAppendPacket(command, packet);
    packet = sdfAllocPacketAligned(0x80);
    func_0033B530(packet, arg1, *(s32 *)(arg1 + 0x40), arg1 + 0x10, arg1 + 0x20);
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void func_0012D070(void) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 texture;
    FieldBufferDescriptor *descriptor;
    sdfResetPacketList(command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB30(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(command, texture);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void func_0012D110(void) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 texture;
    FieldBufferDescriptor *descriptor;
    sdfResetPacketList(command);
    texture = sdfAllocPacketAligned(0x40);
    func_0032DB78(D_00381ED0 + func_00100400() * 0x1F40, texture, 0);
    func_0032CF98(command, texture);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

void func_0012D1B0(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    u64 resource;
    u64 record;
    FieldBufferDescriptor *descriptor;
    D_0038A980[0] = x;
    D_0038A980[1] = y;
    D_0038A980[2] = z;
    D_0038A980[4] = u;
    D_0038A980[5] = v;
    D_0038A980[6] = w;
    D_0038A9A0[1] = second;
    D_0038A9A0[0] = first;
    resource = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(resource);
    record = func_00348158(D_0038A980, D_0038A9A0, 2, 0x80);
    sdfAppendPacket(resource, record);
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, resource);
}

void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    FldPrimDesc desc;
    f32 verts[12];
    s32 indices[3];
    u64 command;
    FieldBufferDescriptor *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfResetPacketList(command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, D_0043607C, 0);
    memset(&desc, 0, 0x2C);
    desc.color = 0x80808080;
    desc.kind = 1;
    desc.count = 3;
    desc.verts = verts;
    desc.indices = indices;
    verts[0] = f0;
    verts[1] = f1;
    verts[2] = f2;
    verts[4] = f3;
    verts[5] = f4;
    verts[6] = f5;
    verts[8] = f6;
    verts[9] = f7;
    verts[10] = f8;
    indices[0] = a0;
    indices[1] = a1;
    indices[2] = a2;
    sdfAppendPacket(command, func_0033B050(&desc));
    descriptor = (FieldBufferDescriptor *)&D_0037FB48[D_00436060 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D3E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D5C0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D7E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012D9D0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DAA0);

void func_0012DB68(s32 x, s32 y, s32 width, s32 height) {
    func_0012B518(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, D_00436080);
    func_0012B518(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, D_00436080);
    func_0012B518(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, D_00436080);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_0012BE18(0);
}

void func_0012DC70(void) {
    if (D_00436068 == 0) {
        D_00436068 = func_003283E0(0x70000);
    }
}

void func_0012DC98(void) {
    if (D_00436068 != 0) {
        func_00328470(D_00436068);
        D_00436068 = 0;
    }
}

void func_0012DCC8(void) {
    if (D_00436068 != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateResourcePacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, D_00436068, 0, 0, 0);
        descriptor = (FieldResourceDescriptor *)&D_0037FB48[D_00436060 * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012DD48(void) {
    if (D_00436068 != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateDescriptorPacket(packet, D_0040B2A0[0], 0, 0, 0x200, 0xE0, D_00436068, 0);
        descriptor = (FieldResourceDescriptor *)&D_0037FB48[D_00436060 * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012DDC0(s32 arg0, s32 arg1, u64 arg2, u64 arg3) {
    u64 temp_v0;

    temp_v0 = func_0019F460(arg0 << 4, arg1 << 4, 0, arg2, arg3, 0);
    func_0019D518(temp_v0);
    func_0019C5B0(temp_v0);
}

void fldAdvanceQuadRow(FldQuadState *quad) {
    quad->y = quad->y + 0x60;
}

void fldStartQuadPacketList(FldQuadState *quad) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v1 = sdfCreateResetPacketList();
    quad->packetList = temp_v1;
    temp_v0 = sdfAllocPacketAligned(0x40);
    func_0032E4B8(temp_v0);
    sdfAppendPacket(quad->packetList, temp_v0);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012DE70);

void fldDrawFloorQuad(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x73C0;
    quad.y = 0x7CC0;
    quad.unk24 = 0x0FFFFF7E;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7D;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x, quad.y + y, quad.unk24, 0);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, arg2));
    fldAdvanceQuadRow(&quad);
    D_00380748.invoke(&D_00380748, quad.packetList);
}

void fldDrawFloorQuadA(s32 x, s32 y, s32 arg2, s32 arg3) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x73C0;
    quad.y = 0x7CC0;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x, quad.y + y, quad.unk24, arg2);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, arg3));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadTiled(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x7000;
    quad.y = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x * 16, quad.y + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, D_00436070, arg2));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadTiledAlt(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x7000;
    quad.y = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x * 16, quad.y + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, D_00436078, arg2));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuad(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x7000;
    quad.y = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x * 16, quad.y + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, arg2));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x7000;
    quad.y = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + x * 16, quad.y + y * 8, quad.unk24, arg2);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, arg3));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

void fldDrawMapQuadScaled(s32 arg0, s32 arg1, f32 x, f32 y) {
    FldQuadState quad;
    u8 packet[16];

    quad.x = 0x7000;
    quad.y = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList(&quad);
    sdfPktInit(packet, quad.x + (s32)(x * 16.0f), quad.y + (s32)(y * 8.0f), quad.unk24, arg0);
    sdfAppendPacket(quad.packetList, func_0033D7B8(packet, arg1));
    fldAdvanceQuadRow(&quad);
    D_00380708.invoke(&D_00380708, quad.packetList);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E720);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012E958);

s32 func_0012EB78(void) {
    s32 state = D_00436090;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_0043608C;
        } else {
            result = func_0022E450();
        }
    } else {
        result = D_0043608C;
    }
    return result;
}

void func_0012EBB8(u32 arg0) {
    D_00436088 = arg0;
}

s32 fldEncProc(void) {
    s32 state = D_00436090;

    if (state < 3) {
        if (state >= 0) {
            func_0022E0E0();
        }
    }
    return 0;
}

void func_0012EBF8(u32 arg0, s32 arg1) {
    if ((arg1 < 0x400) && ((*(u16 *)((s32)arg1 * 0x28 + D_00435E04 + 0x20) & 0x8000) != 0))
    {
        func_00105FE8(0);
        func_0012EC80(3);
        return;
    }
    sndSetSequenceVolumePan(0xf, 0x7f, 0x3f);
    func_0012EC80(arg0);
}

s32 func_0012EC80(s32 mode) {
    D_00436090 = mode;
    D_0043608C = 0;
    if (func_0012EB78() == 0) {
        if (D_00436090 < 3) {
            if (D_00436090 >= 0) {
                btlActivateRuntime(D_00436090);
                if (dds3GetWorldObject() != 0) {
                    func_00110A88(dds3GetWorldObject(), 1);
                }
            }
        }
    }
}

void func_0012ECF0(void) {
    btlResetAsyncState();
}

void func_0012ED08(void) {
    kwlnTaskCreate((s32)D_00436098, 0x2B0F, 0, 1, (s32)fldEncProc, (s32)func_0012ECF0, 0);
    func_0022E338();
}

void func_0012ED48(void) {
    f32 *distance = D_003897DC;
    f32 dx = D_0038BAB0[0] - D_0038BAC0[0];
    f32 dy = D_0038BAB0[1] - D_0038BAC0[1];
    f32 dz = D_0038BAB0[2] - D_0038BAC0[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012EDB0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F078);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F400);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F770);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012F908);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_0012FA58);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001300A0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001302A0);

s64 func_001309B8(void) {
    u64 temp_v0;
    s64 temp_v1;
    s64 temp_v2;

    temp_v0 = dds3GetWorldObject();
    temp_v1 = func_00110C18(temp_v0);
    temp_v2 = func_00125F38();
    if (temp_v2 == temp_v1) {
        temp_v1 = 0;
    }
    return temp_v1;
}

void fldSetCameraMoveMode(u32 value) {
    D_004360AC = value;
    func_00113110(func_00110C18(dds3GetWorldObject()), D_00444980, D_00444970);
    D_004360B0 = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130A40);

void func_00130C20(void) {
    D_004360AC = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130C28);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130DE0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00130F38);

void func_00130F80(void) {
    D_004360C8 = 0;
    D_004360C4 = 0xffffffff;
    D_004360CC = 0;
    D_004360D0 = 0;
}

void func_00130F98(u32 arg0) {
    if (arg0 == 0) {
        D_004360D0 = 0;
        if (D_00435F14 != 0) {
            func_00232E80(D_00435F14);
        }
    } else {
        D_004360D0 = arg0;
        func_00232E38(D_00435F14);
    }
}

void func_00130FF0(u32 arg0, u32 arg1) {
    D_004360C4 = arg0;
    D_004360C8 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131000);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131478);

INCLUDE_ASM(const s32, "game/code_00128FE8", fldUpdateCameraTarget);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00131B50);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001321F8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001322D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132408);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00132540);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133840);

u8 func_00133950(void) {
    if (D_00389770[0x62] == 0) {
        if (D_00389770[0x64] == 0) {
            if (D_00389770[0x63] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void func_00133988(void) {
    dds3SetObjectFlags(D_00435F0C, 1);
    if (D_00435F10 != 0) {
        dds3SetObjectFlags(D_00435F10, 1);
    }
    func_00133C90();
}

void func_001339C0(void) {
    dds3ClearObjectFlags(D_00435F0C, 1);
    if (D_00435F10 != 0) {
        dds3ClearObjectFlags(D_00435F10, 1);
        return;
    }
}

void func_00133A00(void) {
    func_00112B58(D_00435F0C, 0);
    dds3ClearObjectFlags(D_00435F0C, 0x800);
}

void func_00133A28(void) {
    func_00112B58(D_00435F0C, 0x80);
    dds3SetObjectFlags(D_00435F0C, 0x800);
}

void func_00133A50(void) {
    u8 temp_v0;

    temp_v0 = D_00435F10 != 0;
    *(u32 *)(*(s32 *)(D_00435F14 + 0x18) + 0x1c) = 0;
    if (temp_v0) {
        *(u32 *)(*(s32 *)(D_00435F18 + 0x18) + 0x1c) = 0;
    }
}

void func_00133A78(void) {
    u8 temp_v0;

    temp_v0 = D_00435F10 != 0;
    *(u32 *)(*(s32 *)(D_00435F14 + 0x18) + 0x1c) = 0x80808080;
    if (temp_v0) {
        *(u32 *)(*(s32 *)(D_00435F18 + 0x18) + 0x1c) = 0x80808080;
    }
}

void func_00133AA8(void) {
    if (D_00435F10 != 0) {
        dds3SetObjectFlags(D_00435F0C, 0x400);
        return;
    }
    dds3SetObjectFlags(D_00435F0C, 0x200);
}

void func_00133AE8(void) {
    dds3ClearObjectFlags(D_00435F0C, 0x400);
    dds3ClearObjectFlags(D_00435F0C, 0x200);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133B10);

void func_00133C18(void) {
    s16 node = *(s16 *)(D_00435F14 + 0x12);
    if (fldGetLocationCoordinateValue(D_00389770[4], D_00389770[5] + 1) & 0x40) {
        func_00131000(node, 0x12, 10.0f);
        return;
    }
    func_00131000(node, 3, 10.0f);
}

void func_00133C90(void) {
    s16 node = *(s16 *)(D_00435F14 + 0x12);
    if (fldGetLocationCoordinateValue(D_00389770[4], D_00389770[5] + 1) & 0x40) {
        func_00131000(node, 0x12, 0.0f);
        return;
    }
    func_00131000(node, 3, 0.0f);
}

s32 func_00133CF8(s32 value) {
    s32 object = D_00435F14;
    *(f32 *)(*(s32 *)(object + 0x1C) + 0x20) = 1.0f;
    return mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

void func_00133D40(s32 first, s32 second) {
    mdlSetNodeFloat20(D_00435F14, 0, 1.0f);
    mdlSetNodeFloat20(D_00435F14, 1, 1.0f);
    mdlAddEntryFlagged(D_00435F14, 0, first);
    mdlAddEntryFlagged(D_00435F14, 1, second);
}

void func_00133DB8(void) {
    D_00389904[0] = 0;
}

void func_00133DC8(void) {
    D_00389910[0] = 0;
}

void func_00133DD8(f32 first, f32 second) {
    FldCameraOverrides *camera = (FldCameraOverrides *)D_00389770;

    camera->xyValue0 = first;
    camera->xyValue1 = second;
    camera->xyPending = 1;
}

void func_00133DF8(f32 x, f32 unusedY, f32 z) {
    FldCameraOverrides *camera;
    f32 angle;

    angle = sdfAtan2(x, z);
    camera = (FldCameraOverrides *)D_00389770;
    angle *= 180.0f / 3.14f;
    camera->headingPending = 1;
    camera->targetHeading = -angle;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133E38);

void func_00133EE0(void) {
    FldCameraOverrides *camera = (FldCameraOverrides *)D_00389770;

    if (camera->headingPending != 0) {
        camera->headingPending = 2;
        camera->currentHeading = camera->targetHeading;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00133F08);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413280);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413290);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132A0);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_004132E0);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001343E8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134620);

void func_00134790(void) {
    if (D_00436100 != 0) {
        func_0032BBB0(D_00436100);
        D_00436100 = 0;
    }
    if (D_00438ECC != 0) {
        func_003298C0(D_00438ECC);
        D_00438ECC = 0;
    }
    if (D_004360F8 != 0) {
        func_002DEBB0(D_004360F8);
        D_004360F8 = 0;
    }
    D_004360FC = 0;
}

INCLUDE_ASM(const s32, "game/code_00128FE8", fldUploadSkyBuffer);

INCLUDE_RODATA(const s32, "game/code_00128FE8", D_00413350);

void fldUpdateSwayOffset(void) {
    D_00436114 = 0;
    switch (D_0043610C) {
    case 1:
        D_00436110 += 0.1f;
        D_00436114 = func_003406A0(D_00436110) * 32.0f;
        break;
    case 2:
        D_00436110 += 0.2f;
        D_00436114 = func_003406A0(D_00436110) * 32.0f;
        break;
    case 3:
        D_00436110 += 0.05f;
        D_00436114 = func_003406A0(D_00436110) * 32.0f;
        break;
    case 4:
        D_00436110 += 0.1f;
        D_00436114 = func_003406A0(D_00436110) * 48.0f;
        break;
    case 5:
        D_00436110 += 0.2f;
        D_00436114 = func_003406A0(D_00436110) * 48.0f;
        break;
    case 6:
        D_00436110 += 0.05f;
        D_00436114 = func_003406A0(D_00436110) * 48.0f;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00134A18);

void fldSetFadeTarget(s32 area, s32 value, s32 duration) {
    if (D_004360E8 == 0 && D_00389780[0] < 40) {
        duration = 0;
    }
    if (duration == 0) {
        D_0043611C = area;
        D_00436120 = 1.0f;
        D_00436124 = 1.0f;
    } else {
        D_00436120 = 0.0f;
        D_0043611C = D_00436118;
        D_00436124 = (f32)duration;
    }
    D_00436118 = area;
    D_00436104[area * 73] = value;
}

void func_00135568(u32 arg0) {
    D_0043610C = arg0;
    D_00436110 = 0;
    D_00436114 = 0;
}

void func_00135578(u32 arg0) {
    D_00436108 = arg0;
}

u32 func_00135580(void) {
    return D_00436108;
}

void func_00135588(u32 arg0) {
    D_003899C0[0] = arg0;
}

u32 func_00135598(void) {
    return D_003899C0[0];
}

void func_001355A8(u32 arg0) {
    u32 temp_v0 = D_003899B4[0];

    D_0043612C = arg0;
    D_00436130 = 0;
    D_00436134 = temp_v0;
    func_00135A68(temp_v0, 1);
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_001355D8);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135840);

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135A68);

void func_00135D18(s32 speed) {
    if (D_004360E8 == 0 && D_00389780[0] < 40) {
        speed = 0;
    }
    if (D_00436158 != 0 && D_00436128 != D_00436158) {
        if (D_00389898[0] == 0) {
            D_003899B4[0] = D_00436158;
        }
        func_00135A68(D_00436158, speed);
    }
}

INCLUDE_ASM(const s32, "game/code_00128FE8", func_00135D80);

void func_00136098(void) {
    dds3ClearObjectFlags(D_00435F0C, 0x100);
}

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FA8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FAC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FB8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FBC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FC8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FCC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FD0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FD8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FDC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FE8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FEC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FF8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00435FFC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436000);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436004);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436008);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043600C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436010);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436014);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436018);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436020);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436028);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436030);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436038);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436040);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436048);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436050);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436060);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436064);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436068);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436070);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436078);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043607C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436080);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436088);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043608C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436090);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436098);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360A8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360AC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360B8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360BC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360C8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360CC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360D8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360DC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360E8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360EC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F0);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F4);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360F8);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_004360FC);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436100);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436104);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436108);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043610C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436110);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436114);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436118);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043611C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436120);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436124);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436128);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043612C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436130);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436134);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436138);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043613C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436140);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436144);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436148);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043614C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436150);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436154);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436158);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_0043615C);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436160);

INCLUDE_SDATA(const s32, "game/code_00128FE8", D_00436164);

