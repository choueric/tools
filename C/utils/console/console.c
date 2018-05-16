#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>

#include "console.h"

int console_enter_raw(console_t *con)
{
	int ret;

    if ( (ret = tcgetattr(STDIN_FILENO, &con->oldt)) < 0)
		return ret;

    con->newt = con->oldt;

    /*ICANON normally takes care that one line at a time will be processed
    that means it will return if it sees a "\n" or an EOF or an EOL*/
    con->newt.c_lflag &= ~(ICANON | ECHO);

    /*Those new settings will be set to STDIN
    TCSANOW tells tcsetattr to change attributes immediately. */
    return tcsetattr(STDIN_FILENO, TCSANOW, &con->newt);
}

int console_leave_raw(console_t *con)
{
    /*restore the old settings*/
    return tcsetattr(STDIN_FILENO, TCSANOW, &con->oldt);
}
