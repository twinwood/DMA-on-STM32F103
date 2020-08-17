#include "adctemp.h"

/****************************************************************************
* Function Name  : ADCTEMP_Config
* Description    : ��ʼ���ڲ�ADC
* Input          : None
* Output         : None
* Return         : None
****************************************************************************/

void ADCTEMP_Config(void)
{    
	/* ��ʱ�� */
    RCC->APB2ENR |= 0x00000200;

    RCC->CFGR &= ~(3 << 14);
    RCC->CFGR |= 2 << 14;   //6��Ƶ 72M/6=12,ADC ���ʱ�Ӳ��ܳ��� 14M

    /* ��ʼ��ADC���� */
    ADC1->CR1 &= 0xF0FFFF;     //����ģʽ����
	ADC1->CR1 |= 0 << 16;      //��������ģʽ

    ADC1->CR1 &= ~(1 << 8);    //��ɨ��ģʽ	  
	ADC1->CR2 &= ~(1 << 1);    //����ת��ģʽ
	ADC1->CR2 &= ~(7 << 17);	   
	ADC1->CR2 |= 7 << 17;	   //�������ת��  
	ADC1->CR2 |= 1 << 20;      //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
	ADC1->CR2 &= ~(1 << 11);   //�Ҷ���	 
	ADC1->SQR1 &= ~(0XF << 20);
	ADC1->SQR1 |= 0;           //1��ת���ڹ��������� Ҳ����ֻת����������1 			   
	/* ����ͨ��1�Ĳ���ʱ�� */
	ADC1->SMPR2 &= ~(7 << 3);  //ͨ��1����ʱ�����	  
 	ADC1->SMPR2 |= 7 << 3;     //ͨ��1  239.5����,��߲���ʱ�������߾�ȷ��	 
	ADC1->CR2 |= 1;	           //����ADת����	 
	    
    /* �����ڲ��¶ȴ����� */
    ADC1->CR2 |= 1 << 23;
    
    ADC1->CR2 |= 1 << 3;         //ʹ�ܸ�λУ׼  
	while(ADC1->CR2 & (1 << 3)); //�ȴ�У׼���� 			 
    /* ��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ���������*/ 		 
	ADC1->CR2 |= 1 << 2;         //����ADУ׼	   
	while(ADC1->CR2 & (1 << 2)); //�ȴ�У׼����        
}

/****************************************************************************
* Function Name  : ADCTEMP_GetAdc
* Description    : ��ȡ�¶ȴ�������ADֵ
* Input          : None
* Output         : None
* Return         : ���ض�ȡ����ADֵ
****************************************************************************/

static uint16_t ADCTEMP_GetAdc(void)
{
    uint16_t value;

    /* �¶ȴ�������ͨ��16���������˳��ֵΪ1������ʱ��Ϊ 239.5���ڡ� */
    ADC1->SMPR1 &= ~(7 << 21);
    ADC1->SMPR1 |= 7 << 21;

    /* �������˳��ֵΪ1 ,ͨ��16*/
    ADC1->SQR3 &= ~0x1F;
    ADC1->SQR3 |= 16;

    ADC1->CR2 |= 1 << 22;        //��������ת��ͨ��
    while(!(ADC1->SR & 0x02));   //�ȴ�ת������

	/* ��ȡADC1�������ת����� */	
	value = ADC1->DR;; 
        
    return value;
}

/****************************************************************************
* Function Name  : ADCTEMP_GetTemp
* Description    : ��ȡ�¶�ֵ
* Input          : None
* Output         : None
* Return         : ���ض�ȡ�����¶�ֵ���Ŵ�100����������λС���㣩
****************************************************************************/

int16_t ADCTEMP_GetTemp(void)
{
    uint32_t adValue = 0;
    int16_t tempValue;
    float adVoltage;
    uint8_t time;
    
    /*  ��ȡʮ��Ȼ�����ƽ��ֵ */
    for(time = 0; time < 10; time++)
    {
        adValue += ADCTEMP_GetAdc();
    }
    adValue = adValue / 10;

    /* ��ADֵת��Ϊ��ѹֵ */
    adVoltage = (float)adValue * 3.3 / 4096;

    /* ���ݹ�ʽ����¶�ֵ */
    adVoltage = ((float)(ADCTEMP_Vsence25 - adVoltage) / ADCTEMP_AvgSlope) + 25; 
    tempValue = adVoltage * 100;   //������λС����

    return tempValue;
}







