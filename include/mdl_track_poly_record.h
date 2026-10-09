#ifndef MDL_TRACK_POLY_RECORD_H
#define MDL_TRACK_POLY_RECORD_H

#include "common.h"

/* Accessed fields in the viewer's kind-3 track-poly record. This view reads
 * through the four gradient colors at +0x18; it does not define a record
 * allocation extent or stride. */
typedef struct MdlTrackPolyRecordView {
    u8 pad00[8];
    s32 startPositionId;   /* +0x08: first sampled endpoint lookup */
    s32 endPositionId;     /* +0x0C: second sampled endpoint lookup */
    u16 startFrame;        /* +0x10: inclusive update start */
    u16 endFrame;          /* +0x12: inclusive update end */
    u16 sampleInterval;    /* +0x14: update-count modulus */
    u8 historyLength;      /* +0x16: history allocation input */
    u8 drawKind;           /* +0x17: selected by the track's draw data */
    u32 gradientColors[4]; /* +0x18: copied to EffTrackPolyParams */
} MdlTrackPolyRecordView;

#endif /* MDL_TRACK_POLY_RECORD_H */
