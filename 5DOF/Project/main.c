#include "stm32f10x.h"
#include "math.h"

#define DELAY_TIME 20

volatile uint32_t time;
float temper = 20;
float length = 0;
float angle[5] = {90, 70, 160, 90, 155};
float newAngle[5];
float duty[5];
uint32_t i;
uint32_t j;
uint32_t k;
unsigned char key=0;
float dt = 20;
float t = 3000;
float n = 0;
float tempAngle[5];
uint16_t flag = 0;
uint16_t flag_cal = 0;
float theta = 45;
float beta = 0;
float distance = 0;
float d1 = 8;
float d2 = 30;

void Angle2Duty(float Angle[5]){ // 角度转占空比
	for(i=0;i<5;i++){
		duty[i] = Angle[i] / 9 + 5;
	}
}

void delay_us(uint32_t us){ // 延时函数
	TIM5->CNT = 0;
	while(TIM5->CNT < us);
}

float temper2vel(float T) { // 温度下的声速
    return (331.45 + 0.61 * T) - 1;
}

unsigned char KeyScan() { // 按键检测
    static unsigned char key_up = 1;

    if (key_up && (GPIOB->IDR & 0x0020) != 0) {
        delay_us(1000); // 消抖
		if (key_up && (GPIOB->IDR & 0x0020) != 0){
			key_up = 0;
			return 1;
		}
    } else if ((GPIOB->IDR & 0x0020) == 0) {
        key_up = 1;
    }
    return 0;
}

float calDistance(float X, float Y, float Theta){ // 余弦定理测距离
	return sqrt(X*X+Y*Y-2*X*Y*cos((90-Theta)*3.14/180));
}

float calTheta(float X, float Y, float Z){ // 余弦定理测角度
	return (acos((X*X+Y*Y-Z*Z)/(2*X*Y)))*180/3.14;
}

void TimeConfig(void){ // 定时器配置

	// 启用TIM3和TIM4
	RCC->APB1ENR |= (1 << 0);
	RCC->APB1ENR |= (1 << 1);
    RCC->APB1ENR |= (1 << 2);
	RCC->APB1ENR |= (1 << 3);
	
	// 配置TIM2
	TIM2->CR1 = 0;            // 清除控制寄存器1
    TIM2->CR2 = 0;            // 清除控制寄存器2
    TIM2->SMCR = 0;           // 清除从模式控制寄存器
    TIM2->CCER = 0;           // 清除捕获/比较使能寄存器
    TIM2->CCMR1 = 0;          // 清除比较模式寄存器1
    TIM2->CCMR2 = 0;          // 清除比较模式寄存器2
    TIM2->CNT = 0;            // 清除计数器
    TIM2->PSC = 7199;         // 设置预分频器，产生1ms的定时器中断
    TIM2->ARR = 1999;          // 设置自动重装载寄存器，产生20ms的定时器中断
	
	//配置TIM3
    TIM3->CR1 = 0;            // 清除控制寄存器1
    TIM3->CR2 = 0;            // 清除控制寄存器2
    TIM3->SMCR = 0;           // 清除从模式控制寄存器
    TIM3->CCER = 0;           // 清除捕获/比较使能寄存器
    TIM3->CCMR1 = 0;          // 清除比较模式寄存器1
    TIM3->CCMR2 = 0;          // 清除比较模式寄存器2
    TIM3->CNT = 0;            // 清除计数器
    TIM3->PSC = 7199;         // 设置预分频器，产生1ms的定时器中断
    TIM3->ARR = 1999;          // 设置自动重装载寄存器，产生20ms的定时器中断

    // 配置TIM4
    TIM4->CR1 = 0;            // 清除控制寄存器1
    TIM4->CR2 = 0;            // 清除控制寄存器2
    TIM4->SMCR = 0;           // 清除从模式控制寄存器
    TIM4->CCER = 0;           // 清除捕获/比较使能寄存器
    TIM4->CCMR1 = 0;          // 清除比较模式寄存器1
    TIM4->CCMR2 = 0;          // 清除比较模式寄存器2
    TIM4->CNT = 0;            // 清除计数器
    TIM4->PSC = 7199;         // 设置预分频器，产生1ms的定时器中断
    TIM4->ARR = 1999;          // 设置自动重装载寄存器，产生20ms的定时器中断
	
	// 配置TIM5
	TIM5->CR1 = 0;            // 清除控制寄存器1
    TIM5->CR2 = 0;            // 清除控制寄存器2
    TIM5->SMCR = 0;           // 清除从模式控制寄存器
    TIM5->CCER = 0;           // 清除捕获/比较使能寄存器
    TIM5->CCMR1 = 0;          // 清除比较模式寄存器1
    TIM5->CCMR2 = 0;          // 清除比较模式寄存器2
    TIM5->CNT = 0;            // 清除计数器
    TIM5->PSC = 719;         // 设置预分频器，产生1ms的定时器中断
    TIM5->ARR = 9999;          // 设置自动重装载寄存器，产生1ms的定时器中断
	
	//配置TIM3通道4为PWM模式
	TIM3->CCMR2 |= 0x4040 | 0x2020; // PWM 模式1
    TIM3->CCMR2 |= 0x0808;          // 预加载
    TIM3->CCER |= 0x1100;           // 输出使能

    TIM4->CCMR1 |= 0x4040 | 0x2020; // PWM 模式1
    TIM4->CCMR1 |= 0x0808;          // 预加载
    TIM4->CCER |= 0x0011;           // 输出使能

    TIM4->CCMR2 |= 0x4040 | 0x2020; // PWM 模式1                 
    TIM4->CCMR2 |= 0x0808;          // 预加载
    TIM4->CCER |= 0x1100;           // 输出使能
	
	TIM2->CR1 |= 0x0001;
	TIM3->CR1 |= 0x0001;
	TIM4->CR1 |= 0x0001;
	TIM5->CR1 |= 0x0001;
	
}

void InterruptConfig(void){ // 中断配置
    // 启用TIM中断
	NVIC->ISER[28 / 32] = (1 << (28 % 32));
    TIM2->DIER |= 0x0001;
	
//    NVIC->ISER[29 / 32] = (1 << (29 % 32));
//    TIM3->DIER |= 0x0001;
//	
//    NVIC->ISER[30 / 32] = (1 << (30 % 32));
//    TIM4->DIER |= 0x0001;
}

void GpioConfig(void){ // GPIO配置
    RCC->APB2ENR |= (1 << 3); // 启用GPIOB时钟
	RCC->APB2ENR |= (1 << 4);
	
    GPIOB->CRL &= ~(0x00300000 | 0x00C00000); // 清除模式位和配置位
    GPIOB->CRL |= 0x00800000;                    // 设置引脚5为下拉输入
	GPIOB->CRL &= ~(0x00000300 | 0x00000C00);
	GPIOB->CRL |= 0x00000200; // 输出模式 2 MHz
	
    GPIOB->CRL &= ~0xCC0000CC;      // 清除原有配置
    GPIOB->CRL |= 0x88000088;     // 复用推挽输出
    GPIOB->CRL |= 0x33000033;      // 输出模式最大速率 50 MHz
			
	GPIOB->CRH &= ~0x000000CC;      // 清除原有配置
    GPIOB->CRH |= 0x00000088;     // 复用推挽输出
    GPIOB->CRH |= 0x00000033;      // 输出模式最大速率 50 MHz
	
    GPIOC->CRL &= ~(0x0C000000 | 0x03000000);
    GPIOC->CRL |= 0x03000000;  // 设置PC6为推挽输出

	GPIOC->CRL &= ~(0x00300000 | 0x00C00000);
	GPIOC->CRL |= 0x00200000; // 输出模式 2 MHz

    GPIOC->CRL &= ~(0xC0000000 | 0x30000000);
    GPIOC->CRL |= 0x40000000;  // 浮空输入

}

float LinearInterpolation(float startAngle, float targetAngle, float stepAngle){ // 线性插值
	
	if (startAngle - targetAngle < stepAngle && startAngle - targetAngle > -stepAngle){
		return targetAngle;
	}
	else{
		if (startAngle > targetAngle){
			startAngle = startAngle - stepAngle;
		}
		if (startAngle < targetAngle){
			startAngle = startAngle + stepAngle;
		}
		return startAngle;
	}
}
float Polynteropolation(float startAngle, float targetAngle){ // 三次项插值
//	float m = n-DELAY_TIME / 2+1;
//	if (m<0)m=0;
	if(n <= 40){
		float a0, a1, a2, a3;
		a0 = startAngle;
		a1 = 0;
		a2 = 3 * (targetAngle - startAngle) / (t * t);
		a3 = -2 * (targetAngle - startAngle) / (t * t * t);
		return a0 + a1 * dt * n + a2 * dt * n * dt * n + a3 * dt * n * dt * n * dt * n;
	}
	else{
		return targetAngle;
	}
}

//// 定时器4中断处理函数
//void TIM4_IRQHandler(void)
//{
//    if (TIM4->SR & 0x0001){
//        TIM4->SR &= ~0x0001; // 清除中断标志
//    }

//}

//void TIM3_IRQHandler(void){
//    if (TIM3->SR & 0x0001){
//        TIM3->SR &= ~0x0001; // 清除中断标志
//	}

//}

void TIM2_IRQHandler(void) { // 中断子函数
    if (TIM2->SR & 0x0001) {
        TIM2->SR &= ~0x0001; // 清除中断标志
    }
	if (KeyScan()) {
		GPIOC->BSRR |= 0x000000020;
		key = !key;
		if(flag_cal == 3){
			flag_cal = 0;
			key = 0;
		}
		else{
			flag_cal ++;
		}
		n=0;
	}
	for (i=0;i<5;i++){
		tempAngle[i] = angle[i];
	}
	n++;
	for (i=0;i<4;i++){
		angle[i] = Polynteropolation(tempAngle[i], newAngle[i]);
	}
	angle[1] = LinearInterpolation(angle[1], newAngle[1], 1);
	angle[4] = newAngle[4];
	Angle2Duty(angle);
	TIM3->CCR4 = duty[0];
	TIM4->CCR1 = duty[1];
	TIM4->CCR2 = duty[2];
	TIM4->CCR3 = duty[3];
	TIM4->CCR4 = duty[4];
}

int main(void){
	
	GpioConfig();
	TimeConfig();
	InterruptConfig();
	
	while(1){
		if (flag_cal==1){
			TIM3->CCR3 = 10;
			for(j=0;j<50;j++)
				for(i=0;i<10;i++)
					delay_us(1000);
			GPIOC->BSRR = 0x00000040;  // 设置PC6输出高电平
			delay_us(2);
			GPIOC->BRR = 0x0040;  // 设置PC6输出低电平
			TIM5->CNT = 0;
			while (!(GPIOC->IDR & 0x0080)) {
				if (TIM5->CNT > 6000) {
					break;
				}
			}
			TIM5->CNT = 0;
			while (GPIOC->IDR & 0x0080);
			time = TIM5->CNT;
			d2 = (time * 10 * temper2vel(temper)) / 20000;
			delay_us(6000);
			
			TIM3->CCR3 = 15;
			for(j=0;j<50;j++)
				for(i=0;i<10;i++)
					delay_us(1000);
			GPIOC->BSRR = 0x00000040;  // 设置PC6输出高电平
			delay_us(2);
			GPIOC->BRR = 0x0040;  // 设置PC6输出低电平
			TIM5->CNT = 0;
			while (!(GPIOC->IDR & 0x0080)) {
				if (TIM5->CNT > 6000) {
					break;
				}
			}
			TIM5->CNT = 0;
			while (GPIOC->IDR & 0x0080);
			time = TIM5->CNT;
			d1 = (time * 10 * temper2vel(temper)) / 20000;
			delay_us(6000);
			distance = calDistance(d1, d2, theta);
			beta = calTheta(distance, d1, d2);
			GPIOC->BSRR |= 0x00200000;
			flag_cal++;
			key = 0;
		}
		else{
			if (!key) {
				GPIOB->BSRR |= 0x00000004;
				newAngle[0] = 90;
				newAngle[1] = 70;
				newAngle[2] = 160;
				newAngle[3] = 90;
				newAngle[4] = 155;
			} else {
				GPIOB->BSRR |= 0x00040000;
				newAngle[0] = beta;
				newAngle[1] = 100;
				newAngle[2] = 180;
				newAngle[3] = 170;
				newAngle[4] = 120;
			}
		}
	}
}

