#include "shell_port.h"
#include "shell.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

Shell shell;
uint8_t shell_buffer[512];
char g_Uart1RxData = 0;

unsigned short shellGetCmd(char *data, unsigned short len)
{
    HAL_UART_Receive_IT(&huart1, &g_Uart1RxData, 1);
    data = &g_Uart1RxData;
    return 1;
}

unsigned short shellSendData(char *data, unsigned short len)
{
    HAL_UART_Transmit(&huart1, data, len, 0xFFFF);
    return len;
}

void userShellInit(void)
{
    HAL_UART_Receive_IT(&huart1, &g_Uart1RxData, 1);
    shell.read = shellGetCmd;
    shell.write = shellSendData;

    shellInit(&shell, shell_buffer, sizeof(shell_buffer));
}

Shell *getShellIns(void)
{
    return &shell;
}

