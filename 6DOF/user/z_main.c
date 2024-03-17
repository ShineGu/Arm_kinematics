/****************************************************************************
	*	@笔者	：	W
	*	@日期	：	2019年12月03日
	*	@所属	：	杭州众灵科技
	*	@论坛	：	www.ZL-robot.com
	*	@功能	：	ZL-KPZ控制板（32版）模块例程例程8————PWM舵机控制
	*	@函数列表:
	*	1.	void main(void) -- 主函数
 ****************************************************************************/

#include "z_main.h"
#include "math.h"

/*******全局变量定义*******/
u8 i=0;
servo_data_t servo_data[SERVO_NUM];
u8 servo_index;
float startDuty[6] = {1500, 1500, 1500, 1500, 1500, 1167};
float targetDuty[6];
float start[6];
float target[6];
float Start[6];
float Target[6];
u8 flag=0;

//float pos1[3] = {5, 20, 10};
//float pos2[3] = {20, 5, 10};
float pos1[3] = {10, 27.62, 5.58};
float pos2[3] = {50, 40, 10};

void Angle2Duty(float Angle[6], float Duty[6]){ // 角度转占空比
	for(i=0; i<6; i++){
		Duty[i] = 100 * (Angle[i] / 9 + 5); // 100*(角度/9+5)
	}
}

float LinearInterpolation(float startAngle, float targetAngle, float stepAngle){ // 线性插值
	
	if (startAngle - targetAngle < stepAngle && startAngle - targetAngle > -stepAngle){ 
	// 当开始角度和目标角度相差小于一个步长时，直接输出目标角度
		return targetAngle;
	}
	else{
		// 当开始角度和目标角度相差大于一个步长时，每次输出减去一个步长或加上一个步长
		if (startAngle > targetAngle){
			startAngle = startAngle - stepAngle;
		}
		if (startAngle < targetAngle){
			startAngle = startAngle + stepAngle;
		}
		return startAngle;
	}
}

void control(float TargetDuty[6], int flag1, int flag2, int flag3){ // 舵机分块控制
	if(flag1){
		// 0号舵机控制，机械臂整体旋转角度
		servo_data[0].cur = LinearInterpolation(servo_data[0].cur, TargetDuty[0], 10);
	}
	if(flag2){
		// 1、2、3、4号舵机，定位主要舵机
		servo_data[1].cur = LinearInterpolation(servo_data[1].cur, TargetDuty[1], 10);
		servo_data[2].cur = LinearInterpolation(servo_data[2].cur, TargetDuty[2], 10);
		servo_data[3].cur = LinearInterpolation(servo_data[3].cur, TargetDuty[3], 10);
		servo_data[4].cur = LinearInterpolation(servo_data[4].cur, TargetDuty[4], 10);
	}
	if(flag3){
		// 5号舵机，夹取和放下物体
		servo_data[5].cur = LinearInterpolation(servo_data[5].cur, TargetDuty[5], 90);
	}
}

int over(float TargetDuty[6]){ // 舵机位置检测
	if(
		// 1、2、3、4号舵机定位判断
	servo_data[1].cur == TargetDuty[1] &&
	servo_data[2].cur == TargetDuty[2] &&
	servo_data[3].cur == TargetDuty[3] &&
	servo_data[4].cur == TargetDuty[4] ){
		return 1;
	}
	else{
		return 0;
	}
}

void value(float StartDuty[6], float TargetDuty[6]){ // 数组赋值
	for (i=0;i<6;i++){
		StartDuty[i] = TargetDuty[i]; // 将数组每一个值转换到目标数组中
	}
}

void move(float Angle[6], int flag1, int flag2, int flag3, int flag_value){ // 先后顺序控制
	float duty[6];
	Angle2Duty(Angle, duty); // 输入角度转占空比
	value(targetDuty, duty); // 将占空比放入目标占空比
	control(targetDuty, flag1, flag2, flag3); // 分块控制
	delay_ms(40); // 步骤动作延时
	if(over(targetDuty) == 1 && servo_data[0].cur == targetDuty[0]){ // 判断是否可以夹取
		control(targetDuty, 1, 1, 1); // 分块控制
		if(servo_data[5].cur == targetDuty[5]) // 判断是否夹取
			flag = flag_value;
	}
}

void putup(float start[6], float target[6]){ // 拿放动作
	// 四步骤动作
	if(flag == 0){ // 拿取物体
		float angle[6];
		value(angle, start);
		angle[5] = 90;
		move(angle, 1, 0, 0, 1);
		if(servo_data[0].cur == targetDuty[0]){
			move(angle, 1, 1, 0, 1);
		}
	}
	else if(flag == 1){ // 位置重置
		float angle[6] = {90, 90, 90, 90, 90, 90};
		angle[0] = start[0];
		angle[5] = start[5];
		move(angle, 0, 1, 0, 2);
		if(over(targetDuty)){
			move(angle, 1, 1, 0, 2);
		}
	}
	else if(flag == 2){ // 放下物体
		float angle[6];
		value(angle, target);
		angle[5]=60;
		move(angle, 1, 0, 0, 3);
		if(servo_data[0].cur == targetDuty[0]){
			move(angle, 1, 1, 0, 3);
		}
	}
	else if(flag == 3){ // 位置重置
		float angle[6] = {90, 90, 90, 90, 90, 60};
		move(angle, 0, 1, 0, 4);
		if(over(targetDuty)){
			move(angle, 1, 1, 0, 4);
		}
	}
}

void ikine(float pos[3], float result[6]){
	float x=pos[0];
	float y=pos[1];
	float z=pos[2];
	float A1 = 8.55;
	float A2 = 10.5;
	float A3 = 9.8;
	float A4 = 14.7;
	float j1 = atan2(y,x);
	float len = sqrt(y*y+x*x);
	float high = z;
	float alpha; // j2+j3+j4
	float L,H,c3,s3,K1,K2,w; // 中间变量
	float j2,j3,j4; // 2、3、4号舵机旋转角度
//	alpha角度范围求解
	for(alpha=-3.14;alpha<3.14;alpha+=0.01){
		L = len-A4*sin(alpha);
		H = high-A1-A4*cos(alpha);
		c3 = (L*L+H*H-A2*A2-A3*A3)/(2*A2*A3);
		if(c3*c3 < 1){
			s3 = sqrt(1-c3*c3);
			j3 = atan2(s3, c3);
			K1 = A2 + A3*cos(j3);
			K2 = A3*sin(j3);
			w = atan2(K2, K1);
			j2 = atan2(L, H)-w;
			j4 = alpha - j2 - j3;
		}
	}
	result[0] = j1/3.14*180; // 角度弧度转换
	result[1] = 90+j2/3.14*180;
	result[2] = 90+j3/3.14*180;
	result[3] = j4/3.14*180;
}

int main(void) {																																																																																																																															//ZL
	setup_systick();	//初始化滴答时钟，1S增加一次systick_ms的值
	setup_key();		//初始化按键
	setup_servo();		//初始化舵机相关设置
	servo_data[0].cur = startDuty[0];
	servo_data[1].cur = startDuty[1];
	servo_data[2].cur = startDuty[2];
	servo_data[3].cur = startDuty[3];
	servo_data[4].cur = startDuty[4];
	servo_data[5].cur = startDuty[5];
	
	ikine(pos1, Start);
	ikine(pos2, Target);
	
//	Start[0] = 75;
//	Start[1] = 60;
//	Start[2] = 120;
//	Start[3] = 120;
	Start[4] = 90;
	Start[5] = 90;
	
//	Target[0] = 90;
//	Target[1] = 120;
//	Target[2] = 60;
//	Target[3] = 60;
	Target[4] = 90;
	Target[5] = 60;
	
	while (1) {
		loop_key();			//循环检测按键状态
		loop_servo();		//循环处理舵机的指令

		putup(Start, Target);
	}
}
