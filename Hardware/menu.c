#include "stm32f10x.h"                  // Device header
#include "menu.h"
#include "oled.h"
#include "Encoder.h"
#include "MPU6050.h"
#include "Games.h"

int str_len ;
int str_len_buf[100];
int16_t AX, AY, AZ, GX, GY, GZ;
float Cursor_Ac_x = 0;
float Cursor_Ac_y = 63;
float Cursor_Ac_w = 0;
float Cursor_Ac_h = 0;
uint8_t Back_Mode = 0;

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

/*���ڼ��˵�״̬�ĺ��������ƻ������ƣ����ظ��˵����ǽ����Ӳ˵�*/
int Menu_callback_command(enum MENU_COMMAND command , ...)
{
    int retval = 0;
    
    switch(command)
    {
        case OLED_SHOW_BUF:
        {
            OLED_Update();
        }break;
        
        case OLED_CLEAR:
        {
            OLED_Clear();
        }break;
        
        case OLED_SHOW_STRING:
        {
            /*��Ϊ���Ժ����������x,y,string����ȡ*/
            int *arg_list = ((int *)&command) + 1;
            int show_x =(int) arg_list[0];
            int show_y =(int)arg_list[1];
            char *show_string = (char *)arg_list[2];
            
            retval  = OLED_Show_String(show_x,show_y,show_string);
             
        }break;
        
        
        case OLED_SHOW_CURSOR:
        {
            int *arg_list = ((int *)&command) + 1;
            int cursor_x = arg_list[0];
            int cursor_y = arg_list[1];
            int cursor_width = arg_list[2];
            int cursor_height = arg_list[3];
            
            
            Cursor_Ac_x = OLED_CurveMigration(Cursor_Ac_x,cursor_x);
            Cursor_Ac_y = OLED_CurveMigration(Cursor_Ac_y,cursor_y);
            Cursor_Ac_w = OLED_CurveMigration(Cursor_Ac_w,cursor_width);
            Cursor_Ac_h = OLED_CurveMigration(Cursor_Ac_h,cursor_height);
            
           OLED_ReverseArea(Cursor_Ac_x,Cursor_Ac_y,Cursor_Ac_w,Cursor_Ac_h);
        }break;
        
        
        case GET_EVENT_ENTER:             //ȷ��
        {
           
            if((AY / 100) < -1) {retval = 1;AY = 0;}
            
            /*
            if(Enter_flag)
            {
                retval = 1;
                Enter_flag = 0;
            }
            */
            
        }break;
        
        case GET_EVENT_BACK:              //����
        {
            if(Back_flag)
            {
                retval = 1 ;
                Back_flag = 0;
            }
            
        }break;
        
        case GET_EVENT_WHEEL:
        {
           
            if((AX/100) >= 2) {retval = -1;AX = 0;}
            else if((AX/100) <= -2) {retval = 1;AX = 0;}
            /*
            if(Up_flag) { retval = 1;Up_flag = 0;}
            else if(Down_flag) { retval = -1;Down_flag = 0;}
            */
        }break;
        
        default:{} break;
    
        
    }
    return retval;
}
/*���ڶ�����ʾ*/
void MENU_RUN(MUNE_Opt_t *OptList)
{
    uint8_t i,Opt_x,Opt_y;
    int temp_Ax;
    
    uint8_t Catch_i = 1;            //ѡ���±�
    uint8_t Cursor_i = 0;             //��ǰ�±�
    int8_t Show_i = 0;
    int8_t Show_i_pre = 4 ;
    int8_t Show_offset = 0;
    uint8_t Wheel_Event = 0;          //��¼�����¼�
    uint8_t Option_Max_i = 0;         //ѡ���б���
    
    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t cursor_width;
    uint8_t cursor_height;
    
    char* str;
    
    while(OptList[Option_Max_i++].string[0] != ' ');
    Option_Max_i  -= 2;
    
    while(1)
    {
        if(Menu_callback_command(GET_EVENT_ENTER))
        {
            if(OptList[Catch_i].func != NULL) OptList[Catch_i].func();
            else  return;
        }
        
        if (Menu_callback_command(GET_EVENT_BACK))
            return;
        
        Wheel_Event = Menu_callback_command(GET_EVENT_WHEEL);
        
        
        if(Wheel_Event)
        {

            Cursor_i += Wheel_Event;
            Catch_i += Wheel_Event;
            /****************����ѡ�еĹ�꣬�����ѡ���б��Ȼ����Ǿ����***********************/
            if (Catch_i > Option_Max_i) Catch_i = Option_Max_i;
            if (Catch_i < 0)  Catch_i = 0;
            /****************���ƹ����±꣬��Ϊ�����8x16�����壬��64����ô�ܹ���ֻ����ʾ4�У��Ŀ���֮��ֻ�����������ͬ�����ܳ�Ҳ���ܶ�***********************/
            if (Cursor_i > 3)Cursor_i = 3;
            if (Cursor_i > Option_Max_i) Cursor_i = Option_Max_i;
            if (Cursor_i > Catch_i) Cursor_i = Catch_i;
            if (Cursor_i < 0) Cursor_i = 0;
        }
            Menu_callback_command(OLED_CLEAR);
            
            
            //ѡ���±�-��ǰ���±� �ж������ϻ�������
                Show_i = Catch_i - Cursor_i;
            
            
                if(Show_i - Show_i_pre)
                { 
                    if(Show_i > Show_i_pre)
                    Show_offset = (Show_i - Show_i_pre) * 16;
                    else Show_offset = (Show_i_pre - Show_i) * 16 ;
                    
                    //���´�ֱ�ƶ���״̬
                    Show_i_pre = Show_i;
                }
                if(Show_offset)
                {
                    Show_offset /= 2;
                }
                
            
            
            /*��ʾѡ��*/
            for(i=0;i <= 3 ;i++)
            {
                
                if(Show_i + i < 0 ) continue;
                
                if (Show_i + i > Option_Max_i)  break;          //�����˾��˳�ѭ��
                
                Opt_x = 0+2;             // 0--��ʾ�˵�����ʼx��λ�ã�2--��ʾ��Ҫ�ƶ���������
                Opt_y = (i * 16)+ Show_offset;     // 0--��ʾ�˵�����ʵy��λ�ã�i*16--��ʾ�ڼ���ѡ��*������иߣ�8x16��
                str = OptList[Show_i + i].string;
                
                OptList[Show_i+i].Len = Menu_callback_command(OLED_SHOW_STRING,Opt_x,Opt_y,str);
            }
            
           Menu_callback_command(OLED_SHOW_CURSOR,0,Cursor_i*16,4+ OptList[Catch_i].Len*8+2,16);
            
            
           Menu_callback_command(OLED_SHOW_BUF);
        
    }
}
/*��ʾһ�����˵�*/
void MENU_SHOW_PARENTMenu(void)
{
    static MUNE_Opt_t Mune_OptList[]={{"BACK<<<",NULL},
                                      {"TOOL",MENU_SHOW_TOOLMeun},
                                      {"GAME",MENU_SHOW_GAMEMenu},
                                      {"SETTING",MENU_SHOW_SETTINGMenu},
                                      {"INFORMATION",MENU_SHOW_INFORMATIONMenu},
                                      {"   ",NULL}};
    MENU_RUN(Mune_OptList);
}
/*��ʾ���߲˵�*/
void MENU_SHOW_TOOLMeun(void)
{
    
    static MUNE_Opt_t Mune_OptList[]={{"BACK<<<",NULL},
                                      {"LIGHT",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"   ",NULL}};
    MENU_RUN(Mune_OptList);
}
/*��ʾ��Ϸ�˵�*/
void MENU_SHOW_GAMEMenu(void)
{
    static MUNE_Opt_t Mune_OptList[]={{"BACK<<<",NULL},
                                      {"PLAY",My_Plan_Games},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"   ",NULL}};
    MENU_RUN(Mune_OptList);
}
/*��ʾ���ò˵�*/
void MENU_SHOW_SETTINGMenu(void)
{
    static MUNE_Opt_t Mune_OptList[]={{"BACK<<<",NULL},
                                      {"SETTING",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"   ",NULL}};
    MENU_RUN(Mune_OptList);
}
/*��ʾ��Ϣ�˵�*/
void MENU_SHOW_INFORMATIONMenu(void)
{
    static MUNE_Opt_t Mune_OptList[]={{"BACK<<<",NULL},
                                      {"PLAY",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"MORE",NULL},
                                      {"   ",NULL}};
    MENU_RUN(Mune_OptList);
}