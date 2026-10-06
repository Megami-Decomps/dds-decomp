#include "owner-types.h"

extern u32 func_002D2CB0(s32);
extern SdfTex *func_002DDD60(void *, RefObj *);
extern void effSubmitCompositeGsPacket(s32, EffCompositeGsDescriptor *);
extern void fileReadRecordSecondVector(void *, u128 *);
extern void fileReadVectorPtr20(void *, u128 *);

void func_002F2AE8(EffClassWork *work) {
    EffCompositeGsDescriptor draw;
    f32 origin[4] __attribute__((aligned(16)));
    f32 orientation[4] __attribute__((aligned(16)));
    f32 matrix[16] __attribute__((aligned(16)));
    f32 baseColorStorage[4];
    s32 baseInput[4];
    s32 directColorInput[4];
    u32 directColorOutput[4];
    s32 transformedColorInput[4];
    u32 transformedColorOutput[4];
    f32 *baseColor;
    f32 colorUnit;
    u32 packedColor;
    EffAnimationState *state = (EffAnimationState *)work->resource;
    EffSlotUvConfig *config = work->payload;
    FileRecordSlots *records;
    FileKeyBlock *keys;
    FileRecordSlot *slot;
    f32 *position;
    SdfTex *texture;
    u32 surface;
    s32 duration;
    s32 count;
    s32 i;
    u16 textureWidth, textureHeight;
    f32 uExtent, vExtent, uStep, vStep;
    f32 factor;
    u16 u, v, width, height;

    records = (FileRecordSlots *)state->record;
    count = (s32)records->count;
    position = state->positions;
    if (count > 0) {
        baseInput[0] = work->color;
        colorUnit = 0.0078125f;
        EE_MMI_RGBA_UNPACK(baseInput, colorUnit);
        baseColor = baseColorStorage;
        VU0_STORE_VF_UNCLOBBERED(vf10, baseColor);
        keys = (FileKeyBlock *)records->data0;
        slot = records->slots;
        surface = func_002D2CB0(keys->blendMode);
        duration = keys->length;
        draw.blendMode = keys->blendMode;
        uExtent = config->uExtent;
        vExtent = config->vExtent;
        uStep = config->uStep;
        vStep = config->vStep;
        texture = func_002DDD60((void *)surface, (RefObj *)state->textureHandle);
        textureHeight = (u16)texture->height * 8;
        textureWidth = (u16)texture->width * 16;
        draw.primaryClamp = 0x7F000FFFULL;
        draw.secondaryClamp = 0x2007F000FFFULL;
        draw.primaryTexture = draw.secondaryTexture = texture;
        draw.primaryUv.components[0] = 0;
        draw.primaryUv.components[1] = 0;
        draw.primaryUv.components[2] = textureWidth;
        draw.primaryUv.components[3] = 0;
        draw.primaryUv.components[4] = textureWidth;
        draw.primaryUv.components[5] = textureHeight;
        draw.primaryUv.components[6] = 0;
        draw.primaryUv.components[7] = textureHeight;
        if (!(((FileRecordSlots *)state->record)->flags & 1)) {
            for (i = 0; i < count; i++, slot++, position += 2) {
                f32 uBias, vBias;

                if (slot->state >= 0) {
                    directColorInput[0] = slot->color;
                    colorUnit = 0.0078125f;
                    EE_MMI_RGBA_UNPACK(directColorInput, colorUnit);
                    VU0_LOAD_VF(vf11, baseColor);
                    VU0_MUL(vf10, vf10, vf11);
                    colorUnit = 128.0f;
                    EE_MMI_RGBA_PACK_UNIT(packedColor, colorUnit);
                    directColorOutput[0] = packedColor;
                    draw.color = directColorOutput[0];
                    factor = func_002D7770(&config->scale.curve, slot->state, duration);
                    width = (u32)(uExtent * factor * textureWidth);
                    height = (u32)(vExtent * factor * textureHeight);
                    uBias = -(uExtent * (factor - 1.0f)) * 0.5f;
                    vBias = -(vExtent * (factor - 1.0f)) * 0.5f;
                    u = (u32)((position[0] + uBias) * textureWidth);
                    v = (u32)((position[1] + vBias) * textureHeight);
                    u %= textureWidth;
                    v %= textureHeight;
                    position[0] += uStep * factor;
                    position[1] += vStep * factor;
                    draw.scaleX = draw.scaleY = slot->scale;
                    draw.angle = slot->angle;
                    draw.secondaryUv.components[0] = u;
                    draw.secondaryUv.components[1] = v;
                    draw.secondaryUv.components[2] = width + u;
                    draw.secondaryUv.components[3] = v;
                    draw.secondaryUv.components[4] = width + u;
                    draw.secondaryUv.components[5] = height + v;
                    draw.secondaryUv.components[6] = u;
                    draw.secondaryUv.components[7] = height + v;
                    PCP_COPY_VECTOR(draw.position, slot->pos);
                    effSubmitCompositeGsPacket(surface, &draw);
                }
            }
        } else {
            fileReadRecordSecondVector((FileRecordSlots *)state->record, (u128 *)orientation);
            fileReadVectorPtr20((FileRecordSlots *)state->record, (u128 *)origin);
            VU0_LOAD_VF(vf10, orientation);
            effMiscQuaternionToMatrixVU();
            VU0_LOAD_VF(vf31, origin);
            VU0_STORE_MATRIX(matrix);
            for (i = 0; i < count; i++, slot++, position += 2) {
                f32 oldU, oldV;

                if (slot->state >= 0) {
                    VU0_LOAD_MATRIX(matrix);
                    VU0_LOAD_VF(vf10, slot->pos);
                    VU0_TRANSFORM_POINT(vf10, vf10);
                    VU0_STORE_VF(vf10, draw.position);
                    transformedColorInput[0] = slot->color;
                    colorUnit = 0.0078125f;
                    EE_MMI_RGBA_UNPACK(transformedColorInput, colorUnit);
                    VU0_LOAD_VF(vf11, baseColor);
                    VU0_MUL(vf10, vf10, vf11);
                    colorUnit = 128.0f;
                    EE_MMI_RGBA_PACK_UNIT(packedColor, colorUnit);
                    transformedColorOutput[0] = packedColor;
                    draw.color = transformedColorOutput[0];
                    factor = func_002D7770(&config->scale.curve, slot->state, duration);
                    width = (s32)(uExtent * factor * textureWidth);
                    height = (s32)(vExtent * factor * textureHeight);
                    oldU = position[0];
                    oldV = position[1];
                    position[0] = oldU + uStep * factor;
                    position[1] = oldV + vStep * factor;
                    u = (s32)((oldU + -(uExtent * (factor - 1.0f)) * 0.5f) * textureWidth);
                    v = (s32)((oldV + -(vExtent * (factor - 1.0f)) * 0.5f) * textureHeight);
                    u %= textureWidth;
                    v %= textureHeight;
                    draw.scaleX = draw.scaleY = slot->scale;
                    draw.angle = slot->angle;
                    draw.secondaryUv.components[0] = u;
                    draw.secondaryUv.components[1] = v;
                    draw.secondaryUv.components[2] = width + u;
                    draw.secondaryUv.components[3] = v;
                    draw.secondaryUv.components[4] = width + u;
                    draw.secondaryUv.components[5] = height + v;
                    draw.secondaryUv.components[6] = u;
                    draw.secondaryUv.components[7] = height + v;
                    effSubmitCompositeGsPacket(surface, &draw);
                }
            }
        }
    }
}
