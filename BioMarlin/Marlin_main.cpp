#include "Marlin.h"

#include "language.h"
/**
   GCode Command Queue
   A simple ring buffer of BUFSIZE command strings.

   Commands are copied into this buffer by the command injectors
   (immediate, serial, sd card) and they are processed sequentially by
   the main loop. The process_next_command function parses the next
   command and hands off execution to individual handler functions.
*/
bool Running = true;

uint8_t marlin_debug_flags = DEBUG_NONE;

/**
 * Cartesian Current Position
 *   Used to track the logical position as moves are queued.
 *   Used by 'line_to_current_position' to do a move after changing it.
 *   Used by 'SYNC_PLAN_POSITION_KINEMATIC' to update 'planner.position'.
 */
float current_position[XYZE] = { 0.0 };

/**
 * Cartesian Destination
 *   A temporary position, usually applied to 'current_position'.
 *   Set with 'gcode_get_destination' or 'set_destination_to_current'.
 *   'line_to_destination' sets 'current_position' to 'destination'.
 */
float destination[XYZE] = { 0.0 };

/**
 * axis_homed
 *   Flags that each linear axis was homed.
 *   XYZ on cartesian, ABC on delta, ABZ on SCARA.
 *
 * axis_known_position
 *   Flags that the position is known in each linear axis. Set when homed.
 *   Cleared whenever a stepper powers off, potentially losing its position.
 */
bool axis_homed[XYZ] = { false }, axis_known_position[XYZ] = { false };

/**
 * GCode line number handling. Hosts may opt to include line numbers when
 * sending commands to Marlin, and lines will be checked for sequentiality.
 * M110 N<int> sets the current line number.
 */
static long gcode_N, gcode_LastN, Stopped_gcode_LastN = 0;

/**
 * GCode Command Queue
 * A simple ring buffer of BUFSIZE command strings.
 *
 * Commands are copied into this buffer by the command injectors
 * (immediate, serial, sd card) and they are processed sequentially by
 * the main loop. The process_next_command function parses the next
 * command and hands off execution to individual handler functions.
 */
uint8_t commands_in_queue = 0; // Count of commands in the queue
static uint8_t cmd_queue_index_r = 0, // Ring buffer read position
               cmd_queue_index_w = 0; // Ring buffer write position

static char command_queue[BUFSIZE][MAX_CMD_SIZE];

/**
 * Next Injected Command pointer. NULL if no commands are being injected.
 * Used by Marlin internally to ensure that commands initiated from within
 * are enqueued ahead of any pending serial or sd card commands.
 */
static const char *injected_commands_P = NULL;

// Number of characters read in the current line of serial input
static int serial_count = 0;

// For M109 and M190, this flag may be cleared (by M108) to exit the wait loop
volatile bool wait_for_heatup = true;

const char axis_codes[XYZE] = { 'X', 'Y', 'Z', 'E' };

// Inactivity shutdown
millis_t previous_cmd_ms = 0;
static millis_t max_inactive_time = 0;
static millis_t stepper_inactive_time = (DEFAULT_STEPPER_DEACTIVE_TIME) * 1000UL;

static bool send_ok[BUFSIZE];

/**
 * Feed rates are often configured with mm/m
 * but the planner and stepper like mm/s units.
 */
static const float homing_feedrate_mm_s[] PROGMEM = {
  MMM_TO_MMS(HOMING_FEEDRATE_XY), MMM_TO_MMS(HOMING_FEEDRATE_XY),
  MMM_TO_MMS(HOMING_FEEDRATE_Z), 0
};

void gcode_line_error(const char* err, bool doFlush = true){
  SERIAL_ERROR_START();
  serialprintPGM(err);
  SERIAL_ERRORLN(gcode_LastN);
  //Serial.println(gcode_N);
  if (doFlush) FlushSerialRequestResend();
  serial_count = 0;
}

void kill(){
  SERIAL_ERROR_START();
  SERIAL_ERRORLNPGM(MSG_ERR_KILLED);
  
}
/**
 * Once a new command is in the ring buffer, call this to commit it
 */
inline void _commit_command(bool say_ok) {
  send_ok[cmd_queue_index_w] = say_ok;
  if (++cmd_queue_index_w >= BUFSIZE) cmd_queue_index_w = 0;
  commands_in_queue++;
}
/**
 * Copy a command from RAM into the main command buffer.
 * Return true if the command was successfully added.
 * Return false for a full buffer, or if the 'command' is a comment.
 */
inline bool _enqueuecommand(const char* cmd, bool say_ok=false) {
  if (*cmd == ';' || commands_in_queue >= BUFSIZE) return false;
  strcpy(command_queue[cmd_queue_index_w], cmd);
  _commit_command(say_ok);
  return true;
}

inline void get_serial_commands() {
  static char serial_line_buffer[MAX_CMD_SIZE];
  static bool serial_comment_mode = false;

  // If the command buffer is empty for too long
  // send "wait" to indicate Marlin is still waiting.
  #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
    static millis_t last_command_time = 0;
    const millis_t ms = millis(); 
    if (commands_in_queue == 0 && !MYSERIAL.available() && ELAPSED(ms, last_command_time + NO_TIMEOUTS)){
      SERIAL_ECHOLNPGM(MSG_WAIT);
      last_command_time = ms;
    }
   #endif // NO_TIMEOUTS
   /**
    * Loop while serial characters are incoming and the queue is not full
    */
    while (commands_in_queue < BUFSIZE && MYSERIAL.available() > 0) {
      char serial_char = MYSERIAL.read();
      /**
       * If the character ends the line
       */
       if (serial_char == '\n' || serial_char == '\r'){
        serial_comment_mode = false; // end of line == end of comment
        if (!serial_count) continue; // skip empty lines
        serial_line_buffer[serial_count] = 0; // terminate string
        serial_count = 0; // reset buffer

        char* command = serial_line_buffer;

        while (*command == ' ') command++; // skip any leading spaces
        char *npos = (*command == 'N') ? command : NULL, // Require the N parameter to start line
        *apos = strchr(command,'*');

        if (npos) {
          /*
           * Checks for 11, which is the "Set line" M command
           */
          bool M110 = strstr_P(command,PSTR("M110")) != NULL;
          
          if (M110){
            char* n2pos = strchr(command + 4, 'N');
            if (n2pos) npos = n2pos;
          }

          gcode_N = strtol(npos + 1, NULL, 10);

          if (gcode_N != gcode_LastN + 1 && !M110){
            gcode_line_error(PSTR(MSG_ERR_LINE_NO));
            return;
          }

          if (apos) {
            byte checksum = 0, count = 0;
            while (command[count] != '*') checksum ^= command[count++];

            if (strtol(apos + 1, NULL, 10) != checksum){
              gcode_line_error(PSTR(MSG_ERR_CHECKSUM_MISMATCH));
              return;
            }
            // if no errors, continue parsin
          }
          else {
            gcode_line_error(PSTR(MSG_ERR_NO_CHECKSUM));
            return;
          }

          gcode_LastN = gcode_N;
          // if no errors, continue parsing
        }
        else if (apos) { // No '*' without 'N'
          gcode_line_error(PSTR(MSG_ERR_NO_LINENUMBER_WITH_CHECKSUM), false);
          return;
        }

        // Movement commands alert when stopped
        if (IsStopped()){
          char *gpos = strchr(command, 'G');
          if (gpos) {
            const int codenum = strtol(gpos + 1, NULL, 10);
            switch (codenum) {
              case 0:
              case 1:
              case 2:
              case 3:
                SERIAL_ERRORLNPGM(MSG_ERR_STOPPED);
                break;
            }
          }
        }

        #if DISABLED(EMERGENCY_PARSER)
          // If command was e-stop process now
          if (strcmp(command, "M108") == 0) {
            wait_for_heatup = false;
          }
          if (strcmp(command, "M112") == 0) kill();
          if (strcmp(command, "M410") == 0) { quickstop_stepper(); }
        #endif // EMERGENCY_PARSER

        #if defined(NO_TIMEOUTS) && NO_TIMEOUTS > 0
          last_command_time = ms;
        #endif // TIMEOUTS

        // Add the command to the queue
        _enqueuecommand(serial_line_buffer, true);
       }
       else if (serial_count >= MAX_CMD_SIZE - 1){
        // Keep fetching, but ignore normal characters beyond the max length
        // The command will be injected when EOL is reached
       }
       else if (serial_char == '\\') { // Handle escapes
        if (MYSERIAL.available() > 0) {
          // If we have one more character, copy it over
          serial_char = MYSERIAL.read();
          if (!serial_comment_mode) serial_line_buffer[serial_count++] = serial_char;
        }
        // Otherwise do nothing
       }
       else { // It's not a newline, carriage return or escape char
        if (serial_char == ';') serial_comment_mode = true;
        if (!serial_comment_mode) serial_line_buffer[serial_count++] = serial_char;
       }
       
    } // Queue has space, serial has data
}

void get_available_commands(){
  // Took out check for drain_injected commands request
  get_serial_commands();
}

void process_next_command(){
  char *const current_command = command_queue[cmd_queue_index_r];
  if (DEBUGGING(ECHO)) {
    // SERIAL_ECHO_START();
  }
}

void FlushSerialRequestResend(){
  //char command_queue[cmd_queue_index_r][100]="Resend:"
  MYSERIAL.flush();
  SERIAL_PROTOCOLPGM(MSG_RESEND);
  SERIAL_PROTOCOLLN(gcode_LastN + 1);
  ok_to_send();
}

void ok_to_send(){
  refresh_cmd_timeout();
  if (!send_ok[cmd_queue_index_r]) return;
  SERIAL_PROTOCOLPGM(MSG_OK);
  #if ENABLED(ADVANCED_OK)
    char *p = command_queue[cmd_queue_index_r];
    if (*p == 'n'){
      SERIAL_PROTOCOL(' ');
      SERIAL_ECHO(*p++);
      while (NUMERIC_SIGNED(*p))
        SERIAL_ECHO(*p++);
    }
  #endif // ADVANCED_OK
}

void quickstop_stepper(){
  
}

void setup() {
  MYSERIAL.begin(BAUDRATE);
  SERIAL_PROTOCOLLNPGM("start");
  #if ENABLED(CUSTOM_SERIAL)
    SERIAL_PROTOCOLLNPGM("Custom serial enabled");
  #endif
  
}

void loop() {
  if (commands_in_queue < BUFSIZE) get_available_commands();
  if (commands_in_queue) process_next_command();
  // The queue may be reset by a command handler or by code invoked by idle() within a handler
  if (commands_in_queue) {
    --commands_in_queue;
    if (++cmd_queue_index_r >= BUFSIZE) cmd_queue_index_r = 0;
  }
  //endstops.report_state();
  //idle();
}
