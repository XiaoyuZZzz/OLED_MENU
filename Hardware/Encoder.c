#include "stm32f10x.h"                  // Device header
#include "LED.h"
#include "oled.h"
#include "Delay.h"

int16_t Up_flag = 0;
int16_t Down_flag = 0;

uint8_t Back_flag = 0;
uint8_t Enter_flag = 0;



void Encoder_Init(void)
{
        GPIO_InitTypeDef GPIO_InitStructure;
        EXTI_InitTypeDef EXTI_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
        
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_11 | GPIO_Pin_10;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(GPIOB, &GPIO_InitStructure);
        
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource10);
        GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
        
        
        EXTI_InitStructure.EXTI_Line = EXTI_Line0 | EXTI_Line1 | EXTI_Line11 | EXTI_Line10;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_Init(&EXTI_InitStructure);
        
        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_Init(&NVIC_InitStructure);

        NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
        NVIC_Init(&NVIC_InitStructure);
        
        NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
        NVIC_Init(&NVIC_InitStructure);
        
        
    }

//int16_t Encoder_Get(void)
//{
//        int16_t Temp;
//        Temp = Encoder_Count;
//        Encoder_Count = 0;
//        return Temp;
//}

void EXTI0_IRQHandler(void)
{
        if (EXTI_GetITStatus(EXTI_Line0) == SET)
        {
            /*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
            {
                if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
                {
                   
                    //LED1_OFF();
                    //Encoder_Count  = -1; //上一条
                    Up_flag = 1;
                }
            }
            EXTI_ClearITPendingBit(EXTI_Line0);
        }
}

void EXTI1_IRQHandler(void)
{
        if (EXTI_GetITStatus(EXTI_Line1) == SET)
        {
            /*如果出现数据乱跳的现象，可再次判断引脚电平，以避免抖动*/
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
            {
                if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0)
                {
                    
                    //LED1_ON();
                    //Encoder_Count = 1; //下一条
                    Down_flag = 1;
                }
            }
            EXTI_ClearITPendingBit(EXTI_Line1);
        }
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line10) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10) == 0)
        {
            Enter_flag = 1;
        }
        
        EXTI_ClearITPendingBit(EXTI_Line10);
    }
    if (EXTI_GetITStatus(EXTI_Line11) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
        {
            Back_flag = 1;
        }
        
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    
    
    
}
