#include "lwrb.h"
#include "shell_port.h"
#include "shell.h"
#include "usart.h"
#include "stm32f1xx_hal.h"

Shell shell;
uint8_t shell_buffer[512];
uint8_t g_Uart1RxData = 0;

#define RB_SHELL_RXBUF_SIZE 64
#define RB_SHELL_TXBUF_SIZE 512

lwrb_t rbShellRx;
uint8_t rbShellRxBuf[RB_SHELL_RXBUF_SIZE + 1];
lwrb_t rbShellTx;
uint8_t rbShellTxBuf[RB_SHELL_TXBUF_SIZE + 1];

uint8_t shellTxRdy = 1;

signed short shellGetCmd(char *data, unsigned short len)
{
    return lwrb_read(&rbShellRx, data, len);
}

signed short shellSendData(char *data, unsigned short len)
{    
    return lwrb_write(&rbShellTx, data, len);
}

void userShellInit(void)
{
    HAL_UART_Receive_IT(&huart1, &g_Uart1RxData, 1);
    shell.read = shellGetCmd;
    shell.write = shellSendData;

    lwrb_init(&rbShellRx, rbShellRxBuf, sizeof(rbShellRxBuf));
    lwrb_init(&rbShellTx, rbShellTxBuf, sizeof(rbShellTxBuf));

    shellInit(&shell, (char *)shell_buffer, sizeof(shell_buffer));
}

void shellHld(void)
{
    shellTask(&shell);

    if(shellTxRdy == 1 && lwrb_get_free(&rbShellTx) < RB_SHELL_TXBUF_SIZE)
    {
        HAL_UART_Transmit_IT(&huart1, 
            lwrb_get_linear_block_read_address(&rbShellTx),1);
        shellTxRdy = 0;
    }
}

Shell *getShellIns(void)
{
    return &shell;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        shellTxRdy = 1;
        lwrb_skip(&rbShellTx, 1);

        if(shellTxRdy == 1 && lwrb_get_free(&rbShellTx) < RB_SHELL_TXBUF_SIZE)
        {
            HAL_UART_Transmit_IT(&huart1, 
                lwrb_get_linear_block_read_address(&rbShellTx),1);
            shellTxRdy = 0;
        }
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart1)
    {
        HAL_UART_Receive_IT(&huart1, &g_Uart1RxData, 1);
        lwrb_write(&rbShellRx, &g_Uart1RxData, sizeof(g_Uart1RxData));
    }
}
