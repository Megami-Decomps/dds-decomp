#ifndef EFF_UPDATE_FLAGS_H
#define EFF_UPDATE_FLAGS_H

/* effModelUpdateControlFlags bits with established update-control behavior. */
#define EFF_MODEL_UPDATE_DEFER_FLOOR_REFRESH 0x1
/* Also suppresses the associated effect-record acquisition gates. */
#define EFF_MODEL_UPDATE_PAUSE_EFFECT_FRAME_ADVANCE 0x2

#endif
