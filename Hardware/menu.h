#ifndef __MENU_H
#define __MENU_H

#define NULL ((void *)0)

extern int16_t Up_flag;
extern int16_t Down_flag;

extern uint8_t Back_flag;
extern uint8_t Enter_flag;
//extern int16_t Encoder_Count;
/*
@定义一个选择结构体
@string : 需要显示的名字
@func(void) ：需要实现的功能
*/
typedef struct{
    char *string;
    void (*func)(void);
    uint8_t Len;
}MUNE_Opt_t;


enum MENU_COMMAND
{
    OLED_SHOW_BUF,          // 无参无返
    OLED_CLEAR,             // 无参无返
    OLED_SHOW_STRING,       // 可变参数列表对应顺序: x, y, string
    OLED_SHOW_CURSOR,       // 可变参数列表对应顺序: x, y, width, height;

    GET_EVENT_ENTER,        // 返回布尔
    GET_EVENT_BACK,         // 返回布尔
    GET_EVENT_DIRECTION,    // 返回方向
    GET_EVENT_WHEEL,        // 返回有符号整型
};



int Menu_callback_command(enum MENU_COMMAND command , ...);

void MENU_RUN(MUNE_Opt_t *OptList);




void MENU_SHOW_PARENTMenu(void);

void MENU_SHOW_TOOLMeun(void);

void MENU_SHOW_GAMEMenu(void);

void MENU_SHOW_SETTINGMenu(void);

void MENU_SHOW_INFORMATIONMenu(void);



#endif
