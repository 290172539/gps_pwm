#include "stm32f10x.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "delay.h"
#include "key.h"
#include "exti.h"

/*模式选择的变量定义*/
volatile int i_while_flag = 1;
volatile int i_old_gprmc = 0;
volatile int i_d_gprmc = 0;
volatile int delay_cnt = 200;			//设置延时函数的值
volatile u8 Time1_hq_status = 0;
	
typedef struct name_Time{
	u16 hour;
	u16 min;
	u16 sec;
}gprmc_Time;

 int main(void)
 {	

	 u16 i_gps_count = 0;
	 u16 i_gps_cnt = 0;
	 u16 i_d_gps_cnt = 0;
	 u8 per_sec_spd = 0;		//每秒发五次gps的数据	
	 int check_result;
	 int check_cnt;
	 char buf_10to_16[3];
	 char gprmc_buf[] = "$GPRMC,000001,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,E*4C";		
	 gprmc_Time gps_time={0,0,0};
		
	 
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// 设置中断优先级分组2
	 
	 uart_init(9600);
	 LED_Init();
	 delay_init();

	 /*PWM*/
	// TIM1_PWM_Init(899,0);				//不分频。pwm频率=72000/（899+1） = 80khz;
	 TIM1_PWM_Init(9999,7199);				//pwm频率=72000/（9999+1）*(7199+1) = 1hz;

	 TIM_SetCompare1(TIM1,7200);
	//TIM_SetCompare1(TIM1,200);
	// EXTIX_Init();		//外部中断初始化
	
#if 0	 
	while(1)
	{
				if(Time1_hq_status == 1)
				{
					Time1_hq_status = 0;
					printf("TIME1\n");
				}
				else
				{
					//printf("noTime\n");
				}
	}
#endif
	
	
#if 1
	 while(1)
	 {

//		 /*pwm test*/
		 LED1 = 0;
if(i_while_flag)
{
	if(i_old_gprmc)
	{
		switch(i_gps_cnt)
				{
				case 0:
					printf("$GPRMC,000001,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*60\n");
					break;
				case 1:
					printf("$GPRMC,000002,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*63\n");
					break;
				case 2:
					printf("$GPRMC,000003,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*62\n");
					break;
				case 3:
					printf("$GPRMC,000004,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*65\n");
					break;
				case 4:
					printf("$GPRMC,000005,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*64\n");
					break;
				case 5:
					printf("$GPRMC,000006,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*67\n");
					break;
				case 6:
					printf("$GPRMC,000007,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*66\n");
					break;
				case 7:
					printf("$GPRMC,000008,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*69\n");
					break;
				case 8:
					printf("$GPRMC,000009,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*68\n");
					break;
				case 9:
					printf("$GPRMC,000010,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,E*60\n");
					break;
				default:
						break;
				}
				delay_ms(delay_cnt+800);
			if(i_gps_cnt < 9)
				i_gps_cnt++;
			else
				i_gps_cnt = 0;
	}
	else if(i_d_gprmc)
	{
		switch(i_d_gps_cnt)
				{
				case 0:
					printf("$GPRMC,000001,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4D\n");
					break;
				case 1:
					printf("$GPRMC,000002,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4E\n");
					break;
				case 2:
					printf("$GPRMC,000003,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4F\n");
					break;
				case 3:
					printf("$GPRMC,000004,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*48\n");
					break;
				case 4:
					printf("$GPRMC,000005,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*49\n");
					break;
				case 5:
					printf("$GPRMC,000006,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4A\n");
					break;
				case 6:
					printf("$GPRMC,000007,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4B\n");
					break;
				case 7:
					printf("$GPRMC,000008,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*44\n");
					break;
				case 8:
					printf("$GPRMC,000009,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*45\n");
					break;
				case 9:
					printf("$GPRMC,000010,A,1111.11,S,11111.11,E,000.0,360.0,030418,111.1,,D*4D\n");
					break;
				default:
						break;
				}
				delay_ms(delay_cnt+800);
			if(i_d_gps_cnt < 9)
				i_d_gps_cnt++;
			else
				i_d_gps_cnt = 0;
	}
	else
	{
				/*数据跟脉冲对其，定时器中断触发的时候Time1_hq_status标志会置1*/
		if(Time1_hq_status == 1)
		{
			
		/*时间递增实现*/
		gps_time.sec++;
		if(gps_time.sec == 60)
		{
			gps_time.sec = 0;
			gps_time.min++;
			if(gps_time.min==60)
			{
				gps_time.min = 0;
				gps_time.hour++;
				if(gps_time.hour ==25)
				{
					gps_time.hour = 0;
					gps_time.min = 0;
					gps_time.sec = 0;
				}
			}
		}
		
		/*按照asicc对照表将时间转换成字符串*/
		gprmc_buf[7] = ((gps_time.hour/10)%10)+48;
		gprmc_buf[8] = (gps_time.hour%10)+48;
		
		gprmc_buf[9] = ((gps_time.min/10)%10)+48;
		gprmc_buf[10] = gps_time.min%10+48;
		
		gprmc_buf[11] = ((gps_time.sec/10)%10)+48;
		gprmc_buf[12] = (gps_time.sec%10)+48;
		
		/*计算校验和*/
		for(check_result = gprmc_buf[1],check_cnt = 2; gprmc_buf[check_cnt] !='*';check_cnt++)
		{
			check_result ^= gprmc_buf[check_cnt];
		}
		
		/*十进制转换成十六进制*/
		sprintf(buf_10to_16,"%x",check_result);
		if(buf_10to_16[1] >= 'a' && buf_10to_16[1] <= 'z')
		{
			buf_10to_16[1] = buf_10to_16[1] - 32;
		}
		gprmc_buf[65] = buf_10to_16[0];
		gprmc_buf[66] = buf_10to_16[1];
		
		/*
		//发五帧同样的数，5hz
		for(per_sec_spd = 0; per_sec_spd < 5; per_sec_spd++)
		{
			printf("%s\n",gprmc_buf);
			delay_ms(delay_cnt);
		}*/
		

			Time1_hq_status = 0;
			delay_ms(400);
			printf("%s\n",gprmc_buf);
	}
	}
}
		

		 LED1 = 0;
////		 TIM_SetCompare1(TIM1,led0_pwm_val);
		 

	 }
#endif
 }
 

