#ifndef DDS3_ADMIN_H
#define DDS3_ADMIN_H

#include "common.h"

#include "kernel.h"

/* Native DDS administration rows. Shared handler identities use HD names
 * with DDS values; Nocturne's row numbers differ. Battle keeps its DDS name:
 * its CheckNewBattleTest callback alone does not establish the whole row. */
typedef enum Dds3AdminMode {
    DDS3_ADMINI_LIST_DEBUG = 0,
    DDS3_ADMINI_LIST_BOOTMC = 1,
    DDS3_ADMINI_LIST_TITLE = 2,
    DDS3_ADMINI_LIST_LOAD = 3,
    DDS3_ADMINI_LIST_ENDSAVE = 4,
    DDS3_ADMINI_LIST_FIELD = 5,
    DDS3_ADMINI_LIST_EVENT = 6,
    DDS3_ADMINI_LIST_CAMP = 7,
    DDS3_ADMINI_LIST_RECOVER = 8,
    DDS3_ADMINI_LIST_SAVE = 9,
    DDS3_ADMINI_LIST_RESULT2 = 10,
    DDS3_ADMINI_LIST_STAFF = 12,
    DDS3_ADMINI_LIST_CONFIG = 13,
    DDS3_ADMIN_MODE_BATTLE = 14,
    DDS3_ADMINI_LIST_SAVE_TEST = 21,
    DDS3_ADMINI_LIST_SOUND_TEST = 23,
    DDS3_ADMINI_LIST_BILLBOARD_TEST = 24,
    DDS3_ADMINI_LIST_FONT_TEST = 25
} Dds3AdminMode;

#endif /* DDS3_ADMIN_H */
