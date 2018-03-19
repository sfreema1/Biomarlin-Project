#ifndef __ENUM_H__
#define __ENUM_H__

#include "MarlinConfig.h"

/**
 * Debug flags
 * Not yet widely applied
 */
enum DebugFlags {
  DEBUG_NONE          = 0,
  DEBUG_ECHO          = _BV(0), ///< Echo commands in order as they are processed
  DEBUG_INFO          = _BV(1), ///< Print messages for code that has debug output
  DEBUG_ERRORS        = _BV(2), ///< Not implemented
  DEBUG_DRYRUN        = _BV(3), ///< Ignore temperature setting and E movement commands
  DEBUG_COMMUNICATION = _BV(4), ///< Not implemented
  DEBUG_LEVELING      = _BV(5), ///< Print detailed output for homing and leveling
  DEBUG_MESH_ADJUST   = _BV(6), ///< UBL bed leveling
  DEBUG_ALL           = 0xFF
};

#endif // __ENUM_H__
