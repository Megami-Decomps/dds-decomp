#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern u8 D_00337D00[];
typedef struct FldNpcMotion {
    s32 unk0;
    u8 unk4[0x10];
    u8 unk14[0x10];
    u8 unk24[0x10];
    u8 unk34[0x10];
    s32 unk44;
} FldNpcMotion; /* 0x48 bytes */
extern FldNpcMotion D_00336A60[];
extern void *D_003BAE44;
extern void *D_003BAE48;

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u32);
    u32 unk14[3];
} FieldResourceDescriptor;

typedef struct {
    u32 unk0[4];
    void (*open)(void *, u64);
    u32 unk14[3];
} FieldBufferDescriptor;

static inline s32 fldTestBits(u32 flags, u32 mask) {
    return (flags & mask) != 0;
}

typedef struct FldActionSpawn {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} FldActionSpawn;
extern void fldSpawnActionObjects(FldActionSpawn *, u32);
extern u64 dds3GetWorldSecondaryObject(void);
extern s32 evtSpawnActionObj2(s32, s32);
extern s32 func_001462D0(void);
extern s32 D_0032E400[];
extern void func_00131218(void);
extern void func_0012FC20(void);
extern void func_0012F578(void);
extern void func_0012EEA0(s16, s16);
extern void func_0012FF48(void);
extern void func_0012EA50(s16, s32, f32);
extern s32 *func_00123DD0();
extern void dds3SetCameraValue(s32, f32);
extern void fldToggleWorldNodeState(s32);
extern void func_0012C880(void);
extern s32 func_00125DF8(s32);
extern u8 *func_00112888(s32);
extern f32 fldPointDistance(f32, f32, f32, f32, f32, f32);
extern void func_00131268(void);
extern void func_00131290(void);
extern void func_001312B0(void);
typedef struct {
    f32 dist;
    f32 y;
    f32 targetY;
    f32 fov;
    f32 unk10;
    f32 unk14;
} FldCamRow; /* 0x18 bytes */
typedef struct {
    u8 pad0[0x50];
    s32 mode;
    u8 pad54[4];
    s32 rowIdx;
    u8 pad5C[8];
    f32 angle;
    u8 pad68[4];
    f32 dist;
    u8 pad70[0xD0];
    f32 x;
    f32 y;
    f32 z;
} FldCamWork;
extern FldCamRow D_0032FA10[];
extern f32 D_00330650[];
extern f32 D_00330660[];
extern f32 sdfSinPoly(f32);
extern f32 func_002E78F8(f32);
extern void btlActivateRuntime(s32 mode);
extern void func_00110860(u64, s8);
extern void sdfInitPacketList(u64);
extern s32 sdfConsCalculateDrawPacketSize(s32, s32);
extern void func_002E1428(u64, s32, s32, s32, s32);
extern u64 *func_002E1420(u64);
extern s32 sdfConsAllocateColumnPacket(s32);
extern void sdfConsCreateDrawPacket(u64, s32, s32);

extern s32 D_0032E3C0[];
extern s8 D_0032C9A0[];
extern void func_00131D88(void);
extern void fldCreatePlayerObject(void);
extern u8 *func_0013DAC0(void);
extern s32 func_00110D00(u64, u8 *);
extern void func_00126A30(u32, u32, s32);

extern s32 D_003BAE68;

extern u32 D_003BADE8;
extern u32 D_003BADF0;
extern u32 D_003BADFC;
extern u32 D_003BAE00;

extern s32 D_003BADF8;
extern s32 D_003BADEC;
extern u32 D_003BAD78;

extern u32 D_003BAD7C;
extern f32 D_003BAD80;
extern s32 D_003BAD84;

extern s32 D_003BAD68;
extern u32 D_003BAD6C;
extern s32 D_003BAD70;
extern s32 D_003BD7C4;

extern s32 D_003BAB38;

extern u32 D_003BAB34;
extern u8 D_003BAB3C;

extern u32 D_003BAD34;
extern u32 D_003BAD38;
extern u32 D_003BAD3C;
extern u32 D_003BAD40;

extern u32 D_003BAD1C;

extern u64 dds3GetWorldObject(void);
extern s64 func_001109F0(u64);
extern s64 func_00123DE0(void);

extern s32 D_003BAA34;

extern u32 D_003BACF8;

extern u32 sdfCreateInitializedPacketList(void);
extern u64 sdfAllocPacketAligned(u64);

extern u64 func_00197760(s32, s32, u64, u64, u64, u64);

extern s32 D_003BACD8;
extern s32 func_002CF530(u32);

extern u32 D_003BACD0;

extern u32 D_003BAC30;

extern u32 D_003BAC84;
extern u32 D_003BAC88;
extern u32 D_003BAD98;
extern u32 D_003BAD9C;
extern u32 D_003BADA0;
extern u32 D_003BADA4;
extern u32 D_003BADC8;
extern u32 D_003BADD8;
extern u32 D_003BAE28;
extern s32 D_003BAE3C;
extern u32 D_003BAE64;
extern u32 D_0032E428[];
extern s32 D_0032E3B0[];
extern u32 D_0032E538[];
extern u32 D_0032E544[];
extern u32 D_0032E570[];
extern u32 D_0032E59C[];
extern u32 D_0032E5A8[];
extern u32 D_00324B48[];
extern u8 D_003296F0[];
extern u32 D_003BACD4;
extern u32 D_003BACEC;
extern u32 D_003BACF0;
extern u32 D_003BD7C0;
extern u8 D_0032F260[];
extern void *func_002DA730(void);
extern u32 func_002D3288(void *);
extern u8 D_00324610[];
extern u8 D_00324660[];
extern void func_002DDD60(void *src);
extern char D_003C9200[];
extern u32 D_003C92E0[];
extern s16 D_00337D12[];
extern s16 D_003C9518[];
extern void func_00127E20(char *arg0);
extern void func_00132FD0(u32 arg0, s32 arg1);
extern s32 strcmp(const char *a, const char *b);
extern u32 D_003BAD20;
extern char D_003BAD08[];
extern u8 D_003C9230[];
extern u8 D_003C9220[];
extern s32 fldEncProc(void);
extern void func_0012C7C0(void);
extern void func_00213808(void);
extern void btlClearRuntimeState(void);
extern void func_00112EE8(s64 arg0, void *arg1, void *arg2);
extern f32 sdfAtan2(f32 arg0, f32 arg1);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
extern u32 D_003BAC3C;
extern u8 D_003BACE0[];
extern u8 D_003BACE8[];
extern s32 D_003BAD00;
extern u32 D_003BACFC;
extern u32 D_00330738[];
extern u32 D_003306B0[];
extern u32 D_003306C0[];
extern u32 D_003308B0[];
extern u32 fileRequestIsReady(u32 arg0);
extern void *memset(void *s, s32 c, u32 n);
extern void *func_002CFEB8(s32 size);
extern void func_00101A68(u32 arg0, void *arg1);
extern s32 func_00140F70(u32 task);
extern s32 kwlnTaskIsRegistered(u32 arg0);
extern s32 func_00213B50(void);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 D_003BADF4;
extern u8 D_003BA734;
extern s32 func_0010BED8(u32 arg0);
extern s32 func_00110D88(u64 arg0, u32 arg1);
extern u32 D_003BAC48;
extern u32 D_003BAC34;
extern u8 D_003BAC90[];
extern void func_002D0A10(u32 arg0);
extern void func_00288788(u32 arg0);
extern void func_00218320(s32 arg0);
extern void func_00218368(s32 arg0);
extern s32 D_003BAE30;
extern s32 D_003BAE1C;
extern s32 D_003BAE14;
extern s16 D_003C9510[];
extern void *func_002D03F8(s32 size);
extern void *sdfResourceRetainAddress(void *p);
extern u32 D_003BAE4C;
extern s32 D_003BAE50;
extern s32 *func_00110F80();

typedef struct {
    s16 data[12];
} FldRowData; /* 0x18 bytes */

typedef struct {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 count;
    FldRowData body;
} FldS16Row; /* 0x20 bytes */
extern FldS16Row D_00337C60[];
extern u32 *D_003307B0[];
extern void fldDrawMarkerQuad(u32 value);

typedef struct {
    s16 unk0;
    s16 unk2;
} FldIndexPair;
extern FldIndexPair D_0032EF18[];
extern FldIndexPair D_0032EFE0[];
extern s32 D_003BAE54;
extern s32 D_003BAE5C;
extern s32 D_003BAE60;

/* A packed-resource chunk uses an offset from the base and a byte length. */
typedef struct FldTransferChunk {
    u32 unk0;
    s32 offset;
    u32 size;
} FldTransferChunk;

INCLUDE_ASM(const s32, "game/code_00126A30", func_00126A30);

void fldSpawnActionObjects(FldActionSpawn *spawn, u32 count) {
    u32 i;

    dds3GetWorldSecondaryObject();
    for (i = 0; i < count; i++) {
        s32 object = evtSpawnActionObj2(spawn->unk8, spawn->unk4);
        spawn++;
        if (i == 0) {
            D_0032E3B0[0] = object;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001270A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127388);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127588);

/* Handle a field request, creating the player only in non-special scene states. */
void fldProcessFieldRequest(u32 *request) {
    s32 state;
    fldSpawnActionObjects((FldActionSpawn *)request[4], request[3]);
    state = D_0032E3B0[4];
    if (state != 1 && state < 200) fldCreatePlayerObject();
    func_00126A30(request[1], request[0], 0);
    D_0032E3B0[1] = ((u32 *)request[2])[1];
}

void func_00127788(u32 *request) {
    func_00126A30(request[1], *request, 1);
}

s32 func_001277A8(s32 arg0) {
    return arg0 + 0xc;
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldRelocatePackedWords);

/* Relocate the words described by this packed-resource transfer chunk. */
void fldRelocatePackedTransferChunk(u32 buffer, FldTransferChunk *chunk) {
    sdfRelocatePackedResourceWords(buffer, buffer, (s32)buffer + chunk->offset, chunk->size);
}

void func_001278C0(u32 buffer, FldTransferChunk *chunk) {
    fldRelocatePackedWords(buffer, buffer, (s32)buffer + chunk->offset, chunk->size);
}

void fldSetAreaResourceRequest(u32 arg0, u32 arg1) {
    D_003BAC84 = arg0;
    D_003BAC88 = arg1;
}

extern char D_0039FE38[]; /* "%sf%03d_%03d.LB" */
extern s32 func_003014F0(char *, const char *, ...);
extern u32 func_00288A80(char *);
extern void fldFormatAreaDirectory(char *, s32, s32);

s32 fldLoadAreaResource(void) {
    char directory[64];
    char path[80];
    u32 area = D_003BAC84;
    u32 floor = D_003BAC88;

    if (area != 0 || floor != 0) {
        fldFreeDisplayObjects();
        D_0032E3B0[31] = area;
        D_0032E3B0[32] = floor;
        fldFormatAreaDirectory(directory, area, 1);
        func_003014F0(path, D_0039FE38, directory, area, floor);
        D_003BAC3C = func_00288A80(path);
        D_0032E3B0[30] = 1;
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldRequestAreaResource);

typedef struct FldAreaResourceNode {
    struct FldAreaResourceNode *next; /* 0x00 */
    u32 pad04;
    u32 resourceHandle; /* 0x08 */
} FldAreaResourceNode;

typedef struct FldAreaResource {
    u8 pad00[0x60];
    FldAreaResourceNode *nodes; /* 0x60 */
} FldAreaResource;

void fldFreeDisplayObjects(void) {
    if (D_003BAC3C != 0) {
        FldAreaResourceNode *node = ((FldAreaResource *)D_003BAC3C)->nodes;

        if (node != NULL) {
            do {
                func_002D0A10(node->resourceHandle);
                node = node->next;
            } while (node != NULL);
        }
        func_00288788(D_003BAC3C);
        D_003BAC3C = 0;
    }
    D_003BAC84 = 0;
    D_003BAC88 = 0;
    D_0032E3B0[31] = 0;
    D_0032E3B0[32] = 0;
    D_0032E3B0[30] = 0;
}

u32 fldPollAreaResourceLoad(void) {
    u32 sceneState = D_0032E3B0[0x1E];

    if (sceneState != 0) {
        if (sceneState == 1) {
            if (fileRequestIsReady(D_003BAC3C) != 0) {
                D_0032E3B0[0x1E] = 0;
                D_003BA734 = 0;
            }
        }
    }
    return 0;
}

u32 fldGetResourceReadyFlag(void) {
    return D_0032E428[0];
}

u8 fldIsAreaResourceReady(void) {
    if (D_003BAC3C != 0) {
        if (fileRequestIsReady(D_003BAC3C) != 0) {
            return 1;
        }
    }
    return D_0032E428[0] != 0;
}

s32 fldIsAreaResourceReadyFor(s32 area, s32 room) {
    if (D_0032E3B0[31] != area || D_0032E3B0[32] != room) {
        return 0;
    }
    if (D_003BAC3C != 0 && fileRequestIsReady(D_003BAC3C) != 0) {
        return 1;
    }
    return D_0032E3B0[30] != 0;
}

extern u32 D_003BAC60, D_003BAC64, D_003BAC68, D_003BAC6C;
extern u32 D_003BAC70, D_003BAC74, D_003BAC78, D_003BAC7C;

void *func_00127C40(void **destination, s32 area, s32 room) {
    if (D_0032E3B0[31] == area) {
        if (D_0032E3B0[32] == room) {
            void *buffer = func_002D03F8(D_003BAC70);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC60, D_003BAC70);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127CB8(void **destination, s32 area, s32 room) {
    if (D_0032E3B0[31] == area) {
        if (D_0032E3B0[32] == room) {
            void *buffer = func_002D03F8(D_003BAC74);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC64, D_003BAC74);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127D30(void **destination, s32 area, s32 room) {
    if (D_0032E3B0[31] == area) {
        if (D_0032E3B0[32] == room) {
            void *buffer = func_002D03F8(D_003BAC78);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC68, D_003BAC78);
            return buffer;
        }
    }
    return NULL;
}

void *func_00127DA8(void **destination, s32 area, s32 room) {
    if (D_0032E3B0[31] == area) {
        if (D_0032E3B0[32] == room) {
            void *buffer = func_002D03F8(D_003BAC7C);
            void *data = sdfResourceRetainAddress(buffer);
            *destination = data;
            memcpy(data, (void *)D_003BAC6C, D_003BAC7C);
            return buffer;
        }
    }
    return NULL;
}

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FE38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00127E20);

u8 func_00127FD0(void) {
    char buf[32];

    func_00127E20(buf);
    return strcmp(D_003C9200, buf) != 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldLoadAreaPackedResources);

void fldReleaseAreaResourceCache(void) {
    u32 resourceHandle = D_003BAC48;

    if (resourceHandle != 0) {
        func_002D0A10(resourceHandle);
        D_003BAC48 = 0;
    }
    resourceHandle = D_003BAC34;
    if (resourceHandle != 0) {
        func_00288788(resourceHandle);
        D_003BAC34 = 0;
    }
    D_003C9200[0] = D_003BAC90[0];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001281E0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128780);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128890);

extern s32 func_003014F0(char *, const char *, ...);

extern s32 func_003014F0(char *, const char *, ...);
/* All 25 code words match. Check_unit rodata range extends beyond selector strings into the separately-owned next string. */
void fldFormatAreaDirectory(char *path, s32 field, s32 unused) {
    if (field < 200) func_003014F0(path, "/fld/f/f%03d/", field);
    else if (field < 500) func_003014F0(path, "/fld/b/f%03d/", field);
    else func_003014F0(path, "/fld/e/f%03d/", field);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128BB8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_0039FF88);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00128DA0);

void func_00128F88(u32 arg0) {
    D_003BAC30 = arg0;
}

void fldInitDisplayObjects(void) {
    if (D_003BACD4 == 0) {
        void *object;
        D_003BACD4 = 1;
        object = func_002DA730();
        D_003BACEC = (u32)object;
        *(f32 *)((u8 *)object + 0x1C) = 1.0f;
        D_003BD7C0 = (u32)func_002DA730();
        D_003BACF0 = func_002D3288(D_0032F260);
    }
}

u32 *fldGetDisplayTableRow(void) {
    return &D_00324B48[D_003BACD0 * 8];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129000);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129178);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001292E0);

extern u8 D_00324650[];

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
        : : "r"(D_003296F0) : "memory");
    func_002DDD60(D_00324610);
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
        : : "r"(D_00324650) : "memory");
    __asm__ volatile ("vmul.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_00324660) : "memory");
    __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

extern u8 D_003249B0[];
extern u8 D_003249F0[];
extern u8 D_00324A00[];
extern u8 D_00329790[];

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
        : : "r"(D_00329790) : "memory");
    func_002DDD60(D_003249B0);
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
        : : "r"(D_003249F0) : "memory");
    __asm__ volatile ("vmul.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf11, 0(%0)\n"
        ".set reorder"
        : : "r"(D_00324A00) : "memory");
    __asm__ volatile ("vadd.xyzw vf10, vf10, vf11");
    __asm__ volatile (
        ".set noreorder\n"
        "sqc2 vf10, 0(%0)\n"
        ".set reorder"
        : : "r"(result) : "memory");
    *dstX = result[0];
    *dstY = result[1];
}

void func_00129650(void) {
    u8 *matrix;
    __asm__ volatile (
        ".set noreorder\n"
        "lqc2 vf28, 0(%0)\n"
        "lqc2 vf29, 0x10(%0)\n"
        "lqc2 vf30, 0x20(%0)\n"
        "lqc2 vf31, 0x30(%0)\n"
        ".set reorder"
        : : "r"(D_003296F0) : "memory");
    matrix = D_00324610;
    func_002DDD60(matrix);
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
        : : "r"(D_00324660) : "memory");
}

void func_001296B8(f32 *dstX, f32 *dstY, f32 x, f32 y, f32 z) {
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

void func_00129720(u32 arg0) {
    D_003BACD0 = arg0;
}


extern void sdfInitPacketList(u64);
void func_00129728(s32 lower, s32 bits, u64 upper) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 packet;
    u64 *entry;
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    entry = func_002E13E0(packet, 0x30);
    entry[5] = 0x3B;
    entry[4] = (u64)(bits << 15) | (upper << 32) | lower;
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

extern void sdfAppendPacket(u64, u64);
void fldSubmitFrameQuad(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 packet;
    u64 *data;
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(0x30);
    data = func_002E13E0(packet, 0x30);
    data[4] = (arg7 << 17) | 0x10000 | (arg5 << 15) | (arg4 << 14) | (arg3 << 12) | (arg2 << 4) | (arg1 << 1) | arg0;
    data[5] = 0x47;
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00129900);

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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    func_002E1428(packet, 0x49, 2, 0x41, 2);
    dst = func_002E1420(packet);
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
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 2));
    func_002E1428(packet, 0x49, 2, 0x41, 2);
    dst = func_002E1420(packet);
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
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_002E1428(packet, 0x4D, 2, 0x41, 4);
    dst = func_002E1420(packet);
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
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

void func_00129E30(s32 x, s32 y, s32 w, s32 h, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8) {
    s32 coords[8];
    u64 command;
    u64 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_002E1428(packet, 0x4D, 2, 0x41, 4);
    dst = func_002E1420(packet);
    tag = arg4;
    lo = (u64)arg5 | ((u64)arg6 << 32);
    hi = (u64)arg7 | ((u64)arg8 << 32);
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_002E1428(packet, 0x4D, 2, 0x41, 4);
    dst = func_002E1420(packet);
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
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldSubmitGsGradientTriangle);

INCLUDE_ASM(const s32, "game/code_00126A30", fldSubmitGsGradientQuad);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012A5D8);

void func_0012A890(s32 x, s32 y, s32 w, s32 h, u32 arg4, u32 arg5, u32 arg6, u32 arg7, u32 arg8) {
    s32 coords[8];
    u64 command;
    u64 packet;
    u64 *dst;
    u64 lo;
    u64 hi;
    u64 tag;
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
    sdfInitPacketList(command);
    packet = sdfAllocPacketAligned(sdfConsCalculateDrawPacketSize(2, 4));
    func_002E1428(packet, 0x4D, 2, 0x41, 4);
    dst = func_002E1420(packet);
    lo = (u64)arg4 | ((u64)arg5 << 32);
    hi = (u64)arg6 | ((u64)arg7 << 32);
    tag = arg8;
    pos = coords;
    for (i = 0; i < 4; i++) {
        dst[0] = lo;
        dst[1] = hi;
        dst += 2;
        dst[1] = tag;
        dst[0] = (u64)(u32)(pos[0] + 0x7000) | ((u64)(pos[1] + 0x7900) << 32);
        pos += 2;
        dst += 2;
    }
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

extern s32 sdfTexGetPrimaryBuffer(s32);
extern s32 sdfTexGetPrimaryBufferSize(s32);
extern void sdfConsInitDmaPacketHeader(u64, s32, s32);
extern void sdfAppendReferencePacket(u64, u64);
extern void func_002DD708(f32);
extern void sdfInitGeometryDmaPacket(u64, f32 *);
extern void func_002E2680(u64, u8 *, s32, u8 *, u8 *);
extern void sdfAppendPacket(u64, u64);
void fldSubmitModelPacket(s32 arg0, u8 *arg1) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 header;
    u64 packet;
    f32 mat[16];
    FieldBufferDescriptor *descriptor;

    sdfInitPacketList(command);
    header = sdfAllocPacketAligned(0x20);
    sdfConsInitDmaPacketHeader(header, sdfTexGetPrimaryBuffer(arg0), sdfTexGetPrimaryBufferSize(arg0));
    sdfAppendReferencePacket(command, header);
    func_002DD708(*(f32 *)(arg1 + 0x44));
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
    func_002E2680(packet, arg1, *(s32 *)(arg1 + 0x40), arg1 + 0x10, arg1 + 0x20);
    sdfAppendPacket(command, packet);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

extern s32 func_00100518(void);
extern u8 D_00326ED0[];
extern void func_002D4C80(const void *, u64, s32);
extern void func_002D4CC8(const void *, u64, s32);
extern void func_002D40E8(u64, u64);

void func_0012AB40(void) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 texture;
    FieldBufferDescriptor *descriptor;
    sdfInitPacketList(command);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4C80(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    func_002D40E8(command, texture);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

void func_0012ABE0(void) {
    u64 command = sdfAllocPacketAligned(0x20);
    u64 texture;
    FieldBufferDescriptor *descriptor;
    sdfInitPacketList(command);
    texture = sdfAllocPacketAligned(0x40);
    func_002D4CC8(D_00326ED0 + func_00100518() * 0x1F40, texture, 0);
    func_002D40E8(command, texture);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

extern f32 D_0032F4E0[];
extern u32 D_0032F500[];
extern u64 func_002EF2B0(const void *, const void *, s32, s32);
extern void sdfAppendPacket(u64, u64);

void func_0012AC80(u32 first, u32 second, f32 x, f32 y, f32 z, f32 u, f32 v, f32 w) {
    u64 resource;
    u64 record;
    FieldBufferDescriptor *descriptor;
    D_0032F4E0[0] = x;
    D_0032F4E0[1] = y;
    D_0032F4E0[2] = z;
    D_0032F4E0[4] = u;
    D_0032F4E0[5] = v;
    D_0032F4E0[6] = w;
    D_0032F500[1] = second;
    D_0032F500[0] = first;
    resource = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(resource);
    record = func_002EF2B0(D_0032F4E0, D_0032F500, 2, 0x80);
    sdfAppendPacket(resource, record);
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, resource);
}

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
extern u64 func_002E21A0(FldPrimDesc *);
void fldSubmitGsTriangle(s32 a0, s32 a1, s32 a2, f32 f0, f32 f1, f32 f2, f32 f3, f32 f4, f32 f5, f32 f6, f32 f7, f32 f8) {
    FldPrimDesc desc;
    f32 verts[12];
    s32 indices[3];
    u64 command;
    FieldBufferDescriptor *descriptor;

    command = sdfAllocPacketAligned(0x20);
    sdfInitPacketList(command);
    sdfConsAppendClearPacket(command, 0);
    sdfConsAppendAssetPacket(command, D_003BACEC, 0);
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
    sdfAppendPacket(command, func_002E21A0(&desc));
    descriptor = (FieldBufferDescriptor *)&D_00324B48[D_003BACD0 * 8];
    descriptor->open(descriptor, command);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012AEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B090);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B2B0);

INCLUDE_ASM(const s32, "game/code_00126A30", fldDrawMarkerQuad);

INCLUDE_ASM(const s32, "game/code_00126A30", fldDrawMarkerQuadColored);

extern void fldSubmitFrameQuad(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_00129000(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32);

void func_0012B638(s32 x, s32 y, s32 width, s32 height) {
    func_00129000(x, y, 0x10, height, 0, 0, 0x10, 0x20, 0x60000040, D_003BACF0);
    func_00129000(x + 0x10, y, width - 0x20, height, 0x10, 0, 1, 0x20, 0x60000040, D_003BACF0);
    func_00129000(x + width - 0x10, y, 0x10, height, 0x10, 0, 0x10, 0x20, 0x60000040, D_003BACF0);
    fldSubmitFrameQuad(1, 5, 0x80, 1, 0, 0, 1, 2);
    func_00129900(0);
}

void func_0012B740(void) {
    if (D_003BACD8 == 0) {
        D_003BACD8 = func_002CF530(0x70000);
    }
}

void func_0012B768(void) {
    if (D_003BACD8 != 0) {
        func_002CF5C0(D_003BACD8);
        D_003BACD8 = 0;
    }
}

extern u32 D_003980F0[];
extern u32 sdfAllocatePacketList(s32);
extern void sdfCreateDescriptorPacket(u32, u32, s32, s32, s32, s32, u32, s32);
extern void sdfCreateResourcePacket(u32, u32, s32, s32, s32, s32, u32, s32, s32, s32);

void func_0012B798(void) {
    if (D_003BACD8 != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateResourcePacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, D_003BACD8, 0, 0, 0);
        descriptor = (FieldResourceDescriptor *)&D_00324B48[D_003BACD0 * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012B818(void) {
    if (D_003BACD8 != 0) {
        u32 packet = sdfAllocatePacketList(0);
        FieldResourceDescriptor *descriptor;
        sdfCreateDescriptorPacket(packet, D_003980F0[0], 0, 0, 0x200, 0xE0, D_003BACD8, 0);
        descriptor = (FieldResourceDescriptor *)&D_00324B48[D_003BACD0 * 8];
        descriptor->open(descriptor, packet);
    }
}

void func_0012B890(x, y, first, second)
s32 x;
s32 y;
u64 first;
u64 second;
{
    u64 object;

    object = func_00197760(x << 4, y << 4, 0, first, second, 0);
    func_00195868(object);
    func_00194920(object);
}

void fldAdvanceQuadRow(s32 arg0) {
    *(s32 *)(arg0 + 0x20) = *(s32 *)(arg0 + 0x20) + 0x60;
}

void fldStartQuadPacketList(s32 quadState) {
    u64 packet;
    u32 packetList;

    packetList = sdfCreateInitializedPacketList();
    *(u32 *)(quadState + 0x28) = packetList;
    packet = sdfAllocPacketAligned(0x40);
    func_002D5608(packet);
    sdfAppendPacket(*(s32 *)(quadState + 0x28), packet);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012B940);

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
} FldQuadState; /* 0x2C bytes */

typedef struct {
    u8 pad0[0x10];
    void (*invoke)(void *, s32);
} FldGfxCallback;
extern FldGfxCallback D_00325708;
extern FldGfxCallback D_00325748;
extern void sdfPktInit(void *, s32, s32, s32, s32);
extern s32 func_002E4908();

void fldDrawFloorQuad(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x73C0;
    quad.unk20 = 0x7CC0;
    quad.unk24 = 0x0FFFFF7E;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7D;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x, quad.unk20 + y, quad.unk24, 0);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, arg2));
    fldAdvanceQuadRow((s32)&quad);
    D_00325748.invoke(&D_00325748, quad.unk28);
}

void fldDrawFloorQuadA(s32 x, s32 y, s32 arg2, s32 arg3) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x73C0;
    quad.unk20 = 0x7CC0;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x73C0;
    quad.unk4 = 0x7CC0;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x, quad.unk20 + y, quad.unk24, arg2);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, arg3));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

void fldDrawMapQuadTiled(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x7000;
    quad.unk20 = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x * 16, quad.unk20 + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, D_003BACE0, arg2));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

void fldDrawMapQuadTiledAlt(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x7000;
    quad.unk20 = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x * 16, quad.unk20 + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, D_003BACE8, arg2));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

void fldDrawMapQuad(s32 x, s32 y, s32 arg2) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x7000;
    quad.unk20 = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x * 16, quad.unk20 + y * 8, quad.unk24, 0);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, arg2));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

void fldDrawMapQuadPacket(s32 x, s32 y, s32 arg2, s32 arg3) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x7000;
    quad.unk20 = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + x * 16, quad.unk20 + y * 8, quad.unk24, arg2);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, arg3));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

void fldDrawMapQuadScaled(s32 arg0, s32 arg1, f32 x, f32 y) {
    FldQuadState quad;
    u8 packet[16];

    quad.unk1C = 0x7000;
    quad.unk20 = 0x7900;
    quad.unk24 = 0x0FFFFF80;
    quad.unkC = 0;
    quad.unk14 = 0x10000000;
    quad.unk18 = 0x80806020;
    quad.unk0 = 0x7000;
    quad.unk4 = 0x7900;
    quad.unk8 = 0x1A40;
    quad.unk10 = 0x0FFFFF7F;
    fldStartQuadPacketList((s32)&quad);
    sdfPktInit(packet, quad.unk1C + (s32)(x * 16.0f), quad.unk20 + (s32)(y * 8.0f), quad.unk24, arg0);
    sdfAppendPacket(quad.unk28, func_002E4908(packet, arg1));
    fldAdvanceQuadRow((s32)&quad);
    D_00325708.invoke(&D_00325708, quad.unk28);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C1F0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C428);

s32 func_0012C648(void) {
    s32 state = D_003BAD00;
    s32 result;

    if (state < 3) {
        if (state < 0) {
            result = D_003BACFC;
        } else {
            result = func_00213B50();
        }
    } else {
        result = D_003BACFC;
    }
    return result;
}

void func_0012C688(u32 arg0) {
    D_003BACF8 = arg0;
}

s32 fldEncProc(void) {
    s32 state = D_003BAD00;

    if (state < 3) {
        if (state >= 0) {
            func_00213808();
        }
    }
    return 0;
}

void func_0012C6C8(u32 arg0, s32 arg1) {
    if ((arg1 < 0x400) && ((*(u16 *)((s32)arg1 * 0x28 + D_003BAA34 + 0x20) & 0x8000) != 0))
    {
        func_001060C8(0);
        func_0012C750(3);
        return;
    }
    sndSetSequenceVolumePan(0xf, 0x7f, 0x3f);
    func_0012C750(arg0);
}

s32 func_0012C750(s32 mode) {
    D_003BAD00 = mode;
    D_003BACFC = 0;
    if (func_0012C648() == 0) {
        if (D_003BAD00 < 3) {
            if (D_003BAD00 >= 0) {
                btlActivateRuntime(D_003BAD00);
                if (dds3GetWorldObject() != 0) {
                    func_00110860(dds3GetWorldObject(), 1);
                }
            }
        }
    }
}


void func_0012C7C0(void) {
    btlResetAsyncState();
}

void func_0012C7D8(void) {
    kwlnTaskCreate((s32)D_003BAD08, 0x2B0F, 0, 1, (s32)fldEncProc, (s32)func_0012C7C0, 0);
    btlClearRuntimeState();
}

extern f32 D_00330610[];
extern f32 D_00330620[];
extern f32 D_0032E41C[];
void func_0012C818(void) {
    f32 *distance = D_0032E41C;
    f32 dx = D_00330610[0] - D_00330620[0];
    f32 dy = D_00330610[1] - D_00330620[1];
    f32 dz = D_00330610[2] - D_00330620[2];
    *distance = fsqrtf(dx * dx + dy * dy + dz * dz) - 50.0f;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012C880);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CB48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012CED0);

void func_0012D240(void) {
    FldCamWork *cam = (FldCamWork *)D_0032E3B0;
    f32 angle;

    D_00330650[0] = cam->x - sdfSinPoly(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_00330650[1] = cam->y + D_0032FA10[cam->rowIdx].y;
    D_00330650[2] = cam->z - func_002E78F8(cam->angle * 3.14f / 180.0f) * 80.0f;
    D_00330650[3] = 1.0f;
    angle = cam->angle * 3.14f / 180.0f;
    D_00330660[0] = cam->x + sdfSinPoly(angle) * D_0032FA10[cam->rowIdx].dist;
    D_00330660[1] = cam->y + D_0032FA10[cam->rowIdx].targetY;
    D_00330660[2] = cam->z + func_002E78F8(angle) * D_0032FA10[cam->rowIdx].dist;
    D_00330660[3] = 1.0f;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D3D8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012D528);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DB70);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012DD70);

/* Suppress the world object's current entry when it is already selected. */
s64 func_0012E488(void) {
    u64 temp_v0;
    s64 temp_v1;
    s64 temp_v2;

    temp_v0 = dds3GetWorldObject();
    temp_v1 = func_001109F0(temp_v0);
    temp_v2 = func_00123DE0();
    if (temp_v2 == temp_v1) {
        temp_v1 = 0;
    }
    return temp_v1;
}

void fldSetCameraMoveMode(u32 value) {
    D_003BAD1C = value;
    func_00112EE8(func_001109F0(dds3GetWorldObject()), D_003C9230, D_003C9220);
    D_003BAD20 = 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012E510);

void fldClearCameraMoveMode(void) {
    D_003BAD1C = 0;
}

void func_0012E6F8(void) {
    f32 vec[4];
    s32 slot;
    u8 *model;
    u8 **modelRef;
    u8 **matrices;

    memset(vec, 0, sizeof(vec));
    vec[3] = 1.0f;
    if (fldGetLocationCoordinateValue(D_0032E3B0[4], D_0032E3B0[5] + 1) & 0x40) {
        if (func_00125DF8(0x40) == 0) {
            return;
        }
    }
    slot = *(s32 *)(func_00112888(D_003BAB34) + 0x5C);
    modelRef = *(u8 ***)(D_003BAB38 + 0x18);
    if (slot >= 0) {
        model = *modelRef;
        matrices = *(u8 ***)(model + 0xC);
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf28, 0(%0)\n"
            "lqc2 vf29, 0x10(%0)\n"
            "lqc2 vf30, 0x20(%0)\n"
            "lqc2 vf31, 0x30(%0)\n"
            ".set reorder"
            : : "r"(matrices[slot] + 0xC0));
        __asm__ volatile (
            ".set noreorder\n"
            "lqc2 vf10, 0(%0)\n"
            "vmulax.xyzw ACC, vf28, vf10x\n"
            "vmadday.xyzw ACC, vf29, vf10y\n"
            "vmaddaz.xyzw ACC, vf30, vf10z\n"
            "vmaddw.xyzw vf10, vf31, vf10w\n"
            "sqc2 vf10, 0(%0)\n"
            ".set reorder"
            : : "r"(vec));
        if (fldPointDistance(vec[0], vec[1], vec[2], D_00330620[0], D_00330620[1], D_00330620[2]) < 45.0f) {
            func_00125DF8(0x40);
            func_00131268();
            func_001312B0();
            func_00131218();
        } else {
            func_00131290();
            if (((FldCamWork *)D_0032E3B0)->mode == 1 || ((FldCamWork *)D_0032E3B0)->mode == 3) {
                func_00131218();
            } else if (((FldCamWork *)D_0032E3B0)->dist < 100.0f) {
                func_00131240();
            } else {
                func_00131218();
            }
        }
    }
}

s32 func_0012E878(void) {
    FldCamWork *cam;
    s32 *obj = func_00123DD0();

    if (*obj != 0 && D_003BAB34 != 0) {
        func_00131278();
        if (func_0012E488() != 0) {
            fldToggleWorldNodeState(1);
            func_0012E510();
            func_00131218();
            return 0;
        } else {
            func_0012C880();
            cam = (FldCamWork *)D_0032E3B0;
            dds3SetCameraValue(*obj, D_0032FA10[cam->rowIdx].fov * 3.14f / 180.0f);
            switch (cam->mode) {
            case 0:
                func_0012D528();
                if (cam->dist < 50.0f) {
                    func_0012D528();
                }
                break;
            case 1:
                func_0012DD70();
                break;
            case 4:
                func_0012D3D8();
                break;
            }
            func_0012E6F8();
        }
    }
    return 0;
}

void func_0012E9D0(void) {
    D_003BAD38 = 0;
    D_003BAD34 = 0xffffffff;
    D_003BAD3C = 0;
    D_003BAD40 = 0;
}

void func_0012E9E8(u32 arg0) {
    if (arg0 == 0) {
        D_003BAD40 = 0;
        if (D_003BAB38 != 0) {
            func_00218368(D_003BAB38);
        }
    } else {
        D_003BAD40 = arg0;
        func_00218320(D_003BAB38);
    }
}

void func_0012EA40(u32 arg0, u32 arg1) {
    D_003BAD34 = arg0;
    D_003BAD38 = arg1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EA50);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012EEA0);

extern s32 D_003BAB50;
extern void func_00136DA0(f32 *);

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

void fldUpdateCameraTarget(void) {
    union {
        u128 q;
        f32 f[4];
    } vec;
    f32 cur[3];
    FldCamState *st;
    u128 *dst;

    if (D_003BAB34 != 0 && (st = (FldCamState *)D_0032E3B0, st->unk178 != 1) && D_003BAB50 != 0) {
        cur[0] = st->unk140;
        cur[1] = st->unk144;
        cur[2] = st->unk148;
        func_00136DA0(cur);
        if (st->unk84 != 0) {
            st->unk140 = st->unk158;
            st->unk144 = st->unk15C;
            st->unk148 = st->unk160;
            vec.f[0] = st->unk158;
            vec.f[1] = st->unk15C;
            vec.f[2] = st->unk160;
            effObjSetInnerFirstVec(D_003BAB34, vec.f);
            st->unk84 = 0;
            effObjFetchInnerFirstVec(D_003BAB34);
            __asm__ volatile (
                ".set noreorder\n"
                "sqc2 vf10, 0(%0)\n"
                ".set reorder"
                : : "r"(&vec) : "memory");
            dst = (u128 *)(*(u32 *)(D_003BAB34 + 0x1C) + 0x70);
            PCP_COPY_VECTOR(dst, &vec);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012F578);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FC20);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FD00);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FE30);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0012FF48);

s32 func_00131098(void) {
    s16 node;

    if (D_0032E570[0] != 0) {
        return 0;
    }
    if (func_001462D0() != 0) {
        return 0;
    }
    if (func_00125DF8(0x40) == 0) {
        if (D_0032E3B0[20] == 1 || D_0032E3B0[20] == 3) {
            func_00131218();
        }
        func_0012FC20();
        fldUpdateCameraTarget();
        func_0012F578();
        node = *(s16 *)(D_003BAB38 + 0x12);
        func_0012EEA0(node, node);
        if (D_0032E3B0[70] == 1) {
            func_0012EA50(0, 0, 6.0f);
        }
        return 0;
    }
    if (D_0032E400[0] == 1) {
        func_00131218();
        func_0012FF48();
    } else {
        func_0012FF48();
    }
    return 0;
}

u8 func_001311A0(void) {
    if (D_0032E3B0[0x5F] == 0) {
        if (D_0032E3B0[0x61] == 0) {
            if (D_0032E3B0[0x60] == 0) {
                return 0;
            }
        }
    }
    return 1;
}

void func_00131458(void);

void func_001311D8(void) {
    dds3SetObjectFlags(D_003BAB34, 1);
    func_00131458();
}

void func_001311F8(void) {
    dds3ClearObjectFlags(D_003BAB34, 1);
}

void func_00131218(void) {
    func_00112930(D_003BAB34, 0);
    dds3ClearObjectFlags(D_003BAB34, 0x800);
}

void func_00131240(void) {
    func_00112930(D_003BAB34, 0x80);
    dds3SetObjectFlags(D_003BAB34, 0x800);
}

void func_00131268(void) {
    *(u32 *)(*(s32 *)(D_003BAB38 + 0x18) + 0x1c) = 0;
}

void func_00131278(void) {
    *(u32 *)(*(s32 *)(D_003BAB38 + 0x18) + 0x1c) = 0x80808080;
}

void func_00131290(void) {
    dds3SetObjectFlags(D_003BAB34, 0x200);
}

void func_001312B0(void) {
    dds3ClearObjectFlags(D_003BAB34, 0x400);
    dds3ClearObjectFlags(D_003BAB34, 0x200);
}

extern void func_001372D0(f32 *);
extern void effObjClearNodeFlags(void *, s32);
INCLUDE_ASM(const s32, "game/code_00126A30", func_001312D8);

extern s32 fldGetLocationCoordinateValue(s32, s32);
extern void func_0012EA50(s16, s32, f32);

void func_001313E0(void) {
    s16 node = *(s16 *)(D_003BAB38 + 0x12);
    if (fldGetLocationCoordinateValue(D_0032E3B0[4], D_0032E3B0[5] + 1) & 0x40) {
        func_0012EA50(node, 0x12, 10.0f);
        return;
    }
    func_0012EA50(node, 3, 10.0f);
}

void func_00131458(void) {
    s16 node = *(s16 *)(D_003BAB38 + 0x12);
    if (fldGetLocationCoordinateValue(D_0032E3B0[4], D_0032E3B0[5] + 1) & 0x40) {
        func_0012EA50(node, 0x12, 0.0f);
        return;
    }
    func_0012EA50(node, 3, 0.0f);
}

extern s32 D_003BAB38;
extern s32 mdlAddEntryPlainEx(s32, s32, s32, f32, f32);
s32 func_001314C0(s32 value) {
    s32 object = D_003BAB38;
    *(f32 *)(*(s32 *)(object + 0x1C) + 0x20) = 1.0f;
    return mdlAddEntryPlainEx(object, 0, value, 2.0f, 5.0f);
}

extern void mdlSetNodeFloat20(s32, s32, f32);
extern void mdlAddEntryFlagged(s32, s32, s32);

void func_00131508(s32 first, s32 second) {
    mdlSetNodeFloat20(D_003BAB38, 0, 1.0f);
    mdlSetNodeFloat20(D_003BAB38, 1, 1.0f);
    mdlAddEntryFlagged(D_003BAB38, 0, first);
    mdlAddEntryFlagged(D_003BAB38, 1, second);
}

void func_00131580(void) {
    D_0032E538[0] = 0;
}

void func_00131590(void) {
    D_0032E544[0] = 0;
}

/* Camera facing requests share the field-work block. Both request states
 * advance from 1 to 2 when their new angle is installed. */
void fldQueueCameraFacingPoint(f32 arg0, f32 arg1) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    *(f32 *)(temp_v0 + 0x18C) = arg0;
    *(f32 *)(temp_v0 + 0x190) = arg1;
    *(u32 *)(temp_v0 + 0x188) = 1;
}

void fldQueueCameraFacingAngle(f32 arg0, f32 arg1, f32 arg2) {
    u8 *temp_v0;
    f32 temp_f0;

    temp_f0 = sdfAtan2(arg0, arg2);
    temp_v0 = (u8 *)D_0032E3B0;
    temp_f0 *= 180.0f / 3.14f;
    *(u32 *)(temp_v0 + 0x194) = 1;
    *(f32 *)(temp_v0 + 0x198) = -temp_f0;
}

void fldApplyCameraFacingPoint(void) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    if (*(u32 *)(temp_v0 + 0x188) != 0) {
        f32 temp_f12 = *(f32 *)(temp_v0 + 0x140) - *(f32 *)(temp_v0 + 0x18C);
        f32 temp_f13 = *(f32 *)(temp_v0 + 0x148) - *(f32 *)(temp_v0 + 0x190);
        f32 temp_f0;

        *(u32 *)(temp_v0 + 0x188) = 2;
        temp_f0 = sdfAtan2(temp_f12, temp_f13);
        temp_f0 *= 180.0f / 3.14f;
        *(f32 *)(temp_v0 + 0x168) = -temp_f0;
    }
}

void fldApplyCameraFacingAngle(void) {
    u8 *temp_v0 = (u8 *)D_0032E3B0;

    if (*(u32 *)(temp_v0 + 0x194) != 0) {
        *(u32 *)(temp_v0 + 0x194) = 2;
        *(f32 *)(temp_v0 + 0x168) = *(f32 *)(temp_v0 + 0x198);
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131688);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0048);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0058);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0068);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A00A8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00131A88);

extern void *D_003BAD5C;
extern char D_003A0100[];
extern u32 D_003BD7C8;
extern u32 func_002EB028(const char *, u32 *, s32);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 func_002E5C68(u32, void *, u32);
extern void func_002E5C38(u32);

void fldLoadSkyResource(s32 area) {
    char path[64];
    char directory[32];
    u32 command;

    D_003BAD78 = 0x80;
    if (area < 200) {
        fldFormatAreaDirectory(directory, area, 1);
        func_003014F0(path, "%sF%03d.SKY", directory, area);
        command = sdfDevCreateCommandState(path);
        func_002E5C68(command, D_003BAD5C, 0xE000);
        func_002E5C38(command);
        if (area >= 2 && area < 100 && D_003BD7C4 == 0) {
            D_003BD7C4 = func_002EB028(D_003A0100, &D_003BD7C8, 0);
            D_003BAD70 = func_002D3288((void *)D_003BD7C8);
        }
    }
}

void func_00131D88(void) {
    if (D_003BAD70 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAD70);
        D_003BAD70 = 0;
    }
    if (D_003BD7C4 != 0) {
        func_002D0A10(D_003BD7C4);
        D_003BD7C4 = 0;
    }
    if (D_003BAD68 != 0) {
        func_0029CE80(D_003BAD68);
        D_003BAD68 = 0;
    }
    D_003BAD6C = 0;
}

void fldUploadSkyBuffer(void *src) {
    D_003BAD78 = 0x80;
    memcpy(D_003BAD5C, src, 0xE000);
    func_00131D88();
    if (D_0032E3C0[0] >= 2 && D_0032E3C0[0] < 100 && D_003BD7C4 == 0) {
        D_003BD7C4 = func_002EB028(D_003A0100, &D_003BD7C8, 0);
        D_003BAD70 = func_002D3288((void *)D_003BD7C8);
    }
}

extern f32 sdfSinPoly(f32);
INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0100);

void fldUpdateSwayOffset(void) {
    D_003BAD84 = 0;
    switch (D_003BAD7C) {
    case 1:
        D_003BAD80 += 0.1f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 32.0f;
        break;
    case 2:
        D_003BAD80 += 0.2f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 32.0f;
        break;
    case 3:
        D_003BAD80 += 0.05f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 32.0f;
        break;
    case 4:
        D_003BAD80 += 0.1f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 48.0f;
        break;
    case 5:
        D_003BAD80 += 0.2f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 48.0f;
        break;
    case 6:
        D_003BAD80 += 0.05f;
        D_003BAD84 = sdfSinPoly(D_003BAD80) * 48.0f;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132010);

extern s32 D_003BAD58;
extern s32 D_003BAD88, D_003BAD8C;
extern s32 *D_003BAD74;
extern f32 D_003BAD90, D_003BAD94;
void fldSetFadeTarget(s32 area, s32 value, s32 duration) {
    if (D_003BAD58 == 0 && D_0032E3C0[0] < 40) {
        duration = 0;
    }
    if (duration == 0) {
        D_003BAD8C = area;
        D_003BAD90 = 1.0f;
        D_003BAD94 = 1.0f;
    } else {
        D_003BAD90 = 0.0f;
        D_003BAD8C = D_003BAD88;
        D_003BAD94 = (f32)duration;
    }
    D_003BAD88 = area;
    D_003BAD74[area * 73] = value;
}

void fldSetSwayMode(u32 arg0) {
    D_003BAD7C = arg0;
    D_003BAD80 = 0;
    D_003BAD84 = 0;
}

void func_00132B70(u32 arg0) {
    D_003BAD78 = arg0;
}

u32 func_00132B78(void) {
    return D_003BAD78;
}

void func_00132B80(u32 arg0) {
    D_0032E5A8[0] = arg0;
}

u32 func_00132B90(void) {
    return D_0032E5A8[0];
}

void func_00132BA0(u32 arg0) {
    u32 temp_v0 = D_0032E59C[0];

    D_003BAD9C = arg0;
    D_003BADA0 = 0;
    D_003BADA4 = temp_v0;
    func_00132FD0(temp_v0, 1);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132BD0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132E38);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00132FD0);

extern s32 D_0032E3C0[];
extern s16 D_0032E4D8[];
extern s32 D_003BAD58;

void func_00133280(s32 speed) {
    if (D_003BAD58 == 0 && D_0032E3C0[0] < 40) {
        speed = 0;
    }
    if (D_003BADC8 != 0 && D_003BAD98 != D_003BADC8) {
        if (D_0032E4D8[0] == 0) {
            D_0032E59C[0] = D_003BADC8;
        }
        func_00132FD0(D_003BADC8, speed);
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001332E8);

void func_00133600(s32 arg0) {
    if (arg0 == 0) {
        dds3ClearObjectFlags(D_003BAB34, 0x100);
        return;
    }
    dds3SetObjectFlags(D_003BAB34, 0x100);
    func_00113AA8(D_003BAB34);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133640);

INCLUDE_ASM(const s32, "game/code_00126A30", fldSetDisplayState);

void fldInitializeDisplayPointerTable(void) {
    u32 *displayPointers = D_00330738;

    memset(displayPointers, 0, 0x14);
    displayPointers[0] = (u32)D_003306B0;
    displayPointers[1] = (u32)D_003306C0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00133960);

typedef struct {
    u8 type;
    u8 pad1[3];
    s32 unk4;
    s32 unk8;
    s32 unkC;
    u8 pad10[0xC];
    s32 unk1C;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    f32 unk38;
    f32 unk3C;
    f32 unk40;
    f32 unk44;
    f32 unk48;
    f32 unk4C;
    f32 unk50;
    f32 unk54;
    f32 unk58;
    f32 unk5C;
    f32 unk60;
    f32 unk64;
    f32 unk68;
    f32 unk6C;
    f32 unk70;
    f32 unk74;
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    u8 pad8C[0x54];
} FldLightSet; /* 0xE0 bytes */
extern void *D_003BAD60;
extern s32 func_001082D8(s32, void *);
extern s32 func_00107FD8(s32, s32, void *);
extern s32 func_00108218(s32, void *);
extern s32 func_001080D8(s32, s32, void *);
extern s32 evtUnk8360SetVec(s32, f32, f32, f32, f32);
extern void fldSetSwayMode(u32);
void fldApplyLightSetCurrent(void) {
    FldLightSet *light = &((FldLightSet *)D_003BAD60)[D_003BAD98];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    area = light->type;
    D_0032E570[13] = area;
    D_0032E570[14] = light->unk4;
    value = light->unk8;
    D_0032E570[15] = value;
    D_0032E570[16] = light->unkC;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->unk2C * 0.00390625f;
    vec[1] = light->unk30 * 0.00390625f;
    vec[2] = light->unk34 * 0.00390625f;
    vec[3] = 0;
    func_001082D8(0, vec);
    evtUnk8360SetVec(0, light->unk1C, light->unk24, light->unk20, light->unk28);
    dir[0] = light->unk44;
    dir[1] = light->unk48;
    dir[2] = light->unk4C;
    dir[3] = 0;
    func_001080D8(0, 0, dir);
    vec[0] = light->unk38;
    vec[1] = light->unk3C;
    vec[2] = light->unk40;
    vec[3] = 0;
    func_00107FD8(0, 0, vec);
    dir[0] = light->unk5C;
    dir[1] = light->unk60;
    dir[2] = light->unk64;
    dir[3] = 0;
    func_001080D8(0, 1, dir);
    vec[0] = light->unk50;
    vec[1] = light->unk54;
    vec[2] = light->unk58;
    vec[3] = 0;
    func_00107FD8(0, 1, vec);
    dir[0] = light->unk74;
    dir[1] = light->unk78;
    dir[2] = light->unk7C;
    dir[3] = 0;
    func_001080D8(0, 2, dir);
    vec[0] = light->unk68;
    vec[1] = light->unk6C;
    vec[2] = light->unk70;
    vec[3] = 0;
    func_00107FD8(0, 2, vec);
    vec[0] = light->unk80;
    vec[1] = light->unk84;
    vec[2] = light->unk88;
    vec[3] = 1.0f;
    func_00108218(0, vec);
}

void fldApplyLightSetIndex(s32 index) {
    FldLightSet *light = &((FldLightSet *)D_003BAD5C)[index];
    f32 vec[4];
    f32 dir[4];
    s32 area;
    s32 value;

    D_003BAD98 = index;
    area = light->type;
    D_0032E570[13] = area;
    D_0032E570[14] = light->unk4;
    value = light->unk8;
    D_0032E570[15] = value;
    D_0032E570[16] = light->unkC;
    fldSetFadeTarget(area, value, 0);
    fldSetSwayMode(D_0032E570[16]);
    vec[0] = light->unk2C * 0.00390625f;
    vec[1] = light->unk30 * 0.00390625f;
    vec[2] = light->unk34 * 0.00390625f;
    vec[3] = 0;
    func_001082D8(0, vec);
    evtUnk8360SetVec(0, light->unk1C, light->unk24, light->unk20, light->unk28);
    dir[0] = light->unk44;
    dir[1] = light->unk48;
    dir[2] = light->unk4C;
    dir[3] = 0;
    func_001080D8(0, 0, dir);
    vec[0] = light->unk38;
    vec[1] = light->unk3C;
    vec[2] = light->unk40;
    vec[3] = 0;
    func_00107FD8(0, 0, vec);
    dir[0] = light->unk5C;
    dir[1] = light->unk60;
    dir[2] = light->unk64;
    dir[3] = 0;
    func_001080D8(0, 1, dir);
    vec[0] = light->unk50;
    vec[1] = light->unk54;
    vec[2] = light->unk58;
    vec[3] = 0;
    func_00107FD8(0, 1, vec);
    dir[0] = light->unk74;
    dir[1] = light->unk78;
    dir[2] = light->unk7C;
    dir[3] = 0;
    func_001080D8(0, 2, dir);
    vec[0] = light->unk68;
    vec[1] = light->unk6C;
    vec[2] = light->unk70;
    vec[3] = 0;
    func_00107FD8(0, 2, vec);
    vec[0] = light->unk80;
    vec[1] = light->unk84;
    vec[2] = light->unk88;
    vec[3] = 1.0f;
    func_00108218(0, vec);
}

typedef struct FldColorParams {
    s32 enabled;
    s32 unk4;
    s32 mode;
    s32 red;
    s32 green;
    s32 blue;
    s32 unk18;
    s32 unk1C;
} FldColorParams;
typedef struct FldCameraSetting {
    s32 unk0;
    FldColorParams color;
    u8 pad24[0x30];
} FldCameraSetting; /* 0x54 bytes */
typedef struct FldFadeColor {
    u8 pad0[4];
    s32 colorA;
    s32 colorB;
    u8 padC[0x18];
    s32 unk24;
    s32 unk28;
    u8 pad2C[0xC];
    s32 unk38;
    f32 unk3C;
} FldFadeColor;
extern FldFadeColor D_003C9240[];
extern FldCameraSetting *D_003BAD64;
extern FldCameraSetting D_003C9280[];
extern u32 func_0029CE50(const void *);
extern void func_0029CE80(s32);
void func_001340E0(s32 enable) {
    FldCameraSetting *setting;
    FldColorParams *color;

    if (D_003BAD6C == 0 && enable != 0) {
        if (D_003BAD68 != 0) {
            func_0029CE80(D_003BAD68);
        }
        setting = D_003BAD64;
        D_003BAD68 = 0;
        color = &setting->color;
        if (color->enabled != 0) {
            D_003C9240->colorB = D_003C9240->colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
            D_003C9240->unk24 = color->unk18;
            switch (color->mode) {
            case 0:
                D_003C9240->unk28 = 1;
                break;
            case 1:
                D_003C9240->unk28 = 2;
                break;
            default:
                D_003C9240->unk28 = 3;
                break;
            }
            D_003C9240->unk38 = color->unk4;
            D_003C9240->unk3C = color->unk1C;
            D_003BAD68 = func_0029CE50(D_003C9240);
            setting = D_003BAD64;
        }
    } else {
        setting = D_003BAD64;
    }
    *D_003C9280 = *setting;
    D_003BAD6C = enable;
}

extern s32 D_003BADE0;
s32 fldComposeFadeColor(s32 fade, s32 color, s32 alpha) {
    s32 scaled;

    if (fade < 0) {
        fade = 0;
    }
    scaled = alpha * D_003BADE0 / 100;
    if (fade < 0xE0) {
        return color | (scaled << 24);
    }
    scaled = (1.0f - (f32)(fade - 0xE0) * 0.00390625f) * scaled;
    if (scaled < 0) {
        scaled = 0;
    }
    if (scaled > 0x80) {
        scaled = 0x80;
    }
    return color | (scaled << 24);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00134348);

extern char D_003A0100[];
extern u32 D_003BD7C8;
extern u32 func_002EB028(const char *, u32 *, s32);
extern void func_00134E10(FldCameraSetting *);

void func_00134C68(void) {
    D_003BD7C4 = func_002EB028(D_003A0100, &D_003BD7C8, 0);
    D_003BAD70 = func_002D3288((void *)D_003BD7C8);
    D_003BAD68 = func_0029CE50(D_003C9240);
    if (D_003BD7C4 != 0) {
        func_002D0A10(D_003BD7C4);
        D_003BD7C4 = 0;
    }
    func_00134E10(D_003BAD64);
    D_003BAD6C = 1;
}

void func_00134CD8(void) {
    func_00134348();
}

void func_00134CF0(void) {
    D_003BAD6C = 0;
    if (D_003BAD70 != 0) {
        sdfTexReleaseReferenceViaHandler(D_003BAD70);
        D_003BAD70 = 0;
    }
    if (D_003BAD68 != 0) {
        func_0029CE80(D_003BAD68);
        D_003BAD68 = 0;
    }
}

void fldCopyCameraSetting(FldCameraSetting *destination) {
    *destination = *D_003C9280;
}

extern void itfCopyColorFields(s32, void *);
void func_00134E10(FldCameraSetting *setting) {
    FldColorParams *color = &setting->color;

    if (color->enabled != 0) {
        D_003C9240->colorB = D_003C9240->colorA = (color->blue << 16) | color->red | (color->green << 8) | 0x80000000;
        D_003C9240->unk24 = color->unk18;
        switch (color->mode) {
        case 0:
            D_003C9240->unk28 = 1;
            break;
        case 1:
            D_003C9240->unk28 = 2;
            break;
        default:
            D_003C9240->unk28 = 3;
            break;
        }
        D_003C9240->unk38 = color->unk4;
        D_003C9240->unk3C = color->unk1C;
        itfCopyColorFields(D_003BAD68, D_003C9240);
    }
    *D_003C9280 = *setting;
}


/* Keep both the resource handles and retained addresses: callers use the
 * retained storage, whereas the handles are needed at release time. */
void fldAllocateRecordStorage(void) {
    u8 *storage = func_002D03F8(0x72000);

    D_003BAE00 = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    D_003BADF0 = (u32)storage;
    memset(storage, 0, 0x72000);
    storage = func_002D03F8(0x4A00);
    D_003BADFC = (u32)storage;
    storage = sdfResourceRetainAddress(storage);
    D_003BADE8 = (u32)storage;
    memset(storage, 0, 0x4A00);
}

void func_00135018(void) {
    func_002D0A60(D_003BAE00);
    func_002D0A10(D_003BAE00);
    D_003BADF0 = 0;
    func_002D0A60(D_003BADFC);
    func_002D0A10(D_003BADFC);
    D_003BADE8 = 0;
}

float fldDotVector(float *arg0, float *arg1) {
    return *arg0 * *arg1 + arg0[1] * arg1[1] + arg0[2] * arg1[2];
}

f32 fldCalculateVectorLength(const f32 *vector) {
    return fsqrtf(vector[0] * vector[0] + vector[1] * vector[1] + vector[2] * vector[2]);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_001350B8);

typedef struct FldRecE4 {
    u8 pad0[0xCC];
    s32 id;
    s32 value;
    u8 padD4[0x10];
} FldRecE4; /* 0xE4 bytes */
s32 fldGetRecordValueById(s32 key) {
    s32 i = 0;

    if (D_003BADF4 > 0) {
        FldRecE4 *record = (FldRecE4 *)D_003BADF0;
        do {
            if (record->id == key) {
                return record->value;
            }
            i++;
            record++;
        } while (i < D_003BADF4);
    }
    return 0;
}

void fldSetRecordValueById(s32 id, s32 value) {
    s32 i;

    for (i = 0; i < D_003BADF4; i++) {
        if (((FldRecE4 *)D_003BADF0)[i].id == id) {
            ((FldRecE4 *)D_003BADF0)[i].value = value;
        }
    }
}

void fldResetRecordState(void) {
    s32 count = D_003BADF4;
    if (count > 0) {
        /* Required to match: advance a pointer to the value field, not the record base. */
        u8 *record = (u8 *)D_003BADF0 + 0xd0;
        do {
            count--;
            *(s32 *)record = 0;
            record += 0xe4;
        } while (count != 0);
    }
    D_003BADF4 = 0;
    D_0032E3B0[0x28] = -1;
    D_0032E3B0[0x29] = -1;
    D_0032E3B0[0x2b] = -1;
    D_003BADF8 = 0;
    D_003BADEC = 0;
    if (D_003BADF0 != 0) {
        func_00135018();
    }
}


INCLUDE_ASM(const s32, "game/code_00126A30", func_00135360);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136850);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136A78);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136DA0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00136FE8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001372D0);

void func_00137E90(void) {
}

void func_00137E98(void) {
}

void func_00137EA0(void) {
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00137EA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138058);

INCLUDE_ASM(const s32, "game/code_00126A30", func_001384E8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138910);

void func_00138C28(void) {
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138C30);

extern u64 dds3GetWorldSecondaryObject(void);
extern s32 D_003BAE18;
extern s32 D_003BAE20;
extern s32 D_003BAE2C;
typedef struct FldTaskInfo {
    s32 unk0;
    s32 slot;
} FldTaskInfo;
extern s32 func_00110A48(u64, u32, s32);
extern u32 dds3GetPathState(s32);
void fldResetTaskSlots(void) {
    s32 i;
    u64 world;
    u32 id;
    FldTaskInfo *info;

    D_003BAE2C = 1;
    for (i = 0; i < D_003BAE14; i++) {
        D_003308B0[i] = 0;
    }
    D_003BAE18 = -1;
    D_003BAE1C = -1;
    D_003BAE20 = -1;
    world = dds3GetWorldSecondaryObject();
    if (world != 0) {
        for (i = 0; i < D_003BAE14; i++) {
            info = *(FldTaskInfo **)(D_003307B0[i] + 8);
            if (info->slot >= 0) {
                id = dds3GetPathState(func_00110A48(world, *(u32 *)D_003C92E0[info->slot], 0xD));
                if (func_0010BED8(id) != 0) {
                    func_00110D88(dds3GetWorldObject(), id);
                }
            }
        }
    }
}

u32 fldPushDisplayValue(u32 value) {
    u32 i = D_003BAE28;

    D_003C92E0[i] = value;
    D_003BAE28 = i + 1;
    return i;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_00138ED0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013A720);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013A9B0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AC10);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AE28);

typedef struct FldRoomPlanes {
    f32 plane[6][4];
    f32 limit[6];
    u8 pad78[0x140 - 0x78];
} FldRoomPlanes;
extern FldRoomPlanes D_003C9470[];
extern f32 fldDotVector(f32 *, f32 *);

s32 fldRoomContainsPoint(f32 *point, s32 room) {
    FldRoomPlanes *planes = &D_003C9470[room];
    f32 *limit = planes->limit;
    f32 *plane = planes->plane[0];
    s32 i;

    for (i = 0; i < 6; i++) {
        if (fldDotVector(point, plane) - *limit > 0.0f) {
            return 0;
        }
        plane += 4;
        limit++;
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013AFC8);

s32 func_0013B088(u32 flag, u32 slot) {
    D_003308B0[slot] = 0;
    if (func_0010BED8(flag) != 0) {
        func_00110D88(dds3GetWorldObject(), flag);
        return 0;
    }
    return -1;
}

u32 fldDestroyTaskSlot(u32 slot) {
    u32 *task = &D_003308B0[slot];

    if (kwlnTaskIsRegistered(*task) != 0) {
        kwlnTaskDestroyWithHierarchy(*task, 0);
    }
    *task = 0;
    return 0;
}

s32 func_0013B130(void) {
    u8 *object;
    u32 state = D_003BAB3C;
    if (!(state & 1)) {
        return 0;
    }
    if ((state & 2) != 0 && D_0032C9A0[0] != 0) {
        func_00110D88(dds3GetWorldObject(), (u32)D_0032C9A0);
    }
    D_0032C9A0[0] = 0;
    D_003BAB3C = 0;
    object = func_0013DAC0();
    if (func_0010BED8((u32)object) == 0) {
        func_00110D00(dds3GetWorldObject(), object);
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013B1D8);

void func_0013BD70(void) {
    s32 count = D_003BAE14;
    s32 i = 0;
    if (count > 0) {
        u32 **entry = D_003307B0;
        do {
            fldDrawMarkerQuad((*entry)[4]);
            i++;
            entry++;
        } while (i < D_003BAE14);
    }
}

void fldClearInactiveTaskSlots(void) {
    s32 count = D_003BAE14;
    s32 i = 0;
    if (count > 0) {
        u32 *entry = D_003308B0;
        do {
            if (kwlnTaskIsRegistered(*entry) == 0) {
                *entry = 0;
            }
            i++;
            entry++;
        } while (i < D_003BAE14);
    }
}

s32 fldFindTaskRecordId(u32 task) {
    s32 i;
    for (i = 0; i < D_003BAE14; i++) {
        if (D_003308B0[i] == task) {
            return D_003307B0[i][0];
        }
    }
    return -1;
}

typedef struct FldRoomState {
    u8 pad0[0x120];
    s32 unk120;
    u8 pad124[0xE];
    s16 roomId; /* 0x132: returned by fldFindRoomByTask */
    u8 pad134[2];
    s16 mode;
    u8 pad138[8];
} FldRoomState; /* 0x140 bytes */
extern FldRoomState D_003C93E0[];
extern s32 D_003BAE14;
s32 fldFindRoomByTask(u32 task) {
    s32 i;

    for (i = 0; i < D_003BAE14; i++) {
        if (D_003308B0[i] == task) {
            return D_003C93E0[i].roomId;
        }
    }
    return -1;
}


s32 fldGetTaskRecordValue(u32 task) {
    s32 i;
    for (i = 0; i < D_003BAE14; i++) {
        if (D_003308B0[i] == task) {
            return D_003307B0[i][2];
        }
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013BF48);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C138);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C2B8);

s32 fldCheckEntryActive(s32 entry) {
    s32 entryIdx = D_003BAE1C;
    s16 *slot;

    if (D_003BAE30 != 0) {
        return 1;
    }
    if (entryIdx == -1) {
        return 0;
    }
    slot = D_003C9510 + entryIdx * 0xA0;
    if (slot[3] == 1 && slot[4] == entry) {
        return 1;
    }
    return 0;
}

s32 fldHasActiveTasks(void) {
    s32 i;
    for (i = 0; i < D_003BAE14; i++) {
        if (D_003308B0[i] != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_0013C5D0(void) {
    s32 temp_v0 = D_003BAE3C;

    if (temp_v0 < 0) {
        return -1;
    }
    return D_003C9518[temp_v0 * 160];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C600);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C7F8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013C9E0);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0150);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CBA8);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013CEB0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D410);

extern void fldFormatAreaDirectory(char *, s32, s32);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 func_002E5C68(u32, void *, u32);
extern void func_002E5C38(u32);
extern u8 D_00332E30[];

extern void fldFormatAreaDirectory(char *, s32, s32);
extern s32 func_003014F0(char *, const char *, ...);
extern u32 sdfDevCreateCommandState(const char *);
extern u32 func_002E5C68(u32, void *, u32);
extern void func_002E5C38(u32);
extern u8 D_00332E30[];
extern char D_003A01F8[]; /* "%sF%03d.INF": one string split at +8 from the separately included D_003A0200 */
void fldLoadInfoTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field < 200) {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, D_003A01F8, directory, field);
        command = sdfDevCreateCommandState(path);
        func_002E5C68(command, D_00332E30, 0x3B80);
        func_002E5C38(command);
    }
}

void func_0013D598(const void *source) {
    memcpy(D_00332E30, source, 0x3B80);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013D650);

extern s32 mdlFlagTest(s32);
typedef struct FldAreaState {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0xEC];
    s16 unk104;
} FldAreaState;
extern u8 D_003369B0[];
extern u8 D_003369C0[];
extern u8 D_003369D0[];
extern u8 D_003369E0[];
extern u8 D_00336A00[];
extern u8 D_00336A10[];
extern u8 D_00336A30[];
extern u8 D_00336A40[];
extern u8 D_00336A50[];
extern s32 D_003BAE6C;
INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A01F8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0200);

u8 *func_0013D6D0(const char *name) {
    s32 i = 0;
    u8 *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = D_00337D00 + i * 108;
        flag = *(s16 *)(entry + 2);
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (((FldAreaState *)D_0032E3B0)->unk104 == 0 || !(entry[0x31] & 4))
            && *(s16 *)(entry + 4) == ((FldAreaState *)D_0032E3B0)->unk14 + 1
            && strcmp(name, (char *)(entry + 6)) == 0) {
            switch (*(s8 *)entry) {
            case 1:
                if (*(s8 *)(entry + 0x30) == 0) {
                    sub = *(s16 *)(entry + 0x32);
                    if (sub != 0) {
                        if (sub == 1) {
                            D_003BAE6C = 6;
                            return D_00336A10;
                        }
                    }
                }
                D_003BAE6C = 0;
                return D_003369B0;
            case 2:
                D_003BAE6C = 1;
                return D_003369C0;
            case 3:
                D_003BAE6C = 2;
                return D_003369D0;
            case 4:
                D_003BAE6C = 3;
                return D_003369E0;
            case 5:
                D_003BAE6C = 5;
                return D_00336A00;
            case 10:
                D_003BAE6C = 8;
                return D_00336A30;
            case 11:
                D_003BAE6C = 9;
                return D_00336A40;
            case 12:
                D_003BAE6C = 10;
                return D_00336A50;
            }
        }
        i++;
    } while (i < 0x100);
    D_003BAE6C = -1;
    return 0;
}

u8 *fldFindActorEntryByName(const char *name) {
    s32 i = 0;
    u8 *entry;
    s16 flag;
    s16 sub;

    if (name == 0) {
        return 0;
    }
    do {
        entry = D_00337D00 + i * 108;
        flag = *(s16 *)(entry + 2);
        if ((flag == 0 || mdlFlagTest(flag) != 0)
            && (((FldAreaState *)D_0032E3B0)->unk104 == 0 || !(entry[0x31] & 4))
            && *(s16 *)(entry + 4) == ((FldAreaState *)D_0032E3B0)->unk14 + 1
            && strcmp(name, (char *)(entry + 6)) == 0) {
            D_003BAE64 = i;
            switch (*(s8 *)entry) {
            case 1:
                if (*(s8 *)(entry + 0x30) == 0) {
                    sub = *(s16 *)(entry + 0x32);
                    if (sub != 0) {
                        if (sub == 1) {
                            D_003BAE68 = 6;
                            return D_00336A10;
                        }
                    }
                }
                D_003BAE68 = 0;
                return D_003369B0;
            case 2:
                D_003BAE68 = 1;
                return D_003369C0;
            case 3:
                D_003BAE68 = 2;
                return D_003369D0;
            case 4:
                D_003BAE68 = 3;
                return D_003369E0;
            case 5:
                D_003BAE68 = 5;
                return D_00336A00;
            case 10:
                D_003BAE68 = 0;
                return D_00336A30;
            case 11:
                D_003BAE68 = 0;
                return D_00336A40;
            case 12:
                D_003BAE68 = 0;
                return D_00336A50;
            }
        }
        i++;
    } while (i < 0x100);
    return 0;
}

extern s32 D_003BAB40;
extern s32 D_0032E3C4[];
extern u8 D_00336A30[];
u8 *func_0013DAC0(void) {
    s32 index = D_003BAB40;
    u8 *entry = D_00337D00 + index * 108;
    if (*(s16 *)(entry + 4) == D_0032E3C4[0] + 1 && *(s8 *)entry == 10) {
        D_003BAE64 = index;
        D_003BAE68 = 8;
        return D_00336A30;
    }
    return 0;
}

s32 func_0013DB28(void) {
    return D_00337D12[D_003BAE64 * 54];
}

s32 func_0013DB58(s32 arg0) {
    u8 *entry = D_00337D00 + D_003BAE64 * 108;

    if (arg0 == 0 && *(s8 *)entry == 1) {
        if (*(s16 *)(entry + 0x12) == 5 || *(s16 *)(entry + 0x14) == 5 || *(s16 *)(entry + 0x12) == 6
            || *(s16 *)(entry + 0x14) == 6 || *(s16 *)(entry + 0x12) == 7 || *(s16 *)(entry + 0x14) == 7
            || *(s16 *)(entry + 0x12) == 8 || *(s16 *)(entry + 0x14) == 8) {
            return 0x28;
        }
        return 0x14;
    }
    if (arg0 == 1) {
        return fldTestBits(entry[0x54], 8);
    }
    return 0;
}

extern s32 func_0010D6A0(void);
extern void fldPlayFieldSeVolumePan(s32);
extern void kwlnFadeInStart(s32, s32, s32, s32);
extern void kwlnFadeSetRGB(s32, s32, s32);
extern void func_00140AB8(s32);
extern u32 D_003CE3E0[][23];
extern s32 D_0032E530[];
void func_0013DC08(s32 arg0) {
    s32 index;
    s32 kind;
    s32 record;
    u8 *entry;

    if (arg0 != 0) {
        record = fldGetTaskRecordValue(*(u32 *)(func_0010D6A0() + 0xE4));
        if (record == 0) {
            return;
        }
        if (fldFindActorEntryByName((const char *)record) == 0) {
            return;
        }
    }
    index = D_003BAE64;
    entry = D_00337D00 + index * 108;
    kind = *(s8 *)entry;
    if (kind == 1) {
        if (*(s16 *)(entry + 4) == D_0032E3C4[0] + 1) {
            fldPlayFieldSeVolumePan(*(s16 *)(entry + 0x16));
            func_00140AB8(D_003CE3E0[index][1]);
            return;
        }
    } else if (kind == 2) {
        if (*(s16 *)(entry + 4) == D_0032E3B0[5] + 1) {
            D_0032E3B0[94] = 1;
            *(f32 *)&D_0032E3B0[93] = *(f32 *)&D_003CE3E0[index][11];
            if (*(s16 *)(entry + 0x12) == 1) {
                kwlnFadeInStart(0xC0, 0xC0, 0xC0, 0xF);
                return;
            }
            kwlnFadeInStart(0, 0, 0, 0xF);
            return;
        }
    } else if (kind == 3) {
        kwlnFadeSetRGB(0, 0, 0);
        return;
    } else if (kind == 5) {
        if (*(s16 *)(entry + 0x12) == 0) {
            D_0032E530[0] = 0x64;
        } else {
            D_0032E530[0] = -0x64;
        }
    } else if (kind == 10) {
    } else if (kind == 11) {
    } else if (kind == 12) {
    } else if (kind == 4) {
        fldApplyCameraFacingAngle();
    }
}

extern s32 func_00110ED0(u64, s32, void *);
extern void func_001109B8(u64, s32);
INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DDF0);

u8 func_0013DF18(void) {
    return D_003BAE68 == 8;
}

void func_0013DF28(s8 *arg0) {
    if (arg0[0x53] != 0) {
        D_0032E3B0[0x22] = arg0[0x53] - 1;
    }
    D_0032E3B0[0x16] = arg0[0x45];
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013DF60);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013E5A8);

void func_0013EC10(s32 mode, s32 index) {
    switch (mode) {
    case 0:
        D_003BAE5C = 3;
        D_003BAE54 = D_0032EF18[index].unk0;
        D_003BAE60 = index;
        break;
    case 1:
        D_003BAE60 = index;
        D_003BAE54 = D_0032EFE0[index].unk0;
        break;
    }
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EC68);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013EF10);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F100);

extern void fldRequestAreaResource();
INCLUDE_ASM(const s32, "game/code_00126A30", func_0013F340);

s32 fldGetActorStat0(s32 mode) {
    u8 *actor = D_00337D00 + D_003BAE64 * 108;
    s32 *entry;
    s32 flags;

    switch (mode) {
    case 0:
        return *(s16 *)(actor + 0x12);
    case 1:
        entry = func_00110F80(dds3GetWorldObject(), actor + 0x18);
        if (entry != NULL) {
            return entry[1];
        }
    case 2:
        entry = func_00110F80(dds3GetWorldObject(), actor + 0x24);
        if (entry != NULL) {
            return entry[1];
        }
    case 3:
        flags = *(u16 *)(actor + 0x14);
        if (flags & 1) {
            return 1;
        }
        return 0;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0270);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0280);

s32 fldGetActorStat1(u32 kind) {
    u8 *entry = D_00337D00 + D_003BAE64 * 108;
    s32 *found;

    switch (kind) {
    case 0:
        switch (*(s16 *)(entry + 0x12)) {
        case 0:
            return 12;
        case 1:
            return 15;
        case 2:
            return 16;
        case 3:
            return 13;
        case 4:
            return 14;
        }
        return 0;
    case 1:
        found = func_00110F80(dds3GetWorldObject(), entry + 0x18);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = func_00110F80(dds3GetWorldObject(), entry + 0x24);
        if (found != NULL) {
            return found[1];
        }
        return *(s16 *)(entry + 0x14);
    case 3:
        return *(s16 *)(entry + 0x14);
    case 4:
        return *(s16 *)(entry + 0x16);
    }
    return 0;
}

s32 fldGetActorMotionEntry(u32 kind) {
    u8 *entry = D_00337D00 + D_003BAE64 * 108;
    s16 index = *(s16 *)(entry + 0x12);
    s32 *found;
    s32 flags;

    switch (kind) {
    case 0:
        return D_00336A60[index].unk0;
    case 1:
        found = func_00110F80(dds3GetWorldObject(), D_00336A60[index].unk4);
        if (found != NULL) {
            return found[1];
        }
    case 2:
        found = func_00110F80(dds3GetWorldObject(), D_00336A60[index].unk14);
        if (found != NULL) {
            return found[1];
        }
    case 3:
        D_003BAE44 = D_00336A60[index].unk24;
        return 0;
    case 4:
        D_003BAE48 = D_00336A60[index].unk34;
        return 0;
    case 5:
        return D_00336A60[index].unk44;
    case 6:
        flags = entry[0x64];
        if (flags & 1) {
            return *(s8 *)(entry + 0x67);
        }
        return -1;
    }
    return 0;
}

s32 fldGetRowValue(u32 kind) {
    s32 slot = D_003BAE4C;

    switch (kind) {
    case 0:
        return D_00337C60[slot].count;
    case 1:
        return D_00337C60[slot].unk4;
    case 2:
        return D_00337C60[slot].body.data[0];
    case 3:
        return D_00337C60[slot].body.data[1];
    case 4:
        return D_00337C60[slot].body.data[2];
    case 5:
        return D_00337C60[slot].body.data[3];
    case 6:
        return D_00337C60[slot].body.data[4];
    case 7:
        return D_00337C60[slot].body.data[5];
    case 8:
        return D_00337C60[slot].body.data[6];
    case 9:
        return D_00337C60[slot].body.data[7];
    case 10:
        return D_00337C60[slot].body.data[8];
    case 11:
        return D_00337C60[slot].body.data[9];
    case 12:
        return D_00337C60[slot].body.data[10];
    case 13:
        return D_00337C60[slot].body.data[11];
    case 14:
        return D_00337C60[slot].count - D_003BAE50 - 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00126A30", fldFindTableEntry);

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FA40);

extern u32 D_003CE3E0[][23];
void fldResetActorSlots(void) {
    s32 i;

    for (i = 0; i < 256; i++) {
        D_003CE3E0[i][0] = 0;
        D_003CE3E0[i][1] = 0;
        D_003CE3E0[i][2] = 0;
        D_003CE3E0[i][3] = -1;
        D_003CE3E0[i][4] = 0;
        D_003CE3E0[i][5] = -1;
        D_003CE3E0[i][6] = -1;
        D_003CE3E0[i][7] = 0;
        D_003CE3E0[i][8] = 0;
        D_003CE3E0[i][9] = 0;
        D_003CE3E0[i][11] = 0;
        D_003CE3E0[i][12] = 0;
        D_003CE3E0[i][13] = 0;
        D_003CE3E0[i][14] = 0;
        D_003CE3E0[i][15] = 0;
        D_003CE3E0[i][16] = 0;
        D_003CE3E0[i][17] = 0;
        D_003CE3E0[i][18] = 0;
        D_003CE3E0[i][19] = 0;
        D_003CE3E0[i][20] = 0;
        D_003CE3E0[i][21] = 0;
        D_003CE3E0[i][22] = 0;
    }
    D_003BAE64 = -1;
}

void fldLoadActorWaypointTable(s32 field) {
    char path[64];
    char directory[32];
    u32 command;
    if (field >= 100) {
        memset(D_00337C60, 0, 0x6CA0);
    } else {
        fldFormatAreaDirectory(directory, field, 1);
        func_003014F0(path, "%sF%03d.WAP", directory, field);
        command = sdfDevCreateCommandState(path);
        func_002E5C68(command, D_00337C60, 0x6CA0);
        func_002E5C38(command);
    }
}

void fldCopyActorWaypointTable(const void *source) {
    memcpy(D_00337C60, source, 0x6CA0);
}

INCLUDE_ASM(const s32, "game/code_00126A30", func_0013FEC0);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140AB8);

INCLUDE_RODATA(const s32, "game/code_00126A30", D_003A0380);

INCLUDE_ASM(const s32, "game/code_00126A30", func_00140BE8);

void func_00140F68(void) {
}

extern s32 func_00124F08(void), func_00125140(void), func_0028F600(void);
extern s32 func_00124E90(void), func_00124EB8(void), func_00124EE0(void);
extern s32 func_0014D0D0(void);
extern u16 *func_00101A70(u32);
extern s32 func_00195CD8(void *, s32, s32);
extern void fldDrawGaugeBar(s32);
extern void fldDrawTitleBanner(s32, s32);
extern u8 D_0033E900[];

s32 func_00140F70(u32 task) {
    u16 *ticket;
    u8 *label;
    s32 width;
    s32 x;

    if (func_00124F08() != 0) {
        return 0;
    }
    if (func_00125140() != 0) {
        return 0;
    }
    if (func_0028F600() != 0) {
        return 0;
    }
    if (func_00124E90() != 0) {
        return 0;
    }
    if (func_00124EB8() != 0) {
        return 0;
    }
    if (func_00124EE0() != 0) {
        return 0;
    }
    if (func_0014D0D0() != 0) {
        return 0;
    }
    ticket = func_00101A70(task);
    if (ticket[2] != 0) {
        label = D_0033E900 + ticket[1] * 32;
        width = func_00195CD8(label, 1, 0x13);
        x = 0xF6 - (width >> 1);
        fldDrawGaugeBar(width);
        fldDrawTitleBanner(x, 0x131);
        func_0012B890(x + 0x14, 0x98, 0xA09DC380, (u64)(D_0033E900 + ticket[1] * 32));
        ticket[0] = ticket[0] + 1;
        ticket[2] = 0;
    }
    return 0;
}

void * func_00141098(u32 arg0) {
    u16 *temp_v0 = func_002CFEB8(8);

    temp_v0[1] = 1;
    temp_v0[0] = 0;
    temp_v0[2] = 0;
    temp_v0[3] = 0;
    func_00101A68(arg0, temp_v0);
    return (void *)func_00140F70;
}

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC70);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC74);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC78);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC7C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC80);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC84);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC88);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAC98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACD8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACE8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACF0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BACFC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD00);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD08);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD70);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD74);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD78);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD7C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD80);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD84);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD88);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD8C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD90);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD94);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD98);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAD9C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADA8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADAC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADB8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADBC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADC8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADCC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADD8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADDC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADE8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADEC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF0);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF4);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADF8);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BADFC);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE00);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE04);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE08);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE0C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE10);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE14);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE18);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE1C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE20);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE24);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE28);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE2C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE30);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE34);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE38);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE3C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE40);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE44);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE48);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE4C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE50);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE54);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE58);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE5C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE60);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE64);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE68);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE6C);

INCLUDE_SDATA(const s32, "game/code_00126A30", D_003BAE70);

