/*
 * Display function definitions for ChipKit Uno32 sheild.
 *
 * @author h4xxel
 * https://github.com/is1200-example-projects/hello-display
 *
 * Modified by: Matas Kairaitis & Marko Lazic
 * @version 2016-12-12
 */

#ifndef HEADER_DISPLAY
#define HEADER_DISPLAY

/*
 * Initialises the display pins.
 */
void display_init();

/*
 * Displays a string on the screen on the given line.
 */
void display_string(int line, char *s);

/*
 * Updates the display.
 */
void display_update();

/*
 * Converts ints to strings.
 */
char *itoa (int value, char *result, int base);

#endif
