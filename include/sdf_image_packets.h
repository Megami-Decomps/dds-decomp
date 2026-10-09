#ifndef SDF_IMAGE_PACKETS_H
#define SDF_IMAGE_PACKETS_H

#include "sdf.h"

/* Resource upload payload and its terminal primitive-reset tag (0xF0). */
typedef struct SdfResourcePacket {
    u64 header[14];
    SdfPacket transfer[2];
    s32 quadwordCount;
    u32 unkB4;
    u32 unkB8;
    u32 unkBC;
    u64 tail[6];
} SdfResourcePacket;

typedef char SdfResourcePacket_size_must_be_0xF0[
    (sizeof(SdfResourcePacket) == 0xF0) ? 1 : -1];
typedef char SdfResourcePacket_transfer_offset_must_be_0x70[
    ((u32)&((SdfResourcePacket *)0)->transfer == 0x70) ? 1 : -1];
typedef char SdfResourcePacket_count_offset_must_be_0xB0[
    ((u32)&((SdfResourcePacket *)0)->quadwordCount == 0xB0) ? 1 : -1];
typedef char SdfResourcePacket_tail_offset_must_be_0xC0[
    ((u32)&((SdfResourcePacket *)0)->tail == 0xC0) ? 1 : -1];

/* Host-to-local image transfer packet followed by its flush terminal tag (0xB0). */
typedef struct SdfDescriptorPacket {
    u64 header[4];
    SdfPacket transfer[2];
    u64 imageReferenceTag;
    u64 imageReferencePad;
    u64 imageGifTag0;
    u64 imageGifTag1;
    u64 flushTag0;
    u64 flushTag1;
    u64 flushGifTag;
    u64 flushRegister;
    u64 zero;
    u64 finishRegister;
} SdfDescriptorPacket;

typedef char SdfDescriptorPacket_size_must_be_0xB0[
    (sizeof(SdfDescriptorPacket) == 0xB0) ? 1 : -1];
typedef char SdfDescriptorPacket_transfer_offset_must_be_0x20[
    ((u32)&((SdfDescriptorPacket *)0)->transfer == 0x20) ? 1 : -1];
typedef char SdfDescriptorPacket_image_reference_offset_must_be_0x60[
    ((u32)&((SdfDescriptorPacket *)0)->imageReferenceTag == 0x60) ? 1 : -1];
typedef char SdfDescriptorPacket_flush_tag_offset_must_be_0x80[
    ((u32)&((SdfDescriptorPacket *)0)->flushTag0 == 0x80) ? 1 : -1];

/* Metadata node plus the extended local-to-local image-copy payload (0x70). */
typedef struct SdfGraphCopyPacket {
    SdfNode node;
    SdfPacket drawHeader;
    SdfPacket transfer[2];
} SdfGraphCopyPacket;

typedef char SdfGraphCopyPacket_size_must_be_0x70[
    (sizeof(SdfGraphCopyPacket) == 0x70) ? 1 : -1];
typedef char SdfGraphCopyPacket_draw_header_offset_must_be_0x10[
    ((u32)&((SdfGraphCopyPacket *)0)->drawHeader == 0x10) ? 1 : -1];
typedef char SdfGraphCopyPacket_transfer_offset_must_be_0x30[
    ((u32)&((SdfGraphCopyPacket *)0)->transfer == 0x30) ? 1 : -1];

/* Metadata node followed by the resource-transfer payload (0x100). */
typedef struct SdfPatchableResourcePacket {
    SdfNode node;
    SdfResourcePacket resourcePacket;
} SdfPatchableResourcePacket;

typedef char SdfPatchableResourcePacket_size_must_be_0x100[
    (sizeof(SdfPatchableResourcePacket) == 0x100) ? 1 : -1];
typedef char SdfPatchableResourcePacket_payload_offset_must_be_0x10[
    ((u32)&((SdfPatchableResourcePacket *)0)->resourcePacket == 0x10) ? 1 : -1];
typedef char SdfPatchableResourcePacket_transfer_offset_must_be_0x80[
    ((u32)&((SdfPatchableResourcePacket *)0)->resourcePacket.transfer == 0x80) ? 1 : -1];

#endif /* SDF_IMAGE_PACKETS_H */
