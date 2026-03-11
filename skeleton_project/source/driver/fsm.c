#include "stdbool.h"
#include "fsm.h"
#include "elevio.h"
#include "stdbool.h"
#include "orders.h"
#include "controller.h"
#include "door.h"

#define BETWEEN_FLOORS -1

static State current_state;
static MotorDirection current_direction;
static int previous_floor;
static MotorDirection previous_direction;

/*
Denne funksjone skal initialisere heisen. Den sjekker om den er mellom etasjer, 
hvis den er det kjører dne ned til den første etasjen den finner. 
Når den har funnet etasjen stopper heisen, og går inn i IDLE tilstand.
*/

void fsm_initialize(void) {
    int floor = elevio_floorSensor();

    orders_clear();

    if (floor == BETWEEN_FLOORS) {
        elevio_motorDirection(DIRN_DOWN);
    }

    while (1) {
        floor = elevio_floorSensor();
        if (floor != BETWEEN_FLOORS) {
            fsm_state_IDLE(ENTRY);
            break;
        }
    }
}

State fsm_get_state(void) {
    return current_state;
}

MotorDirection fsm_get_direction(void) {
    return current_direction;
}

int fsm_get_current_floor(void) {
    return elevio_floorSensor();
}

int fsm_get_previous_floor(void) {
    return previous_floor;
}

void fsm_set_previous_floor(int floor) {
    previous_floor = floor;
}

MotorDirection fsm_get_previous_direction(void) {
    return previous_direction;
}

void fsm_set_previous_direction(MotorDirection direction) {
    previous_direction = direction;
}

bool fsm_floor_reached(void) {
    if (elevio_floorSensor() != BETWEEN_FLOORS) {
        return true;
    } else {return false;}
}

void fsm_handle_event(Event event) {
    switch(event) {
        case EVENT_FLOOR_REACHED:
            printf("FLOOR REACHED\n");
            int floor = elevio_floorSensor();
            if (orders_should_stop_at_floor(floor) == true) {
                door_timer_start();
                fsm_transition_to(STATE_DOOR_OPEN);
            } else {
                if (elevio_floorSensor() == 0 || elevio_floorSensor() == N_FLOORS - 1) {
                    fsm_transition_to(STATE_IDLE);
                }
            }
                
            break;
        case EVENT_EMERGENCY_STOP_PRESSED:
            printf("EMERGENCY STOP PRESSED\n");
            fsm_transition_to(STATE_EMERGENCY_STOP);
            orders_clear();
            break;
        case EVENT_EMERGENCY_STOP_RELEASED:
            printf("EMERGENCY STOP RELEASED\n");
            orders_clear();
            if (elevio_floorSensor() == BETWEEN_FLOORS) {
                fsm_transition_to(STATE_IDLE);
            } else {
                door_timer_start();
                fsm_transition_to(STATE_DOOR_OPEN);
            }
            break;
    case EVENT_NEW_ORDER:
            printf("NEW ORDER\n");
            printf("PREVIOUS FLOOR: %d\n", previous_floor);
            printf(orders_should_stop_at_floor(previous_floor) ? "SHOULD STOP AT PREVIOUS FLOOR\n" : "SHOULD NOT STOP AT PREVIOUS FLOOR\n");
            printf(orders_pending_orders() ? "PENDING ORDERS\n" : "NO PENDING ORDERS\n");

            if (elevio_floorSensor() == BETWEEN_FLOORS) {
                printf("HEISEN ER MELLOM ETASJER\n");
                if (orders_should_stop_at_floor(previous_floor)) {
                    printf("GÅR TILBAKE TIL FORRIGE ETASJE!\n");
                    printf("PREVIOUS DIRECTION: %d\n", previous_direction);
                    switch(previous_direction) {
                        case DIRN_DOWN:
                            fsm_transition_to(STATE_MOVING_UP);
                            break;
                        case DIRN_UP:
                            fsm_transition_to(STATE_MOVING_DOWN);
                            break;
                    }
                } else {
                    if (orders_should_go_up() == true) {
                        fsm_transition_to(STATE_MOVING_UP);
                    } else {
                        fsm_transition_to(STATE_MOVING_DOWN);
                        printf("GÅR NEDDOVER\n");
                    }
                }
            } else {
                if (orders_should_stop_at_floor(elevio_floorSensor()) == true) {
                    fsm_transition_to(STATE_DOOR_OPEN);
                } else {
                    if (orders_should_go_up() == true) {
                        fsm_transition_to(STATE_MOVING_UP);
                    } else {
                        fsm_transition_to(STATE_MOVING_DOWN);
                    }
                } 
            }
            break;
        case EVENT_DOOR_TIMEOUT:
            printf("DOOR TIMEOUT\n");
            if (elevio_obstruction() == 1) {
                door_timer_start();
            } else {
                fsm_transition_to(STATE_IDLE);
            }
            break;
    }
}


void fsm_transition_to(State new_state) {
    fsm_call_state_function(current_state, EXIT);
    fsm_call_state_function(new_state, ENTRY);
}

void fsm_call_state_function(State state, Transition transition) {
    switch (state) {
        case STATE_IDLE:
            fsm_state_IDLE(transition);
            break;
        case STATE_DOOR_OPEN:
            fsm_state_DOOR_OPEN(transition);
            break;
        case STATE_MOVING_UP:
            fsm_state_MOVING_UP(transition);
            break;
        case STATE_MOVING_DOWN:
            fsm_state_MOVING_DOWN(transition);
            break;
        case STATE_EMERGENCY_STOP:
            fsm_state_EMERGENCY_STOP(transition);
    }
}

/*
FUnksjonene nedenfor skal sette lys og motorretning i henhold til tilstanden heisen er i.
*/


void fsm_state_IDLE(Transition transition) {
        switch(transition) {
        case ENTRY:
            current_state = STATE_IDLE;
            elevio_motorDirection(DIRN_STOP);
            printf("IDLE\n");
            break;
        case EXIT:

            break;
    }
}

void fsm_state_DOOR_OPEN(Transition transition) {
    switch (transition) {
        case ENTRY:
            current_state = STATE_DOOR_OPEN;
            printf("DOOR OPEN\n");
            elevio_motorDirection(DIRN_STOP);
            elevio_doorOpenLamp(1);
            orders_clear_orders_at_floor(elevio_floorSensor());
            break;
        case EXIT:
            elevio_doorOpenLamp(0);
            break;
    }
}

void fsm_state_MOVING_UP(Transition transition) {
        switch(transition) {
        case ENTRY:
            current_state = STATE_MOVING_UP;
            printf("MOVING UP\n");
            elevio_motorDirection(DIRN_UP);
            break;
        case EXIT:
            elevio_motorDirection(DIRN_STOP);
            fsm_set_previous_direction(DIRN_UP);
            break;
    }
}

void fsm_state_MOVING_DOWN(Transition transition) {
        switch(transition) {
        case ENTRY:
            current_state = STATE_MOVING_DOWN;
            printf("MOVING DOWN\n");
            elevio_motorDirection(DIRN_DOWN);
            break;
        case EXIT:
            elevio_motorDirection(DIRN_STOP);
            fsm_set_previous_direction(DIRN_DOWN);
            break;
    }
}

void fsm_state_EMERGENCY_STOP(Transition transition) {
        switch(transition) {
        case ENTRY:
            current_state = STATE_EMERGENCY_STOP;
            printf("EMERGENCY STOP\n");
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);
            if (fsm_floor_reached() == true) {
                door_timer_start();
                elevio_doorOpenLamp(1);
            }
            break;
        case EXIT:
            elevio_stopLamp(0);
            break;
    }
}
