#ifndef __GAMES_H
#define __GAMES_H


typedef struct{
   uint8_t Plan_Most_U; //�ɻ���ߵ�
   uint8_t Plan_Most_L;//�ɻ������
   uint8_t Plan_Most_R;//�ɻ����ұ�
   uint8_t Plan_Most_D//�ɻ���͵�
}Plan_t;

void My_Plan_Games(void);
int Judeg_Plan(uint8_t My_Plan_X,uint8_t My_Plan_Y,uint8_t Hit_Plan_X,uint8_t Hit_Plan_Y);

#endif
