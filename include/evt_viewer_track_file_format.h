#ifndef EVT_VIEWER_TRACK_FILE_FORMAT_H
#define EVT_VIEWER_TRACK_FILE_FORMAT_H

#include "common.h"

/* File formats used to split event-viewer tracks during serialization. */
typedef s32 EvtViewerTrackFileFormat;
enum {
    EVT_VIEWER_TRACK_FILE_FORMAT_PM2 = 2,
    EVT_VIEWER_TRACK_FILE_FORMAT_PM3 = 3
};

#endif /* EVT_VIEWER_TRACK_FILE_FORMAT_H */
