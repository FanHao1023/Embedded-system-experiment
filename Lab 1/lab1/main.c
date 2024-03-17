#include "M480.h"

int32_t main(void)
{
	
	// Input Pin set
	GPIO_SetMode(PA, BIT0, GPIO_MODE_INPUT);  // SW1
	GPIO_SetMode(PA, BIT1, GPIO_MODE_INPUT);  // SW2
	
	GPIO_SetMode(PG, BIT2, GPIO_MODE_INPUT);  // UP
    GPIO_SetMode(PC, BIT10, GPIO_MODE_INPUT);  // DOWN
 	GPIO_SetMode(PG, BIT3, GPIO_MODE_INPUT);  // CENTER

	
	// Output Pin set
	GPIO_SetMode(PH, BIT6, GPIO_MODE_OUTPUT);  // LEDR1
	GPIO_SetMode(PH, BIT7, GPIO_MODE_OUTPUT);  // LEDG1
	
	while(1) {
		
		PH6 = (PG2==0 || PG3==0 || PA0==0)? 0 : 1;  //����O�G������UP�BCENTER�άOSW1�Q���U 
		PH7 = (PC10==0 || PG3==0|| PA1==0)? 0 : 1;  //���O�G������DOWN�BCENTER�άOSW2�Q���U 
	
	}
}
