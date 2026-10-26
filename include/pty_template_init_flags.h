#ifndef PTY_TEMPLATE_INIT_FLAGS_H
#define PTY_TEMPLATE_INIT_FLAGS_H

/* Bit flags controlling a roster template's initial level and skill rebuild. */
typedef enum PtyTemplateInitFlags {
    PTY_TEMPLATE_INIT_NONE = 0,
    PTY_TEMPLATE_KEEP_BASE_LEVEL = 1,
    PTY_TEMPLATE_SKIP_PROFILE_SKILL_REBUILD = 2,
    PTY_TEMPLATE_USE_PARTY_MAX_LEVEL = 4
} PtyTemplateInitFlags;

#endif
