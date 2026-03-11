#include "elevio.h"
#include "controller.h"
#include "fsm.h"
#include "orders.h"
#include "door.h"
#include <time.h>

void controller_test_elevator() {
    fsm_initialize();
    elevio_motorDirection(DIRN_UP);

    while (1) {
        int floor = elevio_floorSensor();
        if (floor = 2) {
            fsm_transition_to(STATE_MOVING_DOWN);
        } else {
            if (floor = 1) {
                fsm_transition_to(STATE_MOVING_UP);
            }
        }
    }   

    nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
}

void controller_run_elevator(void) {
    elevio_init();
    fsm_initialize();

    while (1) {
        if (elevio_stopButton() == 1) {
            printf("STOP");
            fsm_handle_event(EVENT_EMERGENCY_STOP_PRESSED);
        } else {
            if (fsm_floor_reached()) {
                fsm_handle_event(EVENT_FLOOR_REACHED);
            }
        }

        State state = fsm_get_state();

        if (state != STATE_EMERGENCY_STOP) {
            orders_fetch();
        }

        switch(state) {
            case STATE_IDLE:
                fsm_handle_event(EVENT_NEW_ORDER);
                break;
            case STATE_DOOR_OPEN:
                if (door_timer_expired()) {
                    fsm_handle_event(EVENT_DOOR_TIMEOUT);
                }
                break;
            case STATE_EMERGENCY_STOP:
                if (elevio_stopButton() == 0) {
                    fsm_handle_event(EVENT_EMERGENCY_STOP_RELEASED);
                }
                break;
            
        }
        nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);

    }
}