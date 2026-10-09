extern f32 D_00436284;
extern SdfTex *D_00389978[];
extern f32 sdfSinPoly(f32);
extern void func_00336538(f32);
extern s32 sdfAllocPacketAligned(s32);
extern void sdfInitPacketList(SdfListHead *);
extern s32 sdfConsCreateDrawPacket(SdfListHead *, SdfTex *, s32);
extern SdfPoolNode *fldGetDisplayTableRow(void);
extern void sdfQueueTexturedQuad(s32, s32, s32, s32, s32, s32, s32, s32,
                                 s32, s32, s32, s32, s32, s32, s32, s32,
                                 s32, s32, s32, s32, s32 (*)(s32));

void func_00145D48(s32 index, s32 alpha, f32 x, f32 y, f32 rotation) {
    f32 x0, x1, z0, z1;
    s32 textureU, textureV, textureWidth, textureHeight;
    s32 textureURight, textureVBottom;
    f32 corner0[4] __attribute__((aligned(16)));
    f32 corner1[4] __attribute__((aligned(16)));
    f32 corner2[4] __attribute__((aligned(16)));
    f32 corner3[4] __attribute__((aligned(16)));
    f32 xOffset, yOffset;
    u32 color;
    s32 packetList;
    SdfPoolNode *appendTarget;
    SdfPoolNode *drawSurface;

    if (index == 1) {
        s32 pulseAlpha = (u32)(sdfSinPoly(D_00436284) * 64.0f);
        pulseAlpha += 128;
        pulseAlpha = pulseAlpha * alpha / 128;
        D_00436284 += 0.2f;
        if (pulseAlpha < 0) {
            pulseAlpha = 0;
        }
        if (pulseAlpha > 255) {
            pulseAlpha = 255;
        }
        color = (u32)pulseAlpha << 24;
    } else {
        color = (u32)alpha << 24;
    }

    packetList = sdfAllocPacketAligned(0x20);
    sdfInitPacketList((SdfListHead *)packetList);
    sdfConsCreateDrawPacket((SdfListHead *)packetList, D_00389978[0], 0);

    textureU = (s32)D_0039A0E0[index].u * 16;
    textureV = (s32)D_0039A0E0[index].v * 16;
    textureWidth = (s32)D_0039A0E0[index].width * 16;
    textureHeight = (s32)D_0039A0E0[index].height * 16;
    textureURight = textureU + textureWidth;
    textureVBottom = textureV + textureHeight;
    x0 = (f32)-((s32)D_0039A0E0[index].anchorX * 16);
    z0 = (f32)-((s32)D_0039A0E0[index].anchorY * 16);
    x1 = x0 + (f32)textureWidth;
    z1 = z0 + (f32)textureHeight;

    corner0[0] = x0; corner0[1] = 0.0f; corner0[2] = z0;
    corner3[0] = x1; corner3[1] = 0.0f; corner3[2] = z1;
    corner1[0] = corner3[0]; corner1[1] = 0.0f; corner1[2] = corner0[2];
    corner2[0] = corner0[0]; corner2[1] = 0.0f; corner2[2] = corner3[2];

    func_00336538((rotation + 180.0f) * 3.14f / 180.0f);
    VU0_LOAD_VF_MEMORY(vf10, corner0);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner0);
    VU0_LOAD_VF_MEMORY(vf10, corner1);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner1);
    VU0_LOAD_VF_MEMORY(vf10, corner2);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner2);
    VU0_LOAD_VF_MEMORY(vf10, corner3);
    VU0_ROTATE_VEC_EXTENDED(vf10, vf10);
    VU0_STORE_VF(vf10, corner3);

    xOffset = x * 16.0f;
    yOffset = y * 16.0f;
    corner0[0] += xOffset; corner0[2] = corner0[2] * 0.5f + yOffset;
    corner1[2] = corner1[2] * 0.5f + yOffset; corner1[0] += xOffset;
    corner2[0] += xOffset; corner2[2] = corner2[2] * 0.5f + yOffset;
    corner3[0] += xOffset; corner3[2] = corner3[2] * 0.5f + yOffset;

    sdfQueueTexturedQuad(packetList, color | 0x808080, 0x44,
                         (s32)corner0[0], (s32)corner0[2],
                         textureU, textureV,
                         (s32)corner1[0], (s32)corner1[2],
                         textureURight, textureV,
                         (s32)corner2[0], (s32)corner2[2],
                         textureU, textureVBottom,
                         (s32)corner3[0], (s32)corner3[2],
                         textureURight, textureVBottom,
                         0, NULL);

    appendTarget = fldGetDisplayTableRow();
    drawSurface = fldGetDisplayTableRow();
    drawSurface->append((SdfListHead *)appendTarget, (SdfListHead *)packetList);
}
