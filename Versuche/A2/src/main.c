/* Code template for exercise 2
 *
 *
 * 2011, Philipp Kindt <kindt@rcs.ei.tum.de>
 */

#include <stdio.h>
#include "platform.h"
#include <libmps.h>
#include <inttypes.h>

//This variable will allways point onto the bus, using the CS5-Signal. A Write to *bus will result in a write-bus transaction, a read-access to *bus will cause a read-action on the bus
volatile u16 *bus = MEMORY_ADDR_REGISTER_CS5;

u8 PIT_occured;


//these functions and symbols are defined in the switch_lights.s - assembly code. They just need to be declared here.
extern u8 pattern001;
extern u8 pattern010;
extern u8 pattern100;


//this function is also declared in the assembly code
extern void switch_lights(u8* pattern);
extern void determine_lights();


//the interrupt service routine will be called once a second by the PIT (timer!).
void interruptServiceRoutine(){

	//determine_lights();

	// HELLO WORLD
	*bus = 4;
		*bus = 2;


	/*u16 tmp = *bus;							//read from bus
	//xil_printf("Bus RD: %d\n",tmp);			//print the value read to UART

	//check, which pattern has been read from the bus. According to that, write new pattern to the register.
	if(tmp == pattern100){
		//xil_printf("001\n");
		//call switch_lights in order to store the requested pattern to the register
		*bus = 1;
		//switch_lights(&pattern001);
	}else if(tmp == pattern001){
		//xil_printf("010\n");
		//call switch_lights in order to store the requested pattern to the register
		*bus = 2;
		//switch_lights(&pattern010);
	}else{
		//xil_printf("100\n");
		//call switch_lights in order to store the requested pattern to the register
		*bus = 4;
		//switch_lights(&pattern100);
	}*/

	//xil_printf("Im Back!\n");
	//PIT_occured = 1;

}



int main()
{

	//this code sets up the timer that calls interruptServiceRoutine() once a second. There is no need to
	//understand the details - the only thing of importance is that interruptServiceRoutine() will be called automatically.
	init_platform();
    InterruptController_Init();
    ENABLE_INTERRUPTS;
    Wait_Init();

    //insert your CODE for the PIT here
    PIT_InstallHandler(interruptServiceRoutine);
    PIT_Enable(PIT_CalcModulus(1000000));

    //print("Flickering LED Programme loaded\n\r");

    //PIT_occured = 0;

    while(1)
    {
    	/*if(PIT_occured){
    		PIT_occured = 0;
    		xil_printf("PIT_Interrupt_Occured");
    	}*/
    }

    cleanup_platform();

    return 0;
}
