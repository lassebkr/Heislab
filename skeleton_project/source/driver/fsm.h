#pragma once
#include "stdbool.h"
#include "elevio.h"

typedef enum{
    STATE_IDLE,
    STATE_DOOR_OPEN,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_EMERGENCY_STOP
} State;

typedef enum {
    EVENT_EMERGENCY_STOP_PRESSED,
    EVENT_EMERGENCY_STOP_RELEASED,
    EVENT_FLOOR_REACHED,
    EVENT_DOOR_TIMEOUT,
    EVENT_NEW_ORDER
} Event;

typedef enum {
    ACTION_NONE,
    ACTION_START_MOTOR_UP,
    ACTION_START_MOTOR_DOWN,
    ACTION_STOP_MOTOR,
    ACTION_OPEN_DOOR,
    ACTION_CLOSE_DOOR,
} Action;

typedef enum {
    ENTRY,
    EXIT
} Transition;

//Initiialiserer heisen
void fsm_initialize(void);

//Returner nåværende tilstand
State fsm_get_state(void);

//Funksjon som sjekker om en etasje er nådd
bool fsm_floor_reached(void);

//Returnerer nåværende etasje
int fsm_get_current_floor(void);


int fsm_get_previous_floor(void);
void fsm_set_previous_floor(int floor);
MotorDirection fsm_get_previous_direction(void);
void fsm_set_previous_direction(MotorDirection direction);

//Håndterer hendelse, ved å bestemme og gjennomføre state transition, og utføre alle nødvendig exit og entry actions
void fsm_handle_event(Event event);

//Funksjoner som brukes av fsm_handle_event() for gjøre state transition
void fsm_transition_to(State state);
void fsm_call_state_function(State state, Transition transition);

//State funksjoner
void fsm_state_IDLE(Transition transition);
void fsm_state_MOVING_UP(Transition transition);
void fsm_state_MOVING_DOWN(Transition transition);
void fsm_state_DOOR_OPEN(Transition transition);
void fsm_state_EMERGENCY_STOP(Transition transition);
