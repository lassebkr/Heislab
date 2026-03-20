#pragma once
#include "stdio.h"
#include "stdbool.h"
#include "elevio.h"

void orders_fetch(void);
void orders_clear(void);
void orders_clear_orders_at_floor(int floor);

bool orders_should_stop_at_floor(int floor);
bool orders_should_go_up(void);
bool orders_should_go_down(void);
bool orders_pending_orders(void);


