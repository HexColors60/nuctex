/*------------------------------------------------------------------------------
| NuCTE		| command.c
| Author	| Benjamin E Allen
| Date		| 23 November, 2015
|-------------------------------------------------------------------------------
| Overview	| Implement command functions.
\-----------------------------------------------------------------------------*/

#include <string.h>
#include "command.h"
#include "io.h"
#include "gamef.h"

extern int g_isQuit;

void callCommand(char* input) {
	if(strcmp(input, "quit") == 0) {
		quit();
	}
	else {
		printMessage("Invalid Command!");
	}
}