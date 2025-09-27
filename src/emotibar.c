#include "emotibar.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

// Define CLOCK_REALTIME if not defined
#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#endif

// Use a monotonic clock if available for ETA calculation.
#if defined(CLOCK_MONOTONIC)
#define EMOTI_CLOCK CLOCK_MONOTONIC
#else
#define EMOTI_CLOCK CLOCK_REALTIME
#endif

static const char *emotes[] = {
    ":(",  // 0-9%
    ":/",  // 10-29%
    ":|",  // 30-49%
    ":)",  // 50-69%
    ":D",  // 70-89%
    "B)"   // 90-100%
};

static int total_steps = 50; // default
static struct timespec start_time = {0,0};
static const char *message = NULL; // not owned

static double timespec_diff_secs(const struct timespec *a, const struct timespec *b) {
    return (double)(a->tv_sec - b->tv_sec) + (double)(a->tv_nsec - b->tv_nsec)/1e9;
}

void emotibar_init(int total) {
    if (total > 0) total_steps = total;
    clock_gettime(EMOTI_CLOCK, &start_time);
}

void emotibar_set_message(const char *msg) {
    message = msg; // caller must guarantee lifetime
}

static const char *choose_emote(int percent) {
    if (percent < 10) return emotes[0];
    if (percent < 30) return emotes[1];
    if (percent < 50) return emotes[2];
    if (percent < 70) return emotes[3];
    if (percent < 90) return emotes[4];
    return emotes[5];
}

void emotibar_update(int step) {
    if (step < 0) step = 0;
    if (step > total_steps) step = total_steps;

    int percent = (total_steps > 0) ? (100 * step) / total_steps : 100;
    const char *emote = choose_emote(percent);

    // ETA calculation
    struct timespec now;
    clock_gettime(EMOTI_CLOCK, &now);
    double elapsed = timespec_diff_secs(&now, &start_time);
    double eta = 0.0;
    if (step > 0) {
        double rate = elapsed / step; // seconds per step
        eta = rate * (total_steps - step);
    }

    int bar_width = 40;
    int filled = (bar_width * percent) / 100;

    // Build ETA string
    int eta_min = (int)(eta / 60);
    int eta_sec = (int)(eta) % 60;

    // Print carriage return and overwrite line
    printf("\r%s [", emote);
    for (int i = 0; i < bar_width; ++i) {
        if (i < filled) putchar('=');
        else putchar('-');
    }
    if (message) printf("] %3d%% ETA %2dm%02ds %s", percent, eta_min, eta_sec, message);
    else printf("] %3d%% ETA %2dm%02ds", percent, eta_min, eta_sec);
    fflush(stdout);
}

void emotibar_finish(void) {
    // ensure 100% and newline
    emotibar_update(total_steps);
    printf("\n");
}

// i hate c so much :c -xamyl