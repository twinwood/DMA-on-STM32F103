#include "nixietube.h"

/*  数码管从1~F的位码 */
const uint8_t NixieTube_code[16] = {
0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,
0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};

/****************************************************************************
* Function Name  : NIXIETUBE_Config
* Description    : 初始化数码管的IO口.
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void NIXIETUBE_Config(void)
{   
    /* 开启GPIOC和GPIOF时钟 */
    RCC->APB2ENR |= 0x00000010 | 0x00000080; 

    /* PC0~PC7设置为通用推挽输出，50MHZ速度 */
    GPIOC->CRL &= 0x00000000;
    GPIOC->CRL |= 0x33333333;
    
    /* PF6~PF9设置为通用开漏输出 */
    GPIOF->CRL &= 0x00FFFFFF;
    GPIOF->CRL |= 0x77000000;
    GPIOF->CRH &= 0xFFFFFF00;
    GPIOF->CRH |= 0x00000077;
    
}

/****************************************************************************
* Function Name  : NIXIETUBE_Display
* Description    : 数码管显示4位数据
* Input          : *num：数码管显示的四位数据指针
* Output         : None
* Return         : None
****************************************************************************/

void NIXIETUBE_Display(uint8_t *num)
{
    uint8_t st;

    /* 初始化IO */
    NixieTube_1_SET;
    NixieTube_2_SET;
    NixieTube_3_SET;
    NixieTube_4_SET;

    /* 选择开始显示位数，用于设置高位为0时不显示 */
    if(num[3] != 0)
    {
        st = 4;    
    }
    else if(num[2] != 0)
    {
        st = 3;
    }
    else if(num[1] != 0)
    {
        st = 2;
    }
    else if(num[0] != 0)
    {
        st = 1;
    }
    else
    {
        st = 0;
    }

    /* 开始显示 */
    switch (st)
    {
        case(4):   
            /* 显示第四位数码管 */
            NixieTube(0x00);    //消隐
            NixieTube_4_CLR;
            NixieTube(NixieTube_code[num[3]]);
            NixieTube_4_SET;

        case(3):
            /* 显示第三位数码管 */
            NixieTube(0x00);    //消隐
            NixieTube_3_CLR;
            NixieTube(NixieTube_code[num[2]]);
            NixieTube_3_SET;

        case(2):
            /* 显示第二位数码管 */
            NixieTube(0x00);    //消隐
            NixieTube_2_CLR;
            NixieTube(NixieTube_code[num[1]]);
            NixieTube_2_SET;

       case(1):    
            /* 显示第一位数码管 */
            NixieTube(0x00);    //消隐
            NixieTube_1_CLR;
            NixieTube(NixieTube_code[num[0]]);
            NixieTube_1_SET;

       default:
            break;       
    }      
}












