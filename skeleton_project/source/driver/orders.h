#pragma once
#include "stdio.h"
#include "stdbool.h"
#include "elevio.h"

/**
 * @file orders.h
 * @brief Module for handling elevator orders.
 * This module provides functions to fetch, clear, and determine the next actions based on the orders received from the elevator buttons.
 * It interacts with the elevator hardware through the elevio interface and uses the FSM module to determine the current state of the elevator.
 */

void orders_fetch(void);
void orders_print(void);
void go_to_floor(void);
void orders_clear(void);
void orders_clear_orders_at_floor(int floor);

bool orders_should_stop_at_floor(int floor);
bool orders_should_go_up(void);

MotorDirection orders_choose_direction(void);

bool orders_check_buttons_at_floor(int floor);
