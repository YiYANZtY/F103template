#include "shell_port.h"
#include "shell.h"

int func(int argc, char *argv[])
{
    shellPrint(shellGetCurrent(), "%dparameter(s)\r\n", argc);
    for (char i = 1; i < argc; i++)
    {
    	shellPrint(shellGetCurrent(), "%s\r\n", argv[i]);
    }
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_MAIN), func, func, test);
