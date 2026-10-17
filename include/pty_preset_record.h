#ifndef PTY_PRESET_RECORD_H
#define PTY_PRESET_RECORD_H

#include "common.h"

/* Consumer view of the preset rows. The choice records have title-specific
 * strides, while the four profile groups are at their observed title offsets. */
typedef struct PtyPresetScriptChoice {
    u16 scriptId; /* 0x00 */
    u16 initialValue; /* 0x02 */
#ifdef VERSION_DDS1
    u16 requiredProfiles[4]; /* 0x04 */
#endif
} PtyPresetScriptChoice;

typedef char PtyPresetScriptChoiceSizeCheck[
#ifdef VERSION_DDS1
    (sizeof(PtyPresetScriptChoice) == 0x0C) ? 1 : -1];
#else
    (sizeof(PtyPresetScriptChoice) == 0x04) ? 1 : -1];
#endif
typedef char PtyPresetScriptChoiceScriptIdOffsetCheck[
    ((u32)&((PtyPresetScriptChoice *)0)->scriptId == 0x00) ? 1 : -1];
typedef char PtyPresetScriptChoiceInitialValueOffsetCheck[
    ((u32)&((PtyPresetScriptChoice *)0)->initialValue == 0x02) ? 1 : -1];
#ifdef VERSION_DDS1
typedef char PtyPresetScriptChoiceRequiredProfilesOffsetCheck[
    ((u32)&((PtyPresetScriptChoice *)0)->requiredProfiles == 0x04) ? 1 : -1];
#endif

typedef struct PtyPresetRecord {
    PtyPresetScriptChoice scriptChoices[4]; /* 0x00 */
    u8 profileIds[4][12]; /* 0x30 in DDS1, 0x10 in DDS2 */
#ifdef VERSION_DDS1
    u16 skillSlots[8]; /* 0x60 */
    u16 poolSkills[40]; /* 0x70 */
#else
    u8 opaque40[0x60]; /* 0x40; remaining DDS2 row bytes are unclassified */
#endif
} PtyPresetRecord;

#ifdef VERSION_DDS1
typedef char PtyPresetRecordSizeCheck[(sizeof(PtyPresetRecord) == 0xC0) ? 1 : -1];
typedef char PtyPresetRecordScriptChoicesOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->scriptChoices == 0x00) ? 1 : -1];
typedef char PtyPresetRecordProfileIdsOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->profileIds == 0x30) ? 1 : -1];
typedef char PtyPresetRecordSkillSlotsOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->skillSlots == 0x60) ? 1 : -1];
typedef char PtyPresetRecordPoolSkillsOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->poolSkills == 0x70) ? 1 : -1];
#else
typedef char PtyPresetRecordSizeCheck[(sizeof(PtyPresetRecord) == 0xA0) ? 1 : -1];
typedef char PtyPresetRecordScriptChoicesOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->scriptChoices == 0x00) ? 1 : -1];
typedef char PtyPresetRecordProfileIdsOffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->profileIds == 0x10) ? 1 : -1];
typedef char PtyPresetRecordOpaque40OffsetCheck[
    ((u32)&((PtyPresetRecord *)0)->opaque40 == 0x40) ? 1 : -1];
#endif

#endif /* PTY_PRESET_RECORD_H */
