#ifndef LANGUAGE_H
#define LANGUAGE_H

#include "MarlinConfig.h"

#define _UxGT(a) a

// Common LCD messages

  /* nothing here yet */

// Common serial messages
#define MSG_MARLIN "Marlin"

// Serial Console Messages (do not translate those!)

#define MSG_WAIT                            "wait"
#define MSG_ERR_LINE_NO                     "Line Number is not Last Line Number+1, Last Line: "
#define MSG_RESEND                          "Resend: "
#define MSG_OK                              "ok"
#define MSG_ERR_CHECKSUM_MISMATCH           "checksum mismatch, Last Line: "
#define MSG_ERR_NO_CHECKSUM                 "No Checksum with line number, Last Line: "
#define MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM "No Line Number with checksum, Last Line: "
#define MSG_ERR_STOPPED                     "Printer stopped due to errors. Fix the error and use M999 to restart. (Temperature is reset. Set it after restarting)"
#define MSG_ERR_KILLED                      "Printer halted. killed() called!!"

#endif // LANGUAGE_H
