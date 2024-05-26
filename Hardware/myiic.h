#ifndef __MY_IIC_H
#define __MY_IIC_H


void MyIIC_GPIO_Init(void);
void Write_SDA(uint8_t BitValue);
void Write_SCL(uint8_t BitValue);
void MyIIC_Start(void);
void MyIIC_Stop(void);
void MyIIC_SendAck(uint8_t AckBit);
uint8_t MyIIC_ReceiveAck(void);
void MyIIC_SendByte(uint8_t Byte);
uint8_t MyIIC_ReceiveByte(void);

#endif
