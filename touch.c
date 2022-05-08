/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 *
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * touch.c - Touchscreen functions
 */

#include <stdio.h>
#include "touch.h"
#include "protocol.h"

extern int ySize;
extern short scaley[512];

/**
 * touch_init() - Set up touch screen
 */
void touch_init(void)
{
}

/**
 * touch_main() - Main loop for touch screen
 */
void touch_main(int x, int y)
{
    padPt Coord;

    Coord.x = x;
    if (ySize < 512)
        Coord.y = scaley[y];
    else
        Coord.y = y ^ 0x1FF;

    Touch(&Coord);
}

/**
 * touch_allow - Set whether touchpanel is active or not.
 */
void touch_allow(padBool allow)
{
}

/**
 * handle_mouse - Process mouse events and turn into scaled touch events
 */
void handle_mouse(void)
{
}

/**
 * touch_hide() - hide the mouse cursor
 */
void touch_hide(void)
{
}

/**
 * touch_done() - Stop the mouse driver
 */
void touch_done(void)
{
}

