#ifndef DDS_SCR_CURRENT_PROCESS_USER_DATA_API_H
#define DDS_SCR_CURRENT_PROCESS_USER_DATA_API_H

/* Returns the opaque caller-owned context attached to the ambient current
 * script process. This is a borrowed pointer; the caller retains ownership. */
void *scrGetCurrentProcessUserData(void);

#endif /* DDS_SCR_CURRENT_PROCESS_USER_DATA_API_H */
