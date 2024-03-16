/**
 ****************************************************************************************************
 * @file        led.h
 * @author      Lisir
 * @version     V1.0
 * @date        2021-10-14
 * @brief       LED ��������
 * @license     Copyright (c) 2024, ���ڸ���ҽ�ƿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:F407������
 * none
 * none
 * none
 * none
 *
 * �޸�˵��
 * none
 * ��һ�η���
 *
 ****************************************************************************************************
 */
#ifndef __LED_H
#define __LED_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ���� ���� */

#define LED0_GPIO_PORT                  GPIOG
#define LED0_GPIO_PIN                   GPIO_PIN_2
#define LED0_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)             /* PF��ʱ��ʹ�� */

#define LED1_GPIO_PORT                  GPIOG
#define LED1_GPIO_PIN                   GPIO_PIN_3
#define LED1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)             /* PF��ʱ��ʹ�� */





/******************************************************************************************/

/* LED�˿ڶ��� */
#define LED0(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED0_GPIO_PORT, LED0_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED0 = RED */

#define LED1(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED1_GPIO_PORT, LED1_GPIO_PIN, GPIO_PIN_RESET); \
                  }while(0)       /* LED1 = GREEN */

/* LEDȡ������ */
#define LED0_TOGGLE()    do{ HAL_GPIO_TogglePin(LED0_GPIO_PORT, LED0_GPIO_PIN); }while(0)       /* LED0 = !LED0 */
#define LED1_TOGGLE()    do{ HAL_GPIO_TogglePin(LED1_GPIO_PORT, LED1_GPIO_PIN); }while(0)       /* LED1 = !LED1 */

/******************************************************************************************/

typedef enum 
{
   idle_state,
   open_leftbling,
   open_rightbling,
   open_doublebling,
   close_leftbling,
   close_rightbling, 
   close_doublebling 
}Led_State ;


/* �ⲿ�ӿں���*/
void led_init(void);  
void led_bling(void);                                                                                                    /* ��ʼ�� */

#endif                                                                                                                       /*��ת��ƿ��ￄ1�7*/
