/**
 ****************************************************************************************************
 * @file        led.c
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED 驱动代码
 * @license     Copyright (c) 2024, 深圳复成医疗科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:F407开发板
 * none
 * none
 * none
 * none
 *
 * 修改说明
 * none
 * 第一次发布
 *
 ****************************************************************************************************
 */
 
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./SYSTEM/delay/delay.h"

#include "./BSP/BEEP/beep.h"

/**
 * @brief       初始化LED相关IO口, 并使能时钟
 * @param       无
 * @retval      无

 */

void led_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    
    LED0_GPIO_CLK_ENABLE();                                 /* LED0时钟使能 */
    LED1_GPIO_CLK_ENABLE();                                 /* LED1时钟使能 */

    gpio_init_struct.Pin = LED0_GPIO_PIN;                   /* LED0引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;          /* 高速 */
    HAL_GPIO_Init(LED0_GPIO_PORT, &gpio_init_struct);       /* 初始化LED0引脚 */

    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1引脚 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* 初始化LED1引脚 */
    
    LED0(1);                                                /* 关闭 LED0 */
    LED1(1);                                                /* 关闭 LED1 */
}

/**
 * @brief       转向灯及 双闪灯 控制函数
 * @note        按下 K0 左转向  按下K2 右转向  按下K1 双闪灯; 同一个按键二次按下输出相反状态，
				其它按键按下释放之前的状态；
 * @param       
 *   @arg       
 *                  
 *   @arg       
 * @retval     
 *              KEY0_PRES, 1, KEY0按下
 *              KEY1_PRES, 2, KEY1按下
 *              KEY2_PRES, 3, KEY2按下
 *              WKUP_PRES, 4, WKUP按下
 */

Led_State led_state;
uint8_t doubleflingflage = 1;
void led_bling(void)
{
    static uint16_t KEY0_PRES_contes = 1;
	static uint16_t KEY1_PRES_contes = 1;    
	static uint16_t KEY2_PRES_contes = 1;
    if ( KEY0_PRES_contes == 1 && KEY1_PRES_contes == 1 && KEY2_PRES_contes == 1)
    {
          led_state = idle_state; 
    }        
 /*左转向控制*/
    if(key_scan2() == 1)
    {
      
        KEY2_PRES_contes++;
       
        if (KEY2_PRES_contes > 2 )
              KEY2_PRES_contes = 1; 
         if (KEY2_PRES_contes%2 == 0 ) 
            {
               led_state = open_leftbling;
			   KEY0_PRES_contes = 1;
			   KEY1_PRES_contes = 1;
            }
         else
         {
               led_state = close_leftbling ;
         }
    }
    
   
/*右转向控制 */
    if(key_scan3() == 1)
    {
 
        KEY0_PRES_contes++;
       
        if (KEY0_PRES_contes > 2 )
              KEY0_PRES_contes =1; 
         if (KEY0_PRES_contes%2 == 0 ) 
            {
               led_state = open_rightbling;
			   KEY2_PRES_contes = 1;
			   KEY1_PRES_contes = 1;
            }
         else
         {
              led_state = close_rightbling;
         }
    }
/*双闪控制*/  
   if(key_scan6() ==1)
   {
   
       KEY1_PRES_contes++;
      
       if (KEY1_PRES_contes > 2 )
             KEY1_PRES_contes =1; 
        if (KEY1_PRES_contes%2 == 0 ) 
           {
              led_state = open_doublebling;
			   KEY0_PRES_contes = 1;
			   
			   KEY2_PRES_contes = 1;
           }
        else
        {
               led_state = close_doublebling;
        }
   }
    
    switch ( led_state )
    {
        case  idle_state :
            
            LED0(1);
            LED1(1);
        break;    
        
        case  open_leftbling:
            LED0_TOGGLE();
          //  delay_ms(100);
            LED1(1);
        break;
        
        case  close_leftbling:
            LED0(1);    
        break;
        
        case  open_rightbling:
            LED1_TOGGLE();
           // delay_ms(100);
            LED0(1);
        break;
         
        case  close_rightbling:
            LED1(1);
        break;
		
		case open_doublebling:
			if (doubleflingflage)
			{
				LED0(1);
				LED1(1);
				doubleflingflage = 0;
			}
			else
			{
				doubleflingflage = 1;
				LED0(0);
				LED1(0);
			}
			
			
		break;
		    
		case close_doublebling:
		    LED0(1);
		    LED1(1);
			
        break;
		
        default : break; 
    }

    //  主灯控制  
    if (key_scan10() == 1)
    {
        LED0(0);
        LED1(0);
    }
    else
    {
        ;   // 主灯信号 接入后进行控制
    }

    //  喇叭 控制   
    if (key_scan7() == 1)
    {
          BEEP(1);
    }
    else
    {
          BEEP(0); 
    }

}

