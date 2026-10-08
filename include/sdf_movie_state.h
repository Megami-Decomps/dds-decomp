#ifndef SDF_MOVIE_STATE_H
#define SDF_MOVIE_STATE_H

/* Values stored in MovObj.state. State 6 is entered by the device-release
 * callback and observed by the owner while it waits for shutdown. */
enum {
    SDF_MOVIE_STATE_INITIAL = 0,
    SDF_MOVIE_STATE_CONTROL_REQUEST = 1,
    SDF_MOVIE_STATE_PAC_HEADER_READ = 2,
    SDF_MOVIE_STATE_PAC_MASK_READ = 3,
    SDF_MOVIE_STATE_DATA_READ = 4,
    SDF_MOVIE_STATE_WAITING_FOR_BUFFER_SPACE = 5,
    SDF_MOVIE_STATE_DEVICE_RELEASE_CALLBACK = 6,
    SDF_MOVIE_STATE_STOP_REQUESTED = 7
};

#endif /* SDF_MOVIE_STATE_H */
