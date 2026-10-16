#ifndef FLD_PLAYER_SCENE_STATE_H
#define FLD_PLAYER_SCENE_STATE_H

struct EffWorldNode;

/* The scene-state word contains the selected camera/world-node pointer. */
struct EffWorldNode *fldGetPlayerSceneState(void);
struct EffWorldNode **fldGetPlayerSceneStateAddress(void);

#endif /* FLD_PLAYER_SCENE_STATE_H */
