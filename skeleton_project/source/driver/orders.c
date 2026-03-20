#include <stdio.h>
#include "fsm.h"
#include "elevio.h"
#include "orders.h"
#include "stdbool.h"

#define N_FLOORS 4
#define N_BUTTONS 3

bool orders[N_FLOORS][N_BUTTONS];

void orders_fetch(void) {
    for (int i = 0; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++ j) {
            if (elevio_callButton(i,j)) {
                orders[i][j] = elevio_callButton(i, j);
                elevio_buttonLamp(i, j, elevio_callButton(i, j));
            }
        }
    }
}

void orders_clear(void) {
        for (int i = 0; i < N_FLOORS; ++i) {
            for (int j = 0; j < N_BUTTONS; ++ j) {
                orders[i][j] = 0;
                elevio_buttonLamp(i, j, 0);
        }
    }
}

void orders_clear_orders_at_floor(int floor) {
    if (floor != -1) {
        for (int i = 0; i < N_BUTTONS; ++i) {
            orders[floor][i] = 0;
            elevio_buttonLamp(floor, i, 0);
        }
    }
}

bool orders_should_go_up(void) {
    int floor;

    if (fsm_get_current_floor() != -1) {
        floor = fsm_get_current_floor();
    } else {
        floor = fsm_get_previous_floor();
    }

    for (int i = floor + 1; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++j) {
            if (orders[i][j]) {
                return true;
            }
        }
    }
    return false;
}

bool orders_should_go_down(void) {
    int floor;

    if (fsm_get_current_floor() != -1) {
        floor = fsm_get_current_floor();
    } else {
        floor = fsm_get_previous_floor();
    }

    for (int i = floor - 1; i >= 0; --i) {
        for (int j = 0; j < N_BUTTONS; ++j) {
            if (orders[i][j]) {
                return true;
            }
        }
    }
    return false;
}

bool orders_pending_orders(void) {
    for (int i = 0; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++j) {
            if (orders[i][j] == true) {
                return true;
            }
        }
    }
    return false;
}

bool orders_should_stop_at_floor(int floor) {
    if (floor != -1) {
        switch(fsm_get_state()) {
            case STATE_MOVING_UP:
                if (orders_should_go_up() == true) {
                    if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_CAB]) {
                        return true;
                    } else { return false; } 
                } else {
                    if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_CAB] || orders[floor][BUTTON_HALL_DOWN]) {
                        return true;
                    }
                }
                return false;
                break;

            case STATE_MOVING_DOWN:
                if (orders_should_go_down() == true) {
                    if (orders[floor][BUTTON_HALL_DOWN] || orders[floor][BUTTON_CAB]) {
                        return true;
                    } else { return false; } 
                } else {
                    if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_CAB] || orders[floor][BUTTON_HALL_DOWN]) {
                        return true;
                    }
                }
                return false;
                break;
            case STATE_IDLE:
                if (orders[floor][BUTTON_HALL_UP] || orders[floor][BUTTON_HALL_DOWN] || orders[floor][BUTTON_CAB]) {
                    return true;
                }
                break;
            default:
                break;
        }
    }
    return false;
}