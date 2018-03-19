/**
 * Configuration.h
 *
 * Basic settings such as:
 *
 * - Type of electronics
 * - Type of temperature sensor
 * - Printer geometry
 * - Endstop configuration
 * - LCD controller
 * - Extra features
 *
 * Advanced settings can be found in Configuration_adv.h
 *
 */
#ifndef CONFIGURATION_H
#define CONFIGURATION_H
#define CONFIGURATION_H_VERSION 010100

//===============================================================================================
//=================================== ESSENTIALS ================================================
//===============================================================================================

/** SERIAL PORT
 * Select which serial port on the board will be used for communication with the host.
 * This allows the connection of wireless adapters (for instance) to non-default port pins.
 * Serial port 0 is always used by the Arduino bootloader regardless of this setting.
 * :[0, 1, 2, 3, 4, 5, 6, 7]
 */
#define SERIAL_PORT 0

/** BAUDRATE
 * This setting determines the communication speed of the printer.
 * 250000 bps works in most cases, but you might try a lower speed if
 * you commonly experience drop-outs during host printing.
 * :[2400, 9600, 19200, 38400, 57600, 115200, 250000]
 */
#define BAUDRATE 250000

/** MOTHERBOARD
 * Set the electronics board that will be controlling the printer
 */
#ifndef MOTHERBOARD
#define MOTHERBOARD BOARD_RAMPS_14_EFB
#endif //MOTHERBOARD

/** EXTRUDERS
 *  Define the number of extruders the printer is equipped with.
 */
#define EXTRUDERS 1

/** POWER_SUPPLY
 * Select your power supply here. Use 0 if you haven't connected the PS_ON_PIN
 *
 * 0 = No Power Switch
 * 1 = ATX
 * 2 = X-Box 360 203Watts (the blue wire connected to PS_ON and the red wire to VCC)
 *
 * :{ 0:'No power switch', 1:'ATX', 2:'X-Box 360' }
 */
#define POWER_SUPPLY 0

#if POWER_SUPPLY > 0
  // Enable this option to leave the PSU off at startup.
  // Power to steppers and heaters will need to be turned on with M80.
  //#define PS_DEFAULT_OFF
#endif

//===============================================================================================
//=================================== MOVEMENT SETTINGS =========================================
//===============================================================================================
/** DEFAULT_AXIS_STEPS_PER_UNIT (steps/mm)
 * Override with M92
 *                                      X, Y, Z, E0 [, E1[, E2[, E3[, E4]]]]
 */
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80, 80, 4000, 500 }

/**
 * Default Max Feed Rate (mm/s)
 * Override with M203
 *                                      X, Y, Z, E0 [, E1[, E2[, E3[, E4]]]]
 */
#define DEFAULT_MAX_FEEDRATE          { 300, 300, 5, 25 }

/**
 * Default Max Acceleration (change/s) change = mm/s
 * (Maximum start speed for accelerated moves)
 * Override with M201
 *                                      X, Y, Z, E0 [, E1[, E2[, E3[, E4]]]]
 */
#define DEFAULT_MAX_ACCELERATION      { 3000, 3000, 100, 10000 }

/**
 * Default Acceleration (change/s) change = mm/s
 * Override with M204
 *
 *   M204 P    Acceleration
 *   M204 R    Retract Acceleration
 *   M204 T    Travel Acceleration
 */
#define DEFAULT_ACCELERATION          3000    // X, Y, Z and E acceleration for printing moves
#define DEFAULT_RETRACT_ACCELERATION  3000    // E acceleration for retracts
#define DEFAULT_TRAVEL_ACCELERATION   3000    // X, Y, Z acceleration for travel (non printing) moves

/**
 * Default Jerk (mm/s)
 * Override with M205 X Y Z E
 *
 * "Jerk" specifies the minimum speed change that requires acceleration.
 * When changing speed and direction, if the difference is less than the
 * value set here, it may happen instantaneously.
 */
#define DEFAULT_XJERK                 20.0
#define DEFAULT_YJERK                 20.0
#define DEFAULT_ZJERK                  0.4
#define DEFAULT_EJERK                  5.0

// Homing speeds (mm/m)
#define HOMING_FEEDRATE_XY (50*60)
#define HOMING_FEEDRATE_Z  (4*60)

//=============================================================================
//============================= Additional Features ===========================
//=============================================================================

//
// M100 Free Memory Watcher <<-- Will re-implement this later - Sebastian Freeman 10/19/2017
//
//#define M100_FREE_MEMORY_WATCHER // uncomment to add the M100 Free Memory Watcher for debug purpose

// Use custom serial for Marlin
//#define CUSTOM_SERIAL

//===============================================================================================
//=================================== template ==================================================
//===============================================================================================


#endif // CONFIGURATION_H
