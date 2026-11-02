#include "sdk_ps2.h"
#include "common.h"
#include "sdf_handler_node.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "sdf_resource.h"
#include "sdf_packed_resource.h"
#include "sdf_pac_reloc.h"
#include "sdf.h"
#include "sdf_gs_mem_delayed_free_api.h"
#include "sdf_semaphore_api.h"
#include "sdf_movie_stream.h"
#include "sdf_packet_alloc.h"
#include "sdf_packet_list.h"
#include "sdf_gs_packet.h"
#include "sdf_texture_lists.h"
#include "sdf_draw.h"
#include "sdf_resource_list_item.h"
#include "ee_mmi.h"
#include "sdf_stream_read.h"
#include "sdf_stream_read_api.h"
#include "sdf_dev_event.h"
#include "sdf_dev_state.h"
#include "mdl_object_stream.h"
#include "sdf_texture_offset_list.h"
#include "sdf_texture_file.h"
#include "sdf_texture_queue.h"
#include "sdf_stream_input_dma.h"
#include "sdf_sound_rpc_records.h"
#include "snd_midi_state_views.h"
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"

/* Read a named file through the dev RPC into a freshly allocated block; returns the block's handle.
 * outData receives the block address, outSize the file size; without outData the payload is unlocked. */
sdfMemHandle_t *sdfDevLoadFile(const char *name, u32 *outData, u32 *outSize, s32 extra) {
    sdfFileCmdHandle_t *state = sdfFileOpen(name);
    s32 size = sdfFileLength(state);
    sdfMemHandle_t *handle = sdfMemAllocDirect(size + extra);
    void *data = sdfMemLock(handle);

    sdfFileRead(state, data, size);
    sdfFileClose(state);
    if (outData != NULL) {
        *outData = (u32)data;
    } else {
        sdfMemUnlock(handle);
    }
    if (outSize != NULL) {
        *outSize = size;
    }
    return handle;
}

sdfMemHandle_t *sdfDevLoadFileEx(const char *name, u32 *outAddress, u32 *outSize) {
    return sdfDevLoadFile(name, outAddress, outSize, 0);
}

sdfTexHandle_t *sdfLoadNamedResourceAndReleaseLookupHandle(const char *name) {
    sdfMemHandle_t *buffer;
    sdfTexHandle_t *result;
    u32 info[4];

    buffer = sdfDevLoadFileEx(name, info, 0);
    result = sdfTexSet((SdfTextureFileHeader *)(info[0]));
    sdfMemFreeDirect(buffer);
    return result;
}

extern DevRequest *sdfTexListNew(s32 capacity);

DevRequest *sdfTexSetTB(const SdfTextureOffsetListHeader *resource) {
    s32 i = 0;
    s32 count = resource->textureCount;
    DevRequest *handle = sdfTexListNew(count);
    const SdfTextureOffsetListEntry *entry;
    if (count != i) {
        entry = (const SdfTextureOffsetListEntry *)((const u8 *)resource + sizeof(*resource));
        do {
            i++;
            sdfTexListAdd(handle, sdfTexSet((SdfTextureFileHeader *)(
                (void *)((const u8 *)resource + entry->textureHeaderOffset))));
            entry++;
        } while (i != count);
    }
    return handle;
}

DevRequest *sndLoadNamedOffsetResourceList(const char *name) {
    sdfMemHandle_t *buffer;
    DevRequest *result;
    u32 info[4];

    buffer = sdfDevLoadFileEx(name, info, 0);
    result = sdfTexSetTB((const SdfTextureOffsetListHeader *)(u32)info[0]);
    sdfMemFreeDirect(buffer);
    return result;
}

/* Relocate words in the payload and return its address. */
u8 *sdfFixupModelBinary(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfAddressResolver((s32 *)payload, (u32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
sdfMemHandle_t *sdfLoadPackedResourceWithRelocatedPayload(const char *name, u8 **outPayload) {
    u32 info[4];
    sdfMemHandle_t *buffer = sdfDevLoadFileEx(name, info, 0);
    *outPayload = sdfFixupModelBinary((SdfPackedRelocationHeader *)(u32)info[0]);
    return buffer;
}

/* This entry point uses the same payload-relative relocation table. */
u8 *sdfFixupMotionBinary(SdfPackedRelocationHeader *resource) {
    u8 *payload = (u8 *)resource + SDF_PACKED_RESOURCE_HEADER_BYTES;

    sdfAddressResolver((s32 *)payload, (u32)payload,
                                   payload + resource->relocationOffset, resource->relocationByteCount);
    return payload;
}

/* Return the retained allocation handle and publish the relocated payload address. */
sdfMemHandle_t *sdfReadPackedResourceAndRelocateHeader(const char *name, u8 **outPayload) {
    u32 info[4];
    sdfMemHandle_t *buffer = sdfDevLoadFileEx(name, info, 0);
    *outPayload = sdfFixupMotionBinary((SdfPackedRelocationHeader *)(u32)info[0]);
    return buffer;
}
