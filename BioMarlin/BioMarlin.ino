/**
 * BioMarlin 3D Printer Firmware
 * Ye Lab 2017
 * 
 * Based on Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * 
 * Based on Sprinter and grbl
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 * 
 */

 /**
  * Log:
  * 10/20/2017 - Added CUSTOM_SERIAL token to Configuration.h file. I need make sure serial-related functions
  * are not comprimised.
  */
 
#include "Marlin.h"

/** COMPILING
 * Below is a summary of what happens when the program is compiled:
 * 1. This .ino file calls Marlin.h header file
 * 
 * 2. The Marlin.h contains several references to C and Arduino libraries, as well
 *    custom header files listed below:
 *    
 *    MarlinConfig.h
 *    gcode.h <-- If enabled
 *    enum.h
 *    types.h
 *    fastio.h
 *    utility.h
 *    serial.h
 *    printcount.h or stopwatch.h
 *    
 *    Marlin.h also declares the following  functions:
 *    void idle(); 
 *    void manage_inactivity();
 *    void enable_all_steppers();
 *    void disable_e_steppers();
 *    void disable_all_steppers();
 *    void FlushSerialRequestResend();
 *    void ok_to_send()
 *    void kill();
 *    void quickstop_stepper();
 *    
 *    
 *    Marlin.h defines the following macros for the following tasks:
 *      - Enabling stepper motors
 *      - Axis token organization
 *      - Debugging flags
 */

 /** OPERATION
  * Below is a summary of the loop function of Marlin
  * 1. First, void setup() in Marlin_main.cpp is called (*Described later*)
  * 2. Void loop(), which is called next, does the following actions in an infinite loop:
  *   - Gets available commands
  *   - Processes the next command
  *   - Updates the commands in the queue (and their indexing)
  *   - Checks endstops
  *   - Goes idle
  * 
  */
