#ifndef MARLIN_H
#define MARLIN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>

#include "MarlinConfig.h"

#ifdef DEBUG_GCODE_PARSER
  #include "gcode.h"
#endif

#include "serial.h"
#include "Configuration_adv.h"
#include "enum.h"
#include "types.h"

extern uint8_t marlin_debug_flags;
#define DEBUGGING(F) (marlin_debug_flags & (DEBUG_## F))

/**
 * Feedrate scaling and conversion
 */
extern int16_t feedrate_percentage;

#define MMM_TO_MMS(MM_M) ((MM_M)/60.0)
#define MMS_TO_MMM(MM_S) ((MM_S)*60.0)
#define MMS_SCALED(MM_S) ((MM_S)*feedrate_percentage*0.01)

extern millis_t previous_cmd_ms;
inline void refresh_cmd_timeout() { previous_cmd_ms = millis(); }

extern bool Running;
inline bool IsRunning() { return  Running; }
inline bool IsStopped() { return !Running; }

void FlushSerialRequestResend();
void ok_to_send();
void quickstop_stepper();
void kill();

#endif //MARLIN_H
