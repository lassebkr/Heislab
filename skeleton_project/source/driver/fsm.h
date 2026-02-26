#pragma once

typedef enum{
    STATE_INIT,
    STATE_IDLE,
    STATE_DOOR_OPEN,
    STATE_MOVING_UP,
    STATE_MOVING_DOWN,
    STATE_EMERGENCY_STOP
} State;

typedef enum {
    EVENT_START_MOVING_UP,
    EVENT_START_MOVING_DOWN,
    EVENT_FLOOR_REACHED,
    EVENT_DOOR_TIMEOUT
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
    EXIT,
    STAY
} Transition;

//Initiialiserer heisen
void fsm_initialize(void);

//Returner nåværende tilstand
State fsm_get_state(void);

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
