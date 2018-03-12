/*
 * Simple example how to write a finite-state-machine (FSM) in C.
 *
 * This programm will go through several states. The states transit by pushing the right buttons, and each state produces a message on the LCD-display.
 *
 *
 * The FSM will go throgh the following set of states:
 *
 * 								 --->	MESSAGE_LEFT  ---
 * IDLE		--->	MESSAGE_1 ---						 --->	MESSAGE_2  -----------
 *			|					 --->	MESSAGE_RIGHT ---							 |
 *			|________________________________________________________________________|
 *
 *
 * 	Jan 2012, Philipp Kindt <kindt@rcs.ei.tum.de>
 */

#include <stdio.h>
#include "platform.h"
#include "libmps.h"
#include "inttypes.h"

//lets define a type for our state
typedef enum _state_type{STATE_IDLE,STATE_MESSAGE_1,STATE_MESSAGE_1_BTNRELEASE,STATE_MESSAGE_LEFT,STATE_MESSAGE_LEFT_BTNRELEASE,STATE_MESSAGE_RIGHT,STATE_MESSAGE_RIGHT_BTNRELEASE,STATE_MESSAGE_2,STATE_MESSAGE_2_BTNRELEASE} state_type;

//define a state variable
state_type state = STATE_IDLE;

//a semaphore for the scheduler
uint32_t pit_semaphore = 0;

//this handler is called by the PIT and will signal the main program it must schedule the FSM
void PIT_Handler(){
	pit_semaphore = 1;
}


//this function is called 100 times a second and will schedule the FSM
void FSM_control(){

	uint32_t buttons;		//temp. variable

	//go through all states
	switch(state){
	case STATE_IDLE:		//the initial state. go to next state immediately

		//set next state
		state = STATE_MESSAGE_1_BTNRELEASE;
		break;
	case STATE_MESSAGE_1_BTNRELEASE:		//set LCD message for state 1 and wait for button to be released
		if(BUTTONS_GetValue() == 0){
			LCD_ClearDisplay();
			LCD_PrintText("MESSAGE_1.");
			LCD_CursorTo(2,1);
			LCD_PrintText("press l or r");
			state = STATE_MESSAGE_1;
		}
		break;
	case STATE_MESSAGE_1:
		buttons = BUTTONS_GetValue();
		if(buttons == 1){
			state = STATE_MESSAGE_LEFT_BTNRELEASE;
		}
		if(buttons == 4){
			state = STATE_MESSAGE_RIGHT_BTNRELEASE;
		}
	break;
	case STATE_MESSAGE_LEFT_BTNRELEASE:
		if(BUTTONS_GetValue() == 0){
			LCD_ClearDisplay();
			LCD_PrintText("MESSAGE_LEFT.");
			LCD_CursorTo(2,1);
			LCD_PrintText("press any key");
			state = STATE_MESSAGE_LEFT;
		}
	break;
	case STATE_MESSAGE_RIGHT_BTNRELEASE:
		if(BUTTONS_GetValue() == 0){
			LCD_ClearDisplay();
			LCD_PrintText("MESSAGE_RIGHT.");
			LCD_CursorTo(2,1);
			LCD_PrintText("press any key");
			state = STATE_MESSAGE_RIGHT;
		}
	break;
	case STATE_MESSAGE_LEFT:
	case STATE_MESSAGE_RIGHT:
			if(BUTTONS_GetValue() != 0){
				state = STATE_MESSAGE_2_BTNRELEASE;
			}
	break;
	case STATE_MESSAGE_2_BTNRELEASE:
		if(BUTTONS_GetValue() == 0){
			LCD_ClearDisplay();
			LCD_PrintText("MESSAGE_2.");
			LCD_CursorTo(2,1);
			LCD_PrintText("press any key");
			state = STATE_MESSAGE_2;
		}

		break;
	case STATE_MESSAGE_2:
		if(BUTTONS_GetValue() != 0){
			state = STATE_MESSAGE_1_BTNRELEASE;
		}
	break;
	default:
		xil_printf("INVALID STATE!\n");
		break;
	}

}

int main()
{
    init_platform();
    InterruptController_Init();		//needed for Wait_Init();
    ENABLE_INTERRUPTS;
    Wait_Init();					//needed for LCD_Init();
    BUTTONS_Init();
    LCD_Init();


    //start scheduling
    PIT_InstallHandler(PIT_Handler);
    PIT_Enable(35000000/100);//calls PIT 100 times a second

    while(1){
    	if(pit_semaphore){
    		pit_semaphore = 0;
    		FSM_control();
    	}
    }

    cleanup_platform();

    return 0;
}
