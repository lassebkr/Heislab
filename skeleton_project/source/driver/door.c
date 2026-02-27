#include "door.h"
#include "stdbool.h"
#include "elevio.h"
#include "time.h"

static clock_t door_open_time;
static double duration = 3.0;

void door_timer_start() {
    door_open_time = clock();
}

bool door_timer_expired() {
    clock_t now = clock();
    double elapsed_time = (double)(now - door_open_time) / CLOCKS_PER_SEC;
    return elapsed_time >= duration;
}

