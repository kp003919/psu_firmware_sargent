#ifndef APP_H
#define APP_H

#include "measurement.h"
#include "safety.h"
#include "logic.h"
/**
 * @file app.h
 * @brief Main application header defining the control task for the PSU firmware. 
 * This header declares the main control task function `vControlTask`, which is responsible for initializing the measurement, safety, logic, and output modules, and then entering a loop where it continuously reads sensor measurements, checks for faults, updates the logic state, and controls the outputs accordingly. The control task also builds system status and tank level frames to send over CAN to the Control Panel. The implementation of this function will be defined in the corresponding source file (app.c) and is central to the operation of the PSU firmware.
 * @author Muhsin Atto  
 * @date 2026-04-27
 * 
 */


void vControlTask(void *pvParameters);

#endif // APP_H
