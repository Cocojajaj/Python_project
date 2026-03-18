#ifndef COMMON_H
#define COMMON_H

// --- COLORS ---
#define COLOR_FAIL    0x00
#define COLOR_GREEN   0x01
#define COLOR_RED     0x02
#define COLOR_BLUE    0x03
#define COLOR_MAGENTA 0x04
#define COLOR_YELLOW  0x05
#define COLOR_CYAN    0x06
#define COLOR_WHITE   0xFF

// --- COMMANDS ---
// The system reads 1 byte at a time.
// If byte is 0xA1, it expects the next byte to be a color.
#define CMD_ADD_COLOR     0xA1
#define CMD_EXECUTE_MIX   0xB2
#define CMD_CLEAR_QUEUE   0xC3  // Only implemented in later versions

#endif