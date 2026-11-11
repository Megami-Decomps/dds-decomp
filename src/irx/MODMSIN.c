#include "MODMSIN.h"

extern u8 msinLibraryTable[];

/* MODMSIN.IRX, Sony's MIDI message input module: the queue
 * sceMSIn_PutMsg writes into, which the synthesiser drains.
 * See docs/irx-modules.md.
 *
 * The chunks still in assembly are the import stubs and the import and
 * export tables between them. */

/* FUNC modmsin_e10 */
int modmsin_e10(void)
{
    s32 state;
    s32 res;

    intrman_CpuSuspendIntr(&state);
    res = loadcore_RegisterLibraryEntries(msinLibraryTable);
    intrman_CpuResumeIntr(state);
    return res != 0;
}
/* ENDF modmsin_e10 */
ALIGN_TEXT(4);
/* FUNC modmsin_sceMSIn_Init */
int modmsin_sceMSIn_Init(sceMSInWork_t *work)
{
    sceMSInTbl_t *tbl;
    sceMSInEntry_t *e;
    int n;
    int i;

    if (work != NULL) {
        tbl = work->tbl;
        if (work->nTblWord >= 2 && tbl != NULL) {
            n = tbl->nBufWord;
            e = tbl->buf;
            if (n != 0 && e != NULL) {
                for (i = 0; i < n; i++) {
                    if (e[i].buf == NULL) {
                        return -1;
                    }
                    if (e[i].buf->size < 8) {
                        return -1;
                    }
                }
                return 0;
            }
        }
    }
    return -1;
}
/* ENDF modmsin_sceMSIn_Init */
/* FUNC modmsin_e5 */
int modmsin_e5(void)
{
    return 0;
}
/* ENDF modmsin_e5 */
/* FUNC modmsin_e6 */
int modmsin_e6(void)
{
    return 0;
}
/* ENDF modmsin_e6 */
/* FUNC sceMsinBufferAppend */
int sceMsinBufferAppend(sceMSInWork_t *work, u32 port, const u8 *msg, int len)
{
    u32 *seg;
    sceMSInBuf_t *buf;
    u8 *dst;
    u32 cur;

    if (work == NULL) {
        return -1;
    }
    /* Count and table are reached off one base, so the pair at
     * sceMSInTbl_t+8 is a {count, pointer} segment of its own. */
    seg = &work->tbl->nBufWord;
    if (port >= seg[0]) {
        return -1;
    }
    buf = ((sceMSInEntry_t *)seg[1])[port].buf;
    cur = buf->len;
    if (buf->size < cur + len + 8) {
        return -1;
    }
    dst = buf->data + cur;
    buf->len = cur + len;
    while (--len != -1) {
        *dst++ = *msg++;
    }
    return 0;
}
/* ENDF sceMsinBufferAppend */
/* FUNC modmsin_e7 */
int modmsin_e7(sceMSInWork_t *work, u32 port, u32 msg)
{
    int len;

    /* 0x80 takes two bytes here, as retail has it. */
    switch (msg & 0xF0) {
    case 0x80:
        len = 2;
        break;
    case 0x90:
        len = 3;
        break;
    case 0xA0:
        len = 3;
        break;
    case 0xB0:
        len = 3;
        break;
    case 0xC0:
        len = 2;
        break;
    case 0xD0:
        len = 2;
        break;
    case 0xE0:
        len = 3;
        break;
    default:
        return -1;
    }
    return sceMsinBufferAppend(work, port, (const u8 *)&msg, len);
}
/* ENDF modmsin_e7 */
/* FUNC modmsin_e8 */
int modmsin_e8(sceMSInWork_t *work, u32 port, const u8 *msg, u32 maxLen)
{
    const u8 *p;
    u32 len;

    p = msg;
    if (*p++ != 0xF0) {
        return -1;
    }
    for (len = 2; len <= maxLen; len++) {
        if (*p++ == 0xF7) {
            return sceMsinBufferAppend(work, port, msg, len);
        }
    }
    return -1;
}
/* ENDF modmsin_e8 */
/* FUNC modmsin_e9 */
int modmsin_e9(sceMSInWork_t *work, u32 port, const u8 *msg)
{
    int len;

    switch (msg[0]) {
    case 0xF9:
        len = 5;
        break;
    case 0xFD:
        len = 7;
        break;
    default:
        return -1;
    }
    return sceMsinBufferAppend(work, port, msg, len);
}
/* ENDF modmsin_e9 */
INCLUDE_ASM("MODMSIN", msinLibraryTable);
/* FUNC modmsin_e3 */
void modmsin_e3(void)
{
}
/* ENDF modmsin_e3 */
ALIGN_TEXT(4);
IRX_IMPORT_TABLE(intrman, 1, 2);
IRX_IMPORT(intrman, CpuSuspendIntr, 0x11);
IRX_IMPORT(intrman, CpuResumeIntr, 0x12);
IRX_IMPORT_TABLE_END;
IRX_IMPORT_TABLE(loadcore, 1, 3);
IRX_IMPORT(loadcore, RegisterLibraryEntries, 0x6);
IRX_IMPORT_TABLE_END;
