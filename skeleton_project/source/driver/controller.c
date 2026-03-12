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
        if (fsm_get_state() != STATE_EMERGENCY_STOP && elevio_stopButton() == 1) {
            printf("STOP\n");
            fsm_handle_event(EVENT_EMERGENCY_STOP_PRESSED);
        } 
        
        if (fsm_get_state() != STATE_EMERGENCY_STOP) {
            orders_fetch();
        }

        if (fsm_floor_reached()) {
            if (fsm_get_current_floor() != fsm_get_previous_floor()) {
                fsm_update_floor_lights();
            }
        }
       
        State state = fsm_get_state();

        switch(state) {
            case STATE_MOVING_UP:
                if (elevio_floorSensor() != -1) {
                    fsm_set_previous_floor(elevio_floorSensor());
                }
                if (orders_should_stop_at_floor(elevio_floorSensor())) {
                    fsm_handle_event(EVENT_FLOOR_REACHED);
                }
                break;
            case STATE_MOVING_DOWN:
                if (elevio_floorSensor() != -1) {
                    fsm_set_previous_floor(elevio_floorSensor());
                }
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
                if (elevio_obstruction()){
                    door_timer_start();
                }
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