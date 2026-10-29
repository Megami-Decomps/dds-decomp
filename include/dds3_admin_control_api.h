#ifndef DDS3_ADMIN_CONTROL_API_H
#define DDS3_ADMIN_CONTROL_API_H

#include "common.h"

/* Administration-task control word; the history-slot flag is separate. */
#define DDS3_ADMIN_CONTROL_REQUEST_PENDING 0x00000001
#define DDS3_ADMIN_CONTROL_EXIT_REQUESTED 0x00000002
#define DDS3_ADMIN_CONTROL_MARK_HISTORY_SLOT_FOR_RESTORE 0x00000004
#define DDS3_ADMIN_CONTROL_KEEP_CURRENT_HISTORY_SLOT 0x00000008
#define DDS3_ADMIN_CONTROL_RESTORE_PREVIOUS_MODE 0x00010000

/* The accessor returns the byte-backed flag field zero-extended as a word. */
typedef u32 Dds3AdminHistoryFlags;

#define DDS3_ADMIN_HISTORY_FLAG_RESTORE_PREVIOUS_MODE 0x01

/* Read the active mode; -1 means no administration mode is active. */
s8 dds3AdminiGetActiveSequence(void);

/* Read the signed mode stored in the previous history slot. */
s8 dds3AdminiGetPrevSequence(void);

/* Bit 0 marks this slot to restore its preceding mode when the active mode exits. */
Dds3AdminHistoryFlags dds3AdminiGetPrevHistoryFlag(void);

/* Set bit 1 in the active administration task's flags; that task must exist. */
void dds3AdminiExitSequence(void);

#endif /* DDS3_ADMIN_CONTROL_API_H */
