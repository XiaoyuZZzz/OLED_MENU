#ifndef __GAMES_H
#define __GAMES_H


typedef struct{
   uint8_t Plan_Most_U; //飞机最高点
   uint8_t Plan_Most_L;//飞机最左边
   uint8_t Plan_Most_R;//飞机最右边
   uint8_t Plan_Most_D//飞机最低点
}Plan_t;

void My_Plan_Games(void);
int Judeg_Plan(uint8_t My_Plan_X,uint8_t My_Plan_Y,uint8_t Hit_Plan_X,uint8_t Hit_Plan_Y);

#endif
