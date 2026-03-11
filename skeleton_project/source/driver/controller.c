#include "elevio.h"
#include "controller.h"
#include "fsm.h"
#include "orders.h"
#include "door.h"
#include <time.h>
#include <stdio.h> // needed for printf in test function

void controller_test_elevator() {
    fsm_initialize();
    elevio_motorDirection(DIRN_UP);

    while (1) {
        int floor = elevio_floorSensor();
        if (floor == 2) {               // use comparison, not assignment
            fsm_transition_to(STATE_MOVING_DOWN);
        } else if (floor == 1) {
            fsm_transition_to(STATE_MOVING_UP);
        }
    }   

    nanosleep(&(struct timespec){0, 20*1000*1000}, NULL);
}

void controller_run_elevator(void) {
    elevio_init();
    fsm_initialize();

    while (1) {
        if (elevio_stopButton()) {
            printf("STOP\n");
            fsm_handle_event(EVENT_EMERGENCY_STOP_PRESSED);
        } 
        
        orders_fetch();
        State state = fsm_get_state();

        switch(state) {
            case STATE_MOVING_UP:
                if (orders_should_stop_at_floor(elevio_floorSensor())) {
                    fsm_handle_event(EVENT_FLOOR_REACHED);
                }

                break;
            case STATE_MOVING_DOWN:
                if (orders_should_stop_at_floor(elevio_floorSensor())) {
                    fsm_handle_event(EVENT_FLOOR_REACHED);
                }
                break;
            case STATE_IDLE:
                if (orders_pending_orders()) {
                    printf("PENDING ORDERS\n");
                    fsm_handle_event(EVENT_NEW_ORDER);
                }
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