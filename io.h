/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * io.h - Input/output functions (serial/ethernet)
 */

#ifndef IO_H
#define IO_H

#include <winsock.h>

#define XON  0x11
#define XOFF 0x13

/**
 * io_init() - Set-up the I/O
 */
SOCKET io_init(void);

/**
 * io_send_byte(b) - Send specified byte out
 */
void io_send_byte(unsigned char b);

/**
 * io_main() - The IO main loop
 */
int io_main(void);

/**
 * io_done() - Called to close I/O
 */
void io_done(void);

void io_replay_clear(void);

#endif /* IO_H */
