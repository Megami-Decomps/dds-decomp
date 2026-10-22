#ifndef FLD_PLAYER_SCENE_STATE_H
#define FLD_PLAYER_SCENE_STATE_H

struct dds3Basic_t;

/* The scene-state word contains the selected camera/world-node pointer. */
struct dds3Basic_t *fldGetPlayerSceneState(void);
struct dds3Basic_t **fldGetPlayerSceneStateAddress(void);

#endif /* FLD_PLAYER_SCENE_STATE_H */
