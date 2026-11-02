#include "common.h"
#include "sdf_types.h"
#include "sdf_resource.h"

struct sdfMemHandle_t;

#define SDF_QWORD_ALIGNMENT_MASK 0xF

#define SDF_PACKET_BUFFER_ALIGNMENT_MASK 0x7F

#define SDF_PACKET_BUFFER_COUNT 2

void sdfMemFreeDirect(struct sdfMemHandle_t *aHandle);

extern struct sdfMemHandle_t *sdfGrPacketBuffHandle;


u32 sdfMemLock(struct sdfMemHandle_t *aHandle);

extern s32 sdfGrPacketBuffSize;

/* DDS names the buffer pair; SMT3 has no name for it yet. */
#define D_00551870 sdfPacketBuffers
extern s32 D_00551870[2];

extern s32 sdfGrPacketBuffPtr;

extern s32 sdfGrPacketBuffLimit;

typedef struct effDebugPolyParams_t {
    s16 parameterCount;
    s16 vertexCount;
    u16 primitive;
    u16 mask;
    u32 color;
    const u32 *parameters;
    u128 *positions;
    u128 *normals;
    f32 *texcoords;
    u32 *extraTexcoords;
    u32 *colors;
    void *(*allocate)(s32);
    f32 depth;
} effDebugPolyParams_t;

void sdfGrMakePacketBuff(s32 aPacketBuffSize);

void sdfGrSetupPacketBuff(s32 aFrameId);

s32 sdfGrAllocPacketBuff(s32 aSize);

void sdfGrClosePacketBuff(s32 aBuffPtr);

void sdfGrMakePacketBuff(s32 aPacketBuffSize) {
    s32 allocationAddress;
    if (sdfGrPacketBuffHandle != 0) {
        sdfMemFreeDirect(sdfGrPacketBuffHandle);
        sdfGrPacketBuffHandle = 0;
    }
    aPacketBuffSize = (aPacketBuffSize + SDF_PACKET_BUFFER_ALIGNMENT_MASK) & ~SDF_PACKET_BUFFER_ALIGNMENT_MASK;
    sdfGrPacketBuffSize = aPacketBuffSize;
    sdfGrPacketBuffHandle = sdfMemAllocDirect(aPacketBuffSize * SDF_PACKET_BUFFER_COUNT);
    allocationAddress = sdfMemLock(sdfGrPacketBuffHandle);
    D_00551870[0] = allocationAddress;
    D_00551870[1] = allocationAddress + aPacketBuffSize;
}

void sdfGrSetupPacketBuff(s32 aFrameId) {
    sdfGrPacketBuffPtr = D_00551870[aFrameId];
    sdfGrPacketBuffLimit = D_00551870[aFrameId] + sdfGrPacketBuffSize;
}

s32 sdfGrGetPacketBuffRemain(void) {
    return sdfGrPacketBuffLimit - sdfGrPacketBuffPtr;
}

s32 sdfGrAllocPacketBuff(s32 aSize) {
    s32 packet;

    packet = sdfGrPacketBuffPtr;
    sdfGrPacketBuffPtr = sdfGrPacketBuffPtr + ((aSize + 0xfU) & 0xfffffff0);
    return packet;
}

s32 sdfGrOpenPacketBuff(void) {
    return sdfGrPacketBuffPtr;
}

void sdfGrClosePacketBuff(s32 aBuffPtr) {
    sdfGrPacketBuffPtr = (aBuffPtr + SDF_QWORD_ALIGNMENT_MASK) & ~SDF_QWORD_ALIGNMENT_MASK;
}

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffHandle);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffSize);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfPacketBuffers);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffPtr);

INCLUDE_SDATA(const s32, "sdf/sdfPacketBuff", sdfGrPacketBuffLimit);
