
#ifndef __HOSTMODBUS_H
#define __HOSTMODBUS_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* Modbus ���� �� ���� ���� 
 * Ĭ�������Modbus2��.
 * ע��: ͨ���޸���10���궨��, ����֧��UART1~UART7����һ������.
 */
#define HostModbus_RE_GPIO_PORT                  GPIOG
#define HostModbus_RE_GPIO_PIN                   GPIO_PIN_8
#define HostModbus_RE_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOG_CLK_ENABLE(); }while(0)   /*  */

#define HostModbus_TX_GPIO_PORT                  GPIOA
#define HostModbus_TX_GPIO_PIN                   GPIO_PIN_2
#define HostModbus_TX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /*  */

#define HostModbus_RX_GPIO_PORT                  GPIOA
#define HostModbus_RX_GPIO_PIN                   GPIO_PIN_3
#define HostModbus_RX_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* */

#define HostModbus_UX                            UART5
#define HostModbus_UX_IRQn                       UART5_IRQn
#define HostModbusKey_UX_IRQHandler              UART5_IRQHandler
#define HostModbus_UX_CLK_ENABLE()               do{ __HAL_RCC_UART5_CLK_ENABLE(); }while(0)  /* USART6 ʱ��ʹ�� */


/******************************************************************************************/

/* ����Modbus_RE��, ����Modbus����/����״̬
 * Modbus_RE = 0, �������ģʽ
 * Modbus_RE = 1, ���뷢��ģʽ
 */
#define HostModbus_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(HostModbus_RE_GPIO_PORT, HostModbus_RE_GPIO_PIN, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(HostModbus_RE_GPIO_PORT, HostModbus_RE_GPIO_PIN, GPIO_PIN_RESET); \
                      }while(0)



 					  
/******************************************************************************************/					  
typedef struct 
{
	//��Ϊ�ӻ�ʱʹ��
	uint8_t  myadd;        //���豸�ӻ���ַ
	uint8_t  rcbuf[100];   //modbus���ܻ�����
	uint8_t  timout;       //modbus���ݳ���ʱ��
	uint8_t  recount;      //modbus�˿ڽ��յ������ݸ���
	uint8_t  timrun;       //modbus��ʱ���Ƿ��ʱ��־
	uint8_t  reflag;       //modbusһ֡���ݽ�����ɱ�־λ
	uint8_t  sendbuf[100]; //modbus�ӷ��ͻ�����
	
	//��Ϊ������Ӳ���
	uint8_t Host_Txbuf[8];	//modbus��������	
	uint8_t slave_add;		//Ҫƥ��Ĵӻ��豸��ַ��������ʵ��ʱʹ�ã�
	uint8_t Host_send_flag;//�����豸����������ϱ�־λ
	int Host_Sendtime;//������һ֡���ݺ�ʱ�����
	uint8_t Host_time_flag;//����ʱ�䵽��־λ��=1��ʾ����������ʱ����
	uint8_t Host_End;//�������ݺ������	
	
}HOSTMODBUS;

extern HOSTMODBUS hostmodbus;
extern uint16_t Host06Recive[100];

extern uint16_t Host03Recive[100];


// Host
void HostModbusKeyUART_init(uint32_t baudrate);
void HostModbuskey_send_data(uint8_t *buf, uint8_t len);
void HostModbuskey_Init(void);
uint16_t  Modbuskey_CRC16( uint8_t *puchMsg, uint16_t usDataLen );

void Host_Send03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);

void Host_ReceiveFunc3();

void HOST_ModbusRX();

void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num);

void Host_Func6Back();
#endif
 






