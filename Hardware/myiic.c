#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*
@GPIO��ʼ������GPIO���óɿ�©���
@1����ʼ�ź�
@2��ֹͣ�ź�
@3������Ӧ��
@4������Ӧ��
@5�������ֽ�
@6�������ֽ�
*/

/*
GPIO8---->SCL
GPIO9---->SDA
*/
void MyIIC_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
}
/*
@ͨ��0/1������SDA�������߸ߵ͵�ƽ
*/
void Write_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)BitValue);
    //Delay_us(10);
}
/*
@ͨ��0/1������SCLʱ�����߸ߵ͵�ƽ
*/
void Write_SCL(uint8_t BitValue)
{
     GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)BitValue);
     //Delay_us(10);  
}
/*
@��ȡSDA�ĸߵ͵�ƽ
*/
uint8_t Read_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9);
    //Delay_us(10);
    return BitValue;
}
/*
@My_IIC_Start
@SCLΪ�ߵ�ƽʱ��SDA�ɸߵ�ƽ��͵�ƽ���䣬��ʼ���͹���
@�Ƚ�SCL�ͷţ�SDA�ͷţ��ٽ�SDA���ͣ�SCL����
*/
void MyIIC_Start(void)
{
    Write_SDA(1);
    Write_SCL(1);
    Write_SDA(0);
    Write_SCL(0); 
}
/*
@My_IIC_Stop
@SCL Ϊ�ߵ�ƽʱ��SDA �ɵ͵�ƽ��ߵ�ƽ���䣬������������
@�Ƚ�SDA���ͣ���SCl�ͷţ��ٽ�SDA�ͷ�
*/
void MyIIC_Stop(void)
{
    Write_SDA(0);
    Write_SCL(1);
    Write_SDA(1);
}
/*
@My_IIC_SenAck(uint8_t AckBit)
@�����ڽ�����һ���ֽ�֮������һ��ʱ�ӷ���һλ���ݣ�����0��ʾӦ������1��ʾ��Ӧ��
*/
void MyIIC_SendAck(uint8_t AckBit)
{
    Write_SDA(AckBit);
    Write_SCL(1);
    Write_SCL(0);
}

/*
@My_IIC_ReciveAck(void)
@�����ڷ�����һ���ֽ�֮������һ��ʱ�ӽ���һλ���ݣ��жϴӻ��Ƿ�Ӧ������0��ʾӦ������1��ʾ��Ӧ�������ڽ���֮ǰ����Ҫ�ͷ�SDA��
*/
uint8_t MyIIC_ReceiveAck(void)
{
    uint8_t AckBit;
    Write_SDA(1);
    Write_SCL(1);
    AckBit = Read_SDA();
    Write_SCL(0);
    return AckBit;
}
/*
@My_IIC_SendByte(uint8_t Byte)
@SCL�͵�ƽ�ڼ䣬����������λ���ηŵ�SDA���ϣ���λ���У���Ȼ���ͷ�SCL���ӻ�����SCL�ߵ�ƽ�ڼ��ȡ����λ,
@����SCL�ߵ�ƽ�ڼ�SDA�����������ݱ仯������ѭ����������8�Σ����ɷ���һ���ֽ�
*/
void MyIIC_SendByte(uint8_t Byte)
{
    uint8_t i ;
    for(i =0;i<8;i++)
    {
        Write_SDA(Byte & (0x80>>i));           //��λ���У����αȽ�
        Write_SCL(1);
        Write_SCL(0);
    }
}
/*
@My_IIC_ReceiveByte(void)
@SCL�͵�ƽ�ڼ䣬�ӻ�������λ���ηŵ�SDA���ϣ���λ���У���Ȼ���ͷ�SCL����������SCL�ߵ�ƽ�ڼ��ȡ����λ.
@����SCL�ߵ�ƽ�ڼ�SDA�����������ݱ仯������ѭ����������8�Σ����ɽ���һ���ֽڣ������ڽ���֮ǰ����Ҫ�ͷ�SDA��
*/
uint8_t MyIIC_ReceiveByte(void)
{
    uint8_t i,Byte = 0x00;
    Write_SDA(1);
    for(i = 0;i<8;i++)
    {
        Write_SCL(1);
        if(Read_SDA()==1)
        {
            Byte |=(0x80 >>i);
        }
        Write_SCL(0); 
    }
    return Byte;
}