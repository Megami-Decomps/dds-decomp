#ifndef DAT_WORLD_FIELD_FLAGS_H
#define DAT_WORLD_FIELD_FLAGS_H

/* Shared world-state bits have context-specific uses across field systems. */
enum DatWorldFieldFlags {
    DAT_WORLD_FIELD_PRIMARY_SCENE_WORK = 1,
    DAT_WORLD_FIELD_SECONDARY_SCENE_WORK = 2,
    DAT_WORLD_FIELD_TRANSITION_WORK = 4,
    DAT_WORLD_FIELD_LIGHT_COMMAND_PENDING = 8,
    /* These aliases document the same bits in the encounter subsystem. */
    DAT_WORLD_FIELD_ENCOUNTER_LEVEL_GATE = DAT_WORLD_FIELD_PRIMARY_SCENE_WORK,
    DAT_WORLD_FIELD_ENCOUNTER_RATE_BOOST = DAT_WORLD_FIELD_SECONDARY_SCENE_WORK
};

#endif /* DAT_WORLD_FIELD_FLAGS_H */
