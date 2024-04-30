#ifndef __UNDERPANCONTROL_H
#define __UNDERPANCONTROL_H

#include "./SYSTEM/sys/sys.h"


/* ???? */
#define underpan_H    0.55          /*轴距*/

#define PI            3.1415926    /*常数PI*/

#define GEAR_RATIO		29.5              /* 减速比 */
#define Diameter        0.354                /* 轮毂直径 354mm  */
#define MoterMaxr      5000.0                /*最大转速 175PRM */
#define MoterMaxrN      175.0                /*最大转速 175PRM */
#define KMPH_TO_MPS     0.277778             /*km/h --》 m/s转换系数*/
#define MPS_TO_DUTY     3.333333               /*m/s --》 占空比 转换系数*/
#define MPS_TO_KMPH    0.277778             /*km/h --》 m/s转换系数*/

/* 地盘运动状态  */

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

typedef struct /*底盘控制输入*/
{
 double adcx;
 double adcy;
 double max_underpanVelocity;
 uint8_t set_Maximum_Strspeed;
 uint8_t set_Maximum_Steespeed;
}VELOCITY_PIn;

typedef struct /*底盘控制输出*/
{
	double underpanVelocity ; /*底盘速度 m/s */
	double presentation_velocity; /* 显示速度预留变量*/
    double theta;                /*速度调整摇杆在二维坐标系上的位置与二维坐标系的x 轴的角度θ*/
	double acceleration_coeff;   /*加速系数*/
	double steering_angle;   /*转向角*/
	double L_Velocity ;      /* ????? m/s */
	double R_Velocity ;      /* ????? m/s */
	double L_NVelocity;      /* ???? PRM*/
	double R_NVelocity;      /* ????*/
    double L_Dutycycle;      /* ?????*/
	double R_Dutycycle;      /* ????? */

	double A_IN1       ;    /*????????? */
	double A_IN2       ;    /*????????? */
	double B_IN1       ;    /*????????? */
	double B_IN2       ;    /*????????? */
	RunState runstate  ;   /*?????????*/
} VELOCITY_POUT;

/*速度规划*/

/* 定义电机速度曲线类型枚举 */
typedef enum SpeedCurve {
 CURVE_NONE=0,  //直启
 CURVE_TRAP=1,  //梯形曲线
 CURVE_SPTA=2   //S型曲线
}SpeedCurveType;

/* 定义电机速度曲线对象 */
typedef struct CurveObject {
 float startSpeed;    //开始调速时的初始速度
 float currentSpeed;   //当前速度
 float targetSpeed;   //目标速度
 float stepSpeed;    //加速度
 float speedMax;     //最大速度
 float speedMin;     //最小速度
 uint32_t aTimes;    //调速时间
 uint32_t maxTimes;   //调速跨度
 SpeedCurveType curveMode;  //曲线类型
 float flexible;     //S曲线拉伸度
}CurveObjectType;

int32_t  Value_Resetzero(int32_t min_value ,int32_t current_value ,int32_t max_value);
void velocity_plan(VELOCITY_PIn);	
void velocity_maping(VELOCITY_PIn  velPlanIn);

void MotorVelocityCurve(CurveObjectType *curve);
static void CalCurveTRAP(CurveObjectType *trap);
static void CalCurveSPTA(CurveObjectType *spta);

#endif

