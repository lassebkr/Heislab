#pragma once
#include "elevio.h"

typedef struct {
    int floor;
    ButtonType button;
} Order;

void orders_fetch(void);
void orders_print(void);