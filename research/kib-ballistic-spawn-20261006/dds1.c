/* Twelve authored velocity directions, each stored as a four-float vector. */
extern f32 D_0034E0C0[12][4];

void func_001547D8(EffBallisticEmitter *effect, s32 index) {
    f32 position[4];
    f32 direction[4];
    EffPacket *packet = (EffPacket *)effect->head.buffer->records;
    f32 gravity = effect->gravity / 100.0f;
    f32 radius = effect->radius;
    f32 cone = effect->cone;
    f32 speed = effect->speed;
    f32 jitter;
    f32 angle, cosine, sine;
    f32 decay;
    s32 frame;

    packet += index;
    switch (effect->mode) {
    case 0:
        if (effect->randomDir) {
            direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            direction[1] = 0;
            direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            VU0_LOAD_VF(vf10, direction);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            packet->pos[0] = direction[0] * radius;
            packet->pos[1] = 0;
            packet->pos[2] = direction[2] * radius;
            VU0_LOAD_MATRIX(effect->head.matrix);
            VU0_LOAD_VF(vf10, packet->pos);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, packet->pos);
            packet->pos[0] += effect->head.origin[0];
            packet->pos[1] += effect->head.origin[1];
            packet->pos[2] += effect->head.origin[2];
            direction[0] = (effMiscRandUnitFloat(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            direction[1] = -(1.0f - cone);
            direction[2] = (effMiscRandUnitFloat(effEmitterDelayRandomState) - 0.5f) * 2.0f * cone;
            VU0_LOAD_VF(vf10, direction);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            jitter = effMiscRandUnitFloat(D_0034DF38) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f38 = (effMiscRandUnitFloat(D_0034DF38) * 0.5f + 0.5f) * gravity;
        } else {
            angle = (3.14159265f * 2.0f) / (u32)effect->head.packetCount * index;
            cosine = sdfEvaluateCosineViaSinePhaseShift(angle);
            sine = sdfSinPoly(angle);
            packet->pos[0] = cosine * radius;
            packet->pos[1] = 0;
            packet->pos[2] = sine * radius;
            VU0_LOAD_MATRIX(effect->head.matrix);
            VU0_LOAD_VF(vf10, packet->pos);
            VU0_APPLY_MATRIX(vf10, vf10);
            VU0_STORE_VF(vf10, packet->pos);
            packet->pos[0] += effect->head.origin[0];
            packet->pos[1] += effect->head.origin[1];
            packet->pos[2] += effect->head.origin[2];
            packet->vel[0] = cosine * cone * speed;
            packet->vel[1] = -speed * (1.0f - cone);
            packet->vel[2] = sine * cone * speed;
            packet->f38 = gravity;
        }
        break;
    case 1:
        if (effect->randomDir) {
            packet->pos[0] = effect->head.origin[0];
            packet->pos[1] = effect->head.origin[1];
            packet->pos[2] = effect->head.origin[2];
            direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            direction[1] = -((effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f);
            direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            VU0_LOAD_VF(vf10, direction);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            jitter = effMiscRandUnitFloat(D_0034DF38) * effect->jitter + (1.0f - effect->jitter);
            packet->vel[0] = direction[0] * speed * jitter;
            packet->vel[1] = direction[1] * speed * jitter;
            packet->vel[2] = direction[2] * speed * jitter;
            packet->f38 = gravity;
        } else {
            packet->pos[0] = effect->head.origin[0];
            packet->pos[1] = effect->head.origin[1];
            packet->pos[2] = effect->head.origin[2];
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            packet->vel[0] = D_0034E0C0[index % 12][0] * speed;
            packet->vel[1] = D_0034E0C0[index % 12][1] * speed;
            packet->vel[2] = D_0034E0C0[index % 12][2] * speed;
            packet->f38 = gravity;
        }
        break;
    case 2:
        if (effect->randomDir) {
            decay = effect->decayPct / 100.0f + 1.0f;
            EE_MMI_UNIT_MATRIX(effect->head.matrix);
            direction[0] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            direction[1] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            direction[2] = (effMiscRandUnitFloat(D_0034DF38) - 0.5f) * 2.0f;
            VU0_LOAD_VF(vf10, direction);
            VU0_NORMALIZE_VF10();
            VU0_STORE_VF(vf10, direction);
            jitter = effMiscRandUnitFloat(D_0034DF38) * effect->jitter + (1.0f - effect->jitter);
            direction[0] *= speed * jitter;
            direction[1] *= speed * jitter;
            direction[2] *= speed * jitter;
            packet->vel[0] = -direction[0];
            packet->vel[1] = -direction[1];
            packet->vel[2] = -direction[2];
            position[0] = effect->head.origin[0];
            position[1] = effect->head.origin[1];
            position[2] = effect->head.origin[2];
            frame = 0;
            do {
                position[0] += direction[0];
                position[1] += direction[1];
                position[2] += direction[2];
                direction[0] *= decay;
                direction[1] *= decay;
                direction[2] *= decay;
                direction[1] += gravity;
                frame++;
            } while (frame < effect->head.frameCount);
            packet->pos[0] = position[0];
            packet->pos[1] = position[1];
            packet->pos[2] = position[2];
            packet->f38 = -gravity;
        }
        break;
    }
    packet->age = -(effMiscRand(effEmitterDelayRandomState) % (effect->spread + 1));
    packet->color = 0;
    jitter = effect->head.speedJitter;
    packet->speed = effect->head.speed * (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter));
    jitter = effect->head.spinJitter;
    if (jitter != 0) {
        packet->spin = (effMiscRandUnitFloat(D_0034DF38) * jitter + (1.0f - jitter)) * (3.14159265f * 2.0f);
    } else {
        packet->spin = 0;
    }
    parDispatchKindInit(&effect->head.sub, index);
}
