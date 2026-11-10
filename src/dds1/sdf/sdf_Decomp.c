#include "common.h"
#include "ps2asm.h"
#include "sdf_types.h"
#include "eeregs.h"

/* Declared here because no header carries these. */
extern void *memset(void *a, s32 b, size_t c);

enum sdfDecomp_enum {
    SDF_DECOMP_STAT_END = 0,
    SDF_DECOMP_STAT_IDLE = 1,
    SDF_DECOMP_STAT_DIREC_LENG0 = 2,
    SDF_DECOMP_STAT_DIREC_LENG1 = 3,
    SDF_DECOMP_STAT_DIREC = 4,
    SDF_DECOMP_STAT_DEC_NORMAL = 5,
    SDF_DECOMP_STAT_DEC_RLE = 6,
    SDF_DECOMP_STAT_DEC_SD_STEP0 = 7,
    SDF_DECOMP_STAT_DEC_SD_STEP1 = 8,
    SDF_DECOMP_STAT_DEC_SD_STEP2 = 9,
    SDF_DECOMP_STAT_DEC_WORD = 10
};
enum sdfDecomp_enum2 {
    SDF_DECOMP_DIRECTIVE_NORMAL = 0x00,
    SDF_DECOMP_DIRECTIVE_ZERO = 0x20,
    SDF_DECOMP_DIRECTIVE_RLE = 0x40,
    SDF_DECOMP_DIRECTIVE_SD = 0x60,
    SDF_DECOMP_DIRECTIVE_SD2 = 0x80,
    SDF_DECOMP_DIRECTIVE_WORD = 0xA0,
    SDF_DECOMP_DIRECTIVE_CMD = 0xE0
};
enum sdfDecomp_enum3 {
    SDF_DECOMP_CMD_END = 0xFF
};
/* No IDB members: the two fields a directive byte splits into. */
enum {
    SDF_DECOMP_DIRECTIVE_MASK = 0xE0,
    SDF_DECOMP_LENGTH_MASK = 0x1F
};

struct sdfDecompCb_t;

/* Load matrix into the VU0 scratch bank and postmultiply the primary bank. */
void MultMatrixRev(const void *aMatrix);

extern const u8 *sdfDecompHistory[16];

/* sdf3DScreenParam + 0x70: the scale and trans rows. */
/* Shared byte-storage view: the local screen structs are not compatible C types. */
extern u8 sdf3DScreenParam[];

extern u8 sdf3DScreenTm[];

/* Incremental decoder access view shared by both providers. The backing
 * decoder block is allocated as 0x20 bytes; only this 0x14-byte prefix is
 * described here, and its trailing allocation bytes remain unknown. */
typedef struct sdfDecompCb_t {
    u8 stat;             /* 0x00 */
    u8 direc;           /* 0x01 */
    u16 direcLeng;     /* 0x02 */
    u8 *dstPtr;           /* 0x04 */
    const u8 *srcPtr;      /* 0x08 */
    s32 srcRemain;       /* 0x0C */
    u8 sdOfs;        /* 0x10 */
    u8 pad11[3];          /* 0x11: opaque */
} sdfDecompCb_t;

/* Read the active packet cursor or restore it at a quadword boundary. */
s32 sdfGrOpenPacketBuff(void);

void sdfGrClosePacketBuff(saddr_t aBuffPtr);

extern s32 sdfDecompHistoryIndex;

extern void *memcpy(void *, const void *, size_t);

s32 sdfDecomp(sdfDecompCb_t *cb, const u8 *src, s32 length);

void sdfDecompInit(sdfDecompCb_t *cb, u8 *dst);

sdfGrPacketHeader_t *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags);

sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags);

s32 sdfDecomp(sdfDecompCb_t *cb, const u8 *src, s32 length) {
    s32 count;
    s32 offset;
    u8 *output;
    addr_t sourceAddress;

    if (cb->stat == 0) {
        return 1;
    }

    sdfDecompHistoryIndex = 0;
    output = cb->dstPtr;
    if (length > 0) {
        do {
            switch (cb->stat) {
                case SDF_DECOMP_STAT_IDLE: {
                    u8 command;
                    s32 commandBytes;
                    s32 historyIndex = sdfDecompHistoryIndex;

                    length--;
                    sdfDecompHistory[historyIndex] = src;
                    sdfDecompHistoryIndex = (historyIndex + 1) & 0xF;
                    command = *src++;
                    if (command >= SDF_DECOMP_DIRECTIVE_CMD) {
                        if (command == SDF_DECOMP_CMD_END) {
                            cb->dstPtr = output;
                            cb->srcPtr = src;
                            cb->srcRemain = length;
                            cb->stat = 0;
                            return 1;
                        }
                    } else {
                        cb->direc = command & SDF_DECOMP_DIRECTIVE_MASK;
                        commandBytes = command & SDF_DECOMP_LENGTH_MASK;
                        if (commandBytes == 0) {
                            cb->stat = SDF_DECOMP_STAT_DIREC_LENG0;
                        } else {
                            cb->direcLeng = commandBytes;
                            cb->stat = SDF_DECOMP_STAT_DIREC;
                        }
                    }
                    break;
                }
                case SDF_DECOMP_STAT_DIREC_LENG0:
                    cb->direcLeng = *src++;
                    length--;
                    cb->stat = SDF_DECOMP_STAT_DIREC_LENG1;
                    break;
                case SDF_DECOMP_STAT_DIREC_LENG1:
                    cb->direcLeng |= *src++ << 8;
                    length--;
                    cb->stat = SDF_DECOMP_STAT_DIREC;
                    break;
                case SDF_DECOMP_STAT_DIREC:
                    switch (cb->direc) {
                    case SDF_DECOMP_DIRECTIVE_NORMAL:
                        cb->stat = SDF_DECOMP_STAT_DEC_NORMAL;
                        break;
                    case SDF_DECOMP_DIRECTIVE_ZERO:
                        count = cb->direcLeng;
                        memset(output, 0, count);
                        output += count;
                        cb->stat = SDF_DECOMP_STAT_IDLE;
                        break;
                    case SDF_DECOMP_DIRECTIVE_RLE:
                        cb->stat = SDF_DECOMP_STAT_DEC_RLE;
                        break;
                    case SDF_DECOMP_DIRECTIVE_SD:
                        cb->stat = SDF_DECOMP_STAT_DEC_SD_STEP0;
                        break;
                    case SDF_DECOMP_DIRECTIVE_SD2:
                        cb->stat = SDF_DECOMP_STAT_DEC_SD_STEP1;
                        break;
                    case SDF_DECOMP_DIRECTIVE_WORD:
                        cb->stat = SDF_DECOMP_STAT_DEC_WORD;
                        break;
                    }
                    break;
                case SDF_DECOMP_STAT_DEC_NORMAL:
                    count = cb->direcLeng;
                    if (length < count) {
                        count = length;
                    }
                    memcpy(output, src, count);
                    output += count;
                    src += count;
                    length -= count;
                    cb->direcLeng -= count;
                    if (cb->direcLeng == 0) {
                        cb->stat = SDF_DECOMP_STAT_IDLE;
                    }
                    break;
                case SDF_DECOMP_STAT_DEC_RLE:
                    count = cb->direcLeng;
                    memset(output, *src++, count);
                    output += count;
                    length--;
                    cb->stat = SDF_DECOMP_STAT_IDLE;
                    break;
                case SDF_DECOMP_STAT_DEC_SD_STEP0:
                    sourceAddress = EE_ADDR((output - *src++));
                    count = cb->direcLeng;
                    length--;
                    do {
                        *output++ = *EE_ADDR_AS(u8 *, sourceAddress++);
                    } while (--count != 0);
                    cb->stat = SDF_DECOMP_STAT_IDLE;
                    break;
                case SDF_DECOMP_STAT_DEC_SD_STEP1:
                    cb->sdOfs = *src++;
                    length--;
                    if (length == 0) {
                        cb->stat = SDF_DECOMP_STAT_DEC_SD_STEP2;
                        break;
                    }
                    /* fallthrough */
                case SDF_DECOMP_STAT_DEC_SD_STEP2: {
                    s32 combinedOffset;

                    offset = *src++ << 8;
                    sourceAddress = cb->sdOfs;
                    length--;
                    combinedOffset = sourceAddress | offset;
                    sourceAddress = EE_ADDR((output - combinedOffset));
                    count = cb->direcLeng;
                    do {
                        *output++ = *EE_ADDR_AS(u8 *, sourceAddress++);
                    } while (--count != 0);
                    cb->stat = SDF_DECOMP_STAT_IDLE;
                    break;
                }
                case SDF_DECOMP_STAT_DEC_WORD:
                    count = cb->direcLeng;
                    if (length < count) {
                        count = length;
                        length = 0;
                        cb->direcLeng -= count;
                    } else {
                        cb->direcLeng = 0;
                        cb->stat = SDF_DECOMP_STAT_IDLE;
                        length -= count;
                    }
                    do {
                        *output++ = *src++;
                        *output++ = 0;
                    } while (--count != 0);
                    break;
            }
        } while (length > 0);
    }
    cb->srcPtr = src;
    cb->srcRemain = length;
    cb->dstPtr = output;
    return 0;
}

void sdfDecompInit(sdfDecompCb_t *cb, u8 *dst) {
    cb->dstPtr = dst;
    cb->stat = 1;
}

sdfGrPacketHeader_t *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    sdfGrPacketHeader_t *packet;
    f32 (*viewport)[4];
    u128 *cursor;
    s32 i;
    s32 emitted;
    s32 plane;
    u32 clipFlags;
    u32 firstClip;
    u32 secondClip;
    u32 quadwordCount;

    packet = EE_ADDR_AS(sdfGrPacketHeader_t *, sdfGrOpenPacketBuff());
    cursor = (u128 *)(packet + 1);
    viewport = (f32 (*)[4])(sdf3DScreenParam + 0x70);
    VU0_LOAD_VF14(viewport);
    VU0_LOAD_VF15_AT16(viewport);
    emitted = 0;
    for (i = 0; i < vertexCount; i += 2) {
        SDF_VU0_LOADVECTOR(vertices[i]);
        EE_MMI_RGBA_TO_VF8(colors[i]);
        SDF_VU0_APPLYMATRIX_XYZ1();
        SDF_VU0_LOADVECTOR_B(vertices[i + 1]);
        EE_MMI_RGBA_TO_VF9(colors[i + 1]);
        VU0_TRANSFORM_POINT_VF11();
        SDF_VU0_CLIPTEST();
        SDF_VU0_CLIPTEST_B();
        SDF_VU0_CLIPTESTWAIT(clipFlags);
        firstClip = (clipFlags >> 6) & 0x3F;
        secondClip = clipFlags & 0x3F;
        if ((firstClip & secondClip) != 0 || (firstClip & 0x20) != 0 || (secondClip & 0x20) != 0) {
            continue;
        }
        if (firstClip != 0 || secondClip != 0) {
            for (plane = 4; plane != -1; plane--) {
                if (firstClip & (1 << plane)) {
                    if ((plane & 1) == 0) {
                        VU0_POSITIVE_CLIP_PLANE_VF2();
                    } else {
                        VU0_NEGATIVE_CLIP_PLANE_VF2();
                    }
                    VU0_CLIP_SEGMENT_PREP_FIRST();
                    switch (plane) {
                    case 0:
                    case 1:
                        VU0_CLIP_RATIO_X();
                        break;
                    case 2:
                    case 3:
                        VU0_CLIP_RATIO_Y();
                        break;
                    default:
                        VU0_CLIP_RATIO_Z();
                        break;
                    }
                    VU0_CLIP_SEGMENT_UPDATE_FIRST();
                    SDF_VU0_CLIPTESTWAIT(firstClip);
                    firstClip &= 0x3F;
                    if (firstClip & secondClip) {
                        break;
                    }
                }
                if (secondClip & (1 << plane)) {
                    if ((plane & 1) == 0) {
                        VU0_POSITIVE_CLIP_PLANE_VF2();
                    } else {
                        VU0_NEGATIVE_CLIP_PLANE_VF2();
                    }
                    VU0_CLIP_SEGMENT_PREP_SECOND();
                    switch (plane) {
                    case 0:
                    case 1:
                        VU0_CLIP_RATIO_X();
                        break;
                    case 2:
                    case 3:
                        VU0_CLIP_RATIO_Y();
                        break;
                    default:
                        VU0_CLIP_RATIO_Z();
                        break;
                    }
                    VU0_CLIP_SEGMENT_UPDATE_SECOND();
                    SDF_VU0_CLIPTESTWAIT(secondClip);
                    secondClip &= 0x3F;
                    if (firstClip & secondClip) {
                        break;
                    }
                }
            }
            if (firstClip & secondClip) {
                continue;
            }
        }
        VU0_PROJECT_COLORED_SEGMENT();
        VU0_STORE_VF8_AT0(cursor);
        VU0_STORE_VF10_AT16(cursor);
        VU0_STORE_VF9_AT32(cursor);
        VU0_STORE_VF11_AT48(cursor);
        cursor += 4;
        emitted += 2;
    }
    sdfGrClosePacketBuff(EE_PHYSICAL(EE_ADDR_S(cursor)));
    quadwordCount = emitted * 2 + 1;
    packet->dmaWords.tag = (quadwordCount & 0xFFFF) | 0x20000000;
    packet->dmaWords.vif = ((u64)(quadwordCount | 0x50000000) << 32) | 0x10000000;
    packet->giftag.parm = emitted | ((u64)(primitiveFlags | 9) << 47) | 0x2000400000008000ULL;
    packet->giftag.regs = 0x41;
    return packet;
}

sdfGrPacketHeader_t *effDebugDrawWireModel(const sdfVector4_t *vertices, const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    SDF_VU0_LOADMATRIX(sdf3DScreenTm);
    return sdfDrawModelWithMatrix((const f32 (*)[4])vertices, colors, vertexCount, primitiveFlags);
}

sdfGrPacketHeader_t *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    MultMatrixRev(sdf3DScreenTm);
    return sdfDrawModelWithMatrix(vertices, colors, vertexCount, primitiveFlags);
}
