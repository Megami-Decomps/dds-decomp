#include "sdk_ps2.h"
#include "scr.h"
#include "common.h"
#include "dds3_task_create.h"
#include "dds3_task_state_query.h"
#include "itf.h"

extern frFontHandle_t * frDestroyFontRequest(frFontHandle_t *glyph);
extern s32 test_font_Process(dds3ProcessID_t *task);
extern void test_font_Shutdown(dds3ProcessID_t *task);
extern frFontHandle_t *frTestFontHandle;
extern dds3ProcessID_t *itfFontTestScriptTask;

/* Start the background draw task and host-file font-test process. */
void frTestFontInit(void) {
    nbLoadDeadMoonTex();
    dds3StartProcess("test_font", 0x2B06, 0, 0, test_font_Process, test_font_Shutdown, 0);
    itfFontTestScriptTask = scrStartScriptByName(0x258, "host0:../../../dds3data/font/test.bf", 0);
}

/* Advance/retain the test glyph and release its background texture reference. */
void test_font_Shutdown(dds3ProcessID_t *task) {
    frDestroyFontRequest(frTestFontHandle);
    itfReleaseBackgroundSpriteTexture();
}

/* Draw first, then report all bits set only for a task awaiting destruction. */
s32 test_font_Process(dds3ProcessID_t *task) {
    dds3TaskState_t taskState;
    u32 readyMask;

    itfDrawBackgroundSprite();
    taskState = dds3GetProcessStat(itfFontTestScriptTask);
    readyMask = 0xffffffff;
    if (taskState != DDS3_TASK_DESTROY_PENDING) {
        readyMask = 0;
    }
    return readyMask;
}

INCLUDE_SDATA(const s32, "interface/frTestFont", frTestFontHandle);

INCLUDE_SDATA(const s32, "interface/frTestFont", itfFontTestScriptTask);

