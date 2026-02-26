#pragma once
#include "stdio.h"
#include "elevio.h"

void orders_fetch(void);
void orders_print(void);
void go_to_floor(void);
bool orders_check_buttons_at_floor(int floor);
