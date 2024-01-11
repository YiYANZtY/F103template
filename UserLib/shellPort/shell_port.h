#ifndef _SHELL_PORT_H_
#define _SHELL_PORT_H_

#include "stddef.h"

typedef struct shell_def Shell;
#define     SHELL_TASK_WHILE            0

void userShellInit(void);
Shell *getShellIns(void);
void shellHld(void);

#endif /*_SHELL_PORT_H_*/
