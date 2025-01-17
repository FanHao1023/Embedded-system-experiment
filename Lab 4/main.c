#include "NuMicro.h"
#include "ADCAgent.h"
#include "TempSensor.h"
#include "system_init.h"
#include "display.h"
#include "tmr.h"
#include "GUI.h"
#include "sys.h"
#include "BNCTL.h"
#include "StepMotorAgent.h"

/* define max and mini speed */
#define MaxSpeed	17
#define MinSpeed	1

/* global variable define */
uint32_t timecount = 0;
uint32_t speed;
uint8_t  dir;
uint8_t mode;
uint8_t flag;

/* function define */ 
void BTN_Speed_CTL(void);
void ADC_Speed_CTL(void);

int main(void)
{	
	/* local variable define */
	char ADC_value_buf[20];
	char M487sensor_temp_value_buf[20];
	char thermistor_temp_value_buf[20];
	char speed_buf[20];
	char mode_buf[20];
  uint32_t  speedCTL;
	
	/* Init System, peripheral clock */
	SYS_Init();
		
	/* Init temputer sensor */
	Temp_Sensor_Enable();

	/* Init TMR0 for timecount */
	TMR0_Initial();
	
	/* Opem GUI display */
	Display_Init();

	/* Init ADC */
	ADC_Initial();

  /* Init Button */
  BTN_init();
	
  /*Init Step Motor */
  StepMtr_Initial();
	dir = 1;
	speed = 10;
	mode = 0x00;
	flag = 0;

	while(1)
	{	
	    /* define how to change mode */
	    // change mode when pressing both SW1 and SW2
		if(Btn_IsDown(0x01) && Btn_IsDown(0x02) && flag == 0){
			mode ^= 0x01;
			flag = 1;
		}
		else {
			if(Btn_IsDown(0x01) && Btn_IsDown(0x02)){
				flag = 1; 
			}
			else
				flag = 0;
		}
		
		/* define speed control of different mode */
		if(mode==1)
			ADC_Speed_CTL();
		else
			BTN_Speed_CTL();
		
		/* Step motor output */
		if(speed)
			speedCTL = 1000/speed;
		else
			speedCTL = 0;
		
		
		/* Print ADC value */
		sprintf(ADC_value_buf, "ADC value : %03d", ADC_GetVR());
		Display_buf(ADC_value_buf, 1, 1);
		/* Print Sensor temperature */
		sprintf(M487sensor_temp_value_buf, "M487sensor_temp : %2.1f", ADC_GetM487Temperature());
		Display_buf(M487sensor_temp_value_buf, 1, 40);
		/* Print Thermistor temperature */
		sprintf(thermistor_temp_value_buf, "ThermistorTemp : %d", ADC_ConvThermistorTempToReal());
		Display_buf(thermistor_temp_value_buf, 1, 79);
		/* write motor state buffer */
		sprintf(speed_buf,"Speed : %02d rpm" , speed*6);//6~102
		Display_buf(speed_buf, 1, 118);
		//
		sprintf(mode_buf,"Mode : %d", mode);
		Display_buf(mode_buf, 1,157);
		
		/* Drivers */
		/* Motor Task */
		StepMtr_Task(dir, speedCTL);
		/* Get ADC value */
		ADC_Task();
		/* Scan button*/
		BTN_task();
		
	}
}

/* BTN_Speed_CTL define */
void BTN_Speed_CTL(void) {
	if(Btn_IsOneShot(0x01) == 0x01){
			//speed control
			speed = 0;
			//clear the GUI display
			GUI_Clear();
			//clear one-shot flag
			Btn_OneShotClear(0x01);
		}
		if(Btn_IsOneShot(0x02) == 0x02){
			dir ^= 0x01;
			//clear the GUI display
			GUI_Clear();
			Btn_OneShotClear(0x02);
		}
		if(Btn_IsOneShot(0x04) == 0x04){
			// speed up when the speed does not exceed maximum speed
			if(speed < MaxSpeed)
				speed ++;
			else
				speed = MaxSpeed;
			GUI_Clear();
			Btn_OneShotClear(0x04);
		}
		if(Btn_IsOneShot(0x08) == 0x08){
			//speed down when the speed is larger than minimum speed
			if(speed > MinSpeed)
				speed --;
			else 
				speed = MinSpeed;
			
			GUI_Clear();
			Btn_OneShotClear(0x08);
		}
}

/* ADC_Speed_CTL define */
void ADC_Speed_CTL(void) {
	uint8_t v;
	v = ADC_GetVR() ;
	// define four different speed due to different value of v
	if(v >= 0 && v <= 30)
		speed = 2;
	else if (v > 30 && v <= 60)
		speed = 5;
	else if (v > 60 && v <= 90)
		speed = 8;
	else
		speed = 10;
}
