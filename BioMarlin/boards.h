#ifndef BOARDS_H
#define BOARDS_H

#define BOARD_UNKNOWN -1

#define BOARD_RAMPS_OLD         3    // MEGA/RAMPS up to 1.2
#define BOARD_RAMPS_13_EFB      33   // RAMPS 1.3 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_13_EEB      34   // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_13_EFF      35   // RAMPS 1.3 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_13_EEF      36   // RAMPS 1.3 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_13_SF       38   // RAMPS 1.3 (Power outputs: Spindle, Controller Fan)
#define BOARD_RAMPS_14_EFB      43   // RAMPS 1.4 (Power outputs: Hotend, Fan, Bed)
#define BOARD_RAMPS_14_EEB      44   // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Bed)
#define BOARD_RAMPS_14_EFF      45   // RAMPS 1.4 (Power outputs: Hotend, Fan0, Fan1)
#define BOARD_RAMPS_14_EEF      46   // RAMPS 1.4 (Power outputs: Hotend0, Hotend1, Fan)
#define BOARD_RAMPS_14_SF       48   // RAMPS 1.4 (Power outputs: Spindle, Controller Fan)

#define BOARD_SANGUINOLOLU_11   6    // Sanguinololu < 1.2
#define BOARD_SANGUINOLOLU_12   62   // Sanguinololu 1.2 and above

#define MB(board) (MOTHERBOARD==BOARD_##board)

#endif //__BOARDS_H
