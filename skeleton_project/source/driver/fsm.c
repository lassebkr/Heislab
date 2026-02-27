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

/*
Denne funksjone skal initialisere heisen. Den sjekker om den er mellom etasjer, 
hvis den er det kjører dne ned til den første etasjen den finner. 
Når den har funnet etasjen stopper heisen, og går inn i IDLE tilstand.
*/

void fsm_initialize(void) {
    int floor = elevio_floorSensor();

    if (floor == BETWEEN_FLOORS) {
        elevio_motorDirection(DIRN_DOWN);
        elevio_stopLamp(0);
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

bool fsm_floor_reached(void) {
    if (elevio_floorSensor() != BETWEEN_FLOORS) {
        return true;
    } else {return false;}
}

void fsm_handle_event(Event event) {
    switch(event) {
        case EVENT_FLOOR_REACHED:
        int floor = elevio_floorSensor();
            if (orders_should_stop_at_floor(floor) == true) {
                fsm_transition_to(STATE_DOOR_OPEN);
            }

            break;
        case EVENT_EMERGENCY_STOP:
            fsm_transition_to(STATE_EMERGENCY_STOP);
            break;
        case EVENT_NEW_ORDER:
            break;
    }
}

void fsm_transition_to(State new_state) {
    if (new_state == current_state) {
        fsm_call_state_function(current_state, STAY);
    }

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
Nedenfor er funksjoner som skal utføre entry og exit actions for de ulike tilstandene.
Funksjonene tar en Transition som input, som bestemmer om de skal utføre ENTRY eller EXIT actions.
*/


void fsm_state_IDLE(Transition transition) {
        switch(transition) {
        case ENTRY:
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);

            break;
        case EXIT:

            break;
    }
}

void fsm_state_DOOR_OPEN(Transition transition) {
    switch (transition) {
        case ENTRY:
            door_timer_start();
            elevio_doorOpenLamp(1);
            break;
        case EXIT:
            elevio_doorOpenLamp(0);
            break;
    }
}

void fsm_state_MOVING_UP(Transition transition) {
        switch(transition) {
        case ENTRY:
            elevio_motorDirection(DIRN_UP);
            break;
        case EXIT:
            elevio_motorDirection(DIRN_STOP);
            break;
    }
}

void fsm_state_MOVING_DOWN(Transition transition) {
        switch(transition) {
        case ENTRY:
            elevio_motorDirection(DIRN_DOWN);
            break;
        case EXIT:
            elevio_motorDirection(DIRN_STOP);
            break;
    }
}

void fsm_state_EMERGENCY_STOP(Transition transition) {
        switch(transition) {
        case ENTRY:
            elevio_motorDirection(DIRN_STOP);
            elevio_stopLamp(1);
            break;
        case EXIT:
            break;
    }
}
