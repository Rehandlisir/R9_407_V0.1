/**
 ****************************************************************************************************
 * @file        modbus.c
 * @author      R9团队
 * @version     V1.0
 * @date        2021-10-23
 * @brief       Modbus 驱动代码
 * @license    
 ****************************************************************************************************
 * @attention

 *
 ****************************************************************************************************
 */


#include "./SYSTEM/delay/delay.h"
#include "./BSP/R9/Hostmodbus.h"

UART_HandleTypeDef g_hostmodbus_handler;     /* Modbus控制句柄(串口) */

HOSTMODBUS hostmodbus;//结构体变量
	   
uint16_t Host03Recive[100] ={0x0000};

uint16_t Host06Recive[100] ={0x0000};
                                   //主机接受到的数	   

void HostModbusKey_UX_IRQHandler(void)
	
{
    uint8_t res;

    if ((__HAL_UART_GET_FLAG(&g_hostmodbus_handler, UART_FLAG_RXNE) != RESET)) /* 接收到数据 */
    {
        HAL_UART_Receive(&g_hostmodbus_handler, &res, 1, 1000);

		if( hostmodbus.reflag==1)  //有数据包正在处理
		{
		   return ;
		}		
		hostmodbus.rcbuf[hostmodbus.recount++] = res;
		hostmodbus.timout = 0;
		if(hostmodbus.recount == 1)  //已经收到了第二个字符数据
		{
		  hostmodbus.timrun = 1;  //开启modbus定时器计时
		}		
		
    }
}



/**
 * @brief       Modbus初始化函数
 *   @note      该函数主要是初始化串口
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void HostModbusKeyUART_init(uint32_t baudrate)
{
    /* IO 及 时钟配置 */
    HostModbus_RE_GPIO_CLK_ENABLE(); /* 使能 Modbus_RE 脚时钟 */
    HostModbus_TX_GPIO_CLK_ENABLE(); /* 使能 串口TX脚 时钟 */
    HostModbus_RX_GPIO_CLK_ENABLE(); /* 使能 串口RX脚 时钟 */
    HostModbus_UX_CLK_ENABLE();      /* 使能 串口 时钟 */

    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin = HostModbus_TX_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_AF_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    gpio_initure.Alternate = GPIO_AF7_USART2;               /* 复用为串口6 */
    HAL_GPIO_Init(HostModbus_TX_GPIO_PORT, &gpio_initure);       /* 串口TX 脚 模式设置 */

    gpio_initure.Pin = HostModbus_RX_GPIO_PIN;
    HAL_GPIO_Init(HostModbus_RX_GPIO_PORT, &gpio_initure);       /* 串口RX 脚 必须设置成输入模式 */

    gpio_initure.Pin = HostModbus_RE_GPIO_PIN;
    gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_initure.Pull = GPIO_PULLUP;
    gpio_initure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(HostModbus_RE_GPIO_PORT, &gpio_initure);       /* Modbus_RE 脚 模式设置 */

    /* USART 初始化设置 */
    g_hostmodbus_handler.Instance = HostModbus_UX;                    /* 选择485对应的串口 */
    g_hostmodbus_handler.Init.BaudRate = baudrate;               /* 波特率 */
    g_hostmodbus_handler.Init.WordLength = UART_WORDLENGTH_8B;   /* 字长为8位数据格式 */
    g_hostmodbus_handler.Init.StopBits = UART_STOPBITS_1;        /* 一个停止位 */
    g_hostmodbus_handler.Init.Parity = UART_PARITY_NONE;         /* 无奇偶校验位 */
    g_hostmodbus_handler.Init.HwFlowCtl = UART_HWCONTROL_NONE;   /* 无硬件流控 */
    g_hostmodbus_handler.Init.Mode = UART_MODE_TX_RX;            /* 收发模式 */
    HAL_UART_Init(&g_hostmodbus_handler);                        /* 使能对应的串口, 但会调用MSp */
    __HAL_UART_DISABLE_IT(&g_hostmodbus_handler, UART_IT_TC);


      /* 使能接收中断 */
    __HAL_UART_ENABLE_IT(&g_hostmodbus_handler, UART_IT_RXNE);   /* 开启接收中断 */
    HAL_NVIC_EnableIRQ(HostModbus_UX_IRQn);                      /* 使能USART1中断 */
    HAL_NVIC_SetPriority(HostModbus_UX_IRQn, 3, 3);              /* 抢占优先级3，子优先级3 */


    HostModbus_RE(0); /* 默认为接收模式 */
}

/**
 * @brief       Modbus发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 Modbus_REC_LEN 个字节)
 * @retval      无
 */
void HostModbuskey_send_data(uint8_t *buf, uint8_t len)
{
    HostModbus_RE(1);                                            /* 进入发送模式 */
    HAL_UART_Transmit(&g_hostmodbus_handler, buf, len, 1000);    /* 串口6发送数据 */
//    g_Modbus_rx_cnt = 0;
    HostModbus_RE(0); /* 进入接收模式 */
}
/******************************************************************************************/

//作为主机部分的代码
// Modbus初始化函数
void HostModbuskey_Init(void)
{
  HostModbusKeyUART_init(9600);
//  hostmodbus.myadd = 0x11; //从机设备地址为11
  hostmodbus.timrun = 0;    //modbus定时器停止计算
  
}


/******************************************************************
功能: CRC16校验
输入:
输出:
******************************************************************/
/* CRC 高位字节值表 */
const uint8_t host_auchCRCHi[] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1,
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
} ;
/* CRC低位字节值表*/
const uint8_t host_auchCRCLo[] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06,
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD,
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A,
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4,
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3,
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29,
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED,
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60,
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67,
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E,
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71,
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92,
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B,
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B,
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42,
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40
} ;
uint16_t  Modbuskey_CRC16( uint8_t *puchMsg, uint16_t usDataLen )
{
    uint8_t uchCRCHi = 0xFF ; // 高CRC字节初始化
    uint8_t uchCRCLo = 0xFF ; // 低CRC 字节初始化
    unsigned long uIndex ; 		// CRC循环中的索引

    while ( usDataLen-- ) 	// 传输消息缓冲区
    {
        uIndex = uchCRCHi ^ *puchMsg++ ; 	// 计算CRC
        uchCRCHi = uchCRCLo ^ host_auchCRCHi[uIndex] ;
        uchCRCLo = host_auchCRCLo[uIndex] ;
    }

    return ( uchCRCHi << 8 | uchCRCLo ) ;
}

//主机选择从机
//参数1从机，参数2起始地址，参数3寄存器个数
void Host_Send03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num)
{
	int j;
	uint16_t crc;//计算的CRC校验位
	hostmodbus.slave_add=slave;
	hostmodbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
	hostmodbus.Host_Txbuf[1]=0x03;//功能码
	hostmodbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
	hostmodbus.Host_Txbuf[3]=StartAddr%256;//起始地址  低位
	hostmodbus.Host_Txbuf[4]=num/256;//寄存器个数高位
	hostmodbus.Host_Txbuf[5]=num%256;//寄存器个数低位
	crc=Modbuskey_CRC16(&hostmodbus.Host_Txbuf[0],6); //获取CRC校验位
	hostmodbus.Host_Txbuf[6]=crc/256;//寄存器个数高位
	hostmodbus.Host_Txbuf[7]=crc%256;//寄存器个数低位
	
	//待发送数据打包完毕
	HostModbus_RE(1);;//使能485控制端(启动发送)  
//	HAL_Delay(100);
	for(j=0;j<8;j++)
	{
		HostModbuskey_send_data(&hostmodbus.Host_Txbuf[j],1);
	}
	HostModbus_RE(0);//失能485控制端（改为接收）
	hostmodbus.Host_send_flag=1;//表示发送数据完毕
}


//第三个是字节个数
//主机处理接受到的数据
void Host_ReceiveFunc3()
{
	int i,j =0 ;
	int count=(int)hostmodbus.rcbuf[2];//这是数据个数
	
	//printf("从机返回 %d 个寄存器数据：\r\n",count/2);
	for(i=0;i<count;i=i+2)
	{
	//	printf("Temp_Hbit= %d Temp_Lbit= %d temp= %d\r\n",(int)modbus.rcbuf[3+i],(int)modbus.rcbuf[4+i],(int)modbus.rcbuf[4+i]+((int)modbus.rcbuf[3+i])*256);
	
		Host03Recive[j++] = (int)hostmodbus.rcbuf[4+i]+((int)hostmodbus.rcbuf[3+i])*256	;
		
	}
	
	hostmodbus.Host_End=1;//接收的数据处理完毕
}

//主机接收从机的消息进行处理
void HOST_ModbusRX()
{
	uint16_t crc,rccrc;//计算crc和接收到的crc

  if(hostmodbus.reflag == 0)  //如果接收未完成则返回空
	{
	   return;
	}
	//接收数据结束

	//（数组中除了最后两位CRC校验位其余全算）
	crc = Modbuskey_CRC16(&hostmodbus.rcbuf[0],hostmodbus.recount-2); //获取CRC校验位
	rccrc = hostmodbus.rcbuf[hostmodbus.recount-2]*256+hostmodbus.rcbuf[hostmodbus.recount-1];//计算读取的CRC校验位

	if(crc == rccrc) //CRC检验成功 开始分析包
	{	
	   if(hostmodbus.rcbuf[0] == hostmodbus.slave_add)  // 检查地址是是对应从机发过来的
		 {
			 if(hostmodbus.rcbuf[1]==3)//功能码时03

		      Host_ReceiveFunc3();//这是读取寄存器的有效数据位进行计算
		 }
		 
	}	
	 hostmodbus.recount = 0;//接收计数清零
     hostmodbus.reflag = 0; //接收标志清零
	
}

//向一个寄存器中写数据的参数设置
void Host_write06_slave(uint8_t slave,uint8_t fun,uint16_t StartAddr,uint16_t num)
{
	uint16_t crc,j;//计算的CRC校验位
	hostmodbus.slave_add=slave;//从机地址赋值一下，后期有用
	hostmodbus.Host_Txbuf[0]=slave;//这是要匹配的从机地址
	hostmodbus.Host_Txbuf[1]=fun;//功能码
	hostmodbus.Host_Txbuf[2]=StartAddr/256;//起始地址高位
	hostmodbus.Host_Txbuf[3]=StartAddr%256;//起始地址低位
	hostmodbus.Host_Txbuf[4]=num/256;
	hostmodbus.Host_Txbuf[5]=num%256;
	crc=Modbuskey_CRC16(&hostmodbus.Host_Txbuf[0],6); //获取CRC校验位
	hostmodbus.Host_Txbuf[6]=crc/256;//寄存器个数高位
	hostmodbus.Host_Txbuf[7]=crc%256;//寄存器个数低位
	
   //发送数据包装完毕
	HostModbus_RE(1);//使能485控制端(启动发送)  
    delay_ms(50);
	for(j=0;j<8;j++)
	{
		HostModbuskey_send_data(&hostmodbus.Host_Txbuf[j],1);
	}
    delay_ms(10);          
	HostModbus_RE(0);;//失能485控制端（改为接收）
	hostmodbus.Host_send_flag=1;//表示发送数据完毕

}

//从机返回数据
void Host_Func6Back()
{
	int crc,rccrc;
	crc = Modbuskey_CRC16(&hostmodbus.rcbuf[0],6); //获取CRC校验位
	rccrc = hostmodbus.rcbuf[6]*256+hostmodbus.rcbuf[7];//计算读取的CRC校验位
	if(crc == rccrc) //CRC检验成功 开始分析包
	{	
	   if(hostmodbus.rcbuf[0] == hostmodbus.slave_add)  // 检查地址是是对应从机发过来的
		 {
			 if(hostmodbus.rcbuf[1]==6)//功能码时06
			 {
		
//						printf("地址为 %d 的从机寄存器 %d 中写入数据 %d \r\n ",modbus.rcbuf[0],modbus.rcbuf[3]+(modbus.rcbuf[2])*256,modbus.rcbuf[5]+(modbus.rcbuf[4])*256);
//						printf("Host_06 write data right!\r\n");
				
			 }
		 }
		 
	}	
		
		hostmodbus.Host_End=1;//接收的数据处理完毕
}


