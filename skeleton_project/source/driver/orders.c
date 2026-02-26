#include <stdio.h>
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