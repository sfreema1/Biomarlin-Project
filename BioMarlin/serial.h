#ifndef __SERIAL_H__
#define __SERIAL_H__

#include "MarlinConfig.h"

#if ENABLED(CUSTOM_SERIAL)
  #include "MarlinSerial.h"
  #define MYSERIAL customizedSerial
#else
  #include "HardwareSerial.h"
  #define MYSERIAL Serial
#endif // CUSTOM_SERIAL

extern const char echomagic[] PROGMEM;
extern const char errormagic[] PROGMEM;

#define SERIAL_CHAR(x) ((void)MYSERIAL.write(x))
#define SERIAL_EOL() SERIAL_CHAR('\n')

#define SERIAL_PROTOCOLCHAR(x)              SERIAL_CHAR(x)
#define SERIAL_PROTOCOL(x)                  (MYSERIAL.print(x))
#define SERIAL_PROTOCOL_F(x,y)              (MYSERIAL.print(x,y))
#define SERIAL_PROTOCOLPGM(x)               (serialprintPGM(PSTR(x)))
#define SERIAL_PROTOCOLLN(x)                do{ MYSERIAL.print(x); SERIAL_EOL(); }while(0)
#define SERIAL_PROTOCOLLNPGM(x)             (serialprintPGM(PSTR(x "\n")))
#define SERIAL_PROTOCOLPAIR(name, value)    (serial_echopair_P(PSTR(name),(value)))
#define SERIAL_PROTOCOLLNPAIR(name, value)  do{ SERIAL_PROTOCOLPAIR(name, value); SERIAL_EOL(); }while(0)

#define SERIAL_ECHO_START()            (serialprintPGM(echomagic))
#define SERIAL_ECHO(x)                 SERIAL_PROTOCOL(x)
#define SERIAL_ECHOPGM(x)              SERIAL_PROTOCOLPGM(x)
#define SERIAL_ECHOLN(x)               SERIAL_PROTOCOLLN(x)
#define SERIAL_ECHOLNPGM(x)            SERIAL_PROTOCOLLNPGM(x)
#define SERIAL_ECHOPAIR(name,value)    SERIAL_PROTOCOLPAIR(name, value)
#define SERIAL_ECHOLNPAIR(name, value) SERIAL_PROTOCOLLNPAIR(name, value)
#define SERIAL_ECHO_F(x,y)             SERIAL_PROTOCOL_F(x,y)

#define SERIAL_ERROR_START()           (serialprintPGM(errormagic))
#define SERIAL_ERROR(x)                SERIAL_PROTOCOL(x)
#define SERIAL_ERRORPGM(x)             SERIAL_PROTOCOLPGM(x)
#define SERIAL_ERRORLN(x)              SERIAL_PROTOCOLLN(x)
#define SERIAL_ERRORLNPGM(x)           SERIAL_PROTOCOLLNPGM(x)

// These macros compensate for float imprecision
#define SERIAL_PROTOCOLPAIR_F(name, value)    SERIAL_PROTOCOLPAIR(name, FIXFLOAT(value))
#define SERIAL_PROTOCOLLNPAIR_F(name, value)  SERIAL_PROTOCOLLNPAIR(name, FIXFLOAT(value))
#define SERIAL_ECHOPAIR_F(name,value)         SERIAL_ECHOPAIR(name, FIXFLOAT(value))
#define SERIAL_ECHOLNPAIR_F(name, value)      SERIAL_ECHOLNPAIR(name, FIXFLOAT(value))

void serial_echopair_P(const char* s_P, const char *v);
void serial_echopair_P(const char* s_P, char v);
void serial_echopair_P(const char* s_P, int v);
void serial_echopair_P(const char* s_P, long v);
void serial_echopair_P(const char* s_P, float v);
void serial_echopair_P(const char* s_P, double v);
void serial_echopair_P(const char* s_P, unsigned int v);
void serial_echopair_P(const char* s_P, unsigned long v);

FORCE_INLINE void serial_echopair_P(const char* s_P, uint8_t v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, uint16_t v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, bool v) { serial_echopair_P(s_P, (int)v); }
FORCE_INLINE void serial_echopair_P(const char* s_P, void *v) { serial_echopair_P(s_P, (unsigned long)v); }

void serial_spaces(uint8_t count);
#define SERIAL_ECHO_SP(C)     serial_spaces(C)
#define SERIAL_ERROR_SP(C)    serial_spaces(C)
#define SERIAL_PROTOCOL_SP(C) serial_spaces(C)

//
// Functions for serial printing from PROGMEM. (Saves loads of SRAM.)
//
FORCE_INLINE void serialprintPGM(const char* str) {
  while (char ch = pgm_read_byte(str++)) MYSERIAL.write(ch);
}

#endif // __SERIAL_H__
