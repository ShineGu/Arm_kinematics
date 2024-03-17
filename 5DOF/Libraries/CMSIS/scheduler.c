/***********************************************************************************************************************************
 ** ���ļ����ơ�  system_f103.c
 ** ����д��Ա��  ħŮ�������Ŷ�
 ** �����·�����  QȺ�ļ���       1126717453
 ** ����    ����  ħŮ������      https://demoboard.taobao.com
 ***********************************************************************************************************************************
 ** ��ʹ��˵����  1- ƽ̨�޹�
 **               2- ȫ�ֺ�����
 **                  vScheduler_TickCnt()   ������ѯֵ����������������SysTick�жϷ������У����ã�������ѯ�ж�ֵ;
 **                  vScheduler_Run()       ������ѯ; while�в��ϵ��ñ���������ִ������;
 **               3- ��ֲ������
 **                  ��SysTick�жϷ��������ļ��У�#include "scheduler.h"����ʹ��ɵ��ñ��ļ�����
 **                  vScheduler_TickCnt()�ŵ�SysTick�жϷ������У����Systick����1ms�ж�һ�Σ���Ҫ�������޸�;
 **                  vScheduler_Run()�ŵ�main��whileѭ���У�
 **                  Ҫ���ִ�еĴ��룬����ŵ���vTask_xxms()�����У�
 **                  ע�⣺ÿ��vTask_xxms()��������������ʱ������Ҫ��������������ѯʱ������ʹ��System_TestRunTimes()��������ʱ��
 **
 ** �����¼�¼��  2020-04-21  ����
 **               2021-02-25  ����ע��
 **  
==================================================================================================================================*/
#include "scheduler.h"
#include "hardware.h"




/*****************************************************************************
 ** ���ر�����������������
 *****************************************************************************/
static u16 usTick_1ms     =0;
static u16 usTick_2ms     =0;
static u16 usTick_5ms     =0;
static u16 usTick_10ms    =0;
static u16 usTick_50ms    =0;
static u16 usTick_100ms   =0;
static u16 usTick_500ms   =0;
static u16 usTick_1000ms = 0;

void vTask_1ms(void);
void vTask_2ms(void);
void vTask_5ms(void);
void vTask_10ms(void);
void vTask_50ms(void);
void vTask_100ms(void);
void vTask_500ms(void);
void vTask_1000ms(void);



/*============================================================================
 * ��  ����vScheduler_TickCnt
 * ��  �ܣ�������ѯֵ����; ����������SysTick�жϷ������У����ã�������ѯ�ж�ֵ
 * ��  ����
 * ����ֵ��
 * ��  ע�� ħŮ�������Ŷ� 2020��04��21��
============================================================================*/
void Scheduler_TickCnt(void)
{
    ++usTick_1ms;   
    ++usTick_2ms;   
    ++usTick_5ms;   
    ++usTick_10ms;  
    ++usTick_50ms;  
    ++usTick_100ms; 
    ++usTick_500ms; 
    ++usTick_1000ms;
}



/**============================================================================
 ** ��  ����vScheduler_Run
 ** ��  �ܣ�������ѯ; while�в��ϵ��ñ���������ִ������
 ** ��  ����
 ** ����ֵ��
 ** ��  ע��ħŮ�������Ŷ� 2020��04��21��
=============================================================================*/
void Scheduler_Run(void)
{
    if( usTick_1ms   >=1)    { usTick_1ms=0;      vTask_1ms();   }   // ÿ1ms   ִ��1�ε�����
    if( usTick_2ms   >=2)    { usTick_2ms=0;      vTask_2ms();   }   // ÿ2ms   ִ��1�ε�����
    if( usTick_5ms   >=5)    { usTick_5ms=0;      vTask_5ms();   }   // ÿ5ms   ִ��1�ε�����
    if( usTick_10ms  >=10)   { usTick_10ms=0;     vTask_10ms();  }   // ÿ10ms  ִ��1�ε�����
    if( usTick_50ms  >=50)   { usTick_50ms=0;     vTask_50ms();  }   // ÿ50ms  ִ��1�ε�����
    if( usTick_100ms >=100)  { usTick_100ms=0;    vTask_100ms(); }   // ÿ100ms ִ��1�ε�����
    if( usTick_500ms >=500)  { usTick_500ms=0;    vTask_500ms(); }   // ÿ500ms ִ��1�ε�����
    if( usTick_1000ms>=1000) { usTick_1000ms=0;   vTask_1000ms();}   // ÿ1000msִ��1�� ������
}



/*****************************************************************************
 ** 1ms
 ** ÿ���1ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_1ms(void)
{   
    // ÿ1msִ��һ�εĴ��� 
                
    XPT2046_TouchHandler();           // ������״̬��⣺1:����Ƿ��а��£�2:����а��£��ʹ����ð��µ�����                  
    
}



/*****************************************************************************
 ** 2ms
 ** ÿ���2ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_2ms(void)
{  
    
    
    
}



/*****************************************************************************
 ** 5ms
 ** ÿ���5ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_5ms(void)
{    
    
    
}



/*****************************************************************************
 ** 10ms
 ** ÿ���10ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_10ms(void)
{    
    
    
    
}

/*****************************************************************************
 ** 50ms
 ** ÿ���50ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_50ms(void)
{       

    
    // 4: ESP8266���մ���            
//    if(xUSART.USART2ReceivedFlag==1)                             // ���ESP8266���õ�UART4�Ƿ��յ�����
//    {
//        sprintf(strTemp, "\rESP8266���յ���%3d�����ݣ�%s\r", ++CNT_ESP8266, (char *)xUSART.USART2ReceivedBuffer);   // ��ʽ���ַ���        
//        USART1_SendString(strTemp);                              // ͨ��UART1�����������������λ�� 
//        xUSART.USART2ReceivedFlag=0;                             // ������󣬰�RS485���õ�UART4���յ����ݱ�־��0���Է�����һ֡���ݵĽ���
//    } // end if ESP8266    

}



/*****************************************************************************
 ** 100ms
 ** ÿ���100ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_100ms(void)
{   

}



/*****************************************************************************
 ** 500ms
 ** ÿ���500ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_500ms(void)
{
           

}



/*****************************************************************************
 ** 1000ms
 ** ÿ���1000ms����vScheduler_Run()����һ��
 ****************************************************************************/
void vTask_1000ms(void)
{    
    
    
}

