#ifndef FILE_REQUEST_CALLBACK_H
#define FILE_REQUEST_CALLBACK_H

struct FileRequest;

/* Completion receives the queued request and its caller-owned context. */
typedef void (*FileRequestCallback)(struct FileRequest *request, void *arg);

#endif /* FILE_REQUEST_CALLBACK_H */
