#ifndef FILE_REQUEST_ALLOCATION_MODE_H
#define FILE_REQUEST_ALLOCATION_MODE_H

/* fileLoadEx stores this selector in allocationMode: zero uses retryable
 * direct allocation, while every nonzero value selects tail allocation. */
enum {
    FILE_REQUEST_ALLOCATION_MODE_DIRECT = 0,
    FILE_REQUEST_ALLOCATION_MODE_FROM_TAIL = 1
};

#endif
