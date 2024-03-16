#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"


/* 常量定义 */
#define underpan_H    0.55   

#define PI            3.1415926

#define GEAR_RATIO		16.75               /* 减速比  */
#define Diameter        0.270                 /* 轮子直径  */
#define MoterMaxr      2200.0                /*电机转速 r/min  */

/* 控制结构体  */

typedef enum
{
    idle = 0,
    forward,
    backward,
	front_left,
	front_right,
	back_left,
	back_right,
	turnself_left,
	turnself_right
}RunState;

typedef struct /*速度规划输入*/
{
 int16_t adcx;
 int16_t adcy;
 float max_underpanVelocity;
 double  k;
}VELOCITY_PIn;

typedef struct /* 速度规划输出*/
{
	double underpanVelocity ; /*规划实时 速度 单位 m/s */
	double presentation_velocity; /* 上位机显示速度 m/s*/

	double steering_angle;
	double L_Velocity ;      /* 左轮线速度 */
	double R_Velocity ;      /* 右轮线速度*/
	double L_NVelocity;      /* 左轮电机转速*/
	double R_NVelocity;      /* 右轮电机转速*/
    double L_Dutycycle;      /* 左轮占空比*/
	double R_Dutycycle;      /* 右轮占空比 */

	double A_IN1       ;    /*左电机占空比输出控制端口1 */
	double A_IN2       ;    /*左电机占空比输出控制端口2 */
	double B_IN1       ;    /*右电机占空比输出控制端口1 */
	double B_IN2       ;    /*右电机占空比输出控制端口2 */
	RunState runstate  ;
	
	
	
} VELOCITY_POUT;


int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value);
void velocity_plan();	

#endif

