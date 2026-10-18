#ifndef BTL_SELECTION_VIEW_H
#define BTL_SELECTION_VIEW_H

#include "common.h"

/* The battle selection view in the linked selection task's buffer: the selected row, row fades and positions. */
typedef struct SelView { s8 selectedRow; u8 pad01; s16 rowFade[4]; u8 pad0A[2]; s32 pos[4][2]; } SelView;

#endif
