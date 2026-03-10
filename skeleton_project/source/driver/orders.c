#include <stdio.h>
#include "fsm.h"
#include "elevio.h"
#include "orders.h"
#include "stdbool.h"

#define N_BUTTONS 3

bool orders[N_FLOORS][N_BUTTONS];

void orders_fetch(void) {
    for (int i = 0; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++ j) {
            orders[i][j] = elevio_callButton(i, j);
        }
    }
}

void orders_print(void) {
    for (int i = 0; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++ j) {
            printf("Order at floor %d, button %d: %s\n", i, j, orders[i][j] ? "Yes" : "No");
        }
    }
}   

int orders_determine_next_floor(void) {
    bool pending_order = false;
        for (int i = 0; i < N_FLOORS; ++i) {
            for (int j = 0; j < N_BUTTONS; ++ j) {
                if (elevio_callButton == 1) {
                    pending_order = true;
                }
        }
    }

    MotorDirection direction = fsm_get_state();

    switch(direction) {
        case DIRN_DOWN:

            break;
        case DIRN_UP:

            break;
        case DIRN_STOP:

            break;
    }

    if (pending_order == false) {
        return -1;
    }

}

void go_to_floor() {
    while (1) {
        if (orders_check_buttons_at_floor(2) == true) {
            if (elevio_floorSensor > 2) {
                fsm_transition_to(STATE_MOVING_DOWN);
                printf("JEG GÅR NED");
            }   else {
                if (elevio_floorSensor < 2) {
                    fsm_transition_to(STATE_MOVING_UP);
                    printf("JEG GÅR OPP");
                }
            }
            break;
        }
    }
    printf("JEG ER HER");
    while (1) {
        if (elevio_floorSensor == 2) {
            elevio_motorDirection(DIRN_STOP);
        }
    }
}


bool orders_check_buttons_at_floor(int floor) {
    for (int i = 0; i < N_BUTTONS; ++i) {
        ButtonType buttontype = (ButtonType)i;
        if (elevio_callButton(floor, buttontype)) {
            return true;  
        }
    }
    return false;
}

void orders_clear(void) {
        for (int i = 0; i < N_FLOORS; ++i) {
            for (int j = 0; j < N_BUTTONS; ++ j) {
                orders[i][j] = 0;
        }
    }
}

bool orders_should_stop_at_floor(int floor) {
    for (int i = 0; i < N_BUTTONS; ++i) {
        if (orders[floor][i] == true) {
            return true;
        }
    }
    return false;
}

bool orders_should_go_up(void) {
    int current_floor = fsm_get_current_floor();
    for (int i = current_floor + 1; i < N_FLOORS; ++i) {
        for (int j = 0; j < N_BUTTONS; ++j) {
            if (orders[i][j] == true) {
                return true;
            }
        }
    }
    return false;
}

MotorDirection orders_choose_direction(void) {

}