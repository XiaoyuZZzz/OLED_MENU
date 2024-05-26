#include "stm32f10x.h"                  // Device header
#include "myiic.h"
#include "oled.h"
#include "Delay.h"
#include "oled_font.h"
#include <string.h>
typedef struct {
    char Index[4];
    uint8_t Data[32];
} Chinese_t;

const Chinese_t Chinese16x16[] = {
    
    "第",
    0x08,0x04,0x93,0x92,0x96,0x9A,0x92,0xFA,0x94,0x93,0x92,0x96,0xFA,0x02,0x02,0x00,
    0x40,0x40,0x47,0x24,0x24,0x14,0x0C,0xFF,0x04,0x04,0x24,0x44,0x24,0x1C,0x00,0x00,/*0*/

    "一",
    0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*1*/

    "个",
    0x80,0x80,0x40,0x20,0x10,0x08,0x04,0xC3,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,/*2*/

    "显",
    0x00,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,0x00,
    0x40,0x42,0x44,0x58,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x50,0x48,0x46,0x40,0x00,/*3*/

    "示",
    0x40,0x40,0x42,0x42,0x42,0x42,0x42,0xC2,0x42,0x42,0x42,0x42,0x42,0x40,0x40,0x00,
    0x20,0x10,0x08,0x06,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x02,0x04,0x08,0x30,0x00,/*4*/
    
    //.......
    
    "",
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
    0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
};


uint8_t OLED_ShowBuf[8][128];                 //缓存



/*
@OLED_IIC_Init 
@PA8-->SCL  PA9--->SDA
*/
void OLED_IIC_Init()
{
    MyIIC_GPIO_Init();
    Write_SCL(1);
    Write_SDA(1);
}

/*
@OLED_IIC_SendByte(uint8_t Byte)
*/
void OLED_IIC_SendByte(uint8_t Byte)
{
    MyIIC_SendByte(Byte);
    Write_SCL(1);
    Write_SCL(0);
}

/*
@OLED_IIC_WriteCommand(uint8_t Command)
*/
void OLED_IIC_WriteCommand(uint8_t Command)
{
    MyIIC_Start();
    OLED_IIC_SendByte(0x78);            
    OLED_IIC_SendByte(0x00);           
    OLED_IIC_SendByte(Command);
    MyIIC_Stop();
}

/*
@OLED_IIC_WriteData(uint8_t* Data,uint8_t Count)
*/
void OLED_IIC_WriteData(uint8_t* Data,uint8_t Count)
{
    uint8_t i;
    MyIIC_Start();
    OLED_IIC_SendByte(0x78);           
    OLED_IIC_SendByte(0x40);        
    for(i=0;i<Count;i++)
    {
        OLED_IIC_SendByte(Data[i]);
    }
    MyIIC_Stop();
}

/*
@OLED_Set_Cursor(uint8_t X,uint8_t Y)
*/
void OLED_Set_Cursor(uint8_t X,uint8_t page)
{
    OLED_IIC_WriteCommand(0x00 | (X & 0x0F));                //X->LOW
    OLED_IIC_WriteCommand(0x10 | ((X & 0xF0) >> 4));          //X->High
    OLED_IIC_WriteCommand(0xB0 | page);
}

/*
@OLED_Clear()
*/
void OLED_Clear()
{
    uint8_t i,j;
    for(j=0;j<8;j++)
    {
        for(i = 0;i<128;i++)
        {
            OLED_ShowBuf[j][i] = 0x00;
        }
    }
}
/*
@OLED_ClearArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
@用于清除局部的显示，用于覆盖
@遍历所有的点，0x00,因为是取模是列行式，所以需要先判断多少行，每一行需要判断多少列。因此两层循环先是Height再是Width
@缓冲数组是8*128，所以需要➗8........x=10,y=20,width=30,height=30
*/
void OLED_ClearArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
{
    uint8_t i,j;
    for(j=y;j<y+height;j++)
    {
        for(i=x;i<x+width;i++)
        {
            OLED_ShowBuf[j / 8][i] &= ~(0x01 <<(j % 8)); 
        }
    }
}
/*
@OLED_Reverse()
@OLED全部取反
*/
void OLED_Reverse()
{
    uint8_t i,j;
    for(j=0;j<8;j++)
    {
        for(i=0;i<128;i++)
        {
            OLED_ShowBuf[j][i] ^= 0xFF;
        }
    }
}
/*
@OLED_ReverseArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
@显示的区域取反
*/
void OLED_ReverseArea(uint8_t x,uint8_t y,uint8_t width,uint8_t height)
{
    uint8_t i,j;
    for(j=y;j<y+height;j++)
    {
        for(i=x;i<x+width;i++)
        {
            if(i >= 0 && i <= 127 && j >=0 && j <= 63)
                OLED_ShowBuf[j / 8][i] ^=0x01 << (j%8);
        }
    }
}
/*
@OLED_Update()
@将缓存的内容写入 GDDRAM
*/
void OLED_Update()
{
    uint8_t i,j;
    for(i=0;i<8;i++)
    {
        OLED_Set_Cursor(0,i);
        OLED_IIC_WriteData(OLED_ShowBuf[i],128);
    }
    
}
/*
@OLED_Show_Char(uint8_t x,uint8_t page,char Char)
@param x范围0-127，page范围0-8,char ASIIC具体看oled_font.h支持字符
*/
void OLED_Show_Char(uint8_t x,uint8_t y,char Char)
{
    OLED_Show_Image(x,y,8,16,number_8x16[Char - ' '],OLEDCOVER);
}
/*
@OLED_Show_String(uint8_t x,uint8_t page,char *strng)
@param x范围0-127 page 范围0-8
@当字符串超过127的时候会回到起始x，所以会导致移位。
*/
int OLED_Show_String(uint8_t x,uint8_t y,char *String)
{
    uint16_t i;
    int len = 0;
    for(i =0;String[i]!='\0';i++)
    {
        OLED_Show_Char(x+i*8,y,String[i]);
        len++;
    }
    return len;
}
/*
@OLED_Pow(uint32_t X,uint32_t Y)
@param X底数  y次方
@return 返回x^y
*/
uint32_t OLED_Pow(uint32_t X,uint32_t Y)
{
    uint32_t temp = 1;
    while(Y--)
    {
        temp *=X;
   }
    return temp;
}
/*
@OLED_Show_Signnumber(uint8_t x,uint8_t page,int32_t number,uint8_t Length)
@@param x范围0-127 page范围0-8，number范围-2^31~+2^31 Length数字的长度
*/
void OLED_Show_Signnumber(uint8_t x,uint8_t page,int32_t number,uint8_t Length)
{
    uint8_t i;
    uint32_t ShowNumber;
    if(number>0)
    {
        OLED_Show_Char(x,page,'+');
        ShowNumber = number;
    }
    else
    {
        OLED_Show_Char(x,page,'-');
        ShowNumber = -number;
    }
    for(i=0;i<Length;i++)
    {
        OLED_Show_Char(x+i*8+8,page,ShowNumber/OLED_Pow(10,Length-i-1) % 10 + '0');
    }
}
/*
@OLED_Show_straight_line(uint8_t x,uint8_t page,uint8_t Length)
@ param x 范围0-127 page 范围0-8 Length 直线长度
*/
void OLED_Show_straight_line(uint8_t x,uint8_t page,uint8_t Length)
{
    uint8_t i;
    for(i=0;i<Length;i++)
    {
        OLED_Set_Cursor(x+i,page);
       // OLED_IIC_WriteData(0x01);
    }
}
/*
@OLED_Show_Image(uint8_t x,uint8_t y,uint8_t width,uint8_t height,const uint8_t *Image,uint8_t mode)
@param : x 范围 0-127  y 范围 0-63 width 为图片的宽度 height 为图片的高度 Image 图像数组 mode 是否需要清除
*/
void OLED_Show_Image(uint8_t x,uint8_t y,uint8_t width,uint8_t height,const uint8_t *Image,uint8_t mode)
{
    
    
    uint8_t i,j;
    if(mode == 1) OLED_ClearArea(x,y,width,height);
    for(j=0;j < (height-1) / 8+1;j++)
    {
        for(i=0;i<width;i++)
        {
            OLED_ShowBuf[y / 8 + j][x+i] |= Image[j*width+i] << y % 8;
            OLED_ShowBuf[y / 8 + j + 1][x+i] |= Image[j*width+i] >> (8-y % 8);
        }
    }
    
   
}
/*
@OLED_Show_Chinese(uint8_t x,uint8_t page,char *Chinese)
@param : x 范围0-127 y 范围0-63 Chinese 对应的汉字结构体数组
*/
void OLED_Show_Chinese(uint8_t x,uint8_t y,char *Chinese)
{
    char SigleChinese[4] = {0};                   //UTF-8 三个字节定义一个用来接收每个字的变量
    uint8_t pIndex,i,pChinese  = 0;              //pIndex 用于存放汉字的索引
    
    for(i=0;Chinese[i]!='\0';i++)                 //判断是否结束
    {
        SigleChinese[pChinese++] = Chinese[i];
        
        if(pChinese >= 3)
        {
            pChinese = 0;
            
            for(pIndex = 0;strcmp(Chinese16x16[pIndex].Index,"") ;pIndex++)            //判断是否是字符组里面最后的一个元素0xFF....0xFF
            {
                if(strcmp(Chinese16x16[pIndex].Index,SigleChinese) == 0)               //判断中国数组中的索引是否对应刚刚接收到的
                {
                    break;
                }
            }
            OLED_Show_Image(x+((i+1) / 3 - 1) * 16,y,16,16,Chinese16x16[pIndex].Data,OLEDCOVER);
            
        }
    }
    
}
/*
@OLED_Show_Point(uint8_t x,uint8_t y)
@用于画点
*/
void OLED_Show_Point(uint8_t x,uint8_t y)
{
    OLED_ShowBuf[y / 8][x] |= 0x01 << (y % 8);
}
/*
@OLED_Get_Point(uint8_t x,uint8_t y)
@用于检测该点是否被点亮
*/
uint8_t OLED_Get_Point(uint8_t x,uint8_t y)
{
    uint8_t retval;
    retval = ( OLED_ShowBuf[y / 8][x] & 0x01 << (y % 8) ) ?  1 :  0;
    return retval;
}
/*
@OLED_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t IsFilled)
@用于画方框
*/
void OLED_DrawRectangle(int16_t x, int16_t y, uint8_t width, uint8_t height, uint8_t IsFilled)
{
    uint8_t i,j;
    if(!IsFilled)
    {
        for(i=x;i<x+width;i++)
        {
            OLED_Show_Point(i,y);
            OLED_Show_Point(i,y+height-1);
        }
        for(i=y;i<y+height;i++)
        {
            OLED_Show_Point(x,i);
            OLED_Show_Point(x+width-1,i);
        }
    }
    else
    {
        for (i = x; i < x + width; i ++)
        {
            /*遍历Y坐标*/
            for (j = y; j < y + height; j ++)
            {
                OLED_Show_Point(i, j);
            }
        }
    }
}
/*
@OLED_CurveMigration(float Actual_Value, float Target_Value)
@用于OLED中移动
*/
float OLED_CurveMigration(float Actual_Value, float Target_Value)
{
    
    if( (Target_Value - Actual_Value) > 1)
    {
        Actual_Value += (Target_Value - Actual_Value) * 0.2 + 1;
    }
    
    else if ( (Target_Value - Actual_Value) < -1)
    {
        Actual_Value += (Target_Value - Actual_Value) * 0.2 - 1;
    }
    else if (Target_Value  == 64 && Target_Value != Actual_Value )
    {
        Actual_Value ++ ;
    }
    
    else
    {
        Actual_Value = Target_Value;
    }
    return  Actual_Value;
}
void OLED_ShowSignedNum(uint8_t X, uint8_t Y, int32_t Number, uint8_t Length)
{
	uint8_t i;
	uint32_t Number1;
	
	if (Number >= 0)						//数字大于等于0
	{
		OLED_Show_Char(X, Y, '+');	//显示+号
		Number1 = Number;					//Number1直接等于Number
	}
	else									//数字小于0
	{
		OLED_Show_Char(X, Y, '-');	//显示-号
		Number1 = -Number;					//Number1等于Number取负
	}
	
	for (i = 0; i < Length; i++)			//遍历数字的每一位								
	{
		/*调用OLED_ShowChar函数，依次显示每个数字*/
		/*Number1 / OLED_Pow(10, Length - i - 1) % 10 可以十进制提取数字的每一位*/
		/*+ '0' 可将数字转换为字符格式*/
		OLED_Show_Char(X + (i + 1) * 8, Y, Number1 / OLED_Pow(10, Length - i - 1) % 10 + '0');
	}
}
/*
@OLED_Init()
*/
void OLED_Init()
{
    uint32_t i,j;
    for(i=0;i<1000;i++)
    {
        for(j=0;j<1000;j++);
    }
    OLED_IIC_Init();
    
    OLED_IIC_WriteCommand(0xAE);

    OLED_IIC_WriteCommand(0xD5);
    OLED_IIC_WriteCommand(0x80);

    OLED_IIC_WriteCommand(0xA8);
    OLED_IIC_WriteCommand(0x3F);
        
    OLED_IIC_WriteCommand(0xD3);
    OLED_IIC_WriteCommand(0x00);
    
    OLED_IIC_WriteCommand(0x40);
    
    OLED_IIC_WriteCommand(0xA1);
    
    OLED_IIC_WriteCommand(0xC8);

    OLED_IIC_WriteCommand(0xDA);
    OLED_IIC_WriteCommand(0x12);
    
    OLED_IIC_WriteCommand(0x81);
    OLED_IIC_WriteCommand(0xCF);

    OLED_IIC_WriteCommand(0xD9);
    OLED_IIC_WriteCommand(0xF1);

    OLED_IIC_WriteCommand(0xDB);
    OLED_IIC_WriteCommand(0x30);

    OLED_IIC_WriteCommand(0xA4);

    OLED_IIC_WriteCommand(0xA6);

    OLED_IIC_WriteCommand(0x8D);
    OLED_IIC_WriteCommand(0x14);

    OLED_IIC_WriteCommand(0xAF);
        
    OLED_Clear();
}
