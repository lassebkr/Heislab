#include "door.h"
#include "stdbool.h"
#include "elevio.h"
#include "time.h"

static time_t door_open_time;
static double duration = 3.0;

void door_timer_start() {
    door_open_time = time(NULL);
}

bool door_timer_expired() {
    time_t now = time(NULL);
    double elapsed_time = difftime(now, door_open_time);
    return elapsed_time >= duration;
}

