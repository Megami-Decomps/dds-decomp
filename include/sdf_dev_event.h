#ifndef SDF_DEV_EVENT_H
#define SDF_DEV_EVENT_H

/* Completion values delivered to device callbacks, separate from requests. */
#define SDF_DEV_EVENT_INACTIVE 0
#define SDF_DEV_EVENT_OPENED 2
#define SDF_DEV_EVENT_SEEK_REPLY 3
#define SDF_DEV_EVENT_SIZE_REPLY 4
#define SDF_DEV_EVENT_READ_REPLY 5
#define SDF_DEV_EVENT_WRITE_REPLY 6
#define SDF_DEV_EVENT_CLOSED 7
#define SDF_DEV_EVENT_BUFFER_REPLY 8

#endif /* SDF_DEV_EVENT_H */
