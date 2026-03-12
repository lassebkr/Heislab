#define _POSIX_C_SOURCE 199309L
#include "door.h"
#include "stdbool.h"
#include "elevio.h"
#include "time.h"

static struct timespec door_open_time;
static double duration = 3.0;

void door_timer_start() {
    clock_gettime(CLOCK_MONOTONIC, &door_open_time);
}

bool door_timer_expired() {
    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);
    
    double elapsed_time = (now.tv_sec - door_open_time.tv_sec) + 
                          (now.tv_nsec - door_open_time.tv_nsec) / 1e9;
    return elapsed_time >= duration;
}

