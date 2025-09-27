#ifndef EMOTIBAR_H
#define EMOTIBAR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

// Initialize the progress bar with total steps. Call once before updates.
void emotibar_init(int total_steps);

// Update the bar to the given step (0..total_steps). Displays ETA and emote.
void emotibar_update(int step);

// Finish the bar and print final message.
void emotibar_finish(void);

// Optional: set a short message to be shown alongside the bar. The caller
// must keep the string valid for the duration of the bar (no copy).
void emotibar_set_message(const char *msg);

#ifdef __cplusplus
}
#endif

#endif
