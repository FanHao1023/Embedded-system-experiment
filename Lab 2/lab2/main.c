#include "stdio.h"
#include "NuMicro.h"
#include "tmr.h"
#include "system_init.h"
#include "GUI.h"
#include "display.h"

/* define */
#define MaxSpeed	50000		
#define MinSpeed	1000		
#define SW_UP		PC9			//UP				JoyStick
#define SW_DOWN		PG4			//DOWN				JoyStick
#define SW_CTR		PG3			//CENTER			JoyStick

/* �ŧi�ܼ� */ 
uint32_t SpeedCtl;
uint32_t timecount;
uint32_t sec = 0;
uint32_t hour = 0;
uint32_t min = 0;

/* �ŧi��� */		
void Clock_Task(void);
void clock_init(void);
void clock_tick(void);
void LED_showing(uint32_t SpeedCtl);
void GPIO_init(void);
uint32_t JoyStick(unsigned char BTN_state);

/* �w�q�@��union��Joystick_union�A�i�H�z�LJState�Ӧs��struct���� */ 
typedef union{
	struct{
		unsigned UP		:1;
		unsigned DOWN	:1;
		unsigned CTR	:1;
	};
unsigned char JState;
}Joystick_union;


/* �z�L JSUnion �Ӧs�� Joystick_union */ 
Joystick_union JSUnion; 



int main(void)
{	
	    /* initialize */
		char clock_buf[20];
	    char speed_buf[20];
	    uint32_t speed;
	
        SYS_Init();
	
		TMR0_Initial();
		
		clock_init();
	
		GPIO_init();
		
		Display_Init();
	
    while(1)
		{
				
				/* �w�qJSUnion���ʧ@ */ 
				JSUnion.UP 	= SW_UP;  
				JSUnion.DOWN = SW_DOWN;
				JSUnion.CTR = SW_CTR;
			
        		SpeedCtl = JoyStick(JSUnion.JState);  //�I�sJoystick�öǤJJSUnion�ӱ���n�[�t�B��t�ΰ��� 
				speed = 51000 - SpeedCtl;   // �w�q�t�� 
				
				LED_showing(SpeedCtl);   //LED toggle function
				clock_tick();	         //�I�sclock_tick					
				
				/* define how to show the clock */
				sprintf(clock_buf, "%02d:%02d:%02d", hour, min, sec);
				sprintf(speed_buf, "speed = %.1f (s)",speed / 10000.0);
				
				Display_buf(clock_buf, 270, 1);
				Display_buf(speed_buf, 1, 1);
		}
}


/* GPIO initialize */
void GPIO_init(void)
{
		GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT) ;	 // SW1
		GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT) ; // LEDR1
		GPIO_SetMode(PH, BIT7, GPIO_MODE_OUTPUT) ; // LEDG1
		GPIO_SetMode(PC, BIT9, GPIO_MODE_INPUT) ;	 // Joystyick_UP
		GPIO_SetMode(PG, BIT4, GPIO_MODE_INPUT) ;	 // Joystyick_DOWN
		GPIO_SetMode(PG, BIT3, GPIO_MODE_INPUT) ;	 // Joystyick_CENTER
}


//time initialize
void clock_init(void)
{
		sec = 0;
		min = 0;
		hour = 0;
}

/* �w�q�p�� */ 
void clock_tick(void)
{
		static uint32_t old_timecount = 0;
	
		if((uint32_t)(timecount - old_timecount) < 10000)
				return;
		
		old_timecount = timecount;
		sec++;
		if (sec == 60)
    {
        sec = 0;
        min++;
        if (min == 60)
        {
            min = 0;
            hour++;
            if (hour == 24)
                hour = 0;
        }
    }
}


/* �w�qJoystick��ƨӨM�w�n�[�t�B��t�Φ����� */ 
uint32_t JoyStick(unsigned char BTN_state)
{
		static uint32_t old_timecount = 0;
		static uint32_t SpeedCtl = 41000;
	
		if ((uint32_t)(timecount - old_timecount)<1000)
			return SpeedCtl;
		
		old_timecount = timecount;
		
		switch(BTN_state)
		{
			case 0x06:                    // Joystick up 
				if (SpeedCtl < MaxSpeed)  //�p�G�t�פp��̤j�t�סA�N�[�t 
					SpeedCtl -= 1000;
				else
					SpeedCtl = SpeedCtl;  //�_�h�A�����t�� 
				break;
				
			case 0x05:                    // Joystick down 
				if (SpeedCtl > MinSpeed)  //�p�G�t�פj��̤p�t�סA�N��t 
					SpeedCtl += 1000;
				else
					SpeedCtl = SpeedCtl;  //�_�h�A�����t�� 
				break;
				
			case 0x03:                    // Joystick center
					SpeedCtl = 51000;	  //�ѩ�speed���w�q��51000-SpeedCtl�A�]����SpeedCtl��51000�Y��speed=0 
				break;
				
			default:
					SpeedCtl = SpeedCtl;
				break;	
		}//switch	
	return SpeedCtl;
}


/* �w�q��LED�{�{����� */ 
void LED_showing(uint32_t SpeedCtl)
{
		static uint32_t old_timecount = 0;
	    static unsigned char f=0x01;
		if ((timecount - old_timecount) >= SpeedCtl)
			{
				f ^= 0x01;                      // ��f�b0��1������� 
				PH6 = (f)?1:0;                  // �Q��f�ӱ���PH6�O�_�o�� 
				PH7 = (f)?0:1;                  // �Q��f�ӱ���PH6�O�_�o���A�n��PH6��� 
				old_timecount = timecount;      // ��sold_timecount 
			}
		
}
