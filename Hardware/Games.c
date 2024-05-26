#include "stm32f10x.h"                  // Device header
#include "Games.h"
#include "oled.h"
#include "Delay.h"
#include <time.h>
#include <stdlib.h>

extern int16_t AX, AY, AZ, GX, GY, GZ;

const uint8_t Plan_Imag [] ={
    0xFF,0x81,0x81,0xFF,0xFF,0x81,0x81,0xFF,0xFF,0x81,0x9F,0x87,0x87,0x9F,0x81,0xFF,/*"未命名文件",0*/
};
const uint8_t Hit_Plan_Imag[] ={
   0xFF,0x01,0xE1,0xE1,0xE1,0xE1,0x01,0xFF,0xFF,0x80,0x81,0x81,0x81,0x81,0x80,0xFF,/*"未命名文件",0*/


};

//判断飞机是否相撞
int Judeg_Plan(uint8_t My_Plan_X,uint8_t My_Plan_Y,uint8_t Hit_Plan_X,uint8_t Hit_Plan_Y)
{
    //右下角和右上角碰到
    if((My_Plan_Y+16 == Hit_Plan_Y && My_Plan_X+8 == Hit_Plan_X ) || (Hit_Plan_Y+16 ==My_Plan_Y &&My_Plan_X+8 ==Hit_Plan_X)  )
    {
        return 1;//相撞
    }
    //左下角和左上角碰到
     if( (Hit_Plan_X + 8 == My_Plan_X && My_Plan_Y + 16 == Hit_Plan_Y) || (Hit_Plan_X + 8 == My_Plan_X && Hit_Plan_Y+16 == My_Plan_Y ))
    {
        return 1;
    }
    //重叠碰到
     if( ( (My_Plan_X + 8 > Hit_Plan_X && My_Plan_X + 8 < Hit_Plan_X + 8 ) && (My_Plan_Y + 16 >Hit_Plan_Y && My_Plan_Y + 16 < Hit_Plan_Y+16) ) ||  ( (My_Plan_X > Hit_Plan_X && My_Plan_X < Hit_Plan_X + 8)  && (My_Plan_Y + 16 >Hit_Plan_Y && My_Plan_Y <  Hit_Plan_Y +16) ) )
    {
        return 1;
    }
    if((My_Plan_X + 8 >  Hit_Plan_X && My_Plan_X+8 < Hit_Plan_X + 8 ) && ( My_Plan_Y > Hit_Plan_Y && My_Plan_Y < Hit_Plan_Y + 16) ||( (My_Plan_X >Hit_Plan_X && My_Plan_X <  Hit_Plan_X + 8 ) && (My_Plan_Y > Hit_Plan_Y  &&  My_Plan_Y < Hit_Plan_Y +16) ) )
    {
        return 1;
    } 
    return 0;
}
 
void My_Plan_Games(void)
{
    int rand_buf[20];
    uint8_t end_result = 0;
    uint8_t  i;
    Plan_t Plan;
    int Need_rand = 0;
    float L_R =20,O_D = 20;
    int hit_plan_x = 0,hit_plan_y = 5;
    int temp_hit_plan_x = 0,temp_hit_plan_y = 0;
    //srand((unsigned)time(NULL));
    //随机数种子
    for(i = 0;i<10;i++)
    {
        rand_buf[i] = rand() % 64;
    }
    while(1)
    {
        OLED_Clear();
        /*随机生成飞机*/
        
        OLED_Show_Image(rand_buf[hit_plan_x]++ ,rand_buf[hit_plan_y]++,8,16,Hit_Plan_Imag,0);
        
        temp_hit_plan_x  =  rand_buf[hit_plan_x];
        temp_hit_plan_y  =  rand_buf[hit_plan_y];
        //调试
        //OLED_ShowSignedNum(90,10,temp_hit_plan_y,3);
        //OLED_ShowSignedNum(90,37,rand_buf[hit_plan_y],3);
        
        
        //OLED_ShowSignedNum(60,10,L_R,3);
       // OLED_ShowSignedNum(60,37,O_D,3);
        
        //如果生成的飞机飞过了64，那么就需要生成下一个飞机
        
        if(temp_hit_plan_y  >= 64) 
            {
                rand_buf[hit_plan_y] = 0;
                hit_plan_y++; 
                hit_plan_x++;
                if(hit_plan_y == 9) hit_plan_y = 5;
                if(hit_plan_x == 4) hit_plan_x = 0;
                
            }
            
        /*控制飞机*/
        Plan.Plan_Most_U = Plan.Plan_Most_D = O_D;
        Plan.Plan_Most_L  = Plan.Plan_Most_R = L_R;
        OLED_Show_Image(L_R,O_D,8,16,Plan_Imag,0);

        L_R -= ((L_R + AY /100 )- L_R ) / 8;
        O_D -= ((O_D + AX / 100 ) - O_D) / 8;
        
        end_result = Judeg_Plan(L_R,O_D,temp_hit_plan_x,temp_hit_plan_y);
            
        if(Plan.Plan_Most_U == 0 || Plan.Plan_Most_D == 49 || Plan.Plan_Most_L == 0 || Plan.Plan_Most_R == 121 || end_result == 1 )
        {
            OLED_Clear();
            OLED_Show_String(10,10,"GAME OVER");
            Delay_ms(500);
            //L_R = O_D = 20;
        }
        OLED_Update();

    }
}