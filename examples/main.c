#include "emotibar.h"
#include <stdio.h>
#include <stdlib.h>

// Simple CPU-bound work: count primes up to N in chunks and report progress.
static int is_prime(int n) {
    if (n < 2) return 0;
    if (n % 2 == 0) return n == 2;
    for (int i = 3; i * i <= n; i += 2) if (n % i == 0) return 0;
    return 1;
}

int main(int argc, char **argv) {
    int max = 200000; // default work size
    if (argc > 1) max = atoi(argv[1]);

    int steps = 100;
    emotibar_init(steps);

    int chunk = max / steps;
    if (chunk < 1) chunk = 1;

    int total_primes = 0;
    for (int s = 0; s <= steps; ++s) {
        int start = s * chunk;
        int end = (s == steps) ? max : start + chunk - 1;
        for (int n = start; n <= end; ++n) {
            if (is_prime(n)) ++total_primes;
        }
        char msgbuf[64];
        snprintf(msgbuf, sizeof(msgbuf), "primes=%d", total_primes);
        emotibar_set_message(msgbuf);
        emotibar_update(s);
    }

    emotibar_finish();
    printf("Found %d primes up to %d\n", total_primes, max);
    return 0;
}
