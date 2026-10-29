#ifndef EVT_RELOAD_VIEWER_API_H
#define EVT_RELOAD_VIEWER_API_H

#include "evt_viewer.h"
#include "evt_polygon_movie_resource_mode.h"

typedef s32 EvtViewerReloadResult;
enum {
    EVT_VIEWER_RELOAD_FAILED = 0,
    EVT_VIEWER_RELOAD_SUCCEEDED = 1
};

/* Returns SUCCEEDED after installing the new viewer. FAILED means required
 * files were unavailable or movie-work creation failed. Failure may follow
 * resource cleanup and does not promise rollback to the previous viewer. */
EvtViewerReloadResult evtReloadEventViewer(
    EvtPolygonMovieResourceMode resourceMode, evtViewerInfo_t *runtime);

#endif
