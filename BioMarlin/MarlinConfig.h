#ifndef MARLIN_CONFIG_H
#define MARLIN_CONFIG_H

#include "fastio.h"
#include "macros.h"
#include "boards.h"
#include "Version.h"
#include "Configuration.h"
#include "Configuration_adv.h"
#include "pins.h"

#if ENABLED(CUSTOM_SERIAL)
  // trick to disable the standard HWserial using idempotency
  // and the HardwareSerial_h header file's header guard. Clever!!
  #define HardwareSerial_h 
#endif // CUSTOM_SERIAL

#include "Arduino.h"
#include "Conditionals_post.h"
#include "SanityCheck.h" // Need to add to sanity check

#endif // MARLIN_CONFIG_H

