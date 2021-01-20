#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <termios.h>
#include <unistd.h>

#include "input.h"

// input code from https://stackoverflow.com/questions/29723205/how-to-run-program-whilst-listening-for-user-input-in-c

static struct termios oldSettings;

void kbcleanup( void )
{
    tcsetattr( 0, TCSAFLUSH, &oldSettings );     /* restore old settings */
}

void kbsetup( void )
{
    tcgetattr( 0, &oldSettings );

    struct termios newSettings = oldSettings;

    newSettings.c_lflag &= ~ICANON;   /* disable line-at-a-time input */
    newSettings.c_lflag &= ~ECHO;     /* disable echo */
    newSettings.c_cc[VMIN]  = 0;      /* don't wait for characters */
    newSettings.c_cc[VTIME] = 0;      /* no minimum wait time */

    if ( tcsetattr( 0, TCSAFLUSH, &newSettings ) == 0 ){
        atexit( kbcleanup );    /* restore the terminal settings when the program exits */
    } else {
        fprintf( stderr, "Unable to set terminal mode\n" );
        exit( 1 );
    }
}

int getkey( void )
{
    char c;

    if ( read( STDIN_FILENO, &c, 1 ) == 0 )
        return '\0';
    else
        return c;
}