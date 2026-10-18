#ifndef EFF_EDITOR_MENU_H
#define EFF_EDITOR_MENU_H

#include "common.h"

/* A code address stored in a menu row; the menu dispatcher decides how to call it. */
typedef union EffEditorCallback {
    u32 word;
    s32 (*signedPoll)(void);
    u32 (*unsignedPoll)(void);
    void (*action)(void);
    u32 (*wordTransform)(u32);
} EffEditorCallback;

struct EffQueuedFileObject;

/* The row's opcode selects one of these complete 16-byte payload roles. */
typedef union EffEditorPayload {
    struct {
        void *target;
        s32 lower;
        s32 upper;
        f32 step;
    } numeric;
    struct {
        struct EffQueuedFileObject *editor;
        u32 attributes;
        s32 control;
        f32 step;
    } submenu;
    struct {
        EffEditorCallback poll;
        EffEditorCallback action;
        s32 control;
        f32 step;
    } callbacks;
    struct {
        EffEditorCallback action;
        u32 attributes;
        s32 control;
        f32 step;
    } action;
} EffEditorPayload;

/* A menu row of the effect editor and the battle camera presets: text, opcode and a
 * 16-byte payload. */
typedef struct EffEditorRow {
    const char *text;
    u8 kind;
    u8 reserved05[3];
    EffEditorPayload payload;
} EffEditorRow;

/* A menu object (0x38 bytes) of the effect editor and the battle camera presets: the
 * title, placement, cursor, flag bits and rows of one page. */
typedef struct EffQueuedFileObject {
    const char *title; /* 0x00 */
    s32 originX; /* 0x04: copied to the preset selector at 002B74AC. */
    s32 originY; /* 0x08 */
    s32 cursor; /* 0x0C */
    u32 flags; /* 0x10 */
    EffEditorRow *rows; /* 0x14 */
    s32 count; /* 0x18 */
    u32 unknown1C[6];
    u8 *linkedState; /* 0x34: the menu to switch to */
} EffQueuedFileObject;

typedef char EffEditorRowSizeCheck[(sizeof(EffEditorRow) == 0x18) ? 1 : -1];
typedef char EffQueuedFileObjectSizeCheck[(sizeof(EffQueuedFileObject) == 0x38) ? 1 : -1];

#endif
