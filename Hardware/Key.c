#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "oled.h"
#include "LED.h"

void Key_Init(void)
{
    
    
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
        
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
    
    
     EXTI_InitStructure.EXTI_Line = EXTI_Line11;
     EXTI_InitStructure.EXTI_LineCmd = ENABLE;
     EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
     EXTI_Init(&EXTI_InitStructure);
        
     //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        
        
     NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
     NVIC_Init(&NVIC_InitStructure);
}

void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line11) == SET)
    {
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 0)
        {
           LED1_OFF();
        }
        if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11) == 1)
        {
           LED1_ON();
        }
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    
}
