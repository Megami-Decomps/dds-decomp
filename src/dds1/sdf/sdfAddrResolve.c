#include "sdk_ps2.h"
#include "common.h"
#include "eeregs.h"
#include "snd_command_packet.h"
#include "mnu_voice_resource_path_api.h"
#include "ee_mmi.h"
#include "sdf_heap_api.h"
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
#include "sdf_handler_node.h"
#include "mnu_title_sound.h"
#include "sdf_movie_draw_packet.h"

/* Byte-coded commands advance by word deltas, or relocate a run of following words.
 * Low tag bits select one-, two-, or three-byte deltas; tag 7 encodes a run.
 * tableBytes bounds the encoded byte stream, not the number of relocated words.
 */
void sdfAddressResolver(s32 *words, u32 relocationBase, const u8 *table, s32 tableBytes) {
    const u8 *commandCursor = table;
    s32 value;
    s32 runIndex;

    while (commandCursor - table < tableBytes) {
        value = *commandCursor++;
        if ((value & 1) == 0) {
            value >>= 1;
        } else if ((value & 2) == 0) {
            value = (value | *commandCursor++ << 8) >> 2;
        } else if ((value & 4) == 0) {
            value = (value | commandCursor[0] << 8 | commandCursor[1] << 16) >> 3;
            commandCursor += 2;
        } else {
            value = (value >> 3) + 2;
            for (runIndex = 0; runIndex < value; runIndex++) {
                words++;
                *words += relocationBase;
            }
            continue;
        }
        words += value;
        *words += relocationBase;
    }
}
