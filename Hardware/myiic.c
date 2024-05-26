#include "stm32f10x.h"                  // Device header
#include "Delay.h"
/*
@GPIO初始化，将GPIO配置成开漏输出
@1、起始信号
@2、停止信号
@3、发送应答
@4、接收应答
@5、发送字节
@6、接受字节
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
@通过0/1来控制SDA数据总线高低电平
*/
void Write_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB,GPIO_Pin_9,(BitAction)BitValue);
    //Delay_us(10);
}
/*
@通过0/1来控制SCL时钟总线高低电平
*/
void Write_SCL(uint8_t BitValue)
{
     GPIO_WriteBit(GPIOB,GPIO_Pin_8,(BitAction)BitValue);
     //Delay_us(10);  
}
/*
@读取SDA的高低电平
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
@SCL为高电平时，SDA由高电平向低电平跳变，开始传送过程
@先将SCL释放，SDA释放，再将SDA拉低，SCL拉低
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
@SCL 为高电平时，SDA 由低电平向高电平跳变，结束传送数据
@先将SDA拉低，将SCl释放，再将SDA释放
*/
void MyIIC_Stop(void)
{
    Write_SDA(0);
    Write_SCL(1);
    Write_SDA(1);
}
/*
@My_IIC_SenAck(uint8_t AckBit)
@主机在接收完一个字节之后，在下一个时钟发送一位数据，数据0表示应答，数据1表示非应答
*/
void MyIIC_SendAck(uint8_t AckBit)
{
    Write_SDA(AckBit);
    Write_SCL(1);
    Write_SCL(0);
}

/*
@My_IIC_ReciveAck(void)
@主机在发送完一个字节之后，在下一个时钟接收一位数据，判断从机是否应答，数据0表示应答，数据1表示非应答（主机在接收之前，需要释放SDA）
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
@SCL低电平期间，主机将数据位依次放到SDA线上（高位先行），然后释放SCL，从机将在SCL高电平期间读取数据位,
@所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可发送一个字节
*/
void MyIIC_SendByte(uint8_t Byte)
{
    uint8_t i ;
    for(i =0;i<8;i++)
    {
        Write_SDA(Byte & (0x80>>i));           //高位先行，依次比较
        Write_SCL(1);
        Write_SCL(0);
    }
}
/*
@My_IIC_ReceiveByte(void)
@SCL低电平期间，从机将数据位依次放到SDA线上（高位先行），然后释放SCL，主机将在SCL高电平期间读取数据位.
@所以SCL高电平期间SDA不允许有数据变化，依次循环上述过程8次，即可接收一个字节（主机在接收之前，需要释放SDA）
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