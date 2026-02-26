#include "elevio.h"
#include "controller.h"
#include "fsm.h"
#include <time.h>

void controller_test_elevator() {
    fsm_initialize();
    elevio_motorDirection(DIRN_UP);

    while (1) {
        int floor = elevio_floorSensor();
        if (floor = 2) {
            fsm_transition_to(STATE_MOVING_DOWN);
        } else {
            if (floor = 0) {
                fsm_transition_to(STATE_MOVING_UP);
            }
        }
    }   

    nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
}