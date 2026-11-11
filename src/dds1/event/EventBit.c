#include "sdk_ps2.h"
#include "sdf_request.h"
#include "common.h"
#include "mdl_part_entry.h"
#include "mdl_flag_api.h"
#include "dat_state.h"

typedef enum MdlViewerRecordKind {
    MDL_VIEWER_RECORD_KIND_BILLBOARD_PART = 1,
    MDL_VIEWER_RECORD_KIND_EFFECT_PART = 2,
    MDL_VIEWER_RECORD_KIND_TRACK_POLY = 3,
    MDL_VIEWER_RECORD_KIND_STREAM = 4,
    MDL_VIEWER_RECORD_KIND_OBJECT_PART = 5
} MdlViewerRecordKind;

/* Five-character device prefix copied in front of each work-file path. */
typedef struct {
    char text[5];
} DebugWorkPrefix;

extern char D_003BBB80[], D_003BBB88[];

extern char D_003BBCC8[]; /* "fovy=" */

extern char D_003BBCD8[]; /* "fog=" */

extern void evtDisableMoonCalc(void);

extern void evtDisableMoonDraw(void);

extern void evtSetAgeOfMoon(s32 phase);

void evtInitGlobalWork(void) {
    evtBitAllOff();
    evtSetAgeOfMoon(0);
    evtDisableMoonDraw();
    evtDisableMoonCalc();
}

void evtBitAllOff(void) {
    s32 i = 0x7f;
    u32 *word = DDS3_GBWK->EventBit.words;

    do {
        i -= 1;
        *word = 0;
        word += 1;
    } while (i >= 0);
}

void evtBitAllOff_FieldDebug(void) {
    s32 i = 0;
    do {
        evtBitOff(i++);
    } while (i < 0xC00);
    i = 0xD00;
    do {
        evtBitOff(i++);
    } while (i < 0x1000);
}

/* Signed flag indices need a bias before arithmetic right shift divides by 32. */
void evtBitOn(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->EventBit.words[adjustedFlag >> 5] |= 1 << flag;
}

void evtBitOff(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    DDS3_GBWK->EventBit.words[adjustedFlag >> 5] &= ~(1 << flag);
}

EvtBitTestResult evtBitCheck(s32 flag) {
    s32 adjustedFlag = (flag < 0) ? flag + 0x1f : flag;
    return (((s32)DDS3_GBWK->EventBit.words[adjustedFlag >> 5] >> flag) & 1);
}
