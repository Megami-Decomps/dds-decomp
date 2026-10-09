void func_001FA480(BtlLinkedCommand *action, BtlCamState *pose,
                   const BtlCameraTimedInstruction *instruction) {
    s32 done = 0;
    s32 kind;
    u32 lower;
    u32 upper;
    s32 random;
    s32 i;
    f32 distance;
    s16 signedIndex;
    BtlUnit *unit;
    BtlUnit *actor;
    BtlUnit *target;
    BtlCameraParameterSnapshot *savedParameters;
    BtlCameraStoredPose *savedPose;
    f32 quaternion[4];
    f32 oldFocus[4];
    f32 actorPosition[4];
    f32 facing[4];
    f32 forward[4];
    f32 yaw[4];

    memset(forward, 0, sizeof(forward));
    forward[2] = -1.0f;
    do {
        kind = instruction->kind;

        switch (kind) {
        case 0:
            CURSOR->parameterIndex114 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex114, D_003BC5D0);
            break;
        case 1:
            CURSOR->parameterIndex118 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex118, D_003BC650);
            break;
        case 2:
            CURSOR->parameterIndex11C = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex11C, D_003BC2D0);
            break;
        case 3:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex120, D_003BC6D0);
            break;
        case 10:
        case 11:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex120, D_003BCA50);
            break;
        case 12:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex120, D_003BCDD0);
            break;
        case 13:
            CURSOR->parameterIndex120 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex120, D_003BCE50);
            break;
        case 14:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                ++instruction;
                btlClearRuntimeFlag2000();
            } else {
                ++instruction;
            }
            break;
        case 16:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                ++instruction;
                btlClearAllUnitDefeatCandidatesTask();
                btlFlagLinkedGroupDefeatCandidatesTask((s32)action);
            } else {
                ++instruction;
            }
            break;
        case 17:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                if (action->link->unit->status.flags & 0x200) {
                    btlFlagMatchingUnitsDefeatCandidate(0x200);
                } else {
                    btlFlagMatchingUnitsDefeatCandidate(0x400);
                }
            }
            ++instruction;
            break;
        case 18:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                if (action->link->unit->status.flags & 0x200) {
                    btlFlagMatchingUnitsDefeatCandidate(0x400);
                } else {
                    btlFlagMatchingUnitsDefeatCandidate(0x200);
                }
            }
            ++instruction;
            break;
        case 39:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                /* Legacy typed hook transports this command address unchanged. */
                if (func_001EC7E8((BtlUnit *)action) != 0) {
                    CURSOR->busy = 1;
                }
            }
            if (CURSOR->busy == 0) {
                ++instruction;
                break;
            }
            ++instruction;
            done = 1;
            break;
        case 43:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                /* Legacy typed hook transports this command address unchanged. */
                if (func_001EC828((BtlUnit *)action) != 0) {
                    CURSOR->busy = 1;
                }
            }
            if (CURSOR->busy != 0) {
                goto stopAfterInstruction;
            }
            ++instruction;
            break;

        case 37:
        case 38:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                unit = ((BtlState *)btlGetRuntime())->units;
                if (action->link->unit->lookupId == 1) {
                    while (unit != NULL) {
                        if ((btlUnitStatusPair(unit) & 0x201) == 0x201 &&
                            unit->lookupId != action->link->unit->lookupId) {
                            if (instruction->kind == 37) {
                                lower = unit->lookupId;
                                upper = action->link->unit->lookupId;
                            } else {
                                lower = action->link->unit->lookupId;
                                upper = unit->lookupId;
                            }
                            if (lower < upper &&
                                ((unit->status.flags & 0x20) != 0 ||
                                 (unit->partyRecord.status & 0x7FFF) != 0)) {
                                btlClearUnitDefeatCandidate(unit);
                            }
                        }
                        unit = unit->nextActor;
                    }
                }
            }
            ++instruction;
            break;

        case 34:
        case 35:
        case 36:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                unit = ((BtlState *)btlGetRuntime())->units;
                while (unit != NULL) {
                    if ((btlUnitStatusPair(unit) & 0x201) == 0x201 &&
                        unit->lookupId != action->link->unit->lookupId) {
                        switch ((u32)instruction->kind) {
                        case 34:
                            lower = unit->lookupId;
                            upper = action->link->unit->lookupId;
                            break;
                        case 35:
                            lower = action->link->unit->lookupId;
                            upper = unit->lookupId;
                            break;
                        case 36:
                        default:
                            if (CURSOR->phaseCount == 0) {
                                lower = action->link->unit->lookupId;
                                upper = unit->lookupId;
                            } else {
                                lower = unit->lookupId;
                                upper = action->link->unit->lookupId;
                            }
                            break;
                        }
                        if (lower < upper) {
                            btlClearUnitDefeatCandidate(unit);
                        }
                    }
                    unit = unit->nextActor;
                }
            }
            ++instruction;
            break;
        case 19:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlFlagMatchingUnitsDefeatCandidate(0x600);
            }
            ++instruction;
            break;
        case 20:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_001F5320(action, pose, 0, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 21:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                CURSOR->endpointMode = 0;
            }
            ++instruction;
            break;
        case 15:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                ++instruction;
                btlClearAllUnitDefeatCandidatesTask();
                btlApplyCombinedActorFlags((u8 *)action);
            } else {
                ++instruction;
            }
            break;
        case 4:
            CURSOR->parameterIndex124 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex124, D_003BC0D0);
            break;
        case 5:
            CURSOR->parameterIndex128 = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex128, D_003BD1D0);
            break;
        case 8:
            CURSOR->parameterIndex12C = instruction->parameterIndex;
            func_001FD400(action, pose, instruction++, &CURSOR->parameterIndex12C, D_003BD750);
            break;
        case 6:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_001F5868(action, pose, 0, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 22:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_00207268(action);
            }
            ++instruction;
            break;
        case 31:
        case 32:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                ((f32)CURSOR->frame < instruction->startFrame + instruction->duration ||
                 instruction->duration == 1000.0f)) {
                actor = (kind == 31) ? btlGetIndexListEntry(action->targetList, 0) : action->link->unit;
                btlUnitGetPosVU(actor, 1);
                VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                VU0_LOAD_VF(vf10, pose->direction);
                VU0_NEGATE_XYZ(vf10);
                VU0_SCALE_VF(vf10, pose->distance);
                VU0_LOAD_VF(vf11, pose->position);
                VU0_ADD(vf10, vf10, vf11);
                VU0_STORE_VF_UNCLOBBERED(vf10, oldFocus);
                VU0_LOAD_VF(vf10, actorPosition);
                VU0_STORE_VF_UNCLOBBERED(vf10, pose->position);
                VU0_LOAD_VF(vf11, oldFocus);
                VU0_SUB(vf10, vf10, vf11);
                VU0_LENGTH_VF10(distance);
                pose->distance = distance;
                VU0_NORMALIZE_VF10();
                VU0_STORE_VF_UNCLOBBERED(vf10, pose->direction);
            }
            ++instruction;
            break;

        case 26:
        case 27:
        case 28:
        case 29:
        case 30:
        case 44:
        case 45:
        case 46:
        case 47:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                switch (kind) {
                case 26:
                    actor = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                    break;
                case 27:
                case 30:
                case 44:
                case 46:
                    actor = action->link->unit;
                    break;
                case 28:
                case 45:
                case 47:
                    actor = action->linkedA;
                    break;
                default:
                    actor = action->linkedB;
                    break;
                }
                if (actor->status.flags & 0x80000) {
                    btlUnitGetPosVU(actor, 0);
                    VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                    btlCopyUnitRotationQuaternion(actor, quaternion);
                    VU0_LOAD_VF(vf10, quaternion);
                    effMiscQuaternionToMatrixVU();
                    VU0_LOAD_VF(vf10, forward);
                    VU0_APPLY_MATRIX(vf10, vf10);
                    VU0_SCALE_VF(vf10, 1.0f);
                    VU0_LOAD_VF(vf11, actorPosition);
                    VU0_ADD(vf10, vf10, vf11);
                    VU0_STORE_VF_UNCLOBBERED(vf10, oldFocus);
                    switch (instruction->kind) {
                    case 26:
                    case 45:
                        btlUnitGetPosVU(action->link->unit, 0);
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    case 27:
                    case 28:
                    case 29:
                    case 46:
                    case 47:
                        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                        btlUnitGetPosVU(target, 0);
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    case 30:
                        target = (BtlUnit *)btlGetIndexListEntry(action->targetList, 0);
                        if (target->status.flags & 0x400) {
                            func_00208000(0x400, NULL, NULL);
                        } else {
                            func_00208000(0x200, NULL, NULL);
                        }
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    case 44:
                        btlUnitGetPosVU(action->linkedA, 0);
                        VU0_STORE_VF_UNCLOBBERED(vf10, actorPosition);
                        break;
                    }
                    btlAimHorizontalDirectionVU(oldFocus, actorPosition);
                    VU0_STORE_VF_UNCLOBBERED(vf10, facing);
                    if ((u32)instruction->kind < 48) {
                        if ((u32)instruction->kind >= 46) {
                            if (actor->lookupId == 0) {
                                i = 30;
                            } else if (actor->lookupId == 2) {
                                i = -30;
                            } else {
                                i = action->linkedA->lookupId < action->link->unit->lookupId ? -30 : 30;
                                if (actor == action->linkedA) {
                                    i = -i;
                                }
                            }
                            func_00340DC8(0.0f, (f32)i * 0.017453293f, 0.0f);
                            VU0_STORE_VF_UNCLOBBERED(vf10, yaw);
                            VU0_LOAD_VF(vf10, facing);
                            VU0_LOAD_VF(vf11, yaw);
                            effMiscQuatMultiplyVU();
                            VU0_STORE_VF_UNCLOBBERED(vf10, facing);
                        }
                    }
                    btlSetUnitRotation(actor, (s128 *)facing);
                }
            }
            ++instruction;
            break;

        case 25:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlClearAllUnitDefeatCandidatesTask();
                switch (instruction->parameterIndex) {
                case 0:
                    btlFlagUnitDefeatCandidate(action->link->unit);
                    break;
                case 4:
                    btlFlagUnitDefeatCandidate((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
                    break;
                }
            }
            ++instruction;
            break;
        case 24:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                switch (instruction->parameterIndex) {
                case 0:
                    btlFlagUnitDefeatCandidate(action->link->unit);
                    break;
                case 4:
                    btlFlagUnitDefeatCandidate((BtlUnit *)btlGetIndexListEntry(action->targetList, 0));
                    break;
                }
            }
            ++instruction;
            break;
        case 23:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                btlClearUnitDefeatCandidate(action->link->unit);
            }
            ++instruction;
            break;
        case 7:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_001F5868(action, pose, 1, instruction->parameterIndex);
            }
            ++instruction;
            break;
        case 40:
        case 42:
            if (!(instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration)) {
                ++instruction;
                break;
            }
            if (kind == 42) {
                if (btlCanUseLinkedActor((s32)action) != 0) {
                    CURSOR->restoreAlternate = 0;
                    ++instruction;
                    break;
                }
                CURSOR->restoreAlternate = 1;
            }
            VU0_LOAD_VF(vf10, CURSOR->pathEnd);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.pathEnd);
            VU0_LOAD_VF(vf10, CURSOR->pathStart);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.pathStart);
            VU0_LOAD_VF(vf10, CURSOR->vector40);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.vector40);
            VU0_LOAD_VF(vf10, CURSOR->vector50);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.vector50);
            VU0_LOAD_VF(vf10, CURSOR->vector60);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.vector60);
            VU0_LOAD_VF(vf10, CURSOR->pathCenter);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.pathCenter);
            VU0_LOAD_VF(vf10, CURSOR->direction);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.direction);
            VU0_LOAD_VF(vf10, &CURSOR->angle);
            VU0_STORE_VF_UNCLOBBERED(vf10, D_003BD900.angleVector);
            D_003BD900.distance = CURSOR->distance;
            D_003BD900.parameterB0 = CURSOR->parameterB0;
            D_003BD900.parameterC0 = CURSOR->parameterC0;
            D_003BD900.parameterD0 = CURSOR->parameterD0;
            D_003BD900.parameterE0 = CURSOR->parameterE0;
            D_003BD900.parameterF0 = CURSOR->parameterF0;
            D_003BD900.parameter100 = CURSOR->parameter100;
            D_003BD900.parameter110 = CURSOR->parameter110;
            if (instruction->kind == 40) {
                savedParameters = &D_003BDA00;
                savedPose = &D_003BDB00;
            } else {
                savedParameters = &D_003BDB30;
                savedPose = &D_003BDC30;
            }
            memcpy(savedPose, pose, sizeof(*savedPose));
            func_001F5868(action, &savedPose->pose, 1, instruction->parameterIndex);
            captureCameraParameters(savedParameters);
            VU0_LOAD_VF(vf10, D_003BD900.pathEnd);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathEnd);
            VU0_LOAD_VF(vf10, D_003BD900.pathStart);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathStart);
            VU0_LOAD_VF(vf10, D_003BD900.vector40);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->vector40);
            VU0_LOAD_VF(vf10, D_003BD900.vector50);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->vector50);
            VU0_LOAD_VF(vf10, D_003BD900.vector60);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->vector60);
            VU0_LOAD_VF(vf10, D_003BD900.pathCenter);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->pathCenter);
            VU0_LOAD_VF(vf10, D_003BD900.direction);
            VU0_STORE_VF_UNCLOBBERED(vf10, CURSOR->direction);
            VU0_LOAD_VF(vf10, D_003BD900.angleVector);
            VU0_STORE_VF_UNCLOBBERED(vf10, &CURSOR->angle);
            CURSOR->distance = D_003BD900.distance;
            CURSOR->parameterB0 = D_003BD900.parameterB0;
            CURSOR->parameterC0 = D_003BD900.parameterC0;
            CURSOR->parameterD0 = D_003BD900.parameterD0;
            CURSOR->parameterE0 = D_003BD900.parameterE0;
            CURSOR->parameterF0 = D_003BD900.parameterF0;
            CURSOR->parameter100 = D_003BD900.parameter100;
            CURSOR->parameter110 = D_003BD900.parameter110;
            ++instruction;
            break;

        case 41:
            if ((instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration)) {
                if (CURSOR->restoreAlternate == 0) {
                    savedParameters = &D_003BDA00;
                    savedPose = &D_003BDB00;
                } else {
                    savedParameters = &D_003BDB30;
                    savedPose = &D_003BDC30;
                }
                memcpy(pose, savedPose, sizeof(*savedPose));
                restoreCameraParameters(savedParameters);
            }
            ++instruction;
            break;

        case 33:
            if (instruction->startFrame <= (f32)CURSOR->frame &&
                (f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                func_001F5868(action, pose, 2, instruction->parameterIndex);
            }
            ++instruction;
            break;

        case 9:
            if (instruction->startFrame <= (f32)CURSOR->frame) {
                if ((f32)CURSOR->frame < instruction->startFrame + instruction->duration) {
                    CURSOR->mode = 1;
                }
                random = btlNextScaledRandom(8);
                signedIndex = (s16)random;
                CURSOR->parameterIndex114 = 0;
                CURSOR->parameterIndex118 = 0;
                CURSOR->parameterIndex11C = 0;
                CURSOR->parameterIndex120 = 0;
                CURSOR->parameterIndex124 = 0;
                CURSOR->parameterIndex128 = 0;
                CURSOR->parameterIndex12C = 0;
                CURSOR->endpointMode = 0;
                CURSOR->index = signedIndex;
                CURSOR->frame = 0;
                func_001F5320(action, pose, 0, D_003BBD80[signedIndex]);
            }
            done = 1;
            break;

        case 48:
        default:
stopAfterInstruction:
            ++instruction;
            done = 1;
            break;
        }
    } while (done == 0);
}
