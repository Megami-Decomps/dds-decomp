#include "sdk_ps2.h"
#include "common.h"
#include "snd_sequence_volume_pan.h"
#include "sdf_trig.h"
#include "eff_random.h"
#include "eff_resource_slots.h"
#include "sdf_quaternion.h"
#include "mnu_list.h"

extern MovieMenuState *mnuMovieMenuState;

extern void fldDrawCameraColorLayerPass(void);

extern void effRequestResourceByMode(const char *, const char *, u32, u32 *);

extern char D_003BC5D8[];
extern char mnuMovieMenuSpriteResourceNames[];

extern void mnuAdvanceMovieMenuIdleAnimation();


extern s8 D_00324510[];

extern void mnuReleaseMenuResourceSlots(void);

extern struct MenuList *mnuCreateListState(s32 id, s32 visibleCount, s32 rowSpacing);
extern void mnuSelectFirstListNode(struct MenuList *);


extern void mnuDrawMovieMenuListItem();
extern void sndStartTrackExtended(s32);
extern void effDrawSpriteWithCornerFade(s32 x, s32 y, s32 depth, u32 blend, s32 flags,
                                        s32 textureSetHandle, s32 slotIndex, s32 layer);
extern f32 fldVectorLength(f32 *);
extern void sdfVec3SubtractInPlace(f32 *, const f32 *);
extern void fldRotateVectorAroundY(f32 *, f32);
extern void sdfVec3AddInPlace(f32 *, const f32 *);
extern void sdfQuaternionNormalize(f32 *);
extern void sdfQuatForwardVector(f32 *, f32 *);
extern f32 sdfVec3NormalizeInPlaceAndGetLength(f32 *);
extern void sdfQuatFromEuler(f32 *, f32, f32, f32);
extern void fldNormalizedVectorCross(f32 *, f32 *, f32 *);


/* Sprite rows: graphic slot, draw parameter, x offset, y offset. */
extern s16 mnuMovieMenuSpritePlacements[][4];

void mnuDrawSprite(s32 x, s32 y, s32 depth, s32 alpha, s32 drawMode,
                   s32 spriteIndex, s32 drawContext) {
    effDrawSpriteWithCornerFade((x + mnuMovieMenuSpritePlacements[spriteIndex][2]) << 4,
                  (y + mnuMovieMenuSpritePlacements[spriteIndex][3]) << 3,
                  depth,
                  (u32)((f32)alpha * 256.0f * 0.0078125f),
                  drawMode,
                  mnuMovieMenuState
                      ->spriteHandles[mnuMovieMenuSpritePlacements[spriteIndex][0]],
                  mnuMovieMenuSpritePlacements[spriteIndex][1],
                  drawContext);
}

void mnuRecreateMenuSelectionList(void) {
    s32 i;
    struct MenuList *list;

    if (mnuMovieMenuState->selectionList != NULL) {
        mnuDestroyListState(mnuMovieMenuState->selectionList);
    }
    list = mnuCreateListState(0, 3, 0);
    mnuMovieMenuState->selectionList = list;
    list->drawCallback = mnuDrawMovieMenuListItem;
    for (i = 0; i < 3; i++) {
        mnuListAppendNode(mnuMovieMenuState->selectionList, 0);
    }
}

void mnuDestroyMovieMenuSelectionList(void) {
    mnuDestroyListState(mnuMovieMenuState->selectionList);
}

u32 mnuGetMovieMenuSelectionIndex(void) {
    return mnuMovieMenuState->selectionList->cursor->index;
}

void mnuSelectMenuListCursorByAdvance(s32 advanceCount) {
    mnuSelectFirstListNode(mnuMovieMenuState->selectionList);
    if (0 < advanceCount) {
        do {
            advanceCount = advanceCount - 1;
            mnuAdvanceListCursorDefault(mnuMovieMenuState->selectionList);
        } while (advanceCount != 0);
    }
}

s32 mnuIsAnyMenuInputPressed(void) {
    if (D_00324510[0x21] < 0 || D_00324510[0x23] < 0 ||
        D_00324510[0x22] < 0 || D_00324510[0x20] < 0 ||
        D_00324510[0x2a] < 0 || D_00324510[0x2b] < 0 ||
        D_00324510[0x28] < 0 || D_00324510[0x29] < 0 ||
        D_00324510[0x2d] < 0 || D_00324510[0x2c] < 0) {
        return 1;
    }
    return 0;
}

void mnuLoadMovieMenuResourceSlots(void) {
    if (mnuMovieMenuState->spriteHandles[0] == 0) {
        mnuMovieMenuState->spriteHandles[0] =
            (u32)effLoadIndexedResource(D_003BC5D8, mnuMovieMenuSpriteResourceNames, 0);
    }
    if (mnuMovieMenuState->spriteHandles[2] == 0) {
        const char *name = mnuMovieMenuSpriteResourceNames;
        name += 0x20;
        mnuMovieMenuState->spriteHandles[2] =
            (u32)effLoadIndexedResource(D_003BC5D8, name, 0);
    }
}

u32 func_0026C040(void) {
    return 1;
}

void mnuReleaseMenuResourceSlots(void) {
    MovieMenuState *state = mnuMovieMenuState;
    if (state->spriteHandles[0] != 0) {
        effDestroyResourceSlotSet((struct EffectSlotSet *)state->spriteHandles[0]);
        state = mnuMovieMenuState;
        state->spriteHandles[0] = 0;
    }
    if (state->spriteHandles[2] != 0) {
        effDestroyResourceSlotSet((struct EffectSlotSet *)state->spriteHandles[2]);
        state = mnuMovieMenuState;
        state->spriteHandles[2] = 0;
    }
}

void mnuEnsureMovieMenuSpriteHandle(s32 loadImmediately) {
    u32 *handle = &mnuMovieMenuState->spriteHandles[1];

    if (*handle == 0) {
        if (loadImmediately != 0) {
            const char *name = mnuMovieMenuSpriteResourceNames;
            name += 0x10;
            mnuMovieMenuState->spriteHandles[1] =
                (u32)effLoadIndexedResource(D_003BC5D8, name, 0);
        } else {
            const char *name = mnuMovieMenuSpriteResourceNames;
            name += 0x10;
            effRequestResourceByMode(D_003BC5D8, name, 0,
                                     handle);
        }
    }
}

u8 mnuHasSpriteHandle(void) {
    return mnuMovieMenuState->spriteHandles[1] != 0;
}

void mnuReleaseSpriteHandle(void) {
    if (mnuMovieMenuState->spriteHandles[1] != 0) {
        effDestroyResourceSlotSet((struct EffectSlotSet *)mnuMovieMenuState->spriteHandles[1]);
        mnuMovieMenuState->spriteHandles[1] = 0;
    }
}

extern void mnuDrawSprite(s32, s32, s32, s32, s32, s32, s32);

/* These entry points draw distinct movie-menu sprite table slots. */
void mnuDrawMovieMenuSpriteSlot16(s32 alpha) {
    mnuDrawSprite(0, 0, 0, alpha, 0, 0x16, 0x53);
}

void mnuDrawMovieMenuSpriteSlot17(s32 alpha) {
    mnuDrawSprite(0, 0, 0, alpha, 0, 0x17, 0x53);
}

void mnuDrawMovieMenuSpriteSlot18(s32 alpha) {
    mnuDrawSprite(0, 0, 0, alpha, 0, 0x18, 0x53);
}

void mnuDrawMovieMenuSpriteSlot1A(s32 parameter) {
    mnuDrawSprite(0, 0, 0, parameter, 0, 0x1A, 0x53);
}

extern void sdfSubmitGsAlphaOneRegisterPacket(s32, s32);
extern void sdfSubmitGsTestOneRegisterPacket(u64, u32);
extern void uiDrawUniformColorRect(s32, s32, s32, s32, s32, s32, s32);
extern void uiDrawActiveSurfaceRegion(s32);
extern void sdfDispatchSurfaceWithPreparedTexturePacket(s32);
extern void sdfDrawColoredTriangleFan(void *, s32, u32 *, s32, s32, s32);

typedef struct MenuSurfacePair {
    s32 first;
    s32 second;
} MenuSurfacePair;

void mnuDrawMovieMenuBackgroundQuad(s32 parameter) {
    sdfSubmitGsAlphaOneRegisterPacket(0x44, 0x3E);
    sdfSubmitGsTestOneRegisterPacket(0x3000D, 0x3E);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, parameter, 0x3E);
}

void mnuDrawMovieMenuSpriteLayers(s32 context) {
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0x80, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 2, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 1, context);
    mnuDrawSprite(1, -10, 0, 0x80, 0, 4, context);
}

void mnuDrawMovieMenuTransitionWipe(s32 base, s32 source, u32 alpha, s32 context) {
    u32 color = (alpha << 24) | 0x808080;
    MenuSurfacePair surfacePairs[4] = {
        {base, source},
        {base + 0x20, source},
        {base + 0x220, source + 0x1C0},
        {base + 0x200, source + 0x1C0},
    };

    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, -1, 0x2000, 0xE00, 0, context);
    uiDrawActiveSurfaceRegion(context);
    sdfSubmitGsTestOneRegisterPacket(0x3000DL, context);
    sdfDrawColoredTriangleFan(surfacePairs, 0, &color, 4, 1, context);
    sdfDispatchSurfaceWithPreparedTexturePacket(context);
    sdfSubmitGsAlphaOneRegisterPacket(0x48, context);
    sdfSubmitGsTestOneRegisterPacket(0x50000, context);
    mnuDrawSprite(0, 0, 0, 0x80, 0x60, 10, context);
    sdfSubmitGsTestOneRegisterPacket(0x30000, context);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, context);
    sdfSubmitGsAlphaOneRegisterPacket(0x44, context);
    sdfSubmitGsTestOneRegisterPacket(0x5000DL, context);
}

void mnuResetMovieMenuOpeningAnimation(void) {
    MovieMenuState *state = mnuMovieMenuState;

    state->cursor = 0;
    state->slideOffset = 0;
}

s32 mnuAdvanceMovieMenuOpeningAnimation(void) {
    MovieMenuState *state;
    s32 value;
    s32 alpha;
    f32 progress;

    value = mnuMovieMenuState->cursor;
    if (value < 0) {
        value = 0;
    }
    progress = (f32)value / 10.0f;
    if (1.0f < progress) {
        progress = 1.0f;
    }
    alpha = (s32)(progress * 128.0f);
    mnuDrawSprite(0, 0, 0, alpha, 0, 8, 0x53);
    mnuDrawSprite(0, 0, 0, alpha, 0, 0xC, 0x53);

    value = mnuMovieMenuState->cursor - 5;
    if (value < 0) {
        value = 0;
    }
    progress = (f32)value / 10.0f;
    if (1.0f < progress) {
        progress = 1.0f;
    }
    mnuDrawSprite(0, 0, 0, (s32)(progress * 76.8f), 0, 0xA, 0x53);

    state = mnuMovieMenuState;
    if (state->cursor == 5) {
        state->slideOffset = 0x400;
    }
    if (state->slideOffset > 0) {
        state->slideOffset -= 0x20;
    } else {
        state->slideOffset = 0;
    }

    mnuDrawMovieMenuTransitionWipe(mnuMovieMenuState->slideOffset - 0x200, 0, 0x80, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 2, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 1, 0x53);
    mnuDrawSprite(1, -10, 0, 0x80, 0, 4, 0x53);

    value = mnuMovieMenuState->cursor - 5;
    if (value < 0) {
        value = 0;
    }
    progress = (f32)value / 10.0f;
    if (1.0f < progress) {
        progress = 1.0f;
    }
    mnuDrawSprite(0, 0, 0, (s32)(progress * 128.0f), 0, 7, 0x53);

    value = mnuMovieMenuState->cursor - 20;
    if (value < 0) {
        value = 0;
    }
    progress = (f32)value / 10.0f;
    if (1.0f < progress) {
        progress = 1.0f;
    }
    mnuDrawSprite(0, 0, 0, (s32)(progress * 128.0f), 0, 0xB, 0x53);

    value = mnuMovieMenuState->cursor - 15;
    if (value < 0) {
        value = 0;
    }
    progress = (f32)value / 10.0f;
    if (1.0f < progress) {
        progress = 1.0f;
    }
    progress = 1.0f - progress;
    mnuDrawMovieMenuBackgroundQuad((s32)(progress * 128.0f));

    state = mnuMovieMenuState;
    state->cursor++;
    if (state->cursor < 31) {
        return 0;
    }
    return 1;
}

void mnuAdvanceMovieMenuIdleAnimation(void) {
    MovieMenuState *state;
    f32 pulse;

    fldDrawCameraColorLayerPass();

    state = mnuMovieMenuState;
    state->pulseTimer++;
    if (state->pulseTimer >= 61) {
        state->pulseTimer = 0;
        if (state->slideOffset == 0 && sdfGetRandFloat(0) < 0.1f) {
            mnuMovieMenuState->slideOffset = 0x400;
        }
    }

    pulse = (f32)mnuMovieMenuState->pulseTimer / 60.0f;
    pulse = (sdfSinPoly(pulse * 6.2831853f - 1.5707963f) + 1.0f) * 0.5f;
    mnuDrawSprite(0, 0, 0, 0x80, 0, 8, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xC, 0x53);
    mnuDrawSprite(0, 0, 0, 0x4C, 0, 0xA, 0x53);
    mnuDrawSprite(0, 0, 0, (s32)(pulse * 64.0f), 0, 0xA, 0x53);

    if (mnuMovieMenuState->slideOffset > 0) {
        mnuMovieMenuState->slideOffset -= 0x20;
    } else {
        mnuMovieMenuState->slideOffset = 0;
    }

    mnuDrawMovieMenuTransitionWipe(mnuMovieMenuState->slideOffset - 0x200, 0, 0x80, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 2, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 1, 0x53);
    mnuDrawSprite(1, -10, 0, 0x80, 0, 4, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 7, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xB, 0x53);
}

void mnuStartMovieMenuSfxGroup(void) {
    mnuDrawSprite(0, 0, 0, 0x80, 0, 6, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xD, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xE, 0x53);
    mnuDrawSprite(0, 0, 0, 0x80, 0, 0xF, 0x53);
}

void mnuRunMovieMenuIdleFrame(void) {
    mnuAdvanceMovieMenuIdleAnimation();
}

void mnuSwapStateWords(void) {
    MovieMenuState *state = mnuMovieMenuState;

    switch (state->backdropMode) {
    case 0:
        state->orbitFrame = 0;
        state->backdropMode = 1;
        break;
    case 1:
        state->backdropMode = 0;
        state->orbitFrame = 0x6a4;
        break;
    }
}

typedef struct MovieOrbitVec {
    f32 v[4];
} MovieOrbitVec;

typedef struct MovieOrbitPath {
    MovieOrbitVec points[3];
} MovieOrbitPath;

extern const MovieOrbitVec D_003AFE40;
extern const MovieOrbitPath D_003AFE50;
extern const MovieOrbitVec D_003AFE80;

/* Advance the movie-menu camera along its orbit: the frame counter wraps after
 * 3600 frames, the current path point is rotated about the orbit center by the
 * elapsed fraction, and the fixed rotation is normalized into `rotation`. */
void mnuUpdateMovieMenuCameraOrbit(f32 *position, f32 *rotation) {
    MovieOrbitVec rot = D_003AFE40;
    MovieOrbitPath path = D_003AFE50;
    MovieOrbitVec center;
    s32 segment;
    s32 elapsed;
    s32 i;
    f32 baseLength;
    f32 pointLength;

    segment = 0;
    if (mnuMovieMenuState->orbitFrame < 3600) {
        mnuMovieMenuState->orbitFrame++;
    } else {
        mnuMovieMenuState->orbitFrame = 0;
    }
    for (i = 0; i < 1; i++) {
        if (mnuMovieMenuState->orbitFrame <= (i + 1) * 3600) {
            segment = i;
            break;
        }
    }
    elapsed = mnuMovieMenuState->orbitFrame - segment * 3600;
    center = D_003AFE80;
    baseLength = fldVectorLength(path.points[0].v);
    pointLength = fldVectorLength(path.points[segment].v);
    *(MovieOrbitVec *)position = *(MovieOrbitVec *)path.points[segment].v;
    sdfVec3SubtractInPlace(position, center.v);
    fldRotateVectorAroundY(position, (f32)elapsed / 3600.0f * -6.2831850051879883f * baseLength / pointLength);
    sdfVec3AddInPlace(position, center.v);
    *(MovieOrbitVec *)rotation = rot;
    sdfQuaternionNormalize(rotation);
}


void mnuUpdateMovieMenuPoseFromInput(f32 *position, f32 *rotation) {
    f32 delta[4];
    f32 forward[4];
    f32 axis[4];

    if (D_00324510[0x36] != 0) {
        sdfQuatForwardVector(rotation, forward);
        sdfVec3NormalizeInPlaceAndGetLength(forward);
        position[0] += forward[0] * 20.0f;
        position[2] += forward[2] * 20.0f;
    }
    if (D_00324510[0x37] != 0) {
        sdfQuatForwardVector(rotation, forward);
        sdfVec3NormalizeInPlaceAndGetLength(forward);
        position[0] -= forward[0] * 20.0f;
        position[2] -= forward[2] * 20.0f;
    }
    if (D_00324510[0x34] != 0) {
        sdfQuatFromEuler(delta, 0.0f, -0.017453293f, 0.0f);
        sdfQuatMultiply(rotation, rotation, delta);
        sdfQuaternionNormalize(rotation);
    }
    if (D_00324510[0x35] != 0) {
        sdfQuatFromEuler(delta, 0.0f, 0.017453293f, 0.0f);
        sdfQuatMultiply(rotation, rotation, delta);
        sdfQuaternionNormalize(rotation);
    }

    if (D_00324510[0x33] != 0) {
        if (D_00324510[0x38] != 0) {
            position[1] -= 20.0f;
        }
        if (D_00324510[0x3a] != 0) {
            position[1] += 20.0f;
        }
    } else {
        if (D_00324510[0x38] != 0) {
            sdfQuatForwardVector(rotation, forward);
            axis[0] = 0.0f;
            axis[1] = -1.0f;
            axis[2] = 0.0f;
            fldNormalizedVectorCross(axis, axis, forward);
            position[0] += axis[0] * 20.0f;
            position[2] += axis[2] * 20.0f;
        }
        if (D_00324510[0x3a] != 0) {
            sdfQuatForwardVector(rotation, forward);
            axis[0] = 0.0f;
            axis[1] = -1.0f;
            axis[2] = 0.0f;
            fldNormalizedVectorCross(axis, axis, forward);
            position[0] -= axis[0] * 20.0f;
            position[2] -= axis[2] * 20.0f;
        }
    }

    if (D_00324510[0x39] != 0) {
        sdfQuatForwardVector(rotation, forward);
        memcpy(axis, forward, sizeof(axis));
        axis[1] += -10.0f;
        fldNormalizedVectorCross(axis, axis, forward);
        sdfQuatFromAxisAngle(delta, axis[0], axis[1], axis[2], 0.017453293f);
        sdfQuatMultiply(rotation, rotation, delta);
        sdfQuaternionNormalize(rotation);
    }
    if (D_00324510[0x3b] != 0) {
        sdfQuatForwardVector(rotation, forward);
        memcpy(axis, forward, sizeof(axis));
        axis[1] += 10.0f;
        fldNormalizedVectorCross(axis, axis, forward);
        sdfQuatFromAxisAngle(delta, axis[0], axis[1], axis[2], 0.017453293f);
        sdfQuatMultiply(rotation, rotation, delta);
        sdfQuaternionNormalize(rotation);
    }
}

void mnuResetMovieMenuStateAndStartTrack(void) {
    MovieMenuState *state = mnuMovieMenuState;

    state->transitionMode = 0;
    state->cursor = 0;
    state->idleTicks = 0;
    sndStartTrackExtended(0x310000);
}

void mnuClearGlobalMenuStateFields(void) {
    MovieMenuState *state = mnuMovieMenuState;

    state->transitionMode = 0;
    state->cursor = 0;
    state->idleTicks = 0;
}

void mnuResetTitlePageAndPhase(void) {
    MovieMenuState *state = mnuMovieMenuState;

    state->transitionMode = 0;
    state->cursor = 0;
}

s32 mnuPollMovieMenuInputAndTimeout(void) {
    MovieMenuState *state = mnuMovieMenuState;
    switch (state->transitionMode) {
    case 0:
        if (mnuIsAnyMenuInputPressed() == 0) {
            MovieMenuState *opening = mnuMovieMenuState;
            if (opening->cursor < 30) {
                opening->cursor++;
            } else {
                opening->transitionMode = 1;
                opening->cursor = 0;
            }
            return 0;
        }
        sndSetSequenceVolumePan(8, 127, 63);
        return 1;
    case 1:
        if (state->cursor < 90) {
            state->cursor++;
        } else {
            state->cursor = 0;
        }
        if (mnuIsAnyMenuInputPressed() != 0) {
            sndSetSequenceVolumePan(8, 127, 63);
            return 1;
        }
        state = mnuMovieMenuState;
        state->idleTicks++;
        if (state->idleTicks >= 601) {
            state->idleTicks = 0;
            return -1;
        }
        break;
    case 2:
        if (state->cursor < 0) {
            state->cursor++;
        } else {
            return 1;
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE40);

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE50);

INCLUDE_RODATA(const s32, "game/code_0026BD80", D_003AFE80);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5D8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5E0);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5E8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5F0);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC5F8);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC600);

INCLUDE_SDATA(const s32, "game/code_0026BD80", D_003BC608);

