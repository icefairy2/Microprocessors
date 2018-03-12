/* Template for A 3.1 : Pulse Width Modulation
 *
 * 2012, Philipp Kindt <kindt@rcs.ei.tum.de>
 */

#include <stdio.h>
#include "platform.h"
#include <libmps.h>
#include <inttypes.h>


int main()
{
	init_platform();




	InterruptController_Init();
	ENABLE_INTERRUPTS;
	Wait_Init();
	TPU_Init();
	LCD_Init();


	//initialize the button GPIO
	BUTTONS_Init();

	//initialize PWM Module
	PWM_Init();

	//initialize SADC
	SADC_Init();

	TPU_ChipSelect(1);
	TPU_Control(1);
	TPU_FreeRunMode(1);
	TPU_ConfigurePPWA(TPU_CHANNEL_TPU8,10);

	u8 SADC_control = 1;
	SADC_Write(SADC_control);
	int count = 0;
	int seccount = 0;
	u32 SADC_ReadValue = 0;
	u32 freq = 0;

	u32 ButtonValue;
	u32 hi_time = 30;
	while(1){

		for (count = 0; count<2; count++) {
			SADC_control = 0;
			SADC_Write(SADC_control);
			wait_ms(2);
			SADC_control = 2;
			SADC_Write(SADC_control);
			wait_ms(2);


			//xil_printf("******* %d ===\n", SADC_ReadValue);
		}
		for (count = 0; count<8; count++) {
			SADC_control = 0;
			SADC_Write(SADC_control);
			wait_ms(2);
			SADC_control = 2;
			SADC_Write(SADC_control);
			wait_ms(2);
			//SADC_ReadValue |= sadc-read()<<(8-count),

			//xil_printf("******* %d ===\n", SADC_ReadValue);
		}

		//PWM
		PWM_Set(255, SADC_ReadValue);
		//xil_printf("Read First Time!\n   === %d ===", SADC_ReadValue);
		SADC_ReadValue = 0;
		count = 0;
		SADC_control = 1;
		SADC_Write(SADC_control);
		wait_ms(2);

		if(TPU_IsReady()){
			freq = TPU_ClockCyclesToFreq(TPU_GetPPWACount(), 10);
			xil_printf("Hell Yeah!  %d \n",freq);
			LCD_PrintValue(freq);
			wait_ms(250);
			LCD_ClearDisplay();
		}



		//xil_printf("Freq = %d\n",freq);


		/*
	  //place your code here!
	  ButtonValue = BUTTONS_GetValue();
	  if(ButtonValue == 2){
		  //Button Nord means High
		  hi_time += 10;
		  if (hi_time > 100)
			  hi_time = 0;
		  xil_printf("Pushed button N, hi_time = %d\n", hi_time);
		  PWM_Set(100, hi_time);
		  wait_ms(500);
	  }
	  else if(ButtonValue == 8){
		  //Button Sud means Low
		  hi_time -= 10;
		  if (hi_time > 100)
			  hi_time = 100;
		  xil_printf("Pushed button S, hi_time = %d\n", hi_time);
		  PWM_Set(100, hi_time);
		  wait_ms(500);
	  }*/

	}

	return 0;
}
