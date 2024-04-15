#include "stdio.h"
#include "NuMicro.h"
#include "tmr.h"
#include "system_init.h"
#include "GUI.h"
#include "display.h"
#include "BNCTL.h"
#include "StepMotorAgent.h"

/* define max and mini speed */
#define MaxSpeed	17
#define MinSpeed	1

/* global variable define */
uint32_t	timecount;
uint8_t		dir;
uint32_t	speed;
	
int main(void)
{
		// local variable define 
		char motor_state_buf[30];
		char SPD_buf[30];
		char direction_buf[30];
		uint32_t  speedCTL;
	
		// System initialize 
    	SYS_Init();
	
		// button initialize
		BTN_init();
	
		// Step Motor initialize
		StepMtr_Initial();
	
		// GUI display initialize 
		Display_Init();
	
		// Init TMR0 for timecount 
		TMR0_Initial();
	
		// Set initial speed and rotate direction
		dir = 1;
		speed = 10;
		
    while(1)
		{
				// Scan button 
				BTN_task();
				
				// stop rotating
				if(Btn_IsOneShot(0x01) == 0x01){
					GUI_Clear();
					Btn_OneShotClear(0x01);
					speed = 0;  
				}
				
				// rotation direction control
				if(Btn_IsOneShot(0x02) == 0x02){
					dir^=0x01;    // use XOR to change 0 to 1 and change 1 to 0
					GUI_Clear();
					Btn_OneShotClear(0x02);
				}    
				
				// speed up rotating			
				if(Btn_IsOneShot(0x04) == 0x04){
					GUI_Clear();
					Btn_OneShotClear(0x04);
				
					if (speed < MaxSpeed){
						speed ++;    // only speed up when the speed does not exceed the maximum speed
					}    
		   		}    
				
				// speed down rotating
				if(Btn_IsOneShot(0x08) == 0x08){
					GUI_Clear();
					Btn_OneShotClear(0x08);
				
					if (speed == 0){
						speed =1;    
					}					
					else if (speed > MinSpeed){
						speed --;    // only speed down when the speed is larger than minimum speed
					}    
				}    
				
				// Step motor output 
				if(speed)
					speedCTL = 1000/speed;
				else
					speedCTL = 0;
				
				StepMtr_Task(dir, speedCTL);

				sprintf(SPD_buf,"speed : %02d rpm" , speed*6);    //6~102


				// Display current state 
				if (speed == 0)
					Display_buf("motor state : Stopped", 1, 1);  // display "motor state : Stopped" when not rotating
				else
					Display_buf("motor state : Running", 1, 1); // display "motor state : Running" when rotating
				
				Display_buf(SPD_buf, 1, 25);
				
				// Display rotate direction
				if (dir)
					Display_buf("direction : Clockwise", 1, 49);
				else
					Display_buf("direction : CounterClockwise", 1, 49);
		}
}
