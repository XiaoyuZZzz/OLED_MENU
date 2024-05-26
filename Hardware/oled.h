#ifndef __OLED_H
#define __OLED_H

#define OLEDCOVER  1              //每次显示之前是否需要清除局部屏幕，如果需要设置成1 如果不需要设置成0

void OLED_IIC_Init();
void OLED_IIC_SendByte(uint8_t Byte);
void OLED_IIC_WriteCommand(uint8_t Command);
void OLED_IIC_WriteData(uint8_t* Data,uint8_t Count);
void OLED_Set_Cursor(uint8_t X,uint8_t page);
void OLED_Clear();
void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height);
void OLED_Reverse();
void OLED_ReverseArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height);
void OLED_Update();
void OLED_Show_Char(uint8_t x,uint8_t y,char Char);
int OLED_Show_String(uint8_t x,uint8_t y,char *String);
void OLED_Show_Signnumber(uint8_t x,uint8_t page,int32_t number,uint8_t Length);
void OLED_Show_straight_line(uint8_t x,uint8_t page,uint8_t Length);                  //Straight_line
void OLED_Show_Image(uint8_t x,uint8_t y,uint8_t width,uint8_t height,const uint8_t *Image,uint8_t mode);        //Image
void OLED_Show_Chinese(uint8_t x,uint8_t y,char *Chinese);                             //Chinese
void OLED_Show_Point(uint8_t x,uint8_t y);                                             //Point
uint8_t OLED_Get_Point(uint8_t x,uint8_t y);                                           //JudgePoint
void OLED_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t IsFilled);
float OLED_CurveMigration(float Actual_Value, float Target_Value);
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length);  
void OLED_Init();
#endif
