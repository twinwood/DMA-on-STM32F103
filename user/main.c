#include "stm32f10x.h"
#include "led.h"
#include "system.h"
#include "gui.h"
#include "flash.h"
#include "key.h"
#include "usart.h"
#include "dma.h"

/****************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/
uint8_t Buff[4096];

int main(void)
{
    uint8_t ledState, showData[5] = {0, 0, 0, 0, 0};
    uint16_t num;
	uint32_t i;
    
    /* 初始化 */
    SYSTEM_SetClock(72); 
    TFT_Init();
    FLASH_Init();
    LED_Config();
    KEY_Config();
    USART1_Config(9600);
    /* Initialise DMA1 Channel 4，ext address for UART Address*/
    DMA_Config(DMA1_Channel4, (uint32_t)&USART1->DR, (uint32_t)Buff, 4096);

    /* 彩屏显示初始化 */
    TFT_ClearScreen(BLACK);

    GUI_Show12Char(90, 21, "TESTING on DMA", RED, BLACK);
    GUI_Show12Char(60, 42, "Twinwood Smartech Hub", RED, BLACK);
    GUI_Show12Char(60, 63, "DMA UART transmit", RED, BLACK);
    GUI_Show12Char(0, 84, "Balance Senf Buffer：", RED, BLACK);
    GUI_Show12Char(0, 105, "Press Up for DMA UART Send, send 9 of Byte 4096", BLUE, BLACK);   

    for(i=0; i<4096; i++)
    {
        Buff[i] = '9';
    }
	while(1)
	{
        i++;       
        if(i > 0x5FF)
        {   
            i = 0; 
            if(ledState == 0xFE)
            {
                ledState = 0xFF;
            }
            else
            {
                ledState = 0xFE;
            }
            LED_SetState(ledState);
        }

        /* Kay Selection */
        if(KEY_Scan() == KEY_UP)
        {
            USART1->CR3 |= 0x0080;   //Enable USART1 to send with DMA使能串口1的DMA发送 
			DMA_Enable(DMA1_Channel4, 4096);//Start first DMA send
        }

        /*　Clear Flag　*/
        if(DMA1->ISR & (1 << 13))	//Check id channel 4 is send completed 
		{
            DMA1->ISR |= (1 << 13); //Clear Channel 4 completed, set 1 to clear the flag bit
        }

        /* Display Balance byte */
        num = DMA1_Channel4->CNDTR;//Obtain current balance byte buffer
        num = num * 100 / 4096;
        
        showData[0] = (num % 1000 /100) + '0';
        showData[1] = (num % 100 /10) + '0';
        showData[2] = (num % 10) + '0';
        showData[3] = '%';
        GUI_Show12Char(80, 84, showData, RED, BLACK);
	}    
}

