#ifndef SDF_SIF_COMMAND_H
#define SDF_SIF_COMMAND_H

#include "common.h"

/* Complete command header passed to sdfPktInit and the SIF formatter. */
typedef struct SifCommand {
    s32 source;   /* 0x00 */
    s32 end;      /* 0x04 */
    s32 argument; /* 0x08 */
    u32 command;  /* 0x0C */
} SifCommand;

typedef char SifCommandSizeCheck[sizeof(SifCommand) == 0x10 ? 1 : -1];

void sdfPktSetCmd(SifCommand *packet, s32 index);
void sdfPktInit(SifCommand *packet, s32 source, s32 end, s32 argument, s32 index);
void *sdfFormatSifPacket(void *packet, const char *format, ...);
void *sdfCreateFormattedSifCommand(s32 source, s32 end, s32 argument, s32 index,
                                  const char *format, ...);

#endif
