/**
 ****************************************************************************************************
 * @file        rs485.h

 ****************************************************************************************************
 * @attention
 *

 *
 ****************************************************************************************************
 */

#ifndef __RS485_H
#define __RS485_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* RS485 引脚 和 串口 定义 
 * 默认是针对RS4852的.
 * 注意: 通过修改这10个宏定义, 可以支持UART1~UART7任意一个串口.
 */
#define RS485_RE_GPIO_PORT                  GPIOF
#define RS485_RE_GPIO_PIN                   GPIO_PIN_12
#define RS485_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOF_CLK_ENABLE(); }while(0)   /* PD口时钟使能 */

#define RS485_TX_GPIO_PORT                  GPIOD
#define RS485_TX_GPIO_PIN                   GPIO_PIN_5
#define RS485_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define RS485_RX_GPIO_PORT                  GPIOD
#define RS485_RX_GPIO_PIN                   GPIO_PIN_6
#define RS485_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOD_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define RS485_UX                            USART2
#define RS485_UX_IRQn                       USART2_IRQn
#define RS485_UX_IRQHandler                 USART2_IRQHandler
#define RS485_UX_CLK_ENABLE()               do{ __HAL_RCC_USART2_CLK_ENABLE(); }while(0)  /* USART2 时钟使能 */


#define ID               0x01       // 传感器地址
#define READ             0x03       // 读功能码
#define REGISTERADRR1    0x01
#define REGISTERADRR2    0x01       // 实时值
#define REGISTERnum1     0x00       // 寄存器数量
#define REGISTERnum2     0x01       // 寄存器数量
#define CHECK1           0xD4       // 校验位1
#define CHECK2           0x36       // 校验位2


 


/******************************************************************************************/

/* 控制RS485_RE脚, 控制RS485发送/接收状态
 * RS485_RE = 0, 进入接收模式
 * RS485_RE = 1, 进入发送模式
 */
 
#define RS485_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_PORT, RS485_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_PORT, RS485_RE_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)


#define RS485_REC_LEN               7          /* 定义最大接收字节数 64 */
#define RS485_EN_RX                 1           /* 使能（1）/禁止（0）RS485接收 */


extern uint8_t g_RS485_rx_buf[RS485_REC_LEN];   /* 接收缓冲,最大RS485_REC_LEN个字节 */
extern uint8_t g_RS485_rx_cnt;                  /* 接收数据长度 */


void rs485_init( uint32_t baudrate);  /* RS485初始化 */
void rs485_send_data(uint8_t *buf, uint8_t len);    /* RS485发送数据 */
void rs485_receive_data(uint8_t *buf, uint8_t *len);/* RS485接收数据 */
#endif







