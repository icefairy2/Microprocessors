/* Sample solution for the bakery line in combination with the master solution board.
 *
 * 2012, Philipp Kindt <kindt@rcs.ei.tum.de>
 */

#include <stdio.h>
#include "platform.h"
#include <libmps.h>
#include <libmps_exception.h>
#include <inttypes.h>
#include "debug.h"
#include "schedule.h"
#include <inttypes.h>
#include <stdlib.h>
#include "bakingline.h"

//Temporary data
u16 motors_bus_5v_temp = 0;
u16 sensors_bus_5v_temp = 0;
u16 motors_bus_24v_temp = 0;
u16 sensors_bus_24v_temp = 0;
u8 adc_out_value;

/****Functions to set and reset motors and to get sensor data****/
void set(int motor) {
	motors_bus_5v_temp |= (1 << motor);
}

void reset(int motor) {
	motors_bus_5v_temp &= ~(1 << motor);
}

int get(int sensor) {
	return ((sensors_bus_5v_temp & (1 << sensor)) >> sensor);
}

void set24(int motor) {
	motors_bus_24v_temp |= (1 << motor);
}

void reset24(int motor) {
	motors_bus_24v_temp &= ~(1 << motor);
}

int get24(int sensor) {
	return ((sensors_bus_24v_temp & (1 << sensor)) >> sensor);
}
/***************************************************************/

/********CHIP SELECTS**************/
volatile u16 *motors_bus_5v = MEMORY_ADDR_REGISTER_CS5;
volatile u16 *sensors_bus_5v = MEMORY_ADDR_REGISTER_CS6;
volatile u16 *motors_bus_24v = MEMORY_ADDR_REGISTER_CS7;
volatile u16 *sensors_bus_24v = MEMORY_ADDR_REGISTER_CS8;
/************************************/

/*********SCHEDULING****************************************************************************/
u32 scheduler_schedule_semaphore = 0; //a semaphore to signal the main loop a schedule stroke has occured

//the PIT handler is called within a well-defined period, that is set by the scheduler stroke
void PIT_Handler() {
	scheduler_schedule_semaphore = 1; //signal to the main programm that
}
/************************************************************************************************/

int main() {
	init_platform();
	InterruptController_Init();
	ENABLE_INTERRUPTS;
	Wait_Init();
	PWM_Init();
	SADC_Init();
	LCD_Init();

	/******Variable initialization************/
	box_count = 0;
	silo_time = 0;
	oven_time = 0;
	voltage = 0;
	resistance = 0;
	temperature = 0;
	temperature_old = 0;
	pwm_value_oven = 0;

	box_count_current_state = SLE_LOW;
	first_band_current_state = IDLE_FB1;
	first_turntable_current_state = START_FT1;
	second_turntable_current_state = START_FT2;
	silo_current_state = IDLE_SILO;
	mixer_current_state = START_MIXER;
	oven_current_state = IDLE_OVEN;
	/****************************************/

	//start the pit. It will fire within a period defined by the SCHEDULE_TICKS_PER_SECOND-macro. The period is
	//choosen such that it will fire SCHEDULE_TICKS_PER_SECOND times per second and thus, define the schedule stroke
	//for the whole system
	PIT_InstallHandler(PIT_Handler);
	PIT_Enable(PIT_CalcModulus(1000000 / SCHEDULE_TICKS_PER_SECOND));

	/*Reset every motor
	 *BUS_CS5 = 0;*/

	PWM_Set(10000, 0);

	//start main loop
	while (1) {
		//if scheduler-PIT has fired, start all *_scheduled_control-functions
		if (scheduler_schedule_semaphore) {

			scheduler_schedule_semaphore = 0; //reset( the semaphore. It will be set to 1 again next time by the PIT

			//Set motor data according to the FSM results
			*motors_bus_5v = motors_bus_5v_temp;
			*motors_bus_24v = motors_bus_24v_temp;
			//xil_printf("Motors_Data = %d\n",motors_bus_5v_temp);

			//Get sensors data
			sensors_bus_5v_temp = *sensors_bus_5v;
			sensors_bus_24v_temp = *sensors_bus_24v;
			adc_out_value = (sensors_bus_24v_temp >> 8);

			/*voltage = (((adc_out_value >> 4) & 0x0F)/16 + (adc_out_value & 0x0F)/256) * 5.12; //formula from datasheet
			resistance = 2.8 * (5-voltage)/voltage; //get the resistance value from the voltage divider (R2 = 2.8 kOhm)
			temperature = ALPHA * (70.9 - 6.19 * resistance) + (1 - ALPHA) * temperature_old; //exponential smoothing*/
			temperature = ALPHA * (0.8 * adc_out_value - 4.2) + (1 - ALPHA) * temperature_old;
			temperature_old = temperature;

			//Printing to LCD
			//SLOWS IT DOWN REALLY BAD
			/*LCD_ClearDisplay();
			LCD_CursorHome();
			LCD_PrintFloatValue(temperature, 5);
			LCD_CursorTo(2,1);
			LCD_PrintText("Empty silos: ");

			if (get(SLS1) && get(SLS2) && get(SLS3)){
				LCD_PrintText("---");
			}
			else {
				if (!get(SLS1)){
					LCD_PrintText("1");
				}
				if (!get(SLS2)){
					LCD_PrintText("2");
				}
				if (!get(SLS3)){
					LCD_PrintText("3");
				}
			}*/

			//xil_printf("ADC value = %d\n", adc_out_value);
			//xil_printf("ADC bus value = %d\n Temperature = %d\n", sensors_bus_24v_temp , temperature);
			/*int sensors = *BUS_CS8;
			 xil_printf("Sensors_Data = %d\n", sensors);*/

			//Call all FSM functions
			box_count_fsm();
			first_band_fsm();
			fist_turntable_fsm();
			silo_fsm();
			second_turntable_fsm();
			mixer_fsm();
			oven_fsm();

			//write value to PWM
			PWM_Set(10000, pwm_value_oven);
		}

	};

	cleanup_platform();
	return 0;
}

