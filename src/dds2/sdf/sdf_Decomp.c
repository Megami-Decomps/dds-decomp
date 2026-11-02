#include "sdk_ps2.h"
#include "sdf_line_pair_packet_api.h"
#include "common.h"
#include "sdf_vu0.h"
#include "sdf_vu_matrix.h"
#include "sdf_thread_workspace_api.h"
#include "sdf_timer.h"
#include "sdf_chip.h"
#include "sdf.h"
#include "sdf_semaphore_api.h"
#include "pcp_vu0.h"
#include "sdf_draw.h"
#include "sdf_projection.h"
#include "ee_mmi.h"
#include "sdf_texture_file.h"
#include "sdf_named_resource_request_api.h"
#include "sce_io.h"
#include "sdf_dev_state.h"
#include "sdf_dev_event.h"
#include "sdf_pac_state.h"
#include "sdf_pac_decoder.h"
#include "sdf_pac_work.h"
#include "sdf_key_pool.h"
#include "sdf_packet_alloc.h"
#include "sdf_stream_state.h"

extern u8 sdf3DScreenTm[];

/* PAC command high bits choose the decoder operation; low five bits encode length.
 * Values at or above 0xE0 are reserved except for the 0xFF end marker. */
enum {
    SDF_DECODE_COMMAND_CLASS_MASK = 0xE0,
    SDF_DECODE_COMMAND_LENGTH_MASK = 0x1F,
    SDF_DECODE_COMMAND_END_MARKER = 0xFF,
    SDF_DECODE_COMMAND_RESERVED_MIN = 0xE0,
    SDF_DECODE_COMMAND_COPY_LITERAL = 0x00,
    SDF_DECODE_COMMAND_ZERO_FILL = 0x20,
    SDF_DECODE_COMMAND_REPEAT_BYTE = 0x40,
    SDF_DECODE_COMMAND_COPY_SHORT_BACKREFERENCE = 0x60,
    SDF_DECODE_COMMAND_COPY_LONG_BACKREFERENCE = 0x80,
    SDF_DECODE_COMMAND_EXPAND_BYTES = 0xA0
};

extern s32 D_00439220;

extern const u8 *D_0047BC80[16];

/* Incrementally decode the PAC byte stream, preserving partial commands when
 * the caller's input chunk ends. */
SdfPacDecoderResult sdfDecomp(SdfPacDecoder *decoder, const u8 *input, s32 inputBytes) {
    s32 count;
    s32 offset;
    u8 *output;
    u8 *shortBackrefSource;
    u32 sourceAddress;

    if (decoder->state == SDF_DECODE_COMPLETE) {
        return 1;
    }

    D_00439220 = 0;
    output = decoder->output;
    if (inputBytes > 0) {
        do {
            switch (decoder->state) {
                case SDF_DECODE_READ_COMMAND: {
                    u8 command;
                    s32 commandBytes;
                    s32 historyIndex = D_00439220;

                    inputBytes--;
                    D_0047BC80[historyIndex] = input;
                    D_00439220 = (historyIndex + 1) & 0xF;
                    command = *input++;
                    if (command >= SDF_DECODE_COMMAND_RESERVED_MIN) {
                        if (command == SDF_DECODE_COMMAND_END_MARKER) {
                            decoder->output = output;
                            decoder->input = input;
                            decoder->remainingInputBytes = inputBytes;
                            decoder->state = SDF_DECODE_COMPLETE;
                            return SDF_PAC_DECODE_COMPLETE;
                        }
                    } else {
                        decoder->command = command & SDF_DECODE_COMMAND_CLASS_MASK;
                        commandBytes = command & SDF_DECODE_COMMAND_LENGTH_MASK;
                        if (commandBytes == 0) {
                            decoder->state = SDF_DECODE_READ_COUNT_LOW;
                        } else {
                            decoder->pendingBytes = commandBytes;
                            decoder->state = SDF_DECODE_DISPATCH;
                        }
                    }
                    break;
                }
                case SDF_DECODE_READ_COUNT_LOW:
                    decoder->pendingBytes = *input++;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COUNT_HIGH;
                    break;
                case SDF_DECODE_READ_COUNT_HIGH:
                    decoder->pendingBytes |= *input++ << 8;
                    inputBytes--;
                    decoder->state = SDF_DECODE_DISPATCH;
                    break;
                case SDF_DECODE_DISPATCH:
                    switch (decoder->command) {
                    case SDF_DECODE_COMMAND_COPY_LITERAL:
                        decoder->state = SDF_DECODE_COPY_LITERAL;
                        break;
                    case SDF_DECODE_COMMAND_ZERO_FILL:
                        count = decoder->pendingBytes;
                        memset(output, 0, count);
                        output += count;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        break;
                    case SDF_DECODE_COMMAND_REPEAT_BYTE:
                        decoder->state = SDF_DECODE_FILL_BYTE;
                        break;
                    case SDF_DECODE_COMMAND_COPY_SHORT_BACKREFERENCE:
                        decoder->state = SDF_DECODE_COPY_SHORT_BACKREF;
                        break;
                    case SDF_DECODE_COMMAND_COPY_LONG_BACKREFERENCE:
                        decoder->state = SDF_DECODE_READ_BACKREF_LOW;
                        break;
                    case SDF_DECODE_COMMAND_EXPAND_BYTES:
                        decoder->state = SDF_DECODE_EXPAND_BYTES;
                        break;
                    }
                    break;
                case SDF_DECODE_COPY_LITERAL:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                    }
                    memcpy(output, input, count);
                    output += count;
                    input += count;
                    inputBytes -= count;
                    decoder->pendingBytes -= count;
                    if (decoder->pendingBytes == 0) {
                        decoder->state = SDF_DECODE_READ_COMMAND;
                    }
                    break;
                case SDF_DECODE_FILL_BYTE:
                    count = decoder->pendingBytes;
                    memset(output, *input++, count);
                    output += count;
                    inputBytes--;
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_COPY_SHORT_BACKREF:
                    shortBackrefSource = output - *input++;
                    count = decoder->pendingBytes;
                    inputBytes--;
                    do {
                        *output++ = *shortBackrefSource++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                case SDF_DECODE_READ_BACKREF_LOW:
                    decoder->backrefLow = *input++;
                    inputBytes--;
                    if (inputBytes == 0) {
                        decoder->state = SDF_DECODE_READ_BACKREF_HIGH;
                        break;
                    }
                    /* fallthrough */
                case SDF_DECODE_READ_BACKREF_HIGH: {
                    s32 combinedOffset;

                    offset = *input++ << 8;
                    sourceAddress = decoder->backrefLow;
                    inputBytes--;
                    combinedOffset = sourceAddress | offset;
                    sourceAddress = (u32)(output - combinedOffset);
                    count = decoder->pendingBytes;
                    do {
                        *output++ = *(u8 *)sourceAddress++;
                    } while (--count != 0);
                    decoder->state = SDF_DECODE_READ_COMMAND;
                    break;
                }
                case SDF_DECODE_EXPAND_BYTES:
                    count = decoder->pendingBytes;
                    if (inputBytes < count) {
                        count = inputBytes;
                        inputBytes = 0;
                        decoder->pendingBytes -= count;
                    } else {
                        decoder->pendingBytes = 0;
                        decoder->state = SDF_DECODE_READ_COMMAND;
                        inputBytes -= count;
                    }
                    do {
                        *output++ = *input++;
                        *output++ = 0;
                    } while (--count != 0);
                    break;
            }
        } while (inputBytes > 0);
    }
    decoder->input = input;
    decoder->remainingInputBytes = inputBytes;
    decoder->output = output;
    return SDF_PAC_DECODE_INCOMPLETE;
}

void sdfDecompInit(SdfPacDecoder *decoder, u8 *output) {
    decoder->output = output;
    decoder->state = SDF_DECODE_READ_COMMAND;
}

/* vu0 routine: clip homogeneous line pairs and build a coloured GS packet. */
SdfPacket *sdfDrawModelWithMatrix(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    SdfPacket *packet;
    f32 (*viewport)[4];
    u128 *cursor;
    s32 i;
    s32 emitted;
    s32 plane;
    u32 clipFlags;
    u32 firstClip;
    u32 secondClip;
    u32 quadwordCount;

    packet = (SdfPacket *)sdfGrOpenPacketBuff();
    cursor = (u128 *)(packet + 1);
    viewport = (f32 (*)[4])&sdf3DScreenParam.camera.halfWidth;
    VU0_LOAD_VF_AT(vf14, 0x0, viewport);
    VU0_LOAD_VF_AT(vf15, 0x10, viewport);
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
        VU0_PROJECT_COLORED_SEGMENT(vf10, vf11, vf8, vf9, vf14, vf15);
        VU0_STORE_VF_AT_UNCLOBBERED(vf8, 0x0, cursor);
        VU0_STORE_VF_AT_UNCLOBBERED(vf10, 0x10, cursor);
        VU0_STORE_VF_AT_UNCLOBBERED(vf9, 0x20, cursor);
        VU0_STORE_VF_AT_UNCLOBBERED(vf11, 0x30, cursor);
        cursor += 4;
        emitted += 2;
    }
    sdfGrClosePacketBuff((s32)cursor & 0x0FFFFFFF);
    quadwordCount = emitted * 2 + 1;
    packet->unk0 = (quadwordCount & 0xFFFF) | 0x20000000;
    packet->unk8 = ((u64)(quadwordCount | 0x50000000) << 32) | 0x10000000;
    packet->unk10 = emitted | ((u64)(primitiveFlags | 9) << 47) | 0x2000400000008000ULL;
    packet->unk18 = 0x41;
    return packet;
}

/* vu0 routine: render line pairs using the cached composed matrix. */
SdfPacket *effDebugDrawWireModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    VU0_LOAD_MATRIX(sdf3DScreenTm);
    return sdfDrawModelWithMatrix(vertices, colors, vertexCount, primitiveFlags);
}

/* vu0 routine: compose the current matrix with the cached transform, then render line pairs. */
SdfPacket *sdfDrawRotatedModel(const f32 vertices[][4], const u32 *colors, s32 vertexCount, u32 primitiveFlags) {
    sdfPostmultiplyVuMatrixFromMemory((const struct Matrix4 *)sdf3DScreenTm);
    return sdfDrawModelWithMatrix(vertices, colors, vertexCount, primitiveFlags);
}
