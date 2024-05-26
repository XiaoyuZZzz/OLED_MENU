#ifndef __MENU_H
#define __MENU_H

#define NULL ((void *)0)

extern int16_t Up_flag;
extern int16_t Down_flag;

extern uint8_t Back_flag;
extern uint8_t Enter_flag;
//extern int16_t Encoder_Count;
/*
@����һ��ѡ��ṹ��
@string : ��Ҫ��ʾ������
@func(void) ����Ҫʵ�ֵĹ���
*/
typedef struct{
    char *string;
    void (*func)(void);
    uint8_t Len;
}MUNE_Opt_t;


enum MENU_COMMAND
{
    OLED_SHOW_BUF,          // �޲��޷�
    OLED_CLEAR,             // �޲��޷�
    OLED_SHOW_STRING,       // �ɱ�����б��Ӧ˳��: x, y, string
    OLED_SHOW_CURSOR,       // �ɱ�����б��Ӧ˳��: x, y, width, height;

    GET_EVENT_ENTER,        // ���ز���
    GET_EVENT_BACK,         // ���ز���
    GET_EVENT_DIRECTION,    // ���ط���
    GET_EVENT_WHEEL,        // �����з�������
};



int Menu_callback_command(enum MENU_COMMAND command , ...);

void MENU_RUN(MUNE_Opt_t *OptList);




void MENU_SHOW_PARENTMenu(void);

void MENU_SHOW_TOOLMeun(void);

void MENU_SHOW_GAMEMenu(void);

void MENU_SHOW_SETTINGMenu(void);

void MENU_SHOW_INFORMATIONMenu(void);



#endif
